#include <stdio.h>

#define MAX_SIZE 100
typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

//遍历打印
void printList(const SeqList *list) {
    for(int i = 0; i < list->size; i++){
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

//插入：从后往前移动元素
int insertAt(SeqList *list, int index, int value) {
    //判满，下标范围[0, size]
    if(list->size == MAX_SIZE || index < 0 || index > list->size){
        return 0;
    }
    //从后向前移动
    for(int i = list->size; i > index; i--){
        list->data[i] = list->data[i-1];
    }
    list->data[index] = value;
    list->size++;
    return 1;
}

//删除：从前向后移动，先保存被删值
int removeAt(SeqList *list, int index, int *removed) {
    if(removed == NULL) return 0;
    //合法下标 [0,size-1]
    if(index <0 || index >= list->size){
        return 0;
    }
    *removed = list->data[index];
    //从前向后覆盖
    for(int i = index; i < list->size - 1; i++){
        list->data[i] = list->data[i+1];
    }
    list->size--;
    return 1;
}

int main() {
    SeqList list;
    list.size = 4;
    list.data[0]=10;
    list.data[1]=20;
    list.data[2]=30;
    list.data[3]=40;

    printf("初始: ");
    printList(&list);

    //插入下标2，值25
    int ret1 = insertAt(&list,2,25);
    printf("insertAt(2, 25) 返回 %d\n", ret1);
    printList(&list);

    //删除下标1
    int removed;
    int ret2 = removeAt(&list,1,&removed);
    printf("removeAt(1) 返回 %d, 删除的值是 %d\n", ret2, removed);
    printList(&list);

    //非法插入下标10
    int ret3 = insertAt(&list,10,99);
    printf("insertAt(10, 99) 返回 %d\n", ret3);
    printList(&list);

    //非法删除下标4
    int ret4 = removeAt(&list,4,&removed);
    printf("removeAt(4) 返回 %d\n", ret4);
    printList(&list);

    return 0;
}

