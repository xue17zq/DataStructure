#include <stdio.h>
#include <stdlib.h>

/* 环形链表：和单链表同样的结点，差别只在最后一个结点回指哨兵
 * 所有遍历的停止条件都是 p != list->head，不允许出现 p != NULL */

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;   /* 哨兵，恒在环上；data 不使用 */
    int size;     /* 有效结点个数，不含哨兵 */
} CircList;

/* --------------------------------------------------------------------------
 * 教师提供：结点级工具，课堂代码原样搬来，不得修改
 * -------------------------------------------------------------------------- */

static Node* newNode(int value) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) {
        printf("malloc failed, exit\n");
        exit(1);
    }
    p->data = value;
    p->next = NULL;
    return p;
}

static Node* insertAfter(Node* prev, int value) {
    Node* p = newNode(value);
    p->next = prev->next;
    prev->next = p;
    return p;
}

/* 摘下 prev 后面的结点并返回它，只改指针，不释放。
 * 在环形链表里"后面"不包括哨兵，调用前必须保证 prev->next 不是哨兵 */
static Node* unlinkAfter(Node* prev) {
    Node* p = prev->next;
    if (p != NULL) {
        prev->next = p->next;
    }
    return p;
}

/* 秩为 rank 的结点的前驱；rank == 0 时是哨兵本身 */
static Node* prevOf(const CircList* list, int rank) {
    Node* p = list->head;
    for (int i = 0; i < rank; i++) {
        p = p->next;
    }
    return p;
}

/* --------------------------------------------------------------------------
 * 教师提供：建立、销毁与查询
 * -------------------------------------------------------------------------- */

/* 建立空环：哨兵自己指向自己 */
void circInit(CircList* list) {
    list->head = newNode(0);
    list->head->next = list->head;
    list->size = 0;
}

/* 释放整条环，哨兵也在环上。空环时哨兵自己指自己，也要释放 */
void circDestroy(CircList* list) {
    int count = 0;
    Node* p = list->head->next;
    while (p != list->head) {
        Node* next = p->next;
        free(p);
        p = next;
        count++;
    }
    free(list->head);
    count++;
    list->head = NULL;
    list->size = 0;
    printf("(circDestroy: freed %d nodes, including the sentinel)\n", count);
}

int circSize(const CircList* list) {
    return list->size;
}

/* 判空必须用指针关系判断（哨兵自己指向自己），不许用 size */
int circEmpty(const CircList* list) {
    return list->head->next == list->head;
}

/* --------------------------------------------------------------------------
 * 教师提供：建立与销毁
 * -------------------------------------------------------------------------- */

/* 练习 1：返回最后一个有效结点。从 head 出发，停止条件 p->next != head；
 * 空表时返回 head 本身（这样 circPushBack 就能直接接在它后面） */
static Node* circLast(const CircList* list) {
    Node* p = list->head;
    while (p->next != list->head) {
        p = p->next;
    }
    return p;
}

/* 练习 2：顺序追加，必须复用 insertAfter(circLast(list), value) */
int circPushBack(CircList* list, int value) {
    insertAfter(circLast(list), value);
    list->size += 1;
    return 1;
}

/* 练习 3：输出。遍历停止条件用 p != list->head */
void circPrint(const CircList* list) {
    printf("[size = %d] ", list->size);
    for (Node* p = list->head->next; p != list->head; p = p->next) {
        printf("%d ", p->data);
    }
    printf("\n");
}

/* 练习 4：在秩 rank 处插入，参照 listInsert，非法 rank 返回 0 */
int circInsert(CircList* list, int rank, int value) {
    if (rank < 0 || rank > list->size) {
        return 0;
    }
    insertAfter(prevOf(list, rank), value);
    list->size += 1;
    return 1;
}

/* 练习 5：删除秩 rank 处的结点，参照 listRemove，free(p) 单独一行 */
int circRemove(CircList* list, int rank, int* value) {
    if (rank < 0 || rank >= list->size) {
        return 0;
    }
    Node* p = unlinkAfter(prevOf(list, rank));
    if (value != NULL) {
        *value = p->data;
    }
    free(p);
    list->size -= 1;
    return 1;
}

/* 练习 6：按值查找，返回第一次出现的秩，找不到返回 -1 */
int circFind(const CircList* list, int value) {
    int rank = 0;
    for (Node* p = list->head->next; p != list->head; p = p->next) {
        if (p->data == value) {
            return rank;
        }
        rank++;
    }
    return -1;
}

/* 练习 7：从第一个结点出发绕圈走 steps 步，每步输出当前结点。
 * 步数可以超过 size，绕回时继续输出；走到哨兵要跳过，不输出哨兵。
 * 空表输出 "(empty)" 并换行 */
void circPrintAround(const CircList* list, int steps) {
    printf("walk %d steps around: ", steps);
    if (circEmpty(list)) {
        printf("(empty)");
        printf("\n");
        return;
    }
    Node* p = list->head->next;
    for (int i = 0; i < steps; i++) {
        if (p == list->head) {
            p = p->next;
        }
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/* 练习 8：约瑟夫环。
 * 维护 prev：它始终是"下一个报 1 的人"的前驱。初始 prev = head。
 * 出列时先走 k-1 步让 prev 变成"报 k 的人"的前驱，再 unlinkAfter 并 free。
 * 走步与收尾遇到哨兵都要跳过。 */
void circJosephus(CircList* list, int k) {
    printf("josephus n = %d, k = %d, out order: ", list->size, k);
    Node* prev = list->head;   /* 下一个报 1 的人的前驱 */
    while (list->size > 0) {
        for (int i = 1; i < k; i++) {          /* 走 k-1 步 */
            prev = (prev->next == list->head) ? list->head : prev->next;
        }
        if (prev->next == list->head) {        /* 一步到位后的收尾检查 */
            prev = list->head;
        }
        Node* p = unlinkAfter(prev);
        printf("%d ", p->data);
        free(p);
        list->size -= 1;
        if (prev->next == list->head) {        /* 下一轮起始检查 */
            prev = list->head;
        }
    }
    printf("\n");
}

/* 测试顺序与讲义 4.4 一致，方便逐行对照 */
int main(void) {
    CircList ring;
    circInit(&ring);
    printf("after init: ");
    circPrint(&ring);
    printf("circEmpty = %d, circSize = %d\n\n", circEmpty(&ring), circSize(&ring));

    /* 1. 依次追加五个样例元素 */
    int samples[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        circPushBack(&ring, samples[i]);
    }
    printf("after circPushBack 1, 2, 3, 4, 5: ");
    circPrint(&ring);

    /* 2. 绕圈走 7 步与 12 步 */
    printf("circPrintAround(7) on ring: ");
    circPrintAround(&ring, 7);
    printf("circPrintAround(12) on ring: ");
    circPrintAround(&ring, 12);

    /* 3. 在秩 2 处插入 25 */
    int ok = circInsert(&ring, 2, 25);
    printf("circInsert(2, 25): ok = %d, ", ok);
    circPrint(&ring);

    /* 4. 删除秩 2 处的 25 */
    int removed = 777;
    ok = circRemove(&ring, 2, &removed);
    printf("circRemove(2): ok = %d, removed = %d, ", ok, removed);
    circPrint(&ring);

    /* 5. 删除最后一个元素（验证明环闭合），再把它追加回去 */
    ok = circRemove(&ring, 4, &removed);
    printf("after circRemove(4, last): ok = %d, removed = %d, ", ok, removed);
    circPrint(&ring);
    ok = circPushBack(&ring, 5);
    printf("after circPushBack(5): ok = %d, ", ok);
    circPrint(&ring);

    /* 6. 按值查找 */
    printf("circFind(4) = %d\n", circFind(&ring, 4));
    printf("circFind(99) = %d\n", circFind(&ring, 99));

    /* 7. 非法位置 */
    ok = circInsert(&ring, 6, 100);
    printf("circInsert(6, 100): ok = %d\n", ok);
    ok = circRemove(&ring, 5, &removed);
    printf("circRemove(5): ok = %d, removed = %d\n\n", ok, removed);

    /* 8. 约瑟夫环（三组样例：5 3、7 2、1 1） */
    CircList jos1;
    circInit(&jos1);
    int j1[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        circPushBack(&jos1, j1[i]);
    }
    circJosephus(&jos1, 3);
    printf("after josephus k=3: ");
    circPrint(&jos1);

    CircList jos2;
    circInit(&jos2);
    int j2[] = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 7; i++) {
        circPushBack(&jos2, j2[i]);
    }
    circJosephus(&jos2, 2);
    printf("after josephus k=2: ");
    circPrint(&jos2);

    CircList jos3;
    circInit(&jos3);
    int j3[] = {1};
    for (int i = 0; i < 1; i++) {
        circPushBack(&jos3, j3[i]);
    }
    circJosephus(&jos3, 1);
    printf("after josephus k=1: ");
    circPrint(&jos3);

    /* 9. 收尾释放 */
    printf("\ndestroy by hand before main ends, ring then jos1 then jos2 then jos3:\n");
    circDestroy(&ring);
    circDestroy(&jos1);
    circDestroy(&jos2);
    circDestroy(&jos3);
    return 0;
}