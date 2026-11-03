#ifndef VECTOR_H
#define VECTOR_H

/* 按邓俊辉《数据结构（C++ 语言版）》Vector 的命名与组织方式编写的教学子集。
 * 教师提供：本实验不要求修改。仅用于 int、char 等可默认构造、可复制的类型。
 * 保留按秩访问、查找、插入、删除与倍增扩容；不含排序、缩容等操作。
 * 调用者须保证秩和区间合法；接口约定见 Lab8.md 的 2.2。
 */
typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
class Vector {
private:
    Rank _size;                 /* 有效元素占据 [0, _size) */
    int _capacity;              /* 数组总容量，始终不小于 _size */
    T* _elem;                   /* 动态数组的首地址 */

    void expand();              /* 空间不足时，容量翻倍 */

public:
    explicit Vector(int c = DEFAULT_CAPACITY);
    ~Vector() { delete[] _elem; }

    /* 本实验不练习深拷贝；禁止默认浅拷贝，避免两个对象重复释放同一数组。 */
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    Rank size() const { return _size; }
    bool empty() const { return _size == 0; }
    int capacity() const { return _capacity; }

    T& operator[](Rank r);
    const T& operator[](Rank r) const;
    Rank find(const T& e) const;
    Rank insert(Rank r, const T& e);
    Rank insert(const T& e) { return insert(_size, e); }
    T remove(Rank r);
    int remove(Rank lo, Rank hi);
};

template <typename T>
Vector<T>::Vector(int c)
    : _size(0), _capacity(c > 0 ? c : DEFAULT_CAPACITY),
      _elem(new T[_capacity]) {}

template <typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;
    int newCapacity = _capacity * 2;
    T* newElem = new T[newCapacity];
    for (Rank i = 0; i < _size; ++i)
        newElem[i] = _elem[i];
    delete[] _elem;
    _elem = newElem;
    _capacity = newCapacity;
}

template <typename T>
T& Vector<T>::operator[](Rank r) {
    return _elem[r];
}

template <typename T>
const T& Vector<T>::operator[](Rank r) const {
    return _elem[r];
}

template <typename T>
Rank Vector<T>::find(const T& e) const {
    for (Rank r = _size; r > 0; )
        if (_elem[--r] == e) return r;  /* 从后向前，返回最后一次出现的秩 */
    return -1;
}

template <typename T>
Rank Vector<T>::insert(Rank r, const T& e) {
    T value = e;                /* e 即使引用本表元素，扩容或移动后仍可使用 */
    expand();
    for (Rank i = _size; i > r; --i)
        _elem[i] = _elem[i - 1];
    _elem[r] = value;
    ++_size;
    return r;
}

template <typename T>
T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    int n = hi - lo;
    while (hi < _size)
        _elem[lo++] = _elem[hi++];
    _size = lo;                 /* 不缩容；[0, _size) 以外不再是有效元素 */
    return n;
}

#endif
