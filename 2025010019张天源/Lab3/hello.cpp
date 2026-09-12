#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    std::cout << "姓名：张天源" << std::endl;
    std::cout << "学号：2025010019" << std::endl;

    bool isStudent = true;
    std::cout << "是否为学生：" << isStudent << std::endl;

    int result;
    int value = 19;
    bool ok = getDoubled(value, result);
    std::cout << value << " 的两倍是 " << result << std::endl;

    return 0;
}