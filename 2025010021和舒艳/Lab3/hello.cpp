#include <iostream>

bool getDoubled(int value, int& result) {
    result = value * 2;
    return true;
}

int main() {
    // 把中文改成英文，这样就不会乱码了
    std::cout << "Name: He Shuyan" << std::endl;  // 换成你的名字拼音
    std::cout << "ID: 2025010021" << std::endl;   // 换成你的真实学号

    bool isStudent = true;
    std::cout << "Is Student: " << isStudent << std::endl;

    int value = 21; // 这里填学号最后两位，你是 21 就填 21
    int result = 0;
    if (getDoubled(value, result)) {
        std::cout << value << " * 2 = " << result << std::endl;
    }
    return 0;
}