
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


int main(void)
{
    int arr[] = {2048, 4096, 8192, 16384};
    int len = sizeof(arr)/sizeof(arr[0]);
    for(int i = 0; i < len; i++)
    {
        int n = arr[i];
        long long c1 = algorithm1(n);
        long long c2 = algorithm2(n);
        long long c3 = algorithm3(n);
        long long c4 = algorithm4(n);
        printf("n=%d: algorithm1 count=%lld, algorithm2 count=%lld, algorithm3 count=%lld, algorithm4 count=%lld\n",
               n, c1, c2, c3, c4);
    }
    return 0;
}