#include <iostream>
#include <assert.h>

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() {}
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;
    virtual int GetLength() = 0;
    
    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex);
    virtual Sequence<T>* Append(T& item) = 0;
    virtual Sequence<T>* Prepend(T& item) = 0;
    virtual Sequence<T>* InsertAt(T& item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T> &list) = 0;
    virtual T& operator[] (int index) = 0;
};

template <typename T>
class DynamicArray
{
private:
    T* elements = nullptr;
    int capacity;
    int size;
    
    void Reserve (int newCapacity)
    {
        if(newCapacity <= capacity)
        {
            capacity = newCapacity;
            return;
        }
        
        T* newElements = new T[newCapacity];
        for (int i = 0; i < size; i++)
        {
            newElements[i] = elements[i];
        }
        delete elements;
        elements = newElements;
        capacity = newCapacity;
    }
public:
    DynamicArray():size(0), capacity(0), elements(nullptr){}
    
    DynamicArray(int size):size(size), capacity(size + 1), elements(nullptr)
    {
        if(size <= 0)
        {
            throw std::out_of_range("invalid argument for size");
        }
        else
        {
            elements = new T[capacity];
        }
    }
    
    DynamicArray(T* items, int count):DynamicArray(count)
    {
        if (items == nullptr)
        {
            throw std::out_of_range("invalid argument in elems for constructor");
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                elements[i] = items[i];
            }
        }
    }
    
    DynamicArray(const DynamicArray<T> &dynamicArray):DynamicArray<T>(dynamicArray.elements, dynamicArray.size){}
    
    ~DynamicArray()
    {
        delete[] elements;
    }
    
    T Get(int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("invalid argument");
        }
        return elements[index];
    }
    
    int GetSize() const
    {
        return size;
    }
    
    void Set (const T& value, int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("invalid argument");
        }
        Resize(size+1);
        for (int i = size; i > index; i--)
        {
            elements[i] = elements[i - 1];
        }
        elements[index] = value;
    }
    
    void Resize(int newSize)
    {
        if (newSize < 0)
        {
            throw std::invalid_argument("invalid argument");
        }
        if(newSize == 0)
        {
            delete[] elements;
            elements = nullptr;
        }
        if (newSize > capacity)
        {
            Reserve(newSize * 2);
        }
        size = newSize;
    }
    
    T& operator[](int index)
    {
        if(size <= index)
        {
            throw std::out_of_range("Invalid argument");
        }
        return elements[index];
    }
    
    bool operator== (const DynamicArray<T>& array)
    {
        if (array.size != size)
        {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (array[i] != elements[i])
            {
                return false;
            }
        }
        return true;
    }
};

template <typename T>
class ArraySequence: public Sequence<T>
{
private:
    virtual ArraySequence<T>* Instance() = 0;
    DynamicArray<T>* array;
public:
    //constructors
    ArraySequence ()
    {
        this->array = new DynamicArray<T>();
    }
    ArraySequence(int count)
    {
        this->array = new DynamicArray<T>(count);
    }
    ArraySequence(T* items, int count)
    {
        this->array = new DynamicArray<T>(items, count);
    }
    
    //methods
    T GetFirst() override
    {
        return this->array->Get(0);
    }
    T GetLast() override
    {
        return this->array->Get(this->array->GetSize - 1);
    }
    T Get(int index) override
    {
        return this->array->Get(index);
    }
    int GetLength() override
    {
        return this->array->GetSize();
    }
    ArraySequence<T>* Append(T& item)  //adding an element to the end of the array
    {
        ArraySequence<T>* intermediateResult;
        intermediateResult->array->Set(item, array->GetSize()-1);
        return intermediateResult;
    }
    ArraySequence<T>* Prepend(T& item)  //adding an element to the begining of the array
    {
        ArraySequence<T>* intermediateResult;
        intermediateResult->array->Set(item, 0);
        return intermediateResult;
    }
    ArraySequence<T>* InsertAt(T& item, int index)
    {
        ArraySequence<T>* intermediateResult;
        intermediateResult->array->Set(item, index);
        return intermediateResult;
    }
    
    virtual ~ArraySequence()
    {
        delete array;
    }
};

template <typename T>
class MutableArraySequence : public ArraySequence<T>
{
private:
    ArraySequence<T>* Instance() override
    {
        return *this;
    }
public:
    using ArraySequence<T>::ArraySequence;
    
    MutableArraySequence<T>* Concat(ArraySequence<T> &list)
    {
        MutableArraySequence<T>* intermediateResult = new MutableArraySequence (this->GetLength() + list->GetLength);
        for (int i = 0; i < this->GetLength(); i++)
        {
            intermediateResult->Set(this->Get(i), i);
        }
        for (int i = 0; i < list->GetLength(); i++)
        {
            intermediateResult->Set(list->Get(i), this->GetLength() + i);
        }
        return intermediateResult;
    }
    
    MutableArraySequence<T>* GetSubsequence(int startIndex, int endIndex)
    {
        MutableArraySequence<T>* intermediateResult = new MutableArraySequence (endIndex - startIndex + 1);
        for (int i = 0; i < endIndex - startIndex + 1; i++)
        {
            intermediateResult->Set(this->Get(startIndex + i), i);
        }
        return intermediateResult;
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T>
{
private:
    ArraySequence<T>* Instance() override
    {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence(this->GetLength());
        for (int i = 0; i < this->GetLength(); i++)
        {
            result->array[i] = this->array[i];
        }
        return result;
    }
public:
    using ArraySequence<T>::ArraySequence;
    
    ImmutableArraySequence<T>* Concat (Sequence <T>& elements) override
    {
        MutableArraySequence<T>* intermediate = new MutableArraySequence<T>(this->GetLength() + elements.GetLength());
        for (int i = 0; i < this->GetLength(); i++)
        {
            intermediate->InsertAt(this->Get(i), i);
        }
        for (int i = 0; i < elements.GetLength(); i++)
        {
            intermediate->IndertAt(elements.Get(i), i + this->GetLength());
        }
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(intermediate->GetLength());
        for (int i = 0; i < intermediate->GetLength(); i++)
        {
            result->InsertAt(intermediate->Get(i),i);
        }
        delete intermediate;
        return result;
    }
    
    ImmutableArraySequence<T>* GetSubSequence(int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength())
        {
            throw std::invalid_argument("invalid argument");
        }
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(&(*(this->data))[startIndex], endIndex - startIndex + 1);
        return result;
    }
};

template <typename T>
struct TheNode
{
    TheNode(T value): data(value), next(nullptr){}
    T data;
    TheNode<T>* next = nullptr;
};

template <typename T>
class LinkedList
{
private:
    int size;
    TheNode<T>* head;

public:
    LinkedList(T* items, int count): size(0), head(nullptr)
    {
        if (count < 0)
        {
            throw std::invalid_argument("invalid argument");
        }
        for (int i = 0; i < count; i++)
        {
            Append(items[i]);
        }
    }
    
    LinkedList(): size(0), head(nullptr){}
    
    LinkedList(const LinkedList<T>& list): LinkedList()
    {
        TheNode<T>* intermediate = list.head;
        for (int i = 0; i < list.size; i++)
        {
            Append(intermediate->data);
            intermediate = intermediate->next;
        }
        this->size = list.size;
    }
    
    virtual ~LinkedList()
    {
        if (head != nullptr)
        {
            TheNode<T>* current = head;
            TheNode<T>* buf;
            for (int i = 0; i < this->size; i++)
            {
                buf = current->next;
                current = buf;
            }
            delete current;
        }
    }
    
    T GetFirst()
    {
        if (head == nullptr)
        {
            throw std::invalid_argument("index out of range");
        }
        return head->data;
    }
    
    T GetLast()
    {
        if (head == nullptr)
        {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for(int i = 0; i < this->size-1; i++)
        {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }
    
    T Get(int index)
    {
        if (index < 0 || index >= this->size)
        {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for(int i = 0; i < index; i++)
        {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }
    
    LinkedList<T>* GetSubList(int startIndex, int endIndex)
    {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= size)
        {
            throw std::invalid_argument("index out of range");
        }
        LinkedList<T>* ResultList = new LinkedList();
        TheNode<T>* intermediate = head;
        for (int i = 0; i < startIndex; i++)
        {
            intermediate = intermediate->next;
        }
        for (int i = startIndex; i < endIndex; i++)
        {
            ResultList->Append(intermediate->data);
            intermediate = intermediate->next;
        }
        return ResultList;
    }
    
    int GetLength()
    {
        return this->size;
    }
    
    void Append(const T& item)
    {
        if(head == nullptr)
        {
            TheNode<T>* buf = new TheNode<T>(item);
            this->head = buf;
            size++;
        }
        else{
            TheNode<T>* buf = new TheNode<T>(item);
            TheNode<T>* intermediate = head;
            for(int i = 0; i < this->size-1; i++)
            {
                intermediate = intermediate->next;
            }
            intermediate->next = buf;
            this->size++;
        }
    }
    
    void Prepend(const T& item)
    {
        TheNode<T>* buf = new TheNode<T>(item);
        buf->next = head;
        head = buf;
        this->size++;
    }
    
    void InsertAt(T item, int index)
    {
        if (index < 0 || index >= this->size)
        {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* newElem = new TheNode<T>(item);
        TheNode<T>* intermediate = head;
        for (int i = 0; i < index - 1; i++)
        {
            intermediate = intermediate->next;
        }
        newElem->next = intermediate -> next;
        intermediate->next = newElem;
        this->size++;
    }
    
    LinkedList<T>* Concat(LinkedList<T>& list)
    {
        LinkedList<T>* resultList = new LinkedList<T>();
        for (int i = 0; i < this->size; i++)
        {
            resultList->Append(this->Get(i));
        }
        TheNode<T>* intermediate = list.head;
        for (int i = 0; i < list.size; i++)
        {
            resultList->Append(intermediate->data);
            intermediate = intermediate->next;
        }
        return resultList;
    }
    
    T& operator [](int index)
    {
        if (index > this->size || index < 0)
        {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for (int i = 0; i < index; i++)
        {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }
};

template <typename T>
class ListSequence : public LinkedList<T>
{
protected:
    virtual ListSequence<T>* Instance() = 0;
    LinkedList<T> list;
    
    ListSequence<T>* appendWithoutInstance(const T& item)
    {
        ListSequence<T>* result = this;
        result->data->Append(item);
        return result;
    }
public:
    ListSequence()
    {
        this->list = new LinkedList<T>();
    }
    ListSequence(T* items, int count)
    {
        this->list = new LinkedList<T>(items, count);
    }
    ListSequence(const Sequence<T>& seq)
    {
        this->data = new LinkedList<T>();
        for (int i = 0; i < seq.GetLength(); i++)
        {
            appendWithoutInstance(seq.Get(i));
        }
    }
    ListSequence(const ListSequence<T>& seq)
    {
        this->data = new LinkedList<T>();
        for (int i = 0; i < seq.GetLength(); i++)
        {
            appendWithoutInstance(seq.Get(i));
        }
    }
    
    T GetFirst()
    {
        return this->list->GetFirst;
    }
    
    T GetLast()
    {
        return this->list->GetLast;
    }
    
    T Get(int index)
    {
        return this->list->Get(index);
    }
    
    int GetLength()
    {
        return this->list->GetLength();
    }
    
    ListSequence<T>* Append(const T& item) override
    {
        this->list->Append(item);
        return this;
    }
    
    ListSequence<T>* Prepend(const T& item) override
    {
        this->list->Prepend(item);
        return this;
    }
    
    ListSequence<T>* InsertAt(const T& item, int index) override
    {
        this->list->Append(item, index);
        return this;
    }
    
    T& operator[](int index) override{
            return (*(this->list))[index];
        }
    
    ~ListSequence()
    {
        delete list;
    }
};

template <typename T>
class MutableListSequence : public ListSequence<T>
{
private:
    ListSequence<T>* Instance() override
    {
        return *this;
    }
public:
    using ListSequence<T>::ListSequence;
    
    MutableListSequence<T>* Concat(Sequence<T>& elements) override
    {
        MutableListSequence<T>* result = new MutableListSequence<T>(static_cast<Sequence<T>&>(*this));
        for (int i = 0; i < elements.GetLength(); i++)
        {
            result->Append(elements.Get(i));
        }
        return result;
    }
    MutableListSequence<T>* GetSubSequence (int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength())
        {
            throw std::invalid_argument("invalid argument");
        }
        T* intermediate = new T[endIndex - startIndex +1];
        for (int i = 0; i < endIndex - startIndex +1; i++)
        {
            intermediate[i] = this->Get(startIndex + i);
        }
        MutableListSequence<T>* result = new MutableListSequence<T>(intermediate, endIndex - startIndex + 1);
        delete [] intermediate;
        return result;
    }
};

template <typename T>
class ImmutableListSequence: public ListSequence<T>
{
private:
    LinkedList<T> Instance() override
    {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        return result;
    }
public:
    ImmutableListSequence<T>* Concat (Sequence<T>& elements) override
    {
        MutableListSequence<T>* intermediate = new MutableListSequence<T>(static_cast<Sequence<T>&>(*this));
        for (int i = 0; i < elements.GetLength(); i++)
        {
            intermediate->Append(elements.Get(i));
        }
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*intermediate);
        delete intermediate;
        return result;
    }
    
    ImmutableListSequence<T>* GetSubSequence (int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength())
        {
            throw std::invalid_argument("invalid argument");
        }
        T* intermediate = new T[endIndex - startIndex +1];
        for (int i = 0; i < endIndex - startIndex +1; i++)
        {
            intermediate[i] = this->Get(startIndex + i);
        }
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(intermediate, endIndex - startIndex + 1);
        delete [] intermediate;
        return result;
    }
};

void Test_DynamicArray_Constructors(){
    int a[]={1, 2, 3, 4, 5, 6};
    
    DynamicArray<int> test1(6);
    assert(test1.GetSize() == 6);

    DynamicArray<int> test2(a, 6);
    assert(test2.GetSize() == 6);
    for(int i=0; i < test2.GetSize(); i++){
        assert(test2.Get(i) == a[i]);
    }

    DynamicArray<int> test3(test2);
    assert(test3.GetSize() == test2.GetSize());
    for(int i = 0; i<test3.GetSize(); i++){
        assert(test2.Get(i) == test3.Get(i));
    }
}

void Test_DynamicArray_Set()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    DynamicArray<int> test(a, 8);
    assert(test.GetSize() == 8);
    for (int i = 0; i < test.GetSize(); i++)
    {
        assert(test.Get(i) == a[i]);
    }
    test.Set(9, 3);
    assert(test.Get(3) == 9);
    test[3] = 10;
    assert(test[3] == 10);
}

void Test_LinkedList_Constructors()
{
    int a[] = {1, 2, 3, 4, 5, 6};

    LinkedList<int> test2(a, 6);
    assert(test2.GetLength() == 6);
    for(int i = 0;i < test2.GetLength(); i++)
    {
        assert(test2.Get(i) == a[i]);
    }

    LinkedList<int> test4(test2);
    assert(test4.GetLength() == test2.GetLength());
    for(int i = 0; i < test4.GetLength(); i++)
    {
        assert(test2.Get(i) == test4.Get(i));
    }
}

void Test_LinkedList_Insert()
{
    int a[]={1,2,3,4,5,6};
    LinkedList<int> test5(a,6);
    test5.InsertAt(12,3);
    assert(test5[3]==12);
}

void Test_LinkedList_Append()
{
    int a[]={1,2,3,4,5,6};
    LinkedList<int> test10(a,6);
    test10.Append(10);
    assert(test10.GetLast()==10 );
}

void Test_LinkedList_SubList()
{
    int a[]={1,2,3,4,5,6};
    int b[]={3,4,5};
    LinkedList<int> test10(a,6);
    LinkedList<int>* test11 = test10.GetSubList(2,4);
    for(int i=0;i<test11->GetLength();i++){
        assert(test11->Get(i)==b[i]);
    }
}

void Test_LinkedList_Concat()
{
    int b[]={1,2,3};
    int c[]={4,5,6};
    int bc[]={1,2,3,4,5,6};
    LinkedList<int> test6(b,3);
    LinkedList<int> test7(c,3);
    LinkedList<int>* test8 = test6.Concat(test7);
    assert(test8->GetLength()==test6.GetLength()+test7.GetLength());
    for(int i=0;i<test8->GetLength();i++){
        assert(test8->Get(i)==bc[i]);
    }
}

void Test_LinkedList_Prepend()
{
    int a[]={1,2,3,4,5,6};
    LinkedList<int> test10(a,6);
    test10.Prepend(10);
    assert(test10.GetFirst()==10 );

}

void TestLinkedListInput()
{
    
}

int main(int argc, const char * argv[]) {
    int status = 0;
    std::cout << "0. Run tests for DynamicArray\n";
    std::cout << "1. Run tests for LinkedList\n";
    std::cout << "2. Добавить элемент в вектор целых чисел\n";
    std::cout << "3. Добавить элемент в вектор комплексных чисел\n";
    std::cout << "4. Суммировать векторы\n";
    std::cout << "5. Произведение векторов\n";
    std::cout << "6. Показать вектор по имени\n";
    std::cout << "7. Остановить программу\n";
    
    int flag = 1;
    while (flag)
    {
        std::cin >> status;
        switch (status)
        {
        case 0:
                Test_DynamicArray_Constructors();
                Test_DynamicArray_Set();
                std::cout<<"Tests for DynamicArray passed\n";
                break;
        case 1:
                Test_LinkedList_Constructors();
                Test_LinkedList_Append();
                Test_LinkedList_SubList();
                Test_LinkedList_Prepend();
                Test_LinkedList_Insert();
                Test_LinkedList_Concat();
                std::cout<<"Tests for LinkedList passed\n";
                break;

        case 2:
            
        case 3:
            
        case 4:
            
        case 7:
                flag = 0;
                break;

        default:
            fprintf(stderr, "Неизвестная команда\n");
        }
        std::cout << "0. Run tests for DynamicArray\n";
        std::cout << "1. Run tests for LinkedList\n";
        std::cout << "2. Добавить элемент в вектор целых чисел\n";
        std::cout << "3. Добавить элемент в вектор комплексных чисел\n";
        std::cout << "4. Суммировать векторы\n";
        std::cout << "5. Произведение векторов\n";
        std::cout << "6. Показать вектор по имени\n";
        std::cout << "7. Остановить программу\n";
    }
    Test_LinkedList_Constructors();
    Test_LinkedList_Append();
    Test_LinkedList_SubList();
    Test_LinkedList_Prepend();
    Test_LinkedList_Insert();
    Test_LinkedList_Concat();
    return 0;
}
