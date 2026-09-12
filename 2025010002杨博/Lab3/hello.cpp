#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    std::cout << "姓名: 杨博" << std::endl;
    std::cout << "学号: 2025010002" << std::endl;

    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    int id = 2;
    int doubled = 0;
    getDoubled(id, doubled);
    std::cout << id << " 的两倍是 " << doubled << std::endl;

    return 0;
}
