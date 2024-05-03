#pragma once
#include<iostream>

template <typename T> struct TheNode
{
  TheNode (T value) : data (value), next (nullptr) {}
  T data;
  TheNode<T> *next = nullptr;
};

template <typename T> class LinkedList
{
private:
  int size;
  TheNode<T> *head;

public:
  LinkedList (T *items, int count) : size (0), head (nullptr)
  {
    if (count < 0)
      {
        throw std::invalid_argument ("invalid argument");
      }
    for (int i = 0; i < count; i++)
      {
        Append (items[i]);
      }
  }

  LinkedList () : size (0), head (nullptr) {}

  LinkedList (const LinkedList<T> &list) : LinkedList ()
  {
    TheNode<T> *intermediate = list.head;
    for (int i = 0; i < list.size; i++)
      {
        Append (intermediate->data);
        intermediate = intermediate->next;
      }
    this->size = list.size;
  }

  virtual ~LinkedList ()
  {
    if (head != nullptr)
      {
        TheNode<T> *current = head;
        TheNode<T> *buf;
        for (int i = 0; i < this->size; i++)
          {
            buf = current->next;
            delete current;
            current = buf;
          }
        delete current;
      }
  }

  T
  GetFirst ()
  {
    if (head == nullptr)
      {
        throw std::invalid_argument ("index out of range");
      }
    return head->data;
  }

  T
  GetLast ()
  {
    if (head == nullptr)
      {
        throw std::invalid_argument ("index out of range");
      }
    TheNode<T> *intermediate = head;
    for (int i = 0; i < this->size - 1; i++)
      {
        intermediate = intermediate->next;
      }
    return intermediate->data;
  }

  T
  Get (int index)
  {
    if (index < 0 || index >= this->size)
      {
        throw std::invalid_argument ("index out of range");
      }
    TheNode<T> *intermediate = head;
    for (int i = 0; i < index; i++)
      {
        intermediate = intermediate->next;
      }
    return intermediate->data;
  }

  LinkedList<T> *
  GetSubList (int startIndex, int endIndex)
  {
    if (startIndex < 0 || endIndex < 0 || startIndex > endIndex
        || endIndex >= size)
      {
        throw std::invalid_argument ("index out of range");
      }
    LinkedList<T> *ResultList = new LinkedList ();
    TheNode<T> *intermediate = head;
    for (int i = 0; i < startIndex; i++)
      {
        intermediate = intermediate->next;
      }
    for (int i = startIndex; i < endIndex; i++)
      {
        ResultList->Append (intermediate->data);
        intermediate = intermediate->next;
      }
    return ResultList;
  }

  int
  GetLength ()
  {
    return this->size;
  }

  void
  Append (const T &item)
  {
    if (head == nullptr)
      {
        TheNode<T> *buf = new TheNode<T> (item);
        this->head = buf;
        size++;
      }
    else
      {
        TheNode<T> *buf = new TheNode<T> (item);
        TheNode<T> *intermediate = head;
        for (int i = 0; i < this->size - 1; i++)
          {
            intermediate = intermediate->next;
          }
        intermediate->next = buf;
        this->size++;
      }
  }

  void
  Prepend (const T &item)
  {
    TheNode<T> *buf = new TheNode<T> (item);
    buf->next = head;
    head = buf;
    this->size++;
  }

  void
  InsertAt (T item, int index)
  {
    if (index < 0 || index >= this->size)
      {
        throw std::invalid_argument ("index out of range");
      }
    TheNode<T> *newElem = new TheNode<T> (item);
    TheNode<T> *intermediate = head;
    for (int i = 0; i < index - 1; i++)
      {
        intermediate = intermediate->next;
      }
    newElem->next = intermediate->next;
    intermediate->next = newElem;
    this->size++;
  }

  LinkedList<T> *
  Concat (LinkedList<T> &list)
  {
    LinkedList<T> *resultList = new LinkedList<T> ();
    for (int i = 0; i < this->size; i++)
      {
        resultList->Append (this->Get (i));
      }
    TheNode<T> *intermediate = list.head;
    for (int i = 0; i < list.size; i++)
      {
        resultList->Append (intermediate->data);
        intermediate = intermediate->next;
      }
    return resultList;
  }

  T &
  operator[] (int index)
  {
    if (index > this->size || index < 0)
      {
        throw std::invalid_argument ("index out of range");
      }
    TheNode<T> *intermediate = head;
    for (int i = 0; i < index; i++)
      {
        intermediate = intermediate->next;
      }
    return intermediate->data;
  }
};
