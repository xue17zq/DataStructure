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
    int sizes[] = {2048, 4096, 8192, 16384};

    printf("%-8s %-12s %-12s %-12s %-12s\n",
           "n", "algorithm1", "algorithm2", "algorithm3", "algorithm4");

    for (int i = 0; i < 4; i++) {
        int n = sizes[i];
        printf("%-8d %-12lld %-12lld %-12lld %-12lld\n",
               n, algorithm1(n), algorithm2(n), algorithm3(n), algorithm4(n));
    }

    return 0;
}