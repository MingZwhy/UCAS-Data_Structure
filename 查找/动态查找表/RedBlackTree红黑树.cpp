#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define Ifinity 100000
#define Neginfinity -100000
#define STR_SIZE 1000

typedef int ElementType;
typedef enum ColorType{Red,Black} ColorType;
struct RedBlackNode;
typedef RedBlackNode* RedBlackTree;
typedef RedBlackTree Position;

struct RedBlackNode{
    ElementType Element;
    RedBlackTree Left;
    RedBlackTree Right;
    ColorType Color;
};

Position NullNode=NULL;  //Need initialization

RedBlackTree SingleRotateWithLeft(RedBlackTree T);
RedBlackTree SingleRotateWithRight(RedBlackTree T);
RedBlackTree Initialize(void);
static Position Rotate(ElementType Item, Position Parent);
static void HandleRerient(ElementType Item, RedBlackTree T);
RedBlackTree Insert(ElementType Item, RedBlackTree T);
//注意，因为使用了NullNode，在以下两个函数中判断条件中要用是否等于NullNode而非Null
void draw_level(RedBlackTree node, bool left, char* str);
void draw(RedBlackTree node);
static Position Rotate(ElementType Item, Position Parent);
static Position X, P, GP, GGP;

RedBlackTree SingleRotateWithLeft(RedBlackTree T){
    RedBlackTree Left_of_T;
    Left_of_T=T->Left;
    T->Left=Left_of_T->Right;
    Left_of_T->Right=T;
    //Left_of_T是新的根节点
    return Left_of_T;
}

RedBlackTree SingleRotateWithRight(RedBlackTree T){
    RedBlackTree Right_of_T;
    Right_of_T=T->Right;
    T->Right=Right_of_T->Left;
    Right_of_T->Left=T;
    //Right_of_T是新的根节点
    return Right_of_T;
}

RedBlackTree Initialize(void){
    RedBlackTree T;

    if(NullNode==NULL){
        NullNode=(RedBlackTree)malloc(sizeof(struct RedBlackNode)*1);
        if(NullNode==NULL){
            printf("malloc error");
            exit(0);
        }
        NullNode->Left=NullNode->Right=NullNode;
        NullNode->Color=Black;    //默认空结点颜色为黑
        NullNode->Element=Ifinity;
    }

    //Create the header node
    //T只是头指针，不储存结点，后面会令T->Right为树的根节点
    T=(RedBlackTree)malloc(sizeof(struct RedBlackNode)*1);;
    if(T==NULL){
        printf("malloc error");
        exit(0);
    }
    T->Element=Neginfinity;    //将头指针的值定为负无穷，遍历时自然会引导指针向T->Right走
    T->Left=T->Right=NullNode;
    T->Color=Black;         //根节点颜色为黑

    return T;
}

static Position Rotate(ElementType Item, Position Parent){
    if(Item<Parent->Element){
        if(Item<Parent->Left->Element){
            return Parent->Left=SingleRotateWithLeft(Parent->Left);
        }
        else{
            return Parent->Left=SingleRotateWithRight(Parent->Left);
        }
    }
    else{
        if(Item>Parent->Right->Element){
            return Parent->Right=SingleRotateWithRight(Parent->Right);
        }
        else{
            return Parent->Right=SingleRotateWithLeft(Parent->Right);
        }
    }
}

static void HandleRerient(ElementType Item, RedBlackTree T){
    X->Color=Red;              //交换颜色，变为红X有两个黑儿子
    X->Left->Color=Black;
    X->Right->Color=Black;

    if(P->Color==Red)  //have to rotate,在P和GP之间
    {
        GP->Color=Red;           //原本GP是黑的而P是红的，旋转之后要交换P和GP颜色
        if((Item<GP->Element)!=(Item<P->Element)){
            //该条件判断的意思是如果X,P,GP之间为之字形(当==时为一字形)
            //参考p363页之字形需要执行与AVL中相同次序的双旋，如果一字型则只需执行P与GP之间单旋
            P=Rotate(Item,GP);  //注意，传入的参数是GP，但旋转的是P和X之间，返回给的P是新的父节点
        }
        X=Rotate(Item,GGP);   //传入的参数是GGP，旋转的是GP和P之间
        //旋转GP和P之间之后，X指向的是GP节点，也就是说，经历HandleRerient后X上升了两层
        X->Color=Black;       //与GP->Color=Red首尾呼应，交换颜色
    }
    T->Right->Color=Black;    //Make root black
}

RedBlackTree Insert(ElementType Item, RedBlackTree T){
    X=P=GP=T;
    NullNode->Element=Item;
    while(X->Element!=Item){
        GGP=GP; GP=P; P=X;  //向下降落,注意不能改变赋值顺序，因为这样才能保证继承
        if(Item<X->Element){
            X=X->Left;
        }
        else{
            X=X->Right;
        }
        if(X->Left->Color==Red && X->Right->Color){   //X(儿子)有两个红儿子(P有两个红孙子)
            HandleRerient(Item,T);
        }
    }

    if(X!=NullNode){
        return NullNode;
    }

    X=(Position)malloc(sizeof(struct RedBlackNode)*1);
    if(X==NULL){
        printf("malloc error");
        exit(0);
    }
    X->Element=Item;
    X->Left=X->Right=NullNode;

    if(Item<P->Element){    //将X与其父母连接
        P->Left=X;
    }
    else{
        P->Right=X;
    }
    HandleRerient(Item,T);   //插入X后可能需要旋转，并且在调用此函数使会令X->Color=Red;

    return T;
}

//画树函数，其中draw_level是draw的子函数
void draw_level(RedBlackTree node, bool left, char* str){
	if(node->Right!=NullNode){
		draw_level(node->Right, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%d\n", node->Element);
	
	if(node->Left!=NullNode){
		draw_level(node->Left,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(RedBlackTree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->Right!=NullNode){
		draw_level(root->Right, false, str);
	}
	printf("%d\n",root->Element);  //鎵撳嵃鏍硅妭鐐圭殑鏁版嵁
	if(root->Left!=NullNode){
		draw_level(root->Left, true, str);
	} 
}

int main(){
    RedBlackTree T;
    T=Initialize();
    int s[14]={10,85,15,70,20,60,30,50,65,80,90,40,5,55};
    for(int i=0;i<14;i++){
        printf("\n%d\n",i);
        T=Insert(s[i],T);
        draw(T);
    }
    draw(T);
    return 0;
}
