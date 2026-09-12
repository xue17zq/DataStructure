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
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("n\talgorithm1\talgorithm2\talgorithm3\talgorithm4\n");
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        long long c1 = algorithm1(n);
        long long c2 = algorithm2(n);
        long long c3 = algorithm3(n);
        long long c4 = algorithm4(n);
        printf("%d\t%lld\t\t%lld\t\t%lld\t\t%lld\n", n, c1, c2, c3, c4);
    }

    return 0;
}