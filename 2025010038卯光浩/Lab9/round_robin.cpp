#include <iostream>
#include <sstream>
#include <string>
#include "queue.h"

/* 学生完成：服务一次后重新排到队尾；rounds >= 0，空队列直接结束。
 * Q 按引用传入，出队、入队都会改变调用者的队列。
 * rounds 表示最多服务多少个对象，不表示把整个队列轮流服务多少遍。
 */
void roundRobin(Queue<char>& Q, int rounds) {
   for (int i=0;i<rounds;++i) {
       if (Q.empty()) {
           break;
       }
       char e=Q.dequeue();
       std::cout << e << ' ';
       Q.enqueue(e);
   }/* TODO 4：最多进行 rounds 次分配，每次先检查队列是否非空。
     * 取出队首，用 std::cout << e << ' '; 表示服务，再将它重新入队。
     * 每次只服务一个对象；没有对象永久离开，也没有新对象加入。
     * 保存 dequeue() 返回的字符值，不保存已删除节点内元素的引用。
     * 每次服务后队列长度不变；函数中不输出标题、换行或其他文字。
     */
}

/* 以下测试与 main 由教师提供，不得修改。
 * 测试暂时收集 std::cout 的输出，同时检查服务顺序与结束后的队列。
 * 输出收集仅用于自动核对，不属于循环分配算法，学生无需实现这部分。
 */
bool testRoundRobin(const char initial[], int rounds,
                    const char expectedService[], const char expectedQueue[]) {
    Queue<char> Q;
    /* 按字符串从左到右入队，构造测试的初始顺序。 */
    for (Rank i = 0; initial[i] != '\0'; ++i) {
        Q.enqueue(initial[i]);
    }
    std::ostringstream service;
    /* 临时让 cout 把字符写入 service，并保存它原来的输出位置。
     * 调用后立即恢复，以便下面的测试报告仍正常显示在控制台。
     */
    std::streambuf* consoleBuffer = std::cout.rdbuf(service.rdbuf());
    roundRobin(Q, rounds);
    std::cout.rdbuf(consoleBuffer);

    /* 函数返回后再逐个出队检查剩余顺序；清空队列的是测试代码。 */
    std::string remaining;
    while (!Q.empty()) {
        remaining += Q.dequeue();
    }
    bool ok = service.str() == expectedService
              && remaining == expectedQueue
              && Q.size() == 0;
    std::cout << "initial = [" << initial << "], rounds = " << rounds << '\n';
    std::cout << "  service = [" << service.str()
              << "], expected = [" << expectedService << "]\n";
    std::cout << "  queue = [" << remaining << "], expected = [" << expectedQueue << "] "
              << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

int main() {
    int passed = 0;
    /* 覆盖多轮循环、零次、单元素、空队列、恰好一轮和只服务一次。
     * expectedService 中最后一个字符后也有空格，与函数的输出约定一致。
     */
    if (testRoundRobin("ABC", 8, "A B C A B C A B ", "CAB")) {
        ++passed;
    }
    if (testRoundRobin("ABC", 0, "", "ABC")) {
        ++passed;
    }
    if (testRoundRobin("A", 4, "A A A A ", "A")) {
        ++passed;
    }
    if (testRoundRobin("", 3, "", "")) {
        ++passed;
    }
    if (testRoundRobin("ABC", 3, "A B C ", "ABC")) {
        ++passed;
    }
    if (testRoundRobin("ABC", 1, "A ", "BCA")) {
        ++passed;
    }
    std::cout << "round-robin tests: " << passed << "/6 passed\n";
    return passed == 6 ? 0 : 1;
}
