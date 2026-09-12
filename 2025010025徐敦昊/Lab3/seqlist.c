#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

void printList(const SeqList *list) {
    for (int i = 0; i < list->size; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", list->data[i]);
    }
    printf("\n");
}

int insertAt(SeqList *list, int index, int value) {
    if (list->size == MAX_SIZE) {
        return 0;
    }
    if (index < 0 || index > list->size) {
        return 0;
    }
    
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    list->size++;
    return 1;
}

int removeAt(SeqList *list, int index, int *removed) {
    if (removed == NULL) {
        return 0;
    }
    if (index < 0 || index > list->size - 1) {
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
    list.data[0] = 10;
    list.data[1] = 20;
    list.data[2] = 30;
    list.data[3] = 40;
    printf("初始: ");
    printList(&list);
    
    int result = insertAt(&list, 2, 25);
    printf("insertAt(2, 25) 返回 %d\n", result);
    printList(&list);
    
    int removed;
    result = removeAt(&list, 1, &removed);
    printf("removeAt(1) 返回 %d, 删除的值是 %d\n", result, removed);
    printList(&list);
    
    result = insertAt(&list, 10, 99);
    printf("insertAt(10, 99) 返回 %d\n", result);
    printList(&list);
    
    result = removeAt(&list, 4, &removed);
    printf("removeAt(4) 返回 %d\n", result);
    printList(&list);
    
    return 0;
}
