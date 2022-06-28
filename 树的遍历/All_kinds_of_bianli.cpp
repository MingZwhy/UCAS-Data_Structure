#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MAX_SIZE 128
#define STR_SIZE 1024

typedef struct Node{   //定义二叉树 
	char data;
	struct Node *lchild;
	struct Node *rchild;
}BTNode; 

typedef struct Queue{
	int front;     //队头指针  
	int rear;      //队尾指针
	//这里队列里要存放的对象是二叉树，不像链表一样必须连好
	//所以我们的指针只要对应数组下标即可 
	BTNode* data[MAX_SIZE];   //存放队列内元素 
}SqQueue;

//队列函数
void initQueue(SqQueue** q); //先创立一级指针，传递给函数二级指针 
bool emptyQueue(SqQueue* q); //判断队列是否为空
bool enQueue(SqQueue* q, BTNode* node);  //入队
bool deQueue(SqQueue* q, BTNode** node); //出队，因为牵扯往外赋值，所以传递的是二级指针

void initQueue(SqQueue** q){
	(*q)=(SqQueue*)malloc(sizeof(SqQueue)*1);
	if(*q==NULL){
		printf("分配内存失败");
		exit(0);
	}
	(*q)->front=(*q)->rear=-1; //首尾指针均赋为1 
}

bool emptyQueue(SqQueue* q){
	if(q->front==q->rear){
		return true;
	}
	else{
		return false; //false代表队列不为空 
	}
}

bool enQueue(SqQueue* q, BTNode* node){
	//如果队列满了则入列失败返回false
	//when q->rear==MAX_SIZE-1，q->rear-0+1==MAX_SIZE 
	if(q->rear == MAX_SIZE - 1){
		return false;  
	}
	q->rear++;
	(q->data)[q->rear]=node; //入列
	return true; 
}

bool deQueue(SqQueue* q, BTNode** node){
	//如果队列为空则出列失败返回false
	if(q->front==q->rear){
		return false;
	}
	q->front++;
	(*node)=(q->data)[q->front];  //对于树的一级指针赋给一级指针
	return true; 
} 

//构建树函数
BTNode* CreatBiTree(BTNode* T, char* s, int *index);

BTNode* CreatBiTree(BTNode* T, char* s, int *index){
	if(s[*index]=='*'){
		(*index)++;
		return NULL;
	}
	else{
		T=(BTNode* )malloc(sizeof(BTNode)*1);
		T->data=s[*index];
		(*index)++;
		T->lchild = CreatBiTree(T->lchild,s,index);
		T->rchild = CreatBiTree(T->rchild,s,index);
        return T;
	}
}

//树的遍历函数
void preorder(BTNode* BT);
void midorder(BTNode* BT);
void postorder(BTNode* BT);
void levelorder(BTNode* BT);
//1:先序遍历
void preorder(BTNode* BT){
	if(BT!=NULL){
		printf("%c",BT->data);
		preorder(BT->lchild);
		preorder(BT->rchild);
	}
} 

//2:中序遍历
void midorder(BTNode* BT){
	if(BT!=NULL){
		midorder(BT->lchild);
		printf("%c",BT->data);
		midorder(BT->rchild);
	}
}

//3:后序遍历
void postorder(BTNode* BT){
	if(BT!=NULL){
		postorder(BT->lchild);
		postorder(BT->rchild);
		printf("%c",BT->data);
	}
} 

//4:层次遍历
void levelorder(BTNode* BT){
	SqQueue* q;		//定义队列 
	initQueue(&q);  //初始化队列 
	if(BT!=NULL){
		enQueue(q,BT);  //根节点先入队列 
	}
	//一层一层的把节点存入队列，当无子节点时就不再循环
	while(emptyQueue(q)==false){
		deQueue(q,&BT);    //出队时的节点 
		printf("%c",BT->data);   //输出该节点存储的值
		if(BT->lchild!=NULL){
			enQueue(q,BT->lchild);
		} 
		if(BT->rchild!=NULL){
			enQueue(q,BT->rchild);
		}
	} 
}

//水平画树
void draw_level(BTNode* node, bool left, char* str){
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

void draw(BTNode* root){
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

int main(){
	//例子：ABDH###E##CF##G##
	BTNode* BT;
	char test[]="ABDH***E**CF**G**";
	printf("对应前序遍历字符串为：%s\n",test);
	int index=0;
	BT = CreatBiTree(BT,test,&index);
	draw(BT);  //水平画树 
	printf("二叉树建立成功");
	
	printf("\n先序遍历结果：");
	preorder(BT);
	
	printf("\n中序遍历结果：");
	midorder(BT);
	
	printf("\n后序遍历结果：");
	postorder(BT);
	
	printf("\n层序遍历结果：");
	levelorder(BT);
	
	return 0;
}