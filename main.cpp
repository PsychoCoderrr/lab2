#include "ArraySequence.h"
#include "ListSequence.h"
#include "tests.h"
#include <iostream>

int main()
{
    TestMapFunc();
    TestReduceFunc();
    TestWhereFunc();
    int status = 0;
    std::cout << "0. Run tests for DynamicArray\n";
    std::cout << "1. Run tests for LinkedList\n";
    std::cout << "2. Run tests for ArraySequence\n";
    std::cout << "3. Run tests for ListSequence\n";
    std::cout << "4. Run tests for MapWhereReduce functions\n";
    std::cout << "5. Stop programm\n";

    int flag = 1;
    while (flag)
    {
        std::cin >> status;
        switch (status)
        {
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
            TestListSequenceConcat();
            TestListSequenceGetSubSequence();
            std::cout << "Tests for ArraySequence passed\n";
            break;
        case 4:
            TestMapFunc();
            TestReduceFunc();
            TestWhereFunc();
            std::cout << "Tests for MapWhereReduce functions passed\n";
            break;
        case 5:
            flag = 0;
            break;
        default:
            std::cout << "Неизвестная команда\n";
        }
        std::cout << "0. Run tests for DynamicArray\n";
        std::cout << "1. Run tests for LinkedList\n";
        std::cout << "2. Run tests for ArraySequence\n";
        std::cout << "3. Run tests for ListSequence\n";
        std::cout << "4. Run tests for MapWhereReduce functions\n";
        std::cout << "5. Stop programm\n";
    }
    return 0;
}
