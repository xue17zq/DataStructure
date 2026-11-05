#include <iostream>

/* 教师提供：保持代码不变，用 CLion 的 Debug 窗口观察真实调用栈（左栏调用栈、右栏 Variables）。 */
int addOne(int x) {
    int y = x + 1;
    return y;                   /* 断点 A：最深层普通调用，y 已初始化 */
}

int twiceAfterAdd(int x) {
    int t = addOne(x);
    int result = 2 * t;          /* 断点 B：addOne 已返回，result 尚未初始化 */
    return result;
}

int solve(int n) {
    int base = n + 2;
    int answer = twiceAfterAdd(base);
    return answer;
}

int factorial(int n) {
    if (n == 0) {
        int base = 1;
        return base;            /* 断点 C：递归最深处，base 已初始化 */
    }
    int sub = factorial(n - 1);
    int result = n * sub;
    return result;              /* 单步返回时在这里观察每层的 n、sub、result */
}

int main() {
    int n = 3;
    int result = solve(n);
    std::cout << "nested result = " << result << '\n';

    int product = factorial(4);
    std::cout << "factorial(4) = " << product << '\n';
    return 0;
}
