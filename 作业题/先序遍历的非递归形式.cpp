#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INITSIZE 100
#define ADDSIZE 10
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
    if(s->top-s->base>=s->Stacksize){
        s->base=(ptr_node)realloc(s->base,sizeof(ptr_node)*(s->Stacksize+ADDSIZE));
        if(s->base==NULL)   exit(1);
        s->top=s->base+s->Stacksize;
        s->Stacksize+=ADDSIZE;
    }
    s->top->data=x.data;
    s->top->lchild=x.lchild;
    s->top->rchild=x.rchild;
    (s->top)++;
}

node Top(ptr_stack s)
{
    return *(s->top-1);
}

node Pop(ptr_stack s)
{
    (s->top)--;
    return *(s->top);
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
    while(!IsEmpty(s)){
        node temp=Pop(s);
        printf("%c ",temp.data);
        if(temp.rchild!=-1){
            Push(s,tnode[temp.rchild-1]);
        }
        if(temp.lchild!=-1){
            Push(s,tnode[temp.lchild-1]);
        }
    }
    return 0;
}