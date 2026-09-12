#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);


    std::cout << "姓名: 张辉" << std::endl;
    std::cout << "学号: 2025010041" << std::endl;

    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    int value = 41;
    int result = 0;
    getDoubled(value, result);
    std::cout << value << " 的两倍是 " << result << std::endl;

    return 0;
}