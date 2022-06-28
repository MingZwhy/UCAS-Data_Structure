#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STR_SIZE 20
#define Init_Stack_Size 100
#define ADD_Stack_Szie 10

typedef struct Node{
    char data;
    struct Node *lchild;
    struct Node *rchild;
}BTNode, *Ptr_To_BTNode;

typedef Ptr_To_BTNode Root;
typedef Ptr_To_BTNode Position;

typedef struct{
    Position *base;
    Position *top;
    int Stacksize;
}Stack, *Ptr_To_Stack;

Ptr_To_Stack Create_Stack(void);
bool IsEmpty(Ptr_To_Stack s);
void Push(Ptr_To_Stack s, Position node);
Position Top(Ptr_To_Stack s);
Position Pop(Ptr_To_Stack s);
void Destroy_Stack(Ptr_To_Stack s);

Root Create_BiTree(Root T, int *s, int *index);
void Mid_Order_Visit(Root T);
void Post_Order_Visit(Root T);
void Pre_Order_Visit(Root T);
void draw_level(BTNode* node, bool left, char* str);
void draw(BTNode* root);

Ptr_To_Stack Create_Stack(void)
{
    Ptr_To_Stack s=(Ptr_To_Stack)malloc(sizeof(Stack)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->base=(Position *)malloc(sizeof(Position)*Init_Stack_Size);
    if(s->base==NULL){
        printf("malloc error");
        exit(1);
    }
    s->top=s->base;
    s->Stacksize=Init_Stack_Size-1;        //实际存储容量为SIZE-1,因为s->base[0]是没用上的
    return s;
}

bool IsEmpty(Ptr_To_Stack s)
{
    return (s->base==s->top)? true : false;
}

void Push(Ptr_To_Stack s, Position node)
{
    if(s->top-s->base==s->Stacksize){
        s->base=(Position *)realloc(s->base,sizeof(Position)*(1+s->Stacksize+ADD_Stack_Szie));
        if(s->base==NULL){
            printf("malloc error");
            exit(1);
        }
        s->top=s->base+s->Stacksize;
        s->Stacksize+=ADD_Stack_Szie;
    }
    s->top++;
    *(s->top)=node;
}

Position Top(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty");
        return NULL;
    }
    else{
        return *s->top;
    }
}

Position Pop(Ptr_To_Stack s)
{
    if(IsEmpty(s)){
        printf("the stack is empty");
        return NULL;
    }
    else{
        Position temp = *s->top;
        s->top--;
        return temp;
    }
}

void Destroy_Stack(Ptr_To_Stack s)
{
    free(s->base);
    s->top=s->base=NULL;
}

Root Create_BiTree(Root T, char *s, int *index)
{
    if(s[*index]=='*'){
        (*index)++;
        return NULL;
    }
    else{
        T=(Position)malloc(sizeof(BTNode)*1);
        if(T==NULL){
            printf("malloc error");
            exit(1);
        }
        T->data=s[*index];
        (*index)++;
        T->lchild=Create_BiTree(T->lchild,s,index);
        T->rchild=Create_BiTree(T->rchild,s,index);
        return T;
    }
}

void Mid_Order_Visit(Root T)
{
    Ptr_To_Stack s=Create_Stack();
    printf("\nthe result of mid order visit is: ");
    Position visit=T;
    while(!(visit==NULL && IsEmpty(s)))
    {
        while(visit!=NULL){
            Push(s,visit);
            visit=visit->lchild;
        }     //until visit is NULL
        //begin to visit rightchild
        Position temp=Top(s);
        printf("%c ",temp->data);
        Pop(s);
        visit=temp->rchild;
    }
}

void Post_Order_Visit(Root T)
{
    Ptr_To_Stack s=Create_Stack();
    printf("\nthe result of post order visit is: ");
    Position visit=T;
    Position tag=NULL;  //tag means this node has been visited and printed
    while(visit!=NULL || !IsEmpty(s))
    {
        while(visit!=NULL){
            Push(s,visit);
            visit=visit->lchild;
        }
        visit=Top(s);
        if(visit->rchild==NULL || visit->rchild==tag){
            printf("%c ",visit->data);
            Pop(s);
            tag=visit;
            visit=NULL;    //visit has no child and has been visited and printed
        }
        else{
            visit=visit->rchild;
        }
    }
}

void Pre_Order_Visit(Root T)
{
    Ptr_To_Stack s=Create_Stack();
    printf("\nthe result of pre order visit is: ");
    Position visit=T;
    Push(s,visit);
    while(!IsEmpty(s))
    {
        visit=Pop(s);
        printf("%c ",visit->data);
        if(visit->rchild!=NULL){       //根据栈先入后出，后入先出的特点，我们希望后遍历右子树，故先让右子树入栈
            Push(s,visit->rchild);
        }
        if(visit->lchild!=NULL){
            Push(s,visit->lchild);
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
	Root BT;
	char test[]="ABDH***E**CF**G**";
	printf("test is: %s\n",test);
	int index=0;
	BT = Create_BiTree(BT,test,&index);
	draw(BT);  //水平画树 
	printf("Bitree built success");
	
    Pre_Order_Visit(BT);
    Mid_Order_Visit(BT);
    Post_Order_Visit(BT);
	
	return 0;
}