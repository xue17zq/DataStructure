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

int main(void) {
    int inputs[] = {2048, 4096, 8192, 16384};
    int num = sizeof(inputs) / sizeof(inputs[0]);

    printf("%-10s%-15s%-15s%-15s%-15s\n", "n", "algorithm1", "algorithm2", "algorithm3", "algorithm4");

    for (int i = 0; i < num; i++) {
        int n = inputs[i];
        long long c1 = algorithm1(n);
        long long c2 = algorithm2(n);
        long long c3 = algorithm3(n);
        long long c4 = algorithm4(n);
        printf("%-10d%-15lld%-15lld%-15lld%-15lld\n", n, c1, c2, c3, c4);
    }

    return 0;
}
