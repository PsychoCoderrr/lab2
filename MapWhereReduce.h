#pragma once
#include "ArraySequence.h"
#include "ListSequence.h"

template <typename InputType, typename OutputType>
Sequence<OutputType> &map(const Sequence<InputType> &seq, OutputType (*func)(InputType))
{
    MutableArraySequence<OutputType> *result = new MutableArraySequence<OutputType>(seq.GetLength());
    for (int i = 0; i < seq.GetLength(); i++)
    {
        (*result)[i] = (*func)(seq.Get(i));
    }
    return *result;
}

template <typename T> T reduce(const Sequence<T> &seq, T (*func)(T, T), T start)
{
    for (int i = 0; i < seq.GetLength(); i++)
    {
        start = (*func)(start, seq.Get(i));
    }
    return start;
}

template <typename T, typename... Types> Sequence<T> &where(const Sequence<T> &seq, bool (*func)(T, Types &...))
{
    MutableArraySequence<T> *result = new MutableArraySequence<T>();
    for (int i = 0; i < seq.GetLength(); i++)
    {
        if ((*func)(seq.Get(i)) == true)
        {
            result->Append(seq.Get(i));
        }
    }
    return *result;
}
