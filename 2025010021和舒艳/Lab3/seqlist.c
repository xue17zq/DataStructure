#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

// 练习1：遍历打印
void printList(const SeqList *list) {
    for (int i = 0; i < list->size; i++) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// 练习2：在下标 index 处插入 value
int insertAt(SeqList *list, int index, int value) {
    if (list->size >= MAX_SIZE || index < 0 || index > list->size) {
        return 0;
    }
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    list->size++;
    return 1;
}

// 练习3：删除下标 index 处的元素
int removeAt(SeqList *list, int index, int *removed) {
    if (index < 0 || index >= list->size) {
        return 0;
    }
    *removed = list->data[index];
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
    return 1;
}
int main() {
    SeqList list;
    list.size = 4;
    list.data[0] = 10; list.data[1] = 20; list.data[2] = 30; list.data[3] = 40;

    // 把“初始: ”改成 “Initial: ”
    printf("Initial: ");
    printList(&list);

    int ret = insertAt(&list, 2, 25);
    // 把“返回”改成 “return”
    printf("insertAt(2, 25) return %d\n", ret);
    printList(&list);

    int removed;
    ret = removeAt(&list, 1, &removed);
    // 把“返回”、“删除的值是”改成英文
    printf("removeAt(1) return %d, removed value %d\n", ret, removed);
    printList(&list);

    ret = insertAt(&list, 10, 99);
    printf("insertAt(10, 99) return %d\n", ret);
    printList(&list);

    ret = removeAt(&list, 4, &removed);
    printf("removeAt(4) return %d\n", ret);
    printList(&list);

    return 0;
}