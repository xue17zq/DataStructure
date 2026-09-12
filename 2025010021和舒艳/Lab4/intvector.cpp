#include <iostream>

#define MAX_SIZE 100

class IntVector {
private:
    int data[MAX_SIZE];
    int size;

public:
    // 构造函数
    IntVector() {
        size = 0;
    }

    // 获取当前元素个数
    int getSize() const {
        return size;
    }

    // 判断表是否为空
    bool empty() const {
        return size == 0;
    }

    // 练习1：按位置读取
    bool get(int index, int& value) const {
        if (index < 0 || index >= size) return false;
        value = data[index];
        return true;
    }

    // 练习2：按值查找
    int find(int value) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == value) return i;
        }
        return -1;
    }

    // 练习3：插入
    bool insert(int index, int value) {
        if (size >= MAX_SIZE || index < 0 || index > size) return false;
        for (int i = size; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        size++;
        return true;
    }

    // 练习4：删除
    bool remove(int index, int& removed) {
        if (index < 0 || index >= size) return false;
        removed = data[index];
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
        return true;
    }

    // 练习5：输出
    void print() const {
        for (int i = 0; i < size; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

// main 函数（原样使用，千万不要改动里面一句代码，除了改掉了中文提示）
int main() {
    IntVector v;
    // 这里的 "空表" 改成了 "Empty"
    std::cout << "Empty: size = " << v.getSize() << ", empty = " << v.empty() << std::endl;

    v.insert(0, 10);
    v.insert(1, 20);
    v.insert(2, 30);
    v.insert(3, 40);
    // 这里的 "初始" 改成了 "Initial"
    std::cout << "Initial: ";
    v.print();

    bool ok = v.insert(2, 25);
    // 这里的 "返回" 改成了 "return"
    std::cout << "insert(2, 25) return " << ok << std::endl;
    v.print();

    int removed = 0;
    ok = v.remove(1, removed);
    // 这里的 "返回" 和 "删除的值是" 改成了 "return" 和 "removed value"
    std::cout << "remove(1) return " << ok << ", removed value " << removed << std::endl;
    v.print();

    ok = v.insert(10, 99);
    std::cout << "insert(10, 99) return " << ok << std::endl;
    v.print();

    ok = v.remove(4, removed);
    std::cout << "remove(4) return " << ok << std::endl;
    v.print();

    int value = 0;
    ok = v.get(2, value);
    std::cout << "get(2) return " << ok << ", value = " << value << std::endl;
    ok = v.get(4, value);
    std::cout << "get(4) return " << ok << std::endl;

    std::cout << "find(30) = " << v.find(30) << std::endl;
    std::cout << "find(99) = " << v.find(99) << std::endl;
    // 这里的 "最终" 改成了 "Final"
    std::cout << "Final: size = " << v.getSize() << ", empty = " << v.empty() << std::endl;

    IntVector other;
    other.insert(0, 7);
    std::cout << "other: ";
    other.print();
    // 这里的 "两个对象的 size" 改成了 "Two objects size"
    std::cout << "Two objects size: " << v.getSize() << " " << other.getSize() << std::endl;

    return 0;
}