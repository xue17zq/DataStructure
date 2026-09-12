#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    // 1.输出姓名学号，改成你自己真实的
    std::cout << "姓名: 范文韬" << std::endl;
    std::cout << "学号: 2025145025" << std::endl;

    //2.bool变量
    bool isStudent = true;
    std::cout << "是否为学生: " << isStudent << std::endl;

    //3.调用引用函数：学号最后两位：25
    int res;
    getDoubled(25, res);
    std::cout << "25 的两倍是 " << res << std::endl;

    return 0;
}

// Created by fwt on 2026/9/9.
//
//
// Created by fwt on 2026/9/9.
//
