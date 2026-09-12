#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    std::cout << "姓名: 陈万" << std::endl;
    std::cout << "学号: 2025010007" << std::endl;

    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    int input = 7;
    int output;
    getDoubled(input, output);
    std::cout << input << " 的两倍是 " << output << std::endl;

    return 0;
}
