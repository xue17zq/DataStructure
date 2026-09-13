#include <stdio.h>
#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;

//练习1：遍历打印
void printList(const SeqList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        if(i > 0)
            printf(" ");
        printf("%d", list->data[i]);
    }
    printf("\n");
}

//练习2：插入
int insertAt(SeqList *list, int index, int value)
{
    //满了 或者下标非法，返回0
    if (list->size == MAX_SIZE || index < 0 || index > list->size)
    {
        return 0;
    }
    //从后往前移动元素
    for(int i = list->size; i > index; i--)
    {
        list->data[i] = list->data[i-1];
    }
    list->data[index] = value;
    list->size = list->size + 1;
    return 1;
}

//练习3：删除
int removeAt(SeqList *list, int index, int *removed)
{
    //下标非法返回0
    if(index < 0 || index >= list->size)
    {
        return 0;
    }
    *removed = list->data[index];
    //后面元素前移
    for(int i = index; i < list->size - 1; i++)
    {
        list->data[i] = list->data[i+1];
    }
    list->size = list->size - 1;
    return 1;
}

int main()
{
    SeqList list;

    list.size = 4;
    list.data[0] = 10;
    list.data[1] = 20;
    list.data[2] = 30;
    list.data[3] = 40;
    printf("初始: ");
    printList(&list);

    int ret;
    ret = insertAt(&list, 2, 25);
    printf("insertAt(2, 25) 返回 %d\n", ret);
    printList(&list);

    int removed;
    ret = removeAt(&list, 1, &removed);
    printf("removeAt(1) 返回 %d, 删除的值是 %d\n", ret, removed);
    printList(&list);

    ret = insertAt(&list,10,99);
    printf("insertAt(10, 99) 返回 %d\n", ret);
    printList(&list);

    ret = removeAt(&list,4,&removed);
    printf("removeAt(4) 返回 %d\n", ret);
    printList(&list);

    return 0;
}