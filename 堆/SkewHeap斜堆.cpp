//斜堆是左式堆的自调节形式
//它也是具有堆序的二叉树，支持高效合并操作
//斜堆不一定满足左式堆npl的性质，同时
//当然也不需要附加的空间来保留路径长及不需要测试何时交换儿子
//换言之，无论需不需要，它都交换儿子

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define STR_SIZE 100

struct TreeNode;
typedef struct TreeNode *SkewHeap;
typedef int ElementType;

SkewHeap Initialize(ElementType X);
ElementType FindMin(SkewHeap H);
void SwapChildren(SkewHeap H);
bool IsEmpty(SkewHeap H);
SkewHeap Merge(SkewHeap H1, SkewHeap H2);
static SkewHeap Merge1(SkewHeap H1, SkewHeap H2);
SkewHeap Insert1(ElementType X, SkewHeap H);
SkewHeap DeleteMin1(SkewHeap H);

struct TreeNode
{
    ElementType Element;
    SkewHeap Left;
    SkewHeap Right;
};

SkewHeap Initialize(ElementType X)
{
    SkewHeap H=(SkewHeap)malloc(sizeof(struct TreeNode)*1);
    if(H==NULL)
    {
        printf("malloc error");
        exit(0);
    }
    H->Left=H->Right=NULL;
    H->Element=X;
    return H;
}

ElementType FindMin(SkewHeap H)
{
    if(H!=NULL){
        return H->Element;
    }
    else{
        printf("PriorityQueue is empty");
        return 0;
    }
}

bool IsEmpty(SkewHeap H){
    return H==NULL;
}

void SwapChildren(SkewHeap H)
{
    SkewHeap temp=H->Left;
    H->Left=H->Right;
    H->Right=temp;
}

SkewHeap Merge(SkewHeap H1, SkewHeap H2)
{
    if(H1==NULL){
        return H2;
    }
    if(H2==NULL){
        return H1;
    }
    if(H1->Element<H2->Element){
        return Merge1(H1,H2);
    }
    else{
        return Merge1(H2,H1);
    }
}

static SkewHeap Merge1(SkewHeap H1, SkewHeap H2)
{
    //H1是根结点元素较小的那棵树
    if(H1->Left==NULL){
        //if H1->Left==NULL,so H1->Npl=0 and H1->Right=NULL
        H1->Left=H2;
    }
    else{
        H1->Right=Merge(H1->Right,H2);
        //不用测试条件，无条件交换子树
        SwapChildren(H1);
    }
    return H1;
}

SkewHeap Insert1(ElementType X, SkewHeap H)
{
    SkewHeap SingleNode;
    SingleNode=(SkewHeap)malloc(sizeof(struct TreeNode)*1);
    if(SingleNode==NULL){
        printf("malloc error");
        exit(0);
    }
    else{
        SingleNode->Element=X;
        SingleNode->Left=SingleNode->Right=NULL;
        H=Merge(SingleNode,H);
    }
    return H;
}

SkewHeap DeleteMin1(SkewHeap H)
{
    SkewHeap LeftHeap,RightHeap;
    if(IsEmpty(H)){
        printf("PriorityQueue is empty");
        return H;
    }
    LeftHeap=H->Left;
    RightHeap=H->Right;
    free(H);
    return Merge(LeftHeap,RightHeap);
}

//画树函数，其中draw_level是draw的子函数
void draw_level(SkewHeap node, bool left, char* str){
	if(node->Right!=NULL){
		draw_level(node->Right, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%d\n", node->Element);
	
	if(node->Left!=NULL){
		draw_level(node->Left,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(SkewHeap root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->Right!=NULL){
		draw_level(root->Right, false, str);
	}
	printf("%d\n",root->Element);  //鎵撳嵃鏍硅妭鐐圭殑鏁版嵁
	if(root->Left!=NULL){
		draw_level(root->Left, true, str);
	} 
}

int main(){
    SkewHeap H1=Initialize(3);
    int s1[7]={10,8,21,14,17,23,26};
    for(int i=0;i<7;i++){
        H1=Insert1(s1[i],H1);
    }
    draw(H1);
    printf("\n\n");
    SkewHeap H2=Initialize(6);
    int s2[7]={12,7,18,24,37,18,33};
    for(int j=0;j<7;j++){
        H2=Insert1(s2[j],H2);
    }
    draw(H2);
    printf("\n\n");
    SkewHeap H=Merge(H1,H2);
    draw(H);
    return 0;
}