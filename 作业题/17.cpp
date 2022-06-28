#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INIT_SIZE 100
#define ADD_SIZE 10

typedef struct{
    char *base;
    char *top;
    int Stacksize;
}Stack, *Ptr_To_Stack;

Ptr_To_Stack Creat_Stack(void)
{
    Ptr_To_Stack s=(Ptr_To_Stack)malloc(sizeof(Stack)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->base=(char *)malloc(sizeof(char)*INIT_SIZE);
    if(s->base==NULL){
        printf("malloc error");
        exit(1);
    }
    s->top=s->base;
    s->Stacksize=INIT_SIZE;
    return s;
}

int IsEmpty(Ptr_To_Stack s)
{
    return (s->top==s->base)? 1 : 0;
}

void Push(Ptr_To_Stack s, char elem)
{
    if(s->top-s->base>=s->Stacksize){
        s->base=(char *)realloc(s->base,sizeof(char)*(s->Stacksize+ADD_SIZE));
        if(s->base==NULL){
            printf("malloc error");
            exit(1);
        }
        s->top=s->base+s->Stacksize;
        s->Stacksize+=ADD_SIZE;
    }
    *(s->top)=elem;
    (s->top)++;
}

char Top(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        return '0';
    }
    else{
        return *(s->top-1);
    }
}

void Pop(Ptr_To_Stack s)
{
    if(!IsEmpty(s)){
        s->top--;
    }
}

int mygetline(char *s ,int lim)
{
    int i,c;
    i=0;
    while (--lim>0 &&(c=getchar())!=EOF&&c!='\n')
        s[i++]=c;
    s[i]='\0';
    return i;
}

int main(){
    char *s=(char *)malloc(sizeof(char)*10000);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    int len=mygetline(s,10000);
    Ptr_To_Stack k=Creat_Stack();
    int i=0;
    while(i<len){
        if(s[i]=='('){
            Push(k,s[i]);
        }
        if(s[i]==')'){
            char c=Top(k);
            if(c!='('){
                printf("0");;
                return 0;
            }
            else{
                Pop(k);
            }
        }
        i++;
    }
    if(IsEmpty(k)){
        printf("1");
    }
    else{
        printf("0");
    }
    return 0;
}

