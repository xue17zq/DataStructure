/* ==========================================================================
 * Lab7 测试程序：课本版 List<T>（listNode.h + list.h）的统一测试
 *
 * 编译运行：clang++ -std=c++17 -Wall -Wextra main.cpp -o lab7_list && ./lab7_list
 * ========================================================================== */

#include <iostream>
#include "list.h"

/* 第一部分：List<int>，测试顺序与 Lab6 的 linkedlist.c 一致 */
static void testIntList() {
    List<int> list;

    std::cout << "after init: ";
    list.print();
    std::cout << "empty = " << list.empty() << ", size = " << list.size() << "\n\n";

    /* 1. 依次在表尾插入五个样例元素。C 版：listPushBack(&list, samples[i]) */
    int samples[] = {18, -1, 42, 18, 65};
    for (int i = 0; i < 5; i++) {
        list.insertAsLast(samples[i]);
    }
    std::cout << "after insertAsLast 18, -1, 42, 18, 65: ";
    list.print();
    std::cout << '\n';

    /* 2. 按秩插入。C 版：listInsert(&list, 2, 25) */
    bool ok = list.insert(2, 25);
    std::cout << "insert(2, 25): ok = " << ok << ", ";
    list.print();

    /* 3. 按秩删除，被删的值由引用参数带回。C 版：listRemove(&list, 1, &removed) */
    int removed = 777;
    ok = list.remove(1, removed);
    std::cout << "remove(1): ok = " << ok << ", removed = " << removed << ", ";
    list.print();

    /* 4. 在表头插入。C 版：listPushFront(&list, 7) */
    list.insertAsFirst(7);
    std::cout << "insertAsFirst(7): ";
    list.print();
    std::cout << '\n';

    /* 5. 按秩读取：合法的秩与越界的秩。C 版：listGet(&list, 0, &value) */
    int value = 777;
    ok = list.get(0, value);
    std::cout << "get(0): ok = " << ok << ", value = " << value << '\n';

    value = 777;
    ok = list.get(6, value);            /* 此刻 size = 6，秩 6 已越界 */
    std::cout << "get(6): ok = " << ok << ", value = " << value << "\n\n";

    /* 6. 按值查找：课本返回的是"位置"，用 rankOf 折算成秩 */
    std::cout << "rankOf(find(42)) = " << list.rankOf(list.find(42)) << '\n';
    std::cout << "rankOf(find(18)) = " << list.rankOf(list.find(18)) << '\n';
    std::cout << "rankOf(find(99)) = " << list.rankOf(list.find(99)) << "\n\n";

    /* 7. 拿着位置直接插到前后，不用先算秩：单链表做不到这件事 */
    ListNode<int>* p = list.first();
    list.insertB(p, 2000);
    list.insertA(p, 1000);
    std::cout << "insertB(first(), 2000) and insertA(first(), 1000): ";
    list.print();
    std::cout << '\n';

    /* 8. 课本的按秩访问写法，以及双向链表才有的反向遍历 */
    std::cout << "list[2]->data = " << list[2]->data << '\n';
    list.printReverse();
    std::cout << '\n';

    /* 9. 非法位置：负秩、超过 size 的秩、删除空表 */
    ok = list.insert(-1, 100);
    std::cout << "insert(-1, 100): ok = " << ok << '\n';
    ok = list.insert(100, 100);
    std::cout << "insert(100, 100): ok = " << ok << '\n';

    List<int> empty;
    ok = empty.remove(0, removed);
    std::cout << "empty remove(0): ok = " << ok << "\n\n";

    /* 10. 从表头删掉三个，观察 size 回落；剩下的交给析构函数 */
    std::cout << "remove one by one: ";
    for (int i = 0; i < 3; i++) {
        list.remove(0, removed);
        std::cout << removed << "(size = " << list.size() << ") ";
    }
    std::cout << "\nbefore leaving testIntList: ";
    list.print();
    std::cout << "no listDestroy written by hand: both objects clean themselves up ->\n";
}

/* 第二部分：List<double>，同一个类换一种数据类型 */
static void testDoubleList() {
    List<double> nums;

    double values[] = {1.5, 2.25, -3.75};
    for (int i = 0; i < 3; i++) {
        nums.insertAsLast(values[i]);
    }
    std::cout << "List<double>: ";
    nums.print();

    nums.insert(1, 0.5);
    std::cout << "after insert(1, 0.5): ";
    nums.print();

    double firstValue = 0.0;
    nums.get(0, firstValue);
    std::cout << "get(0) = " << firstValue << ", size = " << nums.size() << '\n';
}

int main() {
    std::cout << "sizeof(ListNode<int>) = " << sizeof(ListNode<int>) << '\n';
    std::cout << "sizeof(List<int>) = " << sizeof(List<int>) << "\n\n";

    testIntList();
    std::cout << '\n';
    testDoubleList();
    return 0;
}
