#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define bool int
#define true 1
#define false 0

typedef struct node{
    char data;
    struct node* lchild;
    struct node* rchild;
}Tnode, *Tree;

typedef Tree Position;
typedef Tree Root;

Root Create_Tree(char *s, int *index)
{
    if(s[*index]=='^'){
        (*index)++;
        return NULL;
    }
    else{
        Root T=(Root)malloc(sizeof(Tnode)*1);
        if(T==NULL){
            printf("malloc error");
            exit(1);
        }
        T->data=s[*index];
        (*index)++;
        T->lchild=Create_Tree(s,index);
        T->rchild=Create_Tree(s,index);
        return T;
    }
}

int Find_Dad(Root T, char *dad, char target1, char target2, bool *flag)
{
    if(T==NULL || *flag==true){
        return 0;
    }
    else{
        int left=Find_Dad(T->lchild,dad,target1,target2,flag);
        int right=Find_Dad(T->rchild,dad,target1,target2,flag);
        if(left+right==2){
            *dad=T->data;
            *flag=true;
            return 0;
        }
        else{
            if(T->data==target1 || T->data==target2){
                return left+right+1;
            }
            else{
                return left+right;
            }
        }
    }
}

int main(){
    char s[1000];
    char target1, target2;
    scanf("%s",s);
    char eat=getchar();
    scanf("%c %c",&target1,&target2);
    int index=0;
    Root T=Create_Tree(s,&index);
    char dad;
    bool flag=false;
    int r=Find_Dad(T,&dad,target1,target2,&flag);
    if(r==2){
        dad=T->data;
    }
    printf("%c",dad);
    return 0;
}