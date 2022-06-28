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

Root Mid_Post_CreatrTree(char *midorder, int left_mid, int right_mid, char *postorder, int left_post, int right_post)
{
    if(left_mid>right_mid || left_post>right_post){
        return NULL;
    }
    else{
        BiTree node=(BiTree)malloc(sizeof(TNode)*1);
        if(node==NULL)  exit(1);
        char target=postorder[right_post];
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
        node->lchild=Mid_Post_CreatrTree(midorder,left_mid,mid_index-1,postorder,left_post,left_post+number_left-1);
        node->rchild=Mid_Post_CreatrTree(midorder,mid_index+1,right_mid,postorder,left_post+number_left,right_post-1);
        return node;
    }
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

void preorder(Root r)
{
    if(r){
        printf("%c",r->data);
        preorder(r->lchild);
        preorder(r->rchild);
    }
}

int main(){
    char post[100];
    char mid[100];
    printf("input the mid order:\n");
    scanf("%s",mid);
    printf("input the post order:\n");
    scanf("%s",post);
    Root T=Mid_Post_CreatrTree(mid,0,strlen(mid)-1,post,0,strlen(post)-1);
    printf("the result of pre visit is :");
    preorder(T);
    printf("\n");
    draw(T);
    return 0;
}