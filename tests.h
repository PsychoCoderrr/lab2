#pragma once
#include "ArraySequence.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ListSequence.h"
#include "MapWhereReduce.h"
#include "Sequence.h"
#include "assert.h"
#include "iostream"

void TestDynamicArrayConstructors()
{
    int a[] = {1, 2, 3, 4, 5, 6};

    DynamicArray<int> test1(6);
    assert(test1.GetSize() == 6);

    DynamicArray<int> test2(a, 6);
    assert(test2.GetSize() == 6);
    for (int i = 0; i < test2.GetSize(); i++)
    {
        assert(test2.Get(i) == a[i]);
    }

    DynamicArray<int> test3(test2);
    assert(test3.GetSize() == test2.GetSize());
    for (int i = 0; i < test3.GetSize(); i++)
    {
        assert(test2.Get(i) == test3.Get(i));
    }
}

void TestDynamicArraySet()
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

void TestLinkedListConstructors()
{
    int a[] = {1, 2, 3, 4, 5, 6};

    LinkedList<int> test1(a, 6);
    assert(test1.GetLength() == 6);
    for (int i = 0; i < test1.GetLength(); i++)
    {
        assert(test1.Get(i) == a[i]);
    }

    LinkedList<int> test2(test1);
    assert(test2.GetLength() == test1.GetLength());
    for (int i = 0; i < test2.GetLength(); i++)
    {
        assert(test1.Get(i) == test2.Get(i));
    }
}

void TestLinkedListSubList()
{
    int a[] = {1, 2, 3, 4, 5, 6};
    int b[] = {3, 4, 5};
    LinkedList<int> test10(a, 6);
    LinkedList<int> *test11 = test10.GetSubList(2, 4);
    for (int i = 0; i < test11->GetLength(); i++)
    {
        assert(test11->Get(i) == b[i]);
    }
}

void TestLinkedListInput()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    LinkedList<int> test1(a, 8);
    test1.Append(10);
    assert(test1.GetLast() == 10);

    int b[] = {1, 2, 3, 4, 5, 6, 7, 8};
    LinkedList<int> test3(b, 8);
    test3.Prepend(10);
    assert(test3.GetFirst() == 10);

    int c[] = {1, 2, 3, 4, 5, 6, 7, 8};
    LinkedList<int> test2(c, 8);
    test2.InsertAt(10, 3);
    assert(test2[3] == 10);
}

void TestLinkedListConcat()
{
    int b[] = {1, 2, 3};
    int c[] = {4, 5, 6};
    int bc[] = {1, 2, 3, 4, 5, 6};
    LinkedList<int> test6(b, 3);
    LinkedList<int> test7(c, 3);
    LinkedList<int> *test8 = test6.Concat(test7);
    assert(test8->GetLength() == test6.GetLength() + test7.GetLength());
    for (int i = 0; i < test8->GetLength(); i++)
    {
        assert(test8->Get(i) == bc[i]);
    }
}

void TestArraySequenceConstuctors()
{

    int a[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> testM1(6);
    assert(testM1.GetLength() == 6);
    ImmutableArraySequence<int> testIm1(6);
    assert(testIm1.GetLength() == 6);

    MutableArraySequence<int> testM2(a, 6);
    assert(testM2.GetLength() == 6);
    for (int i = 0; i < testM2.GetLength(); i++)
    {
        assert(testM2.Get(i) == a[i]);
    }
    ImmutableArraySequence<int> testIm2(a, 6);
    assert(testIm2.GetLength() == 6);
    for (int i = 0; i < testIm2.GetLength(); i++)
    {
        assert(testIm2.Get(i) == a[i]);
    }

    MutableArraySequence<int> testM3(testM2);
    assert(testM3.GetLength() == testM2.GetLength());
    for (int i = 0; i < testM3.GetLength(); i++)
    {
        assert(testM3.Get(i) == testM2.Get(i));
    }
    ImmutableArraySequence<int> testIm3(testM2);
    assert(testIm3.GetLength() == testM2.GetLength());
    for (int i = 0; i < testIm3.GetLength(); i++)
    {
        assert(testIm3.Get(i) == testM2.Get(i));
    }
}

void TestArraySequenceInput()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableArraySequence<int> test1(a, 8);
    assert(test1.GetLength() == 8);
    test1.Append(10);
    assert(test1.GetLength() == 9);
    assert(test1.GetLast() == 10);

    int b[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableArraySequence<int> test2(b, 8);
    assert(test2.GetLength() == 8);
    test2.Prepend(10);
    assert(test2.GetLength() == 9);
    assert(test2.GetFirst() == 10);

    int c[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableArraySequence<int> test3(c, 8);
    assert(test3.GetLength() == 8);
    test3.InsertAt(10, 3);
    assert(test3.GetLength() == 9);
    assert(test3.Get(3));
}

void TestArraySequenceConcat()
{
    int b[] = {1, 2, 3};
    int c[] = {4, 5, 6};
    int bc[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> test1(b, 3);
    MutableArraySequence<int> test2(c, 3);
    MutableArraySequence<int> *test3 = test1.Concat(test2);
    assert(test3->GetLength() == test1.GetLength() + test2.GetLength());
    for (int i = 0; i < test3->GetLength(); i++)
    {
        assert(test3->Get(i) == bc[i]);
    }
}

void TestArraySequenceGetSubSequence()
{
    int a[] = {1, 2, 3, 4, 5, 6};
    int b[] = {3, 4, 5};
    MutableArraySequence<int> test1(a, 6);
    MutableArraySequence<int> *test2 = test1.GetSubSequence(3, 5);
    assert(test2->GetLength());
    for (int i = 0; i < test2->GetLength(); i++)
    {
        assert(test2->Get(i) == b[i]);
    }
}

void TestListSequenceConstructors()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableListSequence<int> testM1(a, 8);
    assert(testM1.GetLength() == 8);
    for (int i = 0; i < testM1.GetLength(); i++)
    {
        assert(testM1.Get(i) == a[i]);
    }

    ImmutableListSequence<int> testIm1(a, 8);
    assert(testIm1.GetLength() == 8);
    for (int i = 0; i < testIm1.GetLength(); i++)
    {
        assert(testIm1.Get(i) == a[i]);
    }

    MutableListSequence<int> testM2(testM1);
    assert(testM2.GetLength() == testM1.GetLength());
    for (int i = 0; i < testM2.GetLength(); i++)
    {
        assert(testM2.Get(i) == testM1.Get(i));
    }

    ImmutableListSequence<int> testIm2(testM1);
    assert(testIm2.GetLength() == testM1.GetLength());
    for (int i = 0; i < testIm2.GetLength(); i++)
    {
        assert(testIm2.Get(i) == testM1.Get(i));
    }
}

void TestListInput()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableListSequence<int> test1(a, 8);
    assert(test1.GetLength() == 8);
    test1.Append(10);
    assert(test1.GetLength() == 9);
    assert(test1.GetLast() == 10);

    int b[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableListSequence<int> test2(b, 8);
    assert(test2.GetLength() == 8);
    test2.Prepend(10);
    assert(test2.GetLength());
    assert(test2.GetFirst() == 10);

    int c[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableListSequence<int> test3(c, 8);
    assert(test3.GetLength() == 8);
    test3.InsertAt(10, 3);
    assert(test3.GetLength());
    assert(test3.Get(3) == 10);
}

void TestListSequenceConcat()
{
    int b[] = {1, 2, 3};
    int c[] = {4, 5, 6};
    int bc[] = {1, 2, 3, 4, 5, 6};
    MutableListSequence<int> test1(b, 3);
    MutableListSequence<int> test2(c, 3);
    MutableListSequence<int> *result = test1.Concat(test2);
    assert(result->GetLength() == 6);
    for (int i = 0; i < result->GetLength(); i++)
    {
        assert(result->Get(i) == bc[i]);
    }
}

void TestListSequenceGetSubSequence()
{
    int a[] = {1, 2, 3, 4, 5, 6};
    int b[] = {3, 4, 5};
    MutableListSequence<int> test1(a, 6);
    MutableListSequence<int> *result = test1.GetSubSequence(3, 5);
    assert(result->GetLength() == 3);
    for (int i = 0; i < result->GetLength(); i++)
    {
        assert(result->Get(i) == b[i]);
    }
}

int TestMapFunc(int i)
{
    return i * i;
}

int TestReduceFunc(int a, int b)
{
    return a + b;
}

bool isEven(int i)
{
    if (i % 2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TestMapFunc()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int b[] = {1, 4, 9, 16, 25, 36, 49, 64};
    MutableListSequence<int> test1(a, 8);
    MutableArraySequence<int> testRes = map(test1, &TestMapFunc);
    assert(testRes.GetLength() == 8);
    for (int i = 0; i < testRes.GetLength(); i++)
    {
        assert(testRes.Get(i) == b[i]);
    }
}

void TestReduceFunc()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    MutableListSequence<int> test1(a, 8);
    int result = reduce(test1, &TestReduceFunc, 0);
    assert(result == 36);
}

void TestWhereFunc()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int b[] = {2, 4, 6, 8};
    MutableListSequence<int> test1(a, 8);
    MutableArraySequence<int> testRes = where(test1, &isEven);
    assert(testRes.GetLength() == 4);
    for (int i = 0; i < testRes.GetLength(); i++)
    {
        assert(testRes.Get(i) == b[i]);
    }
}
