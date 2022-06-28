//左式堆支持合并、插入和DeleteMin
//与二叉堆相同，左式堆也具有结构特性与有序性，即左式堆有相同的堆序性质
//左式堆与二叉树的区别在于——左式堆不是理想平衡的，实际上是趋于非常不平衡的
//左式堆方便合并
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define STR_SIZE 100

struct TreeNode;
typedef struct TreeNode *PriorityQueue;
typedef int ElementType;

PriorityQueue Initialize(void);
ElementType FindMin(PriorityQueue H);
void SwapChildren(PriorityQueue H);
bool IsEmpty(PriorityQueue H);
PriorityQueue Merge(PriorityQueue H1, PriorityQueue H2);
static PriorityQueue Merge1(PriorityQueue H1, PriorityQueue H2);
PriorityQueue Insert1(ElementType X, PriorityQueue H);
PriorityQueue DeleteMin1(PriorityQueue H);

struct TreeNode
{
    ElementType Element;
    PriorityQueue Left;
    PriorityQueue Right;
    int Npl;
};

PriorityQueue Initialize()
{
    PriorityQueue H=(PriorityQueue)malloc(sizeof(struct TreeNode)*1);
    if(H==NULL)
    {
        printf("malloc error");
        exit(0);
    }
    H->Left=H->Right=NULL;
    H->Npl=0;
    H->Element=0;
    return H;
}

ElementType FindMin(PriorityQueue H)
{
    if(H!=NULL){
        return H->Element;
    }
    else{
        printf("PriorityQueue is empty");
        return 0;
    }
}

bool IsEmpty(PriorityQueue H){
    if(H->Left==NULL && H->Right==NULL){
        return true;
    }
    else{
        return false;
    }
}

void SwapChildren(PriorityQueue H)
{
    PriorityQueue temp=H->Left;
    H->Left=H->Right;
    H->Right=temp;
}

PriorityQueue Merge(PriorityQueue H1, PriorityQueue H2)
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

static PriorityQueue Merge1(PriorityQueue H1, PriorityQueue H2)
{
    //H1是根结点元素较小的那棵树
    if(H1->Left==NULL){
        //if H1->Left==NULL,so H1->Npl=0 and H1->Right=NULL
        H1->Left=H2;
    }
    else{
        H1->Right=Merge(H1->Right,H2);
        if(H1->Left->Npl<H1->Right->Npl){
            SwapChildren(H1);
        }
        //更新根节点的Npl
        //根据递归性质，每个改变双亲或儿子的结点都会更新Npl
        H1->Npl=H1->Right->Npl+1;
    }
    return H1;
}

PriorityQueue Insert1(ElementType X, PriorityQueue H)
{
    PriorityQueue SingleNode;
    SingleNode=(PriorityQueue)malloc(sizeof(struct TreeNode)*1);
    if(SingleNode==NULL){
        printf("malloc error");
        exit(0);
    }
    else{
        SingleNode->Element=X;
        SingleNode->Npl=0;
        SingleNode->Left=SingleNode->Right=NULL;
        H=Merge(SingleNode,H);
    }
    return H;
}

PriorityQueue DeleteMin1(PriorityQueue H)
{
    PriorityQueue LeftHeap,RightHeap;
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
void draw_level(PriorityQueue node, bool left, char* str){
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

void draw(PriorityQueue root){
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
    PriorityQueue H1=Initialize();
    H1->Element=3;
    int s1[7]={10,8,21,14,17,23,26};
    for(int i=0;i<7;i++){
        H1=Insert1(s1[i],H1);
    }
    draw(H1);
    printf("\n\n");
    PriorityQueue H2=Initialize();
    H2->Element=6;
    int s2[7]={12,7,18,24,37,18,33};
    for(int j=0;j<7;j++){
        H2=Insert1(s2[j],H2);
    }
    draw(H2);
    printf("\n\n");
    PriorityQueue H=Merge(H1,H2);
    draw(H);
    return 0;
}