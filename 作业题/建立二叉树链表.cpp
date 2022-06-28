#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define bool int
#define true 1
#define false 0

typedef struct Tnode{
    char data;
    struct Tnode *lchild;
    struct Tnode *rchild;
}Tnode, *BiTree;

typedef BiTree Root;

Root In_Mid_CreateTree(char *preorder, int left_pre, int right_pre,
                       char *midorder, int left_mid, int right_mid)
{
    if(left_pre>right_pre || left_mid>right_mid){
        return NULL;
    }
    else{
        BiTree node=(BiTree)malloc(sizeof(Tnode)*1);
        if(node==NULL)  exit(1);
        char target=preorder[left_pre];
        node->data=target;
        int mid_index;   //在中序遍历中根的位置
        int number_left; //左子树节点个数
        int number_right; //右子树节点个数
        bool flag=false;
        for(mid_index=left_mid;mid_index<=right_mid;mid_index++){
            if(midorder[mid_index]==target){
                flag=true;
                break;
            }
        }
        if(!flag){
            printf("order error, please input right order!");
            exit(1);
        }
        number_left=mid_index-left_mid;
        node->lchild=In_Mid_CreateTree(preorder,left_pre+1,left_pre+number_left,
                                       midorder,left_mid,mid_index-1);
        node->rchild=In_Mid_CreateTree(preorder,left_pre+number_left+1,right_pre,
                                       midorder,mid_index+1,right_mid);
        return node;
    }
}

void Post_order_visit(Root T)
{
    if(T==NULL){
        return;
    }
    else{
        Post_order_visit(T->lchild);
        Post_order_visit(T->rchild);
        printf("%c",T->data);
    }
}

int main(){
    char pre[1000];
    char mid[1000];
    scanf("%s",pre);
    char eat=getchar();
    scanf("%s",mid);
    Root T=In_Mid_CreateTree(pre,0,strlen(pre)-1,mid,0,strlen(mid)-1);
    Post_order_visit(T);
    return 0;
}