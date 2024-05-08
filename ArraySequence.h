#pragma once
#include "DynamicArray.h"
#include "Sequence.h"
#include <iostream>

template <typename T> class ArraySequence : public Sequence<T>
{
  protected:
    virtual ArraySequence<T> *GetInstance() = 0;
    DynamicArray<T> *array;

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
    ArraySequence(T *items, int count)
    {
        this->array = new DynamicArray<T>(items, count);
    }
    ArraySequence(Sequence<T> &seq)
    {
        this->array = new DynamicArray<T>(seq.GetLength());
        for (int i = 0; i < seq.GetLength(); i++)
        {
            this->array->Set(seq.Get(i), i);
            this->array->Resize(this->GetLength() - 1);
        }
    }
    ArraySequence(ArraySequence<T> &seq)
    {
        this->array = new DynamicArray<T>(seq.GetLength());
        for (int i = 0; i < seq.GetLength(); i++)
        {
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
    ArraySequence<T> *Append(const T &item) override // adding an element to the end of the array
    {
        ArraySequence<T> *intermediateResult = GetInstance();
        intermediateResult->array->Set(item, array->GetSize());
        return intermediateResult;
    }
    ArraySequence<T> *Prepend(const T &item) override // adding an element to the begining of the array
    {
        ArraySequence<T> *intermediateResult = GetInstance();
        intermediateResult->array->Set(item, 0);
        return intermediateResult;
    }
    ArraySequence<T> *InsertAt(const T &item, int index) override
    {
        ArraySequence<T> *intermediateResult = GetInstance();
        intermediateResult->array->Set(item, index);
        return intermediateResult;
    }

    T &operator[](int index) override
    {
        return (*(GetInstance()->array))[index];
    }

    virtual ~ArraySequence() override
    {
        delete array;
    }
};

template <typename T> class MutableArraySequence : public ArraySequence<T>
{
  private:
    ArraySequence<T> *GetInstance() override
    {
        return static_cast<ArraySequence<T> *>(this);
    }

  public:
    using ArraySequence<T>::ArraySequence;

    MutableArraySequence<T> *Concat(Sequence<T> &list) override
    {
        MutableArraySequence<T> *intermediateResult = new MutableArraySequence<T>();
        for (int i = 0; i < this->GetLength(); i++)
        {
            intermediateResult->Append(this->Get(i));
        }
        for (int i = 0; i < list.GetLength(); i++)
        {
            intermediateResult->Append(list.Get(i));
        }
        return intermediateResult;
    }

    MutableArraySequence<T> *GetSubSequence(int startIndex, int endIndex) override
    {
        MutableArraySequence<T> *intermediateResult = new MutableArraySequence();
        for (int i = 0; i < endIndex - startIndex + 1; i++)
        {
            intermediateResult->Append(this->Get(startIndex + i - 1));
        }
        return intermediateResult;
    }
};

template <typename T> class ImmutableArraySequence : public ArraySequence<T>
{
  private:
    ArraySequence<T> *GetInstance() override
    {
        ImmutableArraySequence<T> *result = new ImmutableArraySequence(this->GetLength());
        for (int i = 0; i < this->GetLength(); i++)
        {
            result->array[i] = this->array[i];
        }
        return result;
    }

  public:
    using ArraySequence<T>::ArraySequence;

    ImmutableArraySequence<T> *Concat(Sequence<T> &elements) override
    {
        MutableArraySequence<T> *intermediate = new MutableArraySequence<T>();
        for (int i = 0; i < this->GetLength(); i++)
        {
            intermediate->Append(this->Get(i));
        }
        for (int i = 0; i < elements.GetLength(); i++)
        {
            intermediate->Append(elements.Get(i));
        }
        ImmutableArraySequence<T> *result = new ImmutableArraySequence<T>();
        for (int i = 0; i < intermediate->GetLength(); i++)
        {
            result->Append(intermediate->Get(i));
        }
        delete intermediate;
        return result;
    }

    ImmutableArraySequence<T> *GetSubSequence(int startIndex, int endIndex) override
    {
        if (startIndex < 0 || endIndex < startIndex || endIndex >= this->GetLength())
        {
            throw std::invalid_argument("invalid argument");
        }
        ImmutableArraySequence<T> *result =
            new ImmutableArraySequence<T>(&(*(this->array))[startIndex], endIndex - startIndex + 1);
        return result;
    }
};
