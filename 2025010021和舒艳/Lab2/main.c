#include <stdio.h>

// algorithm1: 单层循环 O(n)
long long algorithm1(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        count++;
    }
    return count;
}

// algorithm2: 双重循环，内层随外层变化 O(n²)
long long algorithm2(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            count++;
        }
    }
    return count;
}

// algorithm3: 倍增循环 O(log n)
long long algorithm3(int n) {
    long long count = 0;
    for (int i = 1; i < n; i *= 2) {
        count++;
    }
    return count;
}

// algorithm4: 单层×倍增组合 O(n log n)
long long algorithm4(int n) {
    long long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j *= 2) {
            count++;
        }
    }
    return count;
}

int main() {
    int ns[] = {2048, 4096, 8192, 16384};
    int len = sizeof(ns) / sizeof(ns[0]);

    // 输出表头
   printf("n\talgorithm1\talgorithm2\talgorithm3\talgorithm4\n");

    for (int i = 0; i < len; i++) {
        int n = ns[i];
        long long a1 = algorithm1(n);
        long long a2 = algorithm2(n);
        long long a3 = algorithm3(n);
        long long a4 = algorithm4(n);
        
        printf("%d\t%lld\t%lld\t%lld\t%lld\n", n, a1, a2, a3, a4);
    }

    return 0;
}
