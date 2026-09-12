#include <stdio.h>

long long algorithm1(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        count++;
    }
    return count;
}

long long algorithm2(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            count++;
        }
    }
    return count;
}

long long algorithm3(int n) {
    long long count = 0;
    for (int i = 1; i < n; i *= 2) {
        count++;
    }
    return count;
}

long long algorithm4(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j *= 2) {
            count++;
        }
    }
    return count;
}

int main()
{
    // 预设好题目给的4组n，程序自动循环计算输出
    int n_list[] = {2048, 4096, 8192, 16384};
    int num = sizeof(n_list) / sizeof(n_list[0]);

    printf("n\tAlgorithm1\tAlgorithm2\tAlgorithm3\tAlgorithm4\n");
    for(int k = 0; k < num; k++)
    {
        int n = n_list[k];
        long long res1 = algorithm1(n);
        long long res2 = algorithm2(n);
        long long res3 = algorithm3(n);
        long long res4 = algorithm4(n);
        printf("%d\t%lld\t\t%lld\t%lld\t\t%lld\n", n, res1, res2, res3, res4);
    }
    return 0;
}