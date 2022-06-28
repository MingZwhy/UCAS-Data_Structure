#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INIT_STACK_SIZE 100
#define ADD_STACK_SIZE 10

typedef struct{
    int *base;
    int *top;
    int Stacksize;
}Stack, *Ptr_To_Stack;

Ptr_To_Stack Create_Stack(void)
{
    Ptr_To_Stack s=(Ptr_To_Stack)malloc(sizeof(Stack)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->base=(int *)malloc(sizeof(int)*(INIT_STACK_SIZE));
    if(s->base==NULL){
        printf("malloc error");
        exit(1);
    }
    s->top=s->base;
    s->Stacksize=INIT_STACK_SIZE-1;
    return s;
}

bool IsEmpty(Ptr_To_Stack s)
{
    return (s->top==s->base)? true : false;
}

void Push(Ptr_To_Stack s, int elem)
{
    if(s->top-s->base==s->Stacksize){
        s->base=(int *)realloc(s->base,sizeof(int)*(1+s->Stacksize+ADD_STACK_SIZE));
        if(s->base==NULL){
            printf("malloc error");
            exit(1);
        }
        s->top=s->base+s->Stacksize;
        s->Stacksize+=ADD_STACK_SIZE;
    }
    s->top++;
    *(s->top)=elem;
}

int Top(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty, top fail");
        return 0;
    }
    else{
        return *(s->top);
    }
}

void Pop(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty, pop fail");
    }
    else{
        s->top--;
    }
}

Ptr_To_Stack Dizeng_Stack(int *n, int size)
{
    Ptr_To_Stack s=Create_Stack();
    int i=0;
    while(i<size)
    {
        if(IsEmpty(s) || n[i]>=Top(s)){
            Push(s,n[i]);
        }
        else{
            while(!IsEmpty(s) && Top(s)>n[i]){
                Pop(s);          //according to specific requests, can treat the node poped this place
            }
            Push(s,n[i]);
        }
    }
    return s;
    //实际情况下，用递增栈时往往是要处理一个动态的过程，因此往往不仅想要返回的栈，而是想利用过程中入栈出栈的顺序
    //递增栈保证了永远是相对大/优先级更高的元素先出栈
}

Ptr_To_Stack Dijian_Stack(int *n, int size)
{
    Ptr_To_Stack s=Create_Stack();
    int i=0;
    while(i<size)
    {
        if(IsEmpty(s) || n[i]<=Top(s)){
            Push(s,n[i]);
        }
        else{
            while(!IsEmpty(s) && Top(s)<n[i]){
                Pop(s);          //according to specific requests, can treat the node poped this place
            }
            Push(s,n[i]);
        }
    }
    return s;
    //实际情况下，用递减栈时往往是要处理一个动态的过程，因此往往不仅想要返回的栈，而是想利用过程中入栈出栈的顺序
    //递减栈保证了永远是相对小/优先级更低的元素先出栈
}
