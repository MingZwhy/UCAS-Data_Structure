//N叉树的三种遍历方法
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 4
#define INITSIZE 100
#define ADDSIZE 10
#define bool int
#define true 1
#define false 0

typedef struct SStree{   //son-sibling tree
    char data;
    struct SStree *firstson;
    struct SStree *sibling;
}SS_Tree, *Ptr_To_SSTree;

typedef Ptr_To_SSTree Root_of_S;
typedef Ptr_To_SSTree Position_of_S;

//N叉树
typedef struct N_tree{
    char data;
    int numberofson;   //节点的儿子数   
    struct N_tree **Sons;
}N_Tree, *Ptr_To_NTree;

typedef Ptr_To_NTree Root_of_NT;
typedef Ptr_To_NTree Position_of_NT;

//非递归遍历要用到的栈
typedef struct{
    Position_of_NT *base;
    Position_of_NT *top;
    int Stacksize;
}Stack, *Ptr_To_Stack;

Ptr_To_Stack Create_Stack(void);
bool IsEmpty(Ptr_To_Stack s);
void Push(Ptr_To_Stack s, Position_of_NT node);
Position_of_NT Top(Ptr_To_Stack s);
Position_of_NT Pop(Ptr_To_Stack s);

Ptr_To_Stack Create_Stack(void)
{
    Ptr_To_Stack s=(Ptr_To_Stack)malloc(sizeof(Stack)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->base=(Position_of_NT *)malloc(sizeof(Position_of_NT)*INITSIZE);
    if(s->base==NULL){
        printf("malloc error");
        exit(1);
    }
    s->top=s->base;
    s->Stacksize=INITSIZE;
    return s;
}

bool IsEmpty(Ptr_To_Stack s)
{
    return (s->base==s->top);
}

void Push(Ptr_To_Stack s, Position_of_NT node)
{
    if(s->top-s->base>=s->Stacksize){
        s->base=(Position_of_NT *)realloc(s->base,sizeof(Position_of_NT)*(s->Stacksize+ADDSIZE));
        if(s->base==NULL){
            printf("realloc error");
            exit(1);
        }
        s->top=s->base+s->Stacksize;
        s->Stacksize+=10;
    }
    *(s->top)=node;
    (s->top)++;
}

Position_of_NT Top(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty");
        return NULL;
    }
    else{
        return *(s->top-1);
    }
}

Position_of_NT Pop(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty");
        return NULL;
    }
    else{
        (s->top)--;
        return *(s->top);
    }
}

//对N叉树的前序和后序遍历，共有三种方法

//法1：递归遍历法
void Digui_pre_Visit_NT(Root_of_NT NT)
{
    if(NT==NULL){
        return;
    }
    else{
        printf("%c",NT->data);
        int i;
        for(i=0;i<NT->numberofson;i++){
            Digui_pre_Visit_NT(NT->Sons[i]);
        }
    }
}

void Digui_post_Visit_NT(Root_of_NT NT)
{
    if(NT==NULL){
        return;
    }
    else{
        int i;
        for(i=0;i<NT->numberofson;i++){
            Digui_post_Visit_NT(NT->Sons[i]);
        }
        printf("%c",NT->data);
    }
}

//法2:非递归遍历,用栈
void NoDigui_pre_Visit_NT(Root_of_NT NT)
{
    if(NT==NULL){
        return;
    }
    else{
        Ptr_To_Stack s=Create_Stack();
        Push(s,NT);
        Position_of_NT visit=NT;
        int i=0;
        while(!IsEmpty(s)){
            visit=Pop(s);
            printf("%c",visit->data);
            if(visit->numberofson>0){
                int j=visit->numberofson-1;
                for(;j>=0;j--){
                    if(visit->Sons[j]!=NULL){
                        Push(s,visit->Sons[j]);
                    }
                }
            }
        }
    }
}

//注意非递归的后序遍历毁坏了树
void NoDigui_post_Visit_NT(Root_of_NT NT)
{
    Ptr_To_Stack s=Create_Stack();
    Position_of_NT visit=NT;
    while(visit!=NULL || !IsEmpty(s))
    {
        while(visit!=NULL){
            Push(s,visit);
            visit=visit->Sons[0];
        }
        visit=Top(s);
        int j;
        bool flag=true;
        for(j=1;j<visit->numberofson;j++){
            if(visit->Sons[j]!=NULL){
                Position_of_NT temp=visit;
                visit=visit->Sons[j];
                temp->Sons[j]=NULL;
                flag=false;
                break;
            }
        }
        if(flag){   //flag=true时visit无儿子需要遍历了
            printf("%c",visit->data);
            Pop(s);
            visit=NULL;
        }
    }
}

//法3: 将N叉树转换为对应二叉树再遍历二叉树
Root_of_S Trans_NTtoS(Root_of_NT NT)
{
    if(NT==NULL){
        return NULL;
    }
    else{
        Root_of_S S;
        S=(Root_of_S)malloc(sizeof(Position_of_S)*1);
        if(S==NULL){
            printf("malloc error");
            exit(1);
        }
        S->data=NT->data;
        S->firstson=Trans_NTtoS(NT->Sons[0]);
        S->sibling=NULL;
        if(S->firstson!=NULL){
            int i;
            Position_of_S visit=S->firstson;
            for(i=1;i<NT->numberofson;i++){
                visit->sibling=Trans_NTtoS(NT->Sons[i]);
                if(visit->sibling!=NULL){
                    visit=visit->sibling;
                }
                else{
                    break;
                }
            }
        }
        return S;
    }
}

void Pre_Visit_BiTree(Root_of_S S)
{
    if(S==NULL){
        return;
    }
    else{
        printf("%c",S->data);
        Pre_Visit_BiTree(S->firstson);
        Pre_Visit_BiTree(S->sibling);
    }
}

void Mid_Visit_BiTree(Root_of_S S)
{
    if(S==NULL){
        return;
    }
    else{
        Mid_Visit_BiTree(S->firstson);
        printf("%c",S->data);
        Mid_Visit_BiTree(S->sibling);
    }
}

//树的先序遍历跟其对应的二叉树先序遍历一样
void Trans_Pre_Visit_NTrees(Root_of_NT NT)
{
    Root_of_S S=Trans_NTtoS(NT);
    Pre_Visit_BiTree(S);
}

//树的后序遍历跟其对应的二叉树中序遍历一样
void Trans_Post_Visit_NTrees(Root_of_NT NT)
{
    Root_of_S S=Trans_NTtoS(NT);
    Mid_Visit_BiTree(S);
}

int main(){
    N_Tree T[7];
    T[0].data='A'; T[0].numberofson=3;
    T[0].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*3);
    T[0].Sons[0]=&T[1]; T[0].Sons[1]=&T[2]; T[0].Sons[2]=&T[3];

    T[1].data='B'; T[1].numberofson=2;
    T[1].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*2);
    T[1].Sons[0]=&T[4]; T[1].Sons[1]=&T[5];

    T[2].data='C'; T[2].numberofson=0;
    T[2].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*1);
    T[2].Sons[0]=NULL;

    T[3].data='D'; T[3].numberofson=1;
    T[3].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*1);
    T[3].Sons[0]=&T[6];

    T[4].data='E'; T[4].numberofson=0;
    T[4].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*1);
    T[4].Sons[0]=NULL;

    T[5].data='F'; T[5].numberofson=0;
    T[5].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*1);
    T[5].Sons[0]=NULL;

    T[6].data='G'; T[6].numberofson=0;
    T[6].Sons=(Root_of_NT *)malloc(sizeof(Root_of_NT)*1);
    T[6].Sons[0]=NULL;

    Root_of_NT NT=&T[0];
    printf("\npre way1 visit: ");
    Digui_pre_Visit_NT(NT);
    printf("\npre way2 visit: ");
    NoDigui_pre_Visit_NT(NT);
    printf("\npre way3 visit: ");
    Trans_Pre_Visit_NTrees(NT);

    printf("\npost way1 visit: ");
    Digui_post_Visit_NT(NT);
    //先3再2是因为NoDigui_post_Visit_NT会毁坏树
    printf("\npost way3 visit: ");
    Trans_Post_Visit_NTrees(NT);
    printf("\npost way2 visit: ");
    NoDigui_post_Visit_NT(NT);
    
    return 0;
}