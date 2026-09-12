#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    std::cout << "姓名: " << "龙锦程" << std::endl;
    std::cout << "学号: " << "2025010026" << std::endl;

    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    int result = 0;
    getDoubled(26, result);
    std::cout << "26 的两倍是 " << result << std::endl;

    return 0;
}
