#include<stdio.h>
long long algorithm1(int n);
long long algorithm2(int n);
long long algorithm3(int n);
long long algorithm4(int n);
int main() {
    long long t1,t2,t3,t4;
    printf("n\t algorithm1 \t algorithm2 \t algorithm3 \t algorithm4");
    printf("\n");
    int n=2048;
    for (int i = 0; i < 4; i++) {
        t1=algorithm1(n);
        t2=algorithm2(n);
        t3=algorithm3(n);
        t4=algorithm4(n);
        printf("%d\t  %d\t     %d\t     %d\t    %d\t\n",n,t1,t2,t3,t4);
        n*=2;
    }
}
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