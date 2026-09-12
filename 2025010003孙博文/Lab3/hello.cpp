#include <iostream>
#include <windows.h>   // 新增，只Windows可用

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    SetConsoleOutputCP(65001);  // 强制控制台使用UTF-8，解决中文乱码！

    // 1.输出姓名学号
    std::cout << "姓名: 孙博文" << std::endl;
    std::cout << "学号: 2025010003" << std::endl;

    // 2.bool变量
    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    // 3.调用引用函数
    int value = 3;
    int output;
    bool ret = getDoubled(value, output);
    if(ret) {
        std::cout << value << " 的两倍是 " << output << std::endl;
    }

    return 0;
}
