#include <stdio.h>
#include <stdlib.h>

/* 结点：与课堂上写的完全相同，不得修改 */
typedef struct Node {
    int data;           /* 数据域：保存一个整数 */
    struct Node* next;  /* 指针域：保存下一个结点的地址，NULL 表示没有后继 */
} Node;

/* 链表：头哨兵的地址和有效结点个数打包在一起。
 * 与 Lab3 的 SeqList 一样，所有操作都通过 LinkedList* 参数访问它们 */
typedef struct {
    Node* head;         /* 头哨兵，data 不使用 */
    int size;           /* 有效结点个数，不含哨兵 */
} LinkedList;

/* --------------------------------------------------------------------------
 * 教师提供：结点级操作，课堂代码原样搬来，不得修改
 * -------------------------------------------------------------------------- */

/* 申请一个存放 value 的新结点。它的寿命不由作用域决定，而由 free 决定 */
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

/* 在 prev 后面插入一个存 value 的新结点，返回新结点。课堂上的两句 */
static Node* insertAfter(Node* prev, int value) {
    Node* p = newNode(value);
    p->next = prev->next;
    prev->next = p;
    return p;
}

/* 摘下 prev 后面的结点并返回它，只改指针，不释放 */
static Node* unlinkAfter(Node* prev) {
    Node* p = prev->next;
    if (p != NULL) {
        prev->next = p->next;
    }
    return p;
}

/* 秩为 rank 的结点的前驱；rank == 0 时是哨兵本身。从哨兵出发走 rank 步 */
static Node* prevOf(const LinkedList* list, int rank) {
    Node* p = list->head;
    for (int i = 0; i < rank; i++) {
        p = p->next;
    }
    return p;
}

/* --------------------------------------------------------------------------
 * 教师提供：建立与销毁
 * -------------------------------------------------------------------------- */

/* 建立空表：立哨兵。对应课堂上 main 开头的 head = newNode(0) */
void listInit(LinkedList* list) {
    list->head = newNode(0);
    list->size = 0;
}

/* 释放整条链，哨兵也在链上。末尾的输出是验证手段，原样保留 */
void listDestroy(LinkedList* list) {
    int count = 0;
    Node* p = list->head;
    while (p != NULL) {
        Node* next = p->next;   /* 先记住下一个，再 free 当前这个 */
        free(p);
        p = next;
        count++;
    }
    list->head = NULL;
    list->size = 0;
    printf("(listDestroy: freed %d nodes, including the sentinel)\n", count);
}

int listSize(const LinkedList* list) {
    return list->size;
}

int listEmpty(const LinkedList* list) {
    return list->size == 0;
}

/* --------------------------------------------------------------------------
 * 学生练习
 * -------------------------------------------------------------------------- */

/* 练习 1：输出。从第一个有效结点开始输出，哨兵不输出，每个数后面一个空格，末尾换行 */
void listPrint(const LinkedList* list) {
    printf("[size = %d] ", list->size);
    for (const Node* p = list->head->next; p != NULL; p = p->next) {
        printf("%d ",p->data);
    }
    /* TODO：for (const Node* p = list->head->next; p != NULL; p = p->next) */
    printf("\n");
}

/* 练习 2：在秩 rank 处插入 value，合法范围 0 <= rank <= size
 * 成功返回 1 并让 size 加一；rank 非法返回 0，链表不变 */
int listInsert(LinkedList* list, int rank, int value) {
    if (rank<0 || rank>list->size) {
        return 0;
    }
    insertAfter(prevOf(list,rank),value);
    list->size++;

    return 1;   /* TODO：检查 rank；insertAfter(prevOf(list, rank), value)；size 加一 */
}

/* 练习 3：顺序追加与表头插入，都复用 listInsert，不得另写接链代码 */
int listPushBack(LinkedList* list, int value) {
    return listInsert(list,list->size,value);   /* TODO：listInsert(list, list->size, value) */
}

int listPushFront(LinkedList* list, int value) {
    return listInsert(list,0,value);   /* TODO：listInsert(list, 0, value) */
}

/* 练习 4：删除秩 rank 处的结点，被删的值写入 *value，合法范围 0 <= rank < size
 * 成功返回 1 并让 size 减一；失败返回 0 且不修改 *value
 * 必须用局部变量 p 接住 unlinkAfter 的返回值，并单独一行写 free(p);（3.5 的断点设在这一行） */
int listRemove(LinkedList* list, int rank, int* value) {
    if (rank<0 || rank>=list->size) {
        return 0;
    }
    Node*p=unlinkAfter(prevOf(list,rank));
    *value=p->data;
    free(p);
    list->size--;
    return 1;
       /* TODO：检查 rank；Node* p = unlinkAfter(prevOf(list, rank))；取出 data；free(p)；size 减一 */
}

/* 练习 5：按秩读取，合法范围 0 <= rank < size；失败时不修改 *value */
int listGet(const LinkedList* list, int rank, int* value) {
    if (rank<0 || rank>=list->size) {
        return 0;
    }
    Node*pre=prevOf(list,rank);
    Node*p=pre->next;
    *value=p->data;
    return 1;

     /* TODO */
}

/* 练习 6：按值查找，返回第一次出现的秩，找不到返回 -1。从 head->next 开始数，秩从 0 起 */
int listFind(const LinkedList* list, int value) {
    int x=0;
    for (Node* p = list->head->next; p != NULL; p = p->next) {
        if (p->data == value) {
            return x;
        }
        x++;
    }
    return -1;  /* TODO */
}

/* 测试顺序与讲义附录一、Lab4 的向量完全一致，方便逐行对照 */
int main(void) {
    LinkedList list;
    listInit(&list);
    printf("after init: ");
    listPrint(&list);
    printf("listEmpty = %d, listSize = %d\n\n", listEmpty(&list), listSize(&list));

    /* 1. 依次追加五个样例元素 */
    int samples[] = {18, -1, 42, 18, 65};
    for (int i = 0; i < 5; i++) {
        listPushBack(&list, samples[i]);
    }
    printf("after listPushBack 18, -1, 42, 18, 65: ");
    listPrint(&list);
    printf("\n");

    /* 2. 在秩 2 处插入 25 */
    int ok = listInsert(&list, 2, 25);
    printf("listInsert(2, 25): ok = %d, ", ok);
    listPrint(&list);

    /* 3. 删除秩 1 处的元素，并输出被删除的值 */
    int removed = 777;
    ok = listRemove(&list, 1, &removed);
    printf("listRemove(1): ok = %d, removed = %d, ", ok, removed);
    listPrint(&list);

    /* 4. 在表头插入 7 */
    ok = listPushFront(&list, 7);
    printf("listPushFront(7): ok = %d, ", ok);
    listPrint(&list);
    printf("\n");

    /* 5. 按秩读取：合法的秩与越界的秩 */
    int value = 777;
    ok = listGet(&list, 0, &value);
    printf("listGet(0): ok = %d, value = %d\n", ok, value);
    value = 777;
    ok = listGet(&list, 6, &value);     /* 此刻 size = 6，秩 6 已越界 */
    printf("listGet(6): ok = %d, value = %d\n\n", ok, value);

    /* 6. 按值查找：重复元素只返回第一次出现的秩 */
    printf("listFind(42) = %d\n", listFind(&list, 42));
    printf("listFind(18) = %d\n", listFind(&list, 18));
    printf("listFind(99) = %d\n\n", listFind(&list, 99));

    /* 7. 非法位置：负秩、超过 size 的秩、删除空表 */
    ok = listInsert(&list, -1, 100);
    printf("listInsert(-1, 100): ok = %d\n", ok);
    ok = listInsert(&list, 100, 100);
    printf("listInsert(100, 100): ok = %d\n", ok);

    LinkedList empty;
    listInit(&empty);
    ok = listRemove(&empty, 0, &removed);
    printf("empty listRemove(0): ok = %d\n\n", ok);

    /* 8. 收尾：C 语言没有析构函数，释放必须自己写 */
    printf("before destroy: ");
    listPrint(&list);
    printf("destroy by hand before main ends, empty first, then list:\n");
    listDestroy(&empty);
    listDestroy(&list);
    return 0;
}