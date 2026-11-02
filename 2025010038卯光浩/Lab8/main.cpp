#include <iostream>
#include "stack.h"

/* 学生完成：只检查 exp[lo, hi)，调用者保证区间合法。 */
bool paren(const char exp[], Rank lo, Rank hi) {
    Stack<char> stack;
    for (int i=lo;i<hi;++i) {
        char c = exp[i];
        if (c=='(' || c=='[' || c=='{') {
            stack.push(c);
        }else if (c==')' || c==']' || c=='}') {
            if (stack.empty()) {
                return false;
            }
            if ((c==')'&&stack.top()!='(') ||(c==']'&&stack.top()!='[') || (c=='}'&&stack.top()!='{')) {
                return false;
            }
            stack.pop();
        }

    }
    return stack.empty();
    /* TODO 5：用 Stack<char> 检查 exp[lo, hi) 里的三种括号，忽略其他字符，返回是否匹配。
     *
     * 思路——从左向右扫一遍：
     *  1. 遇到左括号：暂时还配不上，压入栈，等后面的右括号来配。
     *  2. 遇到右括号：能跟它配的只有“最近一个尚未匹配的左括号”，也就是栈顶。
     *     先判空——栈空说明前面没有左括号可配，失败；
     *     再比类型——栈顶与当前右括号不同类，说明嵌套顺序错了，失败；
     *     两条都过，才算配成一对，弹出栈顶。
     *  3. 其他字符（字母、空格、引号……）不动栈，直接跳过。
     *  4. 扫到 hi 只说明途中没遇到非法右括号，栈里还可能剩着左括号，
     *     因此最后返回 S.empty()：栈空才算整个区间匹配。
     *
     * 注意：只处理 [lo, hi)，不要从 0 开始，也不要读到字符串结尾标记；
     *       取栈顶之前必须先判空。
     */
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
