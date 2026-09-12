#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

// 练习 1：从下标 0 到 size - 1 依次输出所有有效元素，元素之间用一个空格分隔，末尾换行
void printList(const SeqList *list) {
    for (int i = 0; i < list->size; i++) {
        printf("%d", list->data[i]);
        if (i < list->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

// 练习 2：在下标 index 处插入 value，成功返回 1，失败返回 0
// 失败情形：顺序表已满（size = MAX_SIZE），或 index 不在 [0, size] 范围内
int insertAt(SeqList *list, int index, int value) {
    // 判断是否已满或下标不合法
    if (list->size >= MAX_SIZE || index < 0 || index > list->size) {
        return 0;
    }

    // 从最后一个有效元素开始，从后向前逐个后移
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }

    // 插入新值
    list->data[index] = value;
    list->size++;

    return 1;
}

// 练习 3：删除下标 index 处的元素，并通过指针参数 removed 把被删除的值传回调用者
// 成功返回 1，失败返回 0；失败情形：index 不在 [0, size - 1] 范围内
int removeAt(SeqList *list, int index, int *removed) {
    // 判断下标是否合法
    if (index < 0 || index >= list->size) {
        return 0;
    }

    // 先保存被删除的值
    *removed = list->data[index];

    // 让被删元素后面的元素从前向后逐个前移
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->size--;

    return 1;
}

// 3.3 main 函数
int main() {
    SeqList list;

    // 1. 手动把 size 设为 4，data[0..3] 依次设为 10, 20, 30, 40，然后输出
    list.size = 4;
    list.data[0] = 10;
    list.data[1] = 20;
    list.data[2] = 30;
    list.data[3] = 40;

    printf("初始：");
    printList(&list);

    // 2. 调用 insertAt(&list, 2, 25)，输出返回值和顺序表内容
    int insertResult = insertAt(&list, 2, 25);
    printf("insertAt(2, 25) 返回 %d\n", insertResult);
    printList(&list);

    // 3. 定义 int removed；调用 removeAt(&list, 1, &removed)，输出返回值、被删除的值和顺序表内容
    int removed;
    int removeResult = removeAt(&list, 1, &removed);
    printf("removeAt(1) 返回 %d，删除的值是 %d\n", removeResult, removed);
    printList(&list);

    // 4. 调用 insertAt(&list, 10, 99)（下标不合法），输出返回值和顺序表内容
    insertResult = insertAt(&list, 10, 99);
    printf("insertAt(10, 99) 返回 %d\n", insertResult);
    printList(&list);

    // 5. 调用 removeAt(&list, 4, &removed)（此时 size 为 4，下标 4 不合法），输出返回值和顺序表内容
    removeResult = removeAt(&list, 4, &removed);
    printf("removeAt(4) 返回 %d\n", removeResult);
    printList(&list);

    return 0;
}