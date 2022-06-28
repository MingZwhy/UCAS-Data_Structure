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

Root In_Mid_CreatrTree(char *preorder, int left_pre, int right_pre, char *midorder, int left_mid, int right_mid)
{
    if(left_pre>right_pre || left_mid>right_mid){
        return NULL;
    }
    else{
        BiTree node=(BiTree)malloc(sizeof(TNode)*1);
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
        node->lchild=In_Mid_CreatrTree(preorder,left_pre+1,left_pre+number_left,midorder,left_mid,mid_index-1);
        node->rchild=In_Mid_CreatrTree(preorder,left_pre+number_left+1,right_pre,midorder,mid_index+1,right_mid);
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

void postorder(Root r)
{
    if(r){
        postorder(r->lchild);
        postorder(r->rchild);
        printf("%c",r->data);
    }
}

int main(){
    char pre[100];
    char mid[100];
    printf("input the pre order:\n");
    scanf("%s",pre);
    printf("input the mid order:\n");
    scanf("%s",mid);
    Root T=In_Mid_CreatrTree(pre,0,strlen(pre)-1,mid,0,strlen(mid)-1);
    printf("the result of post visit is :");
    postorder(T);
    printf("\n");
    draw(T);
    return 0;
}