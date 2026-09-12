#include <stdio.h>
#define MAX_SIZE 100
typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;
void printList(const SeqList *list);
int insertAt(SeqList *list, int index, int value);
int removeAt(SeqList *list, int index, int *removed);
int main() {
    SeqList list;
    list.size=4;
    int i;
    list.data[0]=10;
    for (int i=1;i<list.size;i++)
        list.data[i]=10*(i+1);
    printf("初始：");
    printList(&list);
    int t=insertAt(&list, 2, 25);
    printf("insertAt(2, 25) 返回%d\n",t);
    printList(&list);
    int removed=0;
    int u=removeAt(&list, 1, &removed);
    printf("removeAt(1) 返回%d,删除的值是%d\n",u,removed);
    printList(&list);
    int y=insertAt(&list, 10, 99);
    printf("insertAt(10, 99) 返回%d\n",y);
    printList(&list);
    int h=removeAt(&list, 4, &removed);
    printf("removeAt(4) 返回%d\n",h);
    printList(&list);
    return 0;
}
void printList(const SeqList *list) {
    for (int i=0; i<list->size; i++) {
        printf("%d ", list->data[i]);
    }printf("\n");
}
int insertAt(SeqList *list, int index, int value) {
    if (list->size==MAX_SIZE || index > list->size) {
        return 0;
    }
    else {
        list->data[list->size]=0;
        for (int i=list->size-1; i>=index; i--) {
            list->data[i+1] = list->data[i];
        }
        list->data[index] = value;
        list->size++;
        return 1;
    }
}
int removeAt(SeqList *list, int index, int *removed) {
    if (list->size==MAX_SIZE || index > list->size-1) {
        return 0;
    }
    else {
        *removed=list->data[index];
        for (int i=index;i<list->size;i++) {
            list->data[i] = list->data[i+1];
        }
        list->size--;
        return 1;
    }
}