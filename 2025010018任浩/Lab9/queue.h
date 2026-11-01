#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

/* 学生完成：按课本方式从 List<T> 公有派生，不增加数据成员。
 * 约定链表首端为队首、末端为队尾：尾部入队，首部出队，实现先进先出。
 * 节点、计数和内存释放均由 List 管理，Queue 只限制日常操作的位置。
 * size()、empty() 直接继承即可，无需重新实现构造、析构或计数器。
 */
template <typename T>
class Queue : public List<T> {
public:
    void enqueue(const T& e);    /* 从队尾入队 */
    T dequeue();                /* 删除并返回队首元素，前提：非空 */
    T& front();                 /* 返回队首元素的引用，前提：非空 */
};

template <typename T>
void Queue<T>::enqueue(const T& e) {
    /* 复用基类的尾插接口，不自行操作节点指针。
     * const T& 避免传参时复制元素；插入节点时会保存元素的一份副本。
     * 模板派生类访问基类成员时，使用 this-> 或 List<T>:: 限定名称。
     */
    this->insertAsLast(e);
}

template <typename T>
T Queue<T>::dequeue() {
    /* 复用基类的首节点访问和删除接口；调用者保证队列非空。
     * 删除首个真实节点，并把删除接口返回的元素值交给调用者。
     * 节点删除后不再存在，因此这里返回 T，不能返回该节点内元素的引用。
     */
    return this->remove(this->first());
}

template <typename T>
T& Queue<T>::front() {
    /* 返回首个真实节点中 data 的引用；不删除节点。
     * first() 返回节点指针，要通过它访问 data；调用者保证队列非空。
     * 返回 T& 使 Q.front() = e 可以修改队内元素，队列大小保持不变。
     */
    return this->first()->data;
}

#endif