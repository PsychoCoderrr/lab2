#pragma once
#include"Sequence.h"
#include"LinkedList.h"

template <typename T> class ListSequence : public Sequence<T>
{
protected:
  virtual ListSequence<T> *Instance () = 0;
  LinkedList<T> *list;

  ListSequence<T> *
  appendWithoutInstance (const T &item)
  {
    ListSequence<T> *result = this;
    result->list->Append (item);
    return result;
  }

public:
  ListSequence () { this->list = new LinkedList<T> (); }
  ListSequence (T *items, int count)
  {
    this->list = new LinkedList<T> (items, count);
  }
  ListSequence (const Sequence<T> &seq)
  {
    this->list = new LinkedList<T> ();
    for (int i = 0; i < seq.GetLength (); i++)
      {
        appendWithoutInstance (seq.Get (i));
      }
  }

  ListSequence (const ListSequence<T> &seq)
  {
    this->list = new LinkedList<T> ();
    for (int i = 0; i < seq.GetLength (); i++)
      {
        appendWithoutInstance (seq.Get (i));
      }
  }

  T
  GetFirst () override
  {
    return this->list->GetFirst ();
  }

  T
  GetLast () override
  {
    return this->list->GetLast ();
  }

  T
  Get (int index) const override
  {
    return this->list->Get (index);
  }

  int
  GetLength () const override
  {
    return this->list->GetLength ();
  }

  ListSequence<T> *
  Append (const T &item) override
  {
    ListSequence<T> *result = Instance ();
    result->list->Append (item);
    return result;
  }

  ListSequence<T> *
  Prepend (const T &item) override
  {
    ListSequence<T> *result = Instance ();
    result->list->Prepend (item);
    return result;
  }

  ListSequence<T> *
  InsertAt (const T &item, int index) override
  {
    ListSequence<T> *result = Instance ();
    result->list->InsertAt (item, index);
    return result;
  }

  T &
  operator[] (int index) override
  {
    return (*(this->list))[index];
  }

  ~ListSequence () { delete list; }
};

template <typename T> class MutableListSequence : public ListSequence<T>
{
private:
  ListSequence<T> *
  Instance () override
  {
    return static_cast<ListSequence<T> *> (this);
  }

public:
  using ListSequence<T>::ListSequence;

  MutableListSequence<T> *
  Concat (Sequence<T> &elements) override
  {
    MutableListSequence<T> *result
        = new MutableListSequence<T> (static_cast<Sequence<T> &> (*this));
    for (int i = 0; i < elements.GetLength (); i++)
      {
        result->Append (elements.Get (i));
      }
    return result;
  }
  MutableListSequence<T> *
  GetSubSequence (int startIndex, int endIndex) override
  {
    if (startIndex < 0 || endIndex < startIndex
        || endIndex >= this->GetLength ())
      {
        throw std::invalid_argument ("invalid argument");
      }
    T *intermediate = new T[endIndex - startIndex + 1];
    for (int i = 0; i < endIndex - startIndex + 1; i++)
      {
        intermediate[i] = this->Get (startIndex + i - 1);
      }
    MutableListSequence<T> *result
        = new MutableListSequence<T> (intermediate, endIndex - startIndex + 1);
    delete[] intermediate;
    return result;
  }
};

template <typename T> class ImmutableListSequence : public ListSequence<T>
{
private:
  ListSequence<T> *
  Instance () override
  {
    ImmutableListSequence<T> *result = new ImmutableListSequence<T> (*this);
    return result;
  }

public:
  using ListSequence<T>::ListSequence;

  ImmutableListSequence<T> *
  Concat (Sequence<T> &elements) override
  {
    MutableListSequence<T> *intermediate
        = new MutableListSequence<T> (static_cast<Sequence<T> &> (*this));
    for (int i = 0; i < elements.GetLength (); i++)
      {
        intermediate->Append (elements.Get (i));
      }
    ImmutableListSequence<T> *result
        = new ImmutableListSequence<T> (*intermediate);
    delete intermediate;
    return result;
  }

  ImmutableListSequence<T> *
  GetSubSequence (int startIndex, int endIndex) override
  {
    if (startIndex < 0 || endIndex < startIndex
        || endIndex >= this->GetLength ())
      {
        throw std::invalid_argument ("invalid argument");
      }
    T *intermediate = new T[endIndex - startIndex + 1];
    for (int i = 0; i < endIndex - startIndex + 1; i++)
      {
        intermediate[i] = this->Get (startIndex + i);
      }
    ImmutableListSequence<T> *result = new ImmutableListSequence<T> (
        intermediate, endIndex - startIndex + 1);
    delete[] intermediate;
    return result;
  }
};
