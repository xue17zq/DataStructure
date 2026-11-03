#include <iostream>
#include "queue.h"

/* 一个窗口对应一个队列，队首正在接受服务，其余顾客按先后顺序等待。 */
struct Customer {
    int window;                    /* 所属窗口编号，从 0 开始 */
    unsigned int time;             /* 剩余服务时间，入队时大于 0 */
};

/* 学生完成：按人数选择最短队列。
 * windows 是队列数组，windows[i] 是第 i 个窗口的顾客队列。
 * nWin 是窗口总数（number of windows），windows[i].size() 是该窗口的顾客人数。
 * 例如有 3 个窗口，分别排着 2、5、0 人：nWin 为 3，三个队列的 size() 分别为 2、5、0。
 * 数组作为函数参数传入时会退化为指针，windows[] 不携带数组长度，
 * 所以需要另外传入 nWin，告诉函数应遍历几个窗口；这里要求 nWin > 0。
 * 返回值是选中的窗口编号，范围为 0 到 nWin - 1。
 */
int bestWindow(Queue<Customer> windows[], Rank nWin) {
    Rank best = 0;
    for (Rank i = 1; i < nWin; ++i) {
        if (windows[i].size() < windows[best].size()) {
            best = i;
        }
    }
    return best;
    /* TODO 5：从窗口 0 开始，逐个比较各窗口的 size()。
     * 发现更短的队列时更新候选；同长时保留编号较小的窗口。
     */
}

/* 学生完成：各窗口服务一个时间单位，返回本轮完成服务的人数。
 * nWin 是窗口总数，依次处理 windows[0] 到 windows[nWin - 1]。
 */
int serveOneTick(Queue<Customer> windows[], Rank nWin) {
    int finished = 0;
    for (Rank i=0;i<nWin;++i) {
        if (windows[i].empty()) {
            continue;
        }
        windows[i].front().time--;
        if (windows[i].front().time==0) {
            windows[i].dequeue();
            ++finished;
        }
    }
    return finished;
    /* TODO 6：依次检查各窗口，空队列跳过。
     * 通过 front() 将队首顾客的 time 减 1，减到 0 时出队并计数。
     * 每个窗口本轮只服务当前队首，新队首留到下一轮再服务。
     */
}

/* 教师提供：按固定到达表推进模拟。
 * nWin 是银行的服务窗口总数；下面据此创建 nWin 个独立的顾客队列。
 * servTime 是营业时长，表示模拟多少个时间单位。
 * arrivals[now] 为新顾客所需服务时间，0 表示该时刻无人到达。
 * nWin > 0，servTime >= 0，到达表至少有 servTime 项。
 */
void simulate(Rank nWin, Rank servTime, const unsigned int arrivals[]) {
    Queue<Customer>* windows = new Queue<Customer>[nWin];
    int arrived = 0;
    int served = 0;

    for (Rank now = 0; now < servTime; ++now) {
        unsigned int time = arrivals[now];
        int chosen = -1;

        /* 先到达：选择人数最少的窗口，将新顾客排到队尾。 */
        if (time > 0) {
            chosen = bestWindow(windows, nWin);
            Customer c;
            c.window = chosen;
            c.time = time;
            windows[chosen].enqueue(c);
            ++arrived;
        }

        /* 再服务：每个非空窗口各服务一次，累计本轮完成的人数。 */
        int finished = serveOneTick(windows, nWin);
        served += finished;

        /* 输出服务结束后的状态，便于对照手工推演。 */
        std::cout << "t=" << now
                  << " arrival=" << time
                  << " window=" << chosen
                  << " served=" << finished;
        for (Rank i = 0; i < nWin; ++i) {
            std::cout << " | W" << i << " size=" << windows[i].size();
            if (!windows[i].empty()) {
                std::cout << " front=" << windows[i].front().time;
            } else {
                std::cout << " front=-";
            }
        }
        std::cout << '\n';
    }

    /* 营业结束时统计尚未完成服务的人数，不再继续服务。 */
    int remaining = 0;
    for (Rank i = 0; i < nWin; ++i) {
        remaining += windows[i].size();
    }
    std::cout << "arrived=" << arrived
              << ", served=" << served
              << ", remaining=" << remaining << '\n';
    delete[] windows;
}

int main() {
    /* nWin = 2：窗口编号为 0、1；servTime = 6：模拟六个时间单位。 */
    const unsigned int arrivals[] = {3, 2, 4, 1, 2, 0};
    simulate(2, 6, arrivals);
    return 0;
}
