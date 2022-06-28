#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    int data;
    struct node *lchild;
    struct node *rchild;
}TNode, *ptr_to_tnode;

typedef ptr_to_tnode Root;
typedef ptr_to_tnode Position;

Root Create_Tree(int *left, int *right, int *index, int right_i, int tag, Position *target, char pos)
{
    if(tag==1 && left[*index]==0){
        (*index)++;
        return NULL;
    }
    if(tag==2 && right[right_i]==0){
        return NULL;
    }
    Position root=(Position)malloc(sizeof(TNode)*1);
    if(root==NULL)  exit(1);
    if(tag==1){
        root->data=left[*index];
        (*index)++;
    }
    else{
        root->data=right[right_i];
    }
    if(root->data==pos){
        *target=root;
    }
    right_i=(*index);
    root->lchild=Create_Tree(left,right,index,right_i,1,target,pos);
    root->rchild=Create_Tree(left,right,index,right_i,2,target,pos);
    return root;
}

void Merge(Position basic, Root add)
{
    add->rchild=basic->lchild;
    basic->lchild=add;
}

void Mid_order_visit(Root T, int *s, int *index)
{
    if(T==NULL){
        return;
    }
    else{
        Mid_order_visit(T->lchild,s,index);
        s[*index]=T->data;
        (*index)++;
        Mid_order_visit(T->rchild,s,index);
    }
}

int main(){
    int left[100];
    int right[100];
    int in_left[100];
    int in_right[100];
    int pos;
    int i=0;
    char c=' ';
    while(c!='\n'){
        scanf("%d",&left[i]);
        i++;
        c=getchar();
    }
    c=' ';
    i=0;
    while(c!='\n'){
        scanf("%d",&right[i]);
        i++;
        c=getchar();
    }
    scanf("%d",&pos);
    c=getchar();
    c=' ';
    i=0;
    while(c!='\n'){
        scanf("%d",&in_left[i]);
        i++;
        c=getchar();
    }
    c=' ';
    i=0;
    while(c!='\n'){
        scanf("%d",&in_right[i]);
        i++;
        c=getchar();
    }
    Position target;
    int index=0;
    Root add=Create_Tree(in_left,in_right,&index,0,1,&target,pos);
    index=0;
    Root basic=Create_Tree(left,right,&index,0,1,&target,pos);
    Merge(target,add);
    int s[1000];
    index=0;
    Mid_order_visit(basic,s,&index);
    for(i=0;i<index;i++){
        if(i!=index-1){
            printf("%d ",s[i]);
        }
        else{
            printf("%d",s[i]);
        }
    }
    return 0;
}