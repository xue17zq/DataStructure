#include <iostream>
#include "stack.h"

/* 判断左括号 left 与右括号 right 是否同类。 */
bool matchPair(char left, char right) {
    return (left == '(' && right == ')')
        || (left == '[' && right == ']')
        || (left == '{' && right == '}');
}

/* 学生完成：只检查 exp[lo, hi)，调用者保证区间合法。 */
bool paren(const char exp[], Rank lo, Rank hi) {
    Stack<char> S;                          /* 只保留尚未匹配的左括号 */
    for (Rank i = lo; i < hi; ++i) {
        char c = exp[i];
        if (c == '(' || c == '[' || c == '{') {
            S.push(c);                      /* 左括号：先压栈，等右括号来配 */
        } else if (c == ')' || c == ']' || c == '}') {
            if (S.empty()) return false;    /* 栈空说明前面没有左括号可配 */
            if (!matchPair(S.top(), c)) return false;  /* 类型或嵌套顺序不对 */
            S.pop();                        /* 配对成功，弹出栈顶 */
        }                                   /* 其他字符不影响栈，直接跳过 */
    }
    return S.empty();                       /* 扫完还剩左括号就没配完 */
}

/* 以下测试与 main 由教师提供，不得修改。按题目中的输出逐项核对。 */
void testStack() {
    Stack<int> S;
    std::cout << "empty = " << S.empty() << ", size = " << S.size() << '\n';
    S.push(10);
    S.push(20);
    S.push(30);
    std::cout << "top = " << S.top() << ", size = " << S.size() << '\n';
    S.top() = 35;
    std::cout << "changed top = " << S.top() << '\n';
    std::cout << "pop: ";
    while (!S.empty()) {
        std::cout << S.pop() << ' ';
    }
    std::cout << '\n';

    for (int i = 1; i <= 8; ++i) S.push(i);  /* 超过初始容量，观察扩容后顺序 */
    std::cout << "after growth: ";
    while (!S.empty()) {
        std::cout << S.pop() << ' ';
    }
    std::cout << '\n';
    S.push(-7);
    std::cout << "reuse = " << S.pop() << ", empty = " << S.empty() << '\n';

    Stack<char> C;
    C.push('(');
    C.push('[');
    std::cout << "char pop: ";
    while (!C.empty()) {
        std::cout << C.pop() << ' ';
    }
    std::cout << '\n';
}

/* 比较一次实际结果和预期结果；bool 默认输出为 1 或 0。 */
bool testParen(const char exp[], Rank lo, Rank hi, bool expected) {
    bool actual = paren(exp, lo, hi);
    std::cout << "[" << lo << ", " << hi << ") " << exp
              << " : actual = " << actual << ", expected = " << expected;
    if (actual == expected) {
        std::cout << " PASS\n";
        return true;
    }
    std::cout << " FAIL\n";
    return false;
}

int main() {
    testStack();
    int passed = 0;
    if (testParen("", 0, 0, true)) ++passed;
    if (testParen("()[]{}", 0, 6, true)) ++passed;
    if (testParen("([{}])", 0, 6, true)) ++passed;
    if (testParen("{a+[b*(c-d)]}", 0, 13, true)) ++passed;
    if (testParen("a + b", 0, 5, true)) ++passed;
    if (testParen("([)]", 0, 4, false)) ++passed;
    if (testParen("(]", 0, 2, false)) ++passed;
    if (testParen(")(", 0, 2, false)) ++passed;
    if (testParen("(()", 0, 3, false)) ++passed;
    if (testParen("())", 0, 3, false)) ++passed;
    if (testParen("(", 0, 1, false)) ++passed;
    if (testParen("]", 0, 1, false)) ++passed;
    if (testParen("x([])y", 1, 5, true)) ++passed;
    if (testParen("x([])y", 1, 4, false)) ++passed;
    if (testParen("()", 1, 1, true)) ++passed;
    std::cout << "paren tests: " << passed << "/15 passed\n";
    if (passed != 15) return 1;   /* 有一组失败就以退出码 1 结束 */

    return 0;
}
