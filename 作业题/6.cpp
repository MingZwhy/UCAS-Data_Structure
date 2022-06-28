#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Lnode{
    int data;
    struct Lnode *next;
}LNode, *List;

typedef List Position;
//List 和 Position都是指向LNode的指针类型，其中List表示头结点，Position表示其他普通节点

int main(){
    //创建头结点
    List L=(List)malloc(sizeof(LNode)*1);
    if(L==NULL){
        printf("malloc error");
        exit(1);
    }
    L->next=NULL;
    Position visit=L;
    char c=' ';
    int n;

    //读入原有节点
    while(c!='\n'){
        scanf("%d",&n);
        Position temp=(Position)malloc(sizeof(LNode)*1);
        if(temp==NULL){
            printf("malloc error");
            exit(1);
        }
        temp->data=n;
        temp->next=NULL;
        visit->next=temp;
        visit=temp;
        c=getchar();
    }

    //因为在上面循环中，最后读入的c是第一行的回车，所以下一个读入的刚好是要插入的元素
    scanf("%d",&n);
    visit=L;  //重置visit使其指向头结点
    //通过循环使visit指向应当插入位置的前一个节点
    while(visit->next!=NULL && n>visit->next->data){
        visit=visit->next;
    }
    //visit已指向欲插入位置前一个节点，进行插入操作
    Position temp=(Position)malloc(sizeof(LNode)*1);
    if(temp==NULL){
        printf("malloc error");
        exit(1);
    }
    temp->data=n;
    temp->next=visit->next;
    visit->next=temp;
    visit=L->next;
    while(visit!=NULL){
        printf("%d ",visit->data);
        visit=visit->next;
    }
    return 0;
}