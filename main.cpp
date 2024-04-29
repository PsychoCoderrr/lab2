#include <assert.h>
#include <iostream>

template <typename T>
class Sequence {
public:
    virtual ~Sequence() { }
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) = 0;
    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>& list) = 0;
    virtual T& operator[](int index) = 0;
};

template <typename T>
class DynamicArray {
private:
    T* elements = nullptr;
    int capacity;
    int size;

    void Reserve(int newCapacity)
    {
        if (newCapacity <= capacity) {
            capacity = newCapacity;
            return;
        }

        T* newElements = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newElements[i] = elements[i];
        }
        delete elements;
        elements = newElements;
        capacity = newCapacity;
    }

public:
    DynamicArray()
        : size(0)
        , capacity(0)
        , elements(nullptr)
    {
    }

    DynamicArray(int size)
        : size(size)
        , capacity(size + 1)
        , elements(nullptr)
    {
        if (size <= 0) {
            throw std::out_of_range("invalid argument for size");
        } else {
            elements = new T[capacity];
        }
    }

    DynamicArray(T* items, int count)
        : DynamicArray(count)
    {
        if (items == nullptr) {
            throw std::out_of_range("invalid argument in elems for constructor");
        } else {
            for (int i = 0; i < count; i++) {
                elements[i] = items[i];
            }
        }
    }

    DynamicArray(const DynamicArray<T>& dynamicArray)
        : DynamicArray<T>(dynamicArray.elements, dynamicArray.size)
    {
    }

    ~DynamicArray()
    {
        delete[] elements;
    }

    T Get(int index)
    {
        if (index < 0 || index > size) {
            throw std::out_of_range("invalid argument");
        }
        return elements[index];
    }

    int GetSize() const
    {
        return size;
    }

    void Set(const T& value, int index)
    {
        if (index < 0 || index > size) {
            throw std::out_of_range("invalid argument");
        }
        Resize(size + 1);
        for (int i = size; i > index; i--) {
            elements[i] = elements[i - 1];
        }
        elements[index] = value;
    }

    void Resize(int newSize)
    {
        if (newSize < 0) {
            throw std::invalid_argument("invalid argument");
        }
        if (newSize == 0) {
            delete[] elements;
            elements = nullptr;
        }
        if (newSize > capacity) {
            Reserve(newSize * 2);
        }
        size = newSize;
    }

    T& operator[](int index)
    {
        if (size <= index) {
            throw std::out_of_range("Invalid argument");
        }
        return elements[index];
    }

    bool operator==(const DynamicArray<T>& array)
    {
        if (array.size != size) {
            return false;
        }
        for (int i = 0; i < size; i++) {
            if (array[i] != elements[i]) {
                return false;
            }
        }
        return true;
    }
};

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    virtual ArraySequence<T>* GetInstance() = 0;
    DynamicArray<T>* array;

public:
    // constructors
    ArraySequence()
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
    ArraySequence(Sequence<T>& seq)
    {
        this->array = new DynamicArray<T>(seq.GetLength());
        for (int i = 0; i < seq.GetLength(); i++) {
            this->array->Set(seq.Get(i), i);
            this->array->Resize(this->GetLength() - 1);
        }
    }
    ArraySequence(ArraySequence<T>& seq)
    {
        this->array = new DynamicArray<T>(seq.GetLength());
        for (int i = 0; i < seq.GetLength(); i++) {
            this->array->Set(seq.Get(i), i);
            this->array->Resize(this->GetLength() - 1);
        }
    }

    // methods
    T GetFirst() override
    {
        return this->array->Get(0);
    }
    T GetLast() override
    {
        return this->array->Get((this->array->GetSize()) - 1);
    }
    T Get(int index) const override
    {
        return this->array->Get(index);
    }
    int GetLength() const override
    {
        return this->array->GetSize();
    }
    ArraySequence<T>* Append(const T& item) override // adding an element to the end of the array
    {
        ArraySequence<T>* intermediateResult = GetInstance();
        intermediateResult->array->Set(item, array->GetSize());
        return intermediateResult;
    }
    ArraySequence<T>* Prepend(const T& item) override // adding an element to the begining of the array
    {
        ArraySequence<T>* intermediateResult = GetInstance();
        intermediateResult->array->Set(item, 0);
        return intermediateResult;
    }
    ArraySequence<T>* InsertAt(const T& item, int index) override
    {
        ArraySequence<T>* intermediateResult = GetInstance();
        intermediateResult->array->Set(item, index);
        return intermediateResult;
    }

    T& operator[](int index) override
    {
        return (*(GetInstance()->array))[index];
    }

    virtual ~ArraySequence() override
    {
        delete array;
    }
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
private:
    ArraySequence<T>* GetInstance() override
    {
        return static_cast<ArraySequence<T>*>(this);
    }

public:
    using ArraySequence<T>::ArraySequence;

    MutableArraySequence<T>* Concat(Sequence<T>& list) override
    {
        MutableArraySequence<T>* intermediateResult = new MutableArraySequence<T>(this->GetLength() + list.GetLength());
        for (int i = 0; i < this->GetLength(); i++) {
            intermediateResult->InsertAt(this->Get(i), i);
            intermediateResult->array->Resize(intermediateResult->GetLength() - 1);
        }
        for (int i = 0; i < list.GetLength(); i++) {
            intermediateResult->InsertAt(list.Get(i), this->GetLength() + i);
            intermediateResult->array->Resize(intermediateResult->GetLength() - 1);
        }
        return intermediateResult;
    }

    MutableArraySequence<T>* GetSubSequence(int startIndex, int endIndex) override
    {
        MutableArraySequence<T>* intermediateResult = new MutableArraySequence(endIndex - startIndex + 1);
        for (int i = 0; i < endIndex - startIndex + 1; i++) {
            intermediateResult->InsertAt(this->Get(startIndex + i - 1), i);
            intermediateResult->array->Resize(intermediateResult->GetLength() - 1);
        }
        return intermediateResult;
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
private:
    ArraySequence<T>* GetInstance() override
    {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence(this->GetLength());
        for (int i = 0; i < this->GetLength(); i++) {
            result->array[i] = this->array[i];
        }
        return result;
    }

public:
    using ArraySequence<T>::ArraySequence;

    ImmutableArraySequence<T>* Concat(Sequence<T>& elements) override
    {
        MutableArraySequence<T>* intermediate = new MutableArraySequence<T>(this->GetLength() + elements.GetLength());
        for (int i = 0; i < this->GetLength(); i++) {
            intermediate->InsertAt(this->Get(i), i);
        }
        for (int i = 0; i < elements.GetLength(); i++) {
            intermediate->InsertAt(elements.Get(i), i + this->GetLength());
        }
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(intermediate->GetLength());
        for (int i = 0; i < intermediate->GetLength(); i++) {
            result->InsertAt(intermediate->Get(i), i);
        }
        delete intermediate;
        return result;
    }

    ImmutableArraySequence<T>* GetSubSequence(int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength()) {
            throw std::invalid_argument("invalid argument");
        }
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(&(*(this->array))[startIndex], endIndex - startIndex + 1);
        return result;
    }
};

template <typename T>
struct TheNode {
    TheNode(T value)
        : data(value)
        , next(nullptr)
    {
    }
    T data;
    TheNode<T>* next = nullptr;
};

template <typename T>
class LinkedList {
private:
    int size;
    TheNode<T>* head;

public:
    LinkedList(T* items, int count)
        : size(0)
        , head(nullptr)
    {
        if (count < 0) {
            throw std::invalid_argument("invalid argument");
        }
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }

    LinkedList()
        : size(0)
        , head(nullptr)
    {
    }

    LinkedList(const LinkedList<T>& list)
        : LinkedList()
    {
        TheNode<T>* intermediate = list.head;
        for (int i = 0; i < list.size; i++) {
            Append(intermediate->data);
            intermediate = intermediate->next;
        }
        this->size = list.size;
    }

    virtual ~LinkedList()
    {
        if (head != nullptr) {
            TheNode<T>* current = head;
            TheNode<T>* buf;
            for (int i = 0; i < this->size; i++) {
                buf = current->next;
                current = buf;
            }
            delete current;
        }
    }

    T GetFirst()
    {
        if (head == nullptr) {
            throw std::invalid_argument("index out of range");
        }
        return head->data;
    }

    T GetLast()
    {
        if (head == nullptr) {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for (int i = 0; i < this->size - 1; i++) {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }

    T Get(int index)
    {
        if (index < 0 || index >= this->size) {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for (int i = 0; i < index; i++) {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex)
    {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= size) {
            throw std::invalid_argument("index out of range");
        }
        LinkedList<T>* ResultList = new LinkedList();
        TheNode<T>* intermediate = head;
        for (int i = 0; i < startIndex; i++) {
            intermediate = intermediate->next;
        }
        for (int i = startIndex; i < endIndex; i++) {
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
        if (head == nullptr) {
            TheNode<T>* buf = new TheNode<T>(item);
            this->head = buf;
            size++;
        } else {
            TheNode<T>* buf = new TheNode<T>(item);
            TheNode<T>* intermediate = head;
            for (int i = 0; i < this->size - 1; i++) {
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
        if (index < 0 || index >= this->size) {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* newElem = new TheNode<T>(item);
        TheNode<T>* intermediate = head;
        for (int i = 0; i < index - 1; i++) {
            intermediate = intermediate->next;
        }
        newElem->next = intermediate->next;
        intermediate->next = newElem;
        this->size++;
    }

    LinkedList<T>* Concat(LinkedList<T>& list)
    {
        LinkedList<T>* resultList = new LinkedList<T>();
        for (int i = 0; i < this->size; i++) {
            resultList->Append(this->Get(i));
        }
        TheNode<T>* intermediate = list.head;
        for (int i = 0; i < list.size; i++) {
            resultList->Append(intermediate->data);
            intermediate = intermediate->next;
        }
        return resultList;
    }

    T& operator[](int index)
    {
        if (index > this->size || index < 0) {
            throw std::invalid_argument("index out of range");
        }
        TheNode<T>* intermediate = head;
        for (int i = 0; i < index; i++) {
            intermediate = intermediate->next;
        }
        return intermediate->data;
    }
};

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    virtual ListSequence<T>* Instance() = 0;
    LinkedList<T>* list;

    ListSequence<T>* appendWithoutInstance(const T& item)
    {
        ListSequence<T>* result = this;
        result->list->Append(item);
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
        this->list = new LinkedList<T>();
        for (int i = 0; i < seq.GetLength(); i++) {
            appendWithoutInstance(seq.Get(i));
        }
    }

    ListSequence(const ListSequence<T>& seq)
    {
        this->list = new LinkedList<T>();
        for (int i = 0; i < seq.GetLength(); i++) {
            appendWithoutInstance(seq.Get(i));
        }
    }

    T GetFirst() override
    {
        return this->list->GetFirst();
    }

    T GetLast() override
    {
        return this->list->GetLast();
    }

    T Get(int index) const override
    {
        return this->list->Get(index);
    }

    int GetLength() const override
    {
        return this->list->GetLength();
    }

    ListSequence<T>* Append(const T& item) override
    {
        ListSequence<T>* result = Instance();
        result->list->Append(item);
        return result;
    }

    ListSequence<T>* Prepend(const T& item) override
    {
        ListSequence<T>* result = Instance();
        result->list->Prepend(item);
        return result;
    }

    ListSequence<T>* InsertAt(const T& item, int index) override
    {
        ListSequence<T>* result = Instance();
        result->list->InsertAt(item, index);
        return result;
    }

    T& operator[](int index) override
    {
        return (*(this->list))[index];
    }

    ~ListSequence()
    {
        delete list;
    }
};

template <typename T>
class MutableListSequence : public ListSequence<T> {
private:
    ListSequence<T>* Instance() override
    {
        return static_cast<ListSequence<T>*>(this);
    }

public:
    using ListSequence<T>::ListSequence;

    MutableListSequence<T>* Concat(Sequence<T>& elements) override
    {
        MutableListSequence<T>* result = new MutableListSequence<T>(static_cast<Sequence<T>&>(*this));
        for (int i = 0; i < elements.GetLength(); i++) {
            result->Append(elements.Get(i));
        }
        return result;
    }
    MutableListSequence<T>* GetSubSequence(int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength()) {
            throw std::invalid_argument("invalid argument");
        }
        T* intermediate = new T[endIndex - startIndex + 1];
        for (int i = 0; i < endIndex - startIndex + 1; i++) {
            intermediate[i] = this->Get(startIndex + i);
        }
        MutableListSequence<T>* result = new MutableListSequence<T>(intermediate, endIndex - startIndex + 1);
        delete[] intermediate;
        return result;
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
private:
    ListSequence<T>* Instance() override
    {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        return result;
    }

public:
    using ListSequence<T>::ListSequence;

    ImmutableListSequence<T>* Concat(Sequence<T>& elements) override
    {
        MutableListSequence<T>* intermediate = new MutableListSequence<T>(static_cast<Sequence<T>&>(*this));
        for (int i = 0; i < elements.GetLength(); i++) {
            intermediate->Append(elements.Get(i));
        }
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*intermediate);
        delete intermediate;
        return result;
    }

    ImmutableListSequence<T>* GetSubSequence(int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength()) {
            throw std::invalid_argument("invalid argument");
        }
        T* intermediate = new T[endIndex - startIndex + 1];
        for (int i = 0; i < endIndex - startIndex + 1; i++) {
            intermediate[i] = this->Get(startIndex + i);
        }
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(intermediate, endIndex - startIndex + 1);
        delete[] intermediate;
        return result;
    }
};

void TestDynamicArrayConstructors()
{
    int a[] = { 1, 2, 3, 4, 5, 6 };

    DynamicArray<int> test1(6);
    assert(test1.GetSize() == 6);

    DynamicArray<int> test2(a, 6);
    assert(test2.GetSize() == 6);
    for (int i = 0; i < test2.GetSize(); i++) {
        assert(test2.Get(i) == a[i]);
    }

    DynamicArray<int> test3(test2);
    assert(test3.GetSize() == test2.GetSize());
    for (int i = 0; i < test3.GetSize(); i++) {
        assert(test2.Get(i) == test3.Get(i));
    }
}

void TestDynamicArraySet()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    DynamicArray<int> test(a, 8);
    assert(test.GetSize() == 8);
    for (int i = 0; i < test.GetSize(); i++) {
        assert(test.Get(i) == a[i]);
    }
    test.Set(9, 3);
    assert(test.Get(3) == 9);
    test[3] = 10;
    assert(test[3] == 10);
}

void TestLinkedListConstructors()
{
    int a[] = { 1, 2, 3, 4, 5, 6 };

    LinkedList<int> test1(a, 6);
    assert(test1.GetLength() == 6);
    for (int i = 0; i < test1.GetLength(); i++) {
        assert(test1.Get(i) == a[i]);
    }

    LinkedList<int> test2(test1);
    assert(test2.GetLength() == test1.GetLength());
    for (int i = 0; i < test2.GetLength(); i++) {
        assert(test1.Get(i) == test2.Get(i));
    }
}

void TestLinkedListSubList()
{
    int a[] = { 1, 2, 3, 4, 5, 6 };
    int b[] = { 3, 4, 5 };
    LinkedList<int> test10(a, 6);
    LinkedList<int>* test11 = test10.GetSubList(2, 4);
    for (int i = 0; i < test11->GetLength(); i++) {
        assert(test11->Get(i) == b[i]);
    }
}

void TestLinkedListInput()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    LinkedList<int> test1(a, 8);
    test1.Append(10);
    assert(test1.GetLast() == 10);

    int b[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    LinkedList<int> test3(b, 8);
    test3.Prepend(10);
    assert(test3.GetFirst() == 10);

    int c[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    LinkedList<int> test2(c, 8);
    test2.InsertAt(10, 3);
    assert(test2[3] == 10);
}

void TestLinkedListConcat()
{
    int b[] = { 1, 2, 3 };
    int c[] = { 4, 5, 6 };
    int bc[] = { 1, 2, 3, 4, 5, 6 };
    LinkedList<int> test6(b, 3);
    LinkedList<int> test7(c, 3);
    LinkedList<int>* test8 = test6.Concat(test7);
    assert(test8->GetLength() == test6.GetLength() + test7.GetLength());
    for (int i = 0; i < test8->GetLength(); i++) {
        assert(test8->Get(i) == bc[i]);
    }
}

void TestArraySequenceConstuctors()
{

    int a[] = { 1, 2, 3, 4, 5, 6 };
    MutableArraySequence<int> testM1(6);
    assert(testM1.GetLength() == 6);
    ImmutableArraySequence<int> testIm1(6);
    assert(testIm1.GetLength() == 6);

    MutableArraySequence<int> testM2(a, 6);
    assert(testM2.GetLength() == 6);
    for (int i = 0; i < testM2.GetLength(); i++) {
        assert(testM2.Get(i) == a[i]);
    }
    ImmutableArraySequence<int> testIm2(a, 6);
    assert(testIm2.GetLength() == 6);
    for (int i = 0; i < testIm2.GetLength(); i++) {
        assert(testIm2.Get(i) == a[i]);
    }

    MutableArraySequence<int> testM3(testM2);
    assert(testM3.GetLength() == testM2.GetLength());
    for (int i = 0; i < testM3.GetLength(); i++) {
        assert(testM3.Get(i) == testM2.Get(i));
    }
    ImmutableArraySequence<int> testIm3(testM2);
    assert(testIm3.GetLength() == testM2.GetLength());
    for (int i = 0; i < testIm3.GetLength(); i++) {
        assert(testIm3.Get(i) == testM2.Get(i));
    }
}

void TestArraySequenceInput()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableArraySequence<int> test1(a, 8);
    assert(test1.GetLength() == 8);
    test1.Append(10);
    assert(test1.GetLength() == 9);
    assert(test1.GetLast() == 10);

    int b[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableArraySequence<int> test2(b, 8);
    assert(test2.GetLength() == 8);
    test2.Prepend(10);
    assert(test2.GetLength() == 9);
    assert(test2.GetFirst() == 10);

    int c[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableArraySequence<int> test3(c, 8);
    assert(test3.GetLength() == 8);
    test3.InsertAt(10, 3);
    assert(test3.GetLength() == 9);
    assert(test3.Get(3));
}

void TestArraySequenceConcat()
{
    int b[] = { 1, 2, 3 };
    int c[] = { 4, 5, 6 };
    int bc[] = { 1, 2, 3, 4, 5, 6 };
    MutableArraySequence<int> test1(b, 3);
    MutableArraySequence<int> test2(c, 3);
    MutableArraySequence<int>* test3 = test1.Concat(test2);
    assert(test3->GetLength() == test1.GetLength() + test2.GetLength());
    for (int i = 0; i < test3->GetLength(); i++) {
        assert(test3->Get(i) == bc[i]);
    }
}

void TestArraySequenceGetSubSequence()
{
    int a[] = { 1, 2, 3, 4, 5, 6 };
    int b[] = { 3, 4, 5 };
    MutableArraySequence<int> test1(a, 6);
    MutableArraySequence<int>* test2 = test1.GetSubSequence(3, 5);
    assert(test2->GetLength());
    for (int i = 0; i < test2->GetLength(); i++) {
        assert(test2->Get(i) == b[i]);
    }
}

void TestListSequenceConstructors()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableListSequence<int> testM1(a, 8);
    assert(testM1.GetLength() == 8);
    for (int i = 0; i < testM1.GetLength(); i++) {
        assert(testM1.Get(i) == a[i]);
    }

    ImmutableListSequence<int> testIm1(a, 8);
    assert(testIm1.GetLength() == 8);
    for (int i = 0; i < testIm1.GetLength(); i++) {
        assert(testIm1.Get(i) == a[i]);
    }

    MutableListSequence<int> testM2(testM1);
    assert(testM2.GetLength() == testM1.GetLength());
    for (int i = 0; i < testM2.GetLength(); i++) {
        assert(testM2.Get(i) == testM1.Get(i));
    }

    ImmutableListSequence<int> testIm2(testM1);
    assert(testIm2.GetLength() == testM1.GetLength());
    for (int i = 0; i < testIm2.GetLength(); i++) {
        assert(testIm2.Get(i) == testM1.Get(i));
    }
}

void TestListInput()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableListSequence<int> test1(a, 8);
    assert(test1.GetLength() == 8);
    test1.Append(10);
    assert(test1.GetLength() == 9);
    assert(test1.GetLast() == 10);

    int b[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableListSequence<int> test2(b, 8);
    assert(test2.GetLength() == 8);
    test2.Prepend(10);
    assert(test2.GetLength());
    assert(test2.GetFirst() == 10);

    int c[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    MutableListSequence<int> test3(c, 8);
    assert(test3.GetLength() == 8);
    test3.InsertAt(10, 3);
    assert(test3.GetLength());
    assert(test3.Get(3) == 10);
}

int main(int argc, const char* argv[])
{
    int status = 0;
    std::cout << "0. Run tests for DynamicArray\n";
    std::cout << "1. Run tests for LinkedList\n";
    std::cout << "2. Run tests for ArraySequence\n";
    std::cout << "3. Run tests for LinkedSequence\n";
    std::cout << "4. Остановить программу\n";

    int flag = 1;
    while (flag) {
        std::cin >> status;
        switch (status) {
        case 0:
            TestDynamicArrayConstructors();
            TestDynamicArraySet();
            std::cout << "Tests for DynamicArray passed\n";
            break;
        case 1:
            TestLinkedListConstructors();
            TestLinkedListInput();
            TestLinkedListConcat();
            TestLinkedListSubList();
            std::cout << "Tests for LinkedList passed\n";
            break;
        case 2:
            TestArraySequenceConstuctors();
            TestArraySequenceInput();
            TestArraySequenceConcat();
            TestArraySequenceGetSubSequence();
            std::cout << "Tests for ArraySequence passed\n";
            break;
        case 3:
            TestListSequenceConstructors();
            TestListInput();
            break;
        case 4:
            flag = 0;
            break;
        default:
            std::cout << "Неизвестная команда\n";
        }
        std::cout << "0. Run tests for DynamicArray\n";
        std::cout << "1. Run tests for LinkedList\n";
        std::cout << "2. Run tests for ArraySequence\n";
        std::cout << "3. Run tests for LinkedSequence\n";
        std::cout << "4. Остановить программу\n";
    }
    return 0;
}
