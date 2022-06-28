#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define STR_SIZE 1024
typedef int ElementType;

struct SplayNode;
typedef struct SplayNode *SplayTree;

SplayTree MakeEmpty(SplayTree T);
SplayTree MakeTree(SplayTree T, int x);
SplayTree Find(ElementType X, SplayTree T);
SplayTree FindMin(SplayTree T);
SplayTree FindMax(SplayTree T);
SplayTree Splay(ElementType Item, SplayTree X);
SplayTree Insert(ElementType X, SplayTree T);
SplayTree Remove(ElementType X, SplayTree T);
ElementType Retrieve(SplayTree T);

void draw_level(SplayTree node, bool left, char* str);
void draw(SplayTree root);

//旋转函数
SplayTree SingleRotateWithLeft(SplayTree T);
SplayTree SingleRotateWithRight(SplayTree T);

struct SplayNode
{
    ElementType Element;
    SplayTree Left;
    SplayTree Right;
};
typedef SplayTree Position;

SplayTree MakeTree(SplayTree T, int x){
    if(T==NULL){
        T=(SplayTree)malloc(sizeof(struct SplayNode)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
        }
        T->Element=x;
        T->Left=NULL;
        T->Right=NULL;
    }
    else{
        if(x<T->Element){
            T->Left=MakeTree(T->Left,x);
        }
        else if(x>T->Element){
            T->Right=MakeTree(T->Right,x);
        }
        else{
            printf("x has existed");
        }
    }
    return T;
}


SplayTree SingleRotateWithLeft(SplayTree T){
    SplayTree Left_of_T;
    Left_of_T=T->Left;
    T->Left=Left_of_T->Right;
    Left_of_T->Right=T;
    //Left_of_T是新的根节点
    return Left_of_T;
}

SplayTree SingleRotateWithRight(SplayTree T){
    SplayTree Right_of_T;
    Right_of_T=T->Right;
    T->Right=Right_of_T->Left;
    Right_of_T->Left=T;
    //Right_of_T是新的根节点
    return Right_of_T;
}

SplayTree Splay(ElementType Item, Position X){
    static struct SplayNode Header;
    Position LeftTreeMax,RightTreeMin;
    
    Header.Left=Header.Right=NULL;
    LeftTreeMax=RightTreeMin=&Header;

    while(Item!=X->Element){
        if(Item<X->Element){
            if(X->Left==NULL){
                break;
            }
            if(Item<X->Left->Element){
                //一字型
                //先对X和X->Left进行左旋
                X=SingleRotateWithLeft(X);
                if(X->Left==NULL){
                    //如果翻转后无左结点，说明未找到
                    break;
                }
            }
            //如果上面的if未执行，说明是之字型，则省略了上面的翻转，直接把X接到R上去就行
            //将以X为根的子树接到R上去
            RightTreeMin->Left=X;     //接到右树的左子是因为按顺序后面接到右树上的结点元素肯定比先接到的小，因为沿左边走，越走越小

            //第一次执行RightTreeMin->Left=X时等同于Header.Left=X, 所以我们将右子树接到了Header.Left上，这样设置只是为了和后面的操作一致

            RightTreeMin=X;    //更新RightTreeMin到下一次该插入的位置
            //此时RightTreeMin的左子还连着原树上的子树，但不要紧，下回更新右树时又会更新RightTreeMin->Left，使其连接正确子树
            X=X->Left;         //将指针移动到最初时刻(未旋转)的X->Left->Left,若未执行旋转，则X->Left
        }
        else{
            if(X->Right==NULL){
                break;
            }
            if(Item>X->Right->Element){
                //一字型
                //先对X和X->Right进行右旋
                X=SingleRotateWithRight(X);
                if(X->Right==NULL){
                    //如果翻转后无右结点，说明未找到
                    break;
                }
            }
            //如果上面的if未执行，说明是之字形，则省略了上面的翻转，直接把X接到L上去就行
            //将以X为根的子树接到L上去
            LeftTreeMax->Right=X;   //接到左树的右子是因为按顺序后面接到左树上的结点元素肯定比先接到的大, 因为沿右边走，越走越大
            LeftTreeMax=X;    //更新LeftTreeMax到下一次该插入的位置
            //此时LeftTreeMax的右子还连着原树上的子树，但不要紧，下回更新右树时又会更新LeftTreeMax->Right，使其连接正确子树
            X=X->Right;       //将指针移动到最初时刻(未旋转)的X->Right->Right,若未执行旋转，则X->Right
        }
    }
    LeftTreeMax->Right=X->Left;
    RightTreeMin->Left=X->Right;
    X->Left=Header.Right;       //根据上面的连接可知，Header.Right实际上是左子树，反之Header.Left是右子树
    X->Right=Header.Left;

    return X;    //返回根节点
}

SplayTree Insert(ElementType X, SplayTree T){
    static Position NewNode=NULL;

    if(NewNode==NULL){
        NewNode=(Position)malloc(sizeof(struct SplayNode)*1);
        if(NewNode==NULL){
            printf("malloc error");
            exit(0);
        }
    }
    NewNode->Element=X;

    if(T==NULL){
        NewNode->Left=NewNode->Right=NULL;
        T=NewNode;
    }
    else{
        T=Splay(X,T);
        //依X展开了T，此时根结点含的值可能等于X
        //此时依据Splay算法性质可知，X的值一定在T和T->Left之间或T和T->Right之间
        if(X<T->Element){
            NewNode->Left=T->Left;   //Splay算法蕴含了NewNode->Element > T->Left->Element
            NewNode->Right=T;
            T->Left=NULL;
            T=NewNode;
        }
        else if(X>T->Element){
            NewNode->Right=T->Right;   //Splay算法蕴含了NewNode->Element < T->Right->Element
            NewNode->Left=T;
            T->Right=NULL;
            T=NewNode;
        }
        else{
            return T;   //Already in the tree;
        }
    }
    NewNode=NULL;   //so next insert will call malloc (static性质)
    return T;
}

SplayTree Remove(ElementType X, SplayTree T){
    Position NewTree;

    if(T!=NULL){
        T=Splay(X,T);
        if(X==T->Element){
            //found X
            if(T->Left==NULL){
                NewTree=T->Right;
            }
            else{
                NewTree=T->Left;     
                NewTree=Splay(X,NewTree);  //调整过后NewTree->Right=NUllNode,原因是假设他不为空，则X的值在NewTree和NewTree->Right之间，即
                NewTree->Right=T->Right;   //NewTree->Right>X,这与T->Left整颗子树(无论展不展开)都比X小矛盾，故假设不成立，有NewTree->Right=NUllNode
                //且T->Right一定大于整个T->Left(无论展不展开),故符合要求
            }
            free(T);
            T=NewTree;  //新的根结点
        }
    }
    return T;
}

void draw_level(SplayTree node, bool left, char* str){
	if(node->Right){
		draw_level(node->Right, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%d\n", node->Element);
	
	if(node->Left){
		draw_level(node->Left,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(SplayTree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->Right){
		draw_level(root->Right, false, str);
	}
	printf("%d\n",root->Element);  //鎵撳嵃鏍硅妭鐐圭殑鏁版嵁
	if(root->Left){
		draw_level(root->Left, true, str);
	} 
}

int main(){
    int i;
    Position T=NULL;
    for(i=7;i>=1;i--){
        T=MakeTree(T,i);
        draw(T);
    }
    T=Splay(1,T);
    draw(T);
    printf("\nInsert the node 8:\n");
    T=Insert(8,T);
    draw(T);
    printf("\nDelete the node 4:\n");
    T=Remove(4,T);
    draw(T);
    return 0;
}