#pragma once
#include "iostream"
template <typename T> class DynamicArray
{
  private:
    T *elements = nullptr;
    int capacity;
    int size;

    void Reserve(int newCapacity)
    {
        if (newCapacity <= capacity)
        {
            capacity = newCapacity;
            return;
        }

        T *newElements = new T[newCapacity];
        for (int i = 0; i < size; i++)
        {
            newElements[i] = elements[i];
        }
        delete elements;
        elements = newElements;
        capacity = newCapacity;
    }

  public:
    DynamicArray() : size(0), capacity(0), elements(nullptr)
    {
    }

    DynamicArray(int size) : size(size), capacity(size + 1), elements(nullptr)
    {
        if (size <= 0)
        {
            throw std::out_of_range("invalid argument for size");
        }
        else
        {
            elements = new T[capacity];
        }
    }

    DynamicArray(T *items, int count) : DynamicArray(count)
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

    DynamicArray(const DynamicArray<T> &dynamicArray) : DynamicArray<T>(dynamicArray.elements, dynamicArray.size)
    {
    }

    ~DynamicArray()
    {
        delete[] elements;
    }

    T Get(int index)
    {
        if (index < 0 || index > size)
        {
            throw std::out_of_range("invalid argument");
        }
        return elements[index];
    }

    int GetSize() const
    {
        return size;
    }

    void Set(const T &value, int index)
    {
        if (index < 0 || index > size)
        {
            throw std::out_of_range("invalid argument");
        }
        Resize(size + 1);
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
        if (newSize == 0)
        {
            delete[] elements;
            elements = nullptr;
        }
        if (newSize >= capacity)
        {
            Reserve(newSize * 2);
        }
        size = newSize;
    }

    T &operator[](int index)
    {
        if (size <= index)
        {
            throw std::out_of_range("Invalid argument");
        }
        return elements[index];
    }

    bool operator==(const DynamicArray<T> &array)
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
