#include<stdio.h>
#define bool int
#define true 1
#define false 0
#define STR_SIZE 1024

typedef int ElemType; 

typedef struct TreeNode{
    ElemType data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
    int height;
}A_Tree, *AVLTree;

typedef AVLTree root;
typedef AVLTree TNode;

//直接调用draw就行

//画树函数，其中draw_level是draw的子函数
void draw_level(AVLTree node, bool left, char* str){
	if(node->rchild){
		draw_level(node->rchild, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	if(sizeof(ElemType)==4){
        printf("%d\n", node->data);
    }
    else{
        printf("%c\n", node->data);
    }
	
	if(node->lchild){
		draw_level(node->lchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(AVLTree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->rchild){
		draw_level(root->rchild, false, str);
	}

	if(sizeof(ElemType)==4){
        printf("%d\n", root->data);
    }
    else{
        printf("%c\n", root->data);
    }

	if(root->lchild){
		draw_level(root->lchild, true, str);
	} 
}