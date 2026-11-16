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
    this->insert(this->size(), e);   /* 末端插入位置就是 size() */
}

template <typename T>
T Stack<T>::pop() {
    return this->remove(this->size() - 1);   /* 末元素的秩是 size() - 1 */
}

template <typename T>
T& Stack<T>::top() {
    return this->operator[](this->size() - 1);   /* 返回引用，不弹出 */
}

#endif
