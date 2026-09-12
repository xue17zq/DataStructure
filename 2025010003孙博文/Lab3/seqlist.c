#include <stdio.h>
#include <windows.h>  // 引入windows接口

#define MAX_SIZE 100
typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

// 遍历打印顺序表
void printList(const SeqList *list) {
    for (int i = 0; i < list->size; i++) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// 下标index插入value
int insertAt(SeqList *list, int index, int value) {
    if (list->size == MAX_SIZE || index < 0 || index > list->size) {
        return 0;
    }
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    list->size += 1;
    return 1;
}

// 删除index位置，把删除值存入*removed
int removeAt(SeqList *list, int index, int *removed) {
    if (removed == NULL) {
        return 0;
    }
    if (index < 0 || index >= list->size) {
        return 0;
    }
    *removed = list->data[index];
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size -= 1;
    return 1;
}

int main() {
    SetConsoleOutputCP(65001);  // 强制控制台UTF-8，解决中文乱码

    SeqList list;
    list.size = 4;
    list.data[0] = 10;
    list.data[1] = 20;
    list.data[2] = 30;
    list.data[3] = 40;
    printf("初始: ");
    printList(&list);

    int ret1 = insertAt(&list, 2, 25);
    printf("insertAt(2, 25) 返回 %d\n", ret1);
    printList(&list);

    int removed;
    int ret2 = removeAt(&list, 1, &removed);
    printf("removeAt(1) 返回 %d, 删除的值是 %d\n", ret2, removed);
    printList(&list);

    int ret3 = insertAt(&list, 10, 99);
    printf("insertAt(10, 99) 返回 %d\n", ret3);
    printList(&list);

    int ret4 = removeAt(&list, 4, &removed);
    printf("removeAt(4) 返回 %d\n", ret4);
    printList(&list);

    return 0;
}
