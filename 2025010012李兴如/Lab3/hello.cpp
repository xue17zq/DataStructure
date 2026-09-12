#include <iostream>
bool getDoubled(int value, int& result);
int main() {
    bool isStudent=true;
    std::cout << "姓名：李兴如" << std::endl;
    std::cout <<"学号：2025010012" << std::endl;
    std::cout <<"是否为学生："<< isStudent<<std::endl;
    int result;
    getDoubled(12,result);
    std::cout << "12的两倍是" << result << std::endl;
    return 0;
}
bool getDoubled(int value, int& result) {
    result = value*2;
    return true;
}