#ifndef STACK_H
#define STACK_H

#include "vector.h"

/* 学生完成：按课本方式从 Vector<T> 公有派生，不增加数据成员。 */
template <typename T>
class Stack : public Vector<T> {
public:
    void push(const T& e);       /* 将 e 压入栈顶 */
    T pop();                    /* 弹出并返回栈顶元素，前提：非空 */
    T& top();                   /* 返回栈顶引用，前提：非空 */
};

template <typename T>
void Stack<T>::push(const T& e) {
    this->insert(e);/* TODO 1：复用基类的按秩插入，把向量末端作为栈顶。 */
}

template <typename T>
T Stack<T>::pop() {
    return this->remove(this->size() -1);/* TODO 2：复用基类的按秩删除；调用者保证栈非空。 */
}

template <typename T>
T& Stack<T>::top() {
   return (*this)[this->size() -1]; /* TODO 3：返回末元素的引用，不删除元素；调用者保证栈非空。 */
}

#endif
