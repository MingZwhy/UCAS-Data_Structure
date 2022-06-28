#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STR_SIZE 30

typedef struct Tnode{
    char data;
    struct Tnode *lchild;
    struct Tnode *rchild;
}TNode, *BiTree;

typedef BiTree Root;

Root In_Post_CreatrTree(char *preorder, int left_pre, int right_pre, char *postorder, int left_post, int right_post)
{
    BiTree node=(BiTree)malloc(sizeof(TNode)*1);
    if(node==NULL)  exit(1);
    if(left_pre==right_pre || left_post==right_post){
        node->data=preorder[left_pre];
        node->lchild=NULL;
        node->rchild=NULL;
    }
    else{
        node->data=preorder[left_pre];
        char target=preorder[left_pre+1];
        int mid_index;   //在后序遍历中根的左子的位置
        int number_left; //左子树节点个数
        int number_right; //右子树节点个数
        bool flag=false;
        for(mid_index=left_post;mid_index<=right_post;mid_index++){
            if(postorder[mid_index]==target){
                flag=true;
                break;
            }
        }
        if(!flag){
            printf("order error, please input right order!");
            exit(1);
        }
        number_left=mid_index-left_post+1;
        node->lchild=In_Post_CreatrTree(preorder,left_pre+1,left_pre+number_left,postorder,left_post,left_post+number_left-1);
        node->rchild=In_Post_CreatrTree(preorder,left_pre+number_left+1,right_pre,postorder,left_post+number_left,right_post-1);
    }
    return node;
}

void draw_level(BiTree node, bool left, char* str){
	if(node->rchild){
		draw_level(node->rchild, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->data);
	
	if(node->lchild){
		draw_level(node->lchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//长度都为6
	str[strlen(str)-6] = '\0'; 
} 

void draw(BiTree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->rchild){
		draw_level(root->rchild, false, str);
	}
	printf("%c\n",root->data);  //打印根节点的数据
	if(root->lchild){
		draw_level(root->lchild, true, str);
	} 
}

void midorder(Root r)
{
    if(r){
        midorder(r->lchild);
        printf("%c",r->data);
        midorder(r->rchild);
    }
}

int main(){
    char post[100];
    char pre[100];
    printf("input the pre order:\n");
    scanf("%s",pre);
    printf("input the post order:\n");
    scanf("%s",post);
    Root T=In_Post_CreatrTree(pre,0,strlen(pre)-1,post,0,strlen(post)-1);
    printf("the result of mid visit is :");
    midorder(T);
    printf("\n");
    draw(T);
    return 0;
}