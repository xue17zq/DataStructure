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

int main() {
    int sizes[] = {2048, 4096, 8192, 16384};
    int num_sizes = 4;

    printf("n       algorithm1    algorithm2    algorithm3    algorithm4\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        long long c1 = algorithm1(n);
        long long c2 = algorithm2(n);
        long long c3 = algorithm3(n);
        long long c4 = algorithm4(n);

        printf("%-8d%-14lld%-14lld%-14lld%-14lld\n", n, c1, c2, c3, c4);
    }

    return 0;
}
// Created by Rrdi on 2026/9/7.
//