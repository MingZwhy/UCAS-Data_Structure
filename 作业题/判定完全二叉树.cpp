/*该处实际上用的队列数据结构
但懒得改stack的名字了*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INITSIZE 100
#define bool int
#define true 1
#define false 0

typedef struct{
    char data;
    int lchild;
    int rchild;
}node, *ptr_node;

typedef struct{
    ptr_node base;
    ptr_node top;
    int Stacksize;
}stack, *ptr_stack;

ptr_stack Create_Stack(void)
{
    ptr_stack s=(ptr_stack)malloc(sizeof(stack)*1);
    if(s==NULL) exit(1);
    s->base=(ptr_node)malloc(sizeof(node)*INITSIZE);
    if(s->base==NULL)   exit(1);
    s->top=s->base;
    s->Stacksize=INITSIZE;
    return s;
}

bool IsEmpty(ptr_stack s)
{
    return (s->base==s->top);
}

void Push(ptr_stack s, node x)
{
    s->top->data=x.data;
    s->top->lchild=x.lchild;
    s->top->rchild=x.rchild;
    (s->top)++;
}

node Top(ptr_stack s)
{
    return *(s->base);
}

node Pop(ptr_stack s)
{
    (s->base)++;
    return *(s->base-1);
}

int main(){
    ptr_node tnode=(ptr_node)malloc(sizeof(node)*1000);
    if(tnode==NULL) exit(1);
    char c=' ';
    int i=0;
    while(c!=EOF){
        scanf("%c ",&tnode[i].data);
        scanf("%d ",&tnode[i].lchild);
        scanf("%d",&tnode[i].rchild);
        i++;
        c=getchar();
    }
    ptr_stack s=Create_Stack();
    Push(s,tnode[0]);
    bool flag=false;
    while(!IsEmpty(s)){
        node temp=Pop(s);
        if(flag){
            if(temp.lchild==-1 && temp.rchild==-1){
                ;
            }
            else{
                printf("No");
                return 0;
            }
        }
        else{
            if(temp.lchild!=-1 && temp.rchild!=-1){
                Push(s,tnode[temp.lchild-1]);
                Push(s,tnode[temp.rchild-1]);
            }
            else if(temp.lchild!=-1){
                Push(s,tnode[temp.lchild-1]);
                flag=true;   //进入第二阶段
            }
            else if(temp.rchild!=-1){
                printf("No");
                return 0;
            }
            else{
                flag=true;
            }
        }
    }
    printf("Yes");
    return 0;
}