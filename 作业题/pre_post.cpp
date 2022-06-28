#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct TreeNode
{
 char val;
 struct TreeNode *left,*right;
}Binode,*Bitree;
/*根据中序和后续创建二叉树*/ 
void Traversal(struct TreeNode**  root,char* preorder, int preorderSize, char* postorder, int postorderSize)
{
    int i;
    int rootValue = preorder[0];
    int count = 0;
    *root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    (*root)->left   = NULL;
    (*root)->right  = NULL;
    (*root)->val    = rootValue;
    
    if (preorderSize > 1)
    {
        int lefeRootValue = preorder[1];
        for (i = 0; i < postorderSize; i++)
        {
            count++;//左子树的数量
            if (postorder[i] == lefeRootValue)
            {
                //找到符合的值
                break;
            }
        }
    }
    //左子树
    if (count > 0)
    {
        int leftNum = count;
        char* newPreorder = (char*)malloc(sizeof(char) * leftNum);
        memcpy(newPreorder, preorder + 1, sizeof(char) * leftNum);

        char* newPostorder = (char*)malloc(sizeof(char) * leftNum);
        memcpy(newPostorder, postorder, sizeof(char) * leftNum);
        Traversal(&(*root)->left, newPreorder, leftNum, newPostorder, leftNum);

        free(newPostorder);
        free(newPreorder);
    }


    //右子树
    if ((count  + 1) < postorderSize)
    {
        int rightNum = postorderSize - count - 1;
        char* newPreorder = (char*)malloc(sizeof(char) * rightNum);
        memcpy(newPreorder, preorder + count + 1, sizeof(char) * rightNum);

        char* newPostorder = (char*)malloc(sizeof(char) * rightNum);
        memcpy(newPostorder, postorder + count, sizeof(char) * rightNum);
        Traversal(&(*root)->right, newPreorder, rightNum, newPostorder, rightNum);
        free(newPostorder);
        free(newPreorder);
    }
}


void draw_level(Bitree node, bool left, char* str){
	if(node->right){
		draw_level(node->right, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->val);
	
	if(node->left){
		draw_level(node->left,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(Bitree root){
	char str[100];
	memset(str, '\0', 100);
	
	if(root->right){
		draw_level(root->right, false, str);
	}
	printf("%c\n",root->val);  //鎵撳嵃鏍硅妭鐐圭殑鏁版嵁
	if(root->left){
		draw_level(root->left, true, str);
	} 
}

int main()
{
 char preorder[100];
 char postorder[100];
 int lowin,highin,lowpo,highpo;
 Bitree L=NULL;
 printf("input the preorder:\n");
 scanf("%s",preorder);
 printf("input the postorder:\n");
 scanf("%s",postorder);
 Traversal(&L,preorder,strlen(preorder),postorder,strlen(postorder));
 printf("\n");
 draw(L);
 return 0;
}