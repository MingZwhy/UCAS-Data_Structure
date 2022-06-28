#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
const int STR_SIZE = 1024;
const int INIT_STACKSIZE = 1000;

using namespace std;
typedef int ElemType; 

typedef struct TreeNode{
    ElemType data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
    int height;
}A_Tree, *AVLTree;

typedef AVLTree root;
typedef AVLTree TNode;

class My_Stack
{
private:
    AVLTree * base;
    AVLTree * top;
    int Stacksize;
public:
    My_Stack();   //默认构造函数
    My_Stack(int size = INIT_STACKSIZE);   //构造函数
    bool If_Empty() const;   //判断是否为空
    bool Push(AVLTree t); 
    AVLTree Top() const;
    bool Pop();
    ~My_Stack(); 
};

My_Stack::My_Stack()
{
    base = new AVLTree[INIT_STACKSIZE];
    top = base;
    Stacksize = INIT_STACKSIZE;
}

My_Stack::My_Stack(int size)
{
    base = new AVLTree[size];
    top = base;
    Stacksize = size;
}

bool My_Stack::If_Empty() const
{
    return (top==base);
}

bool My_Stack::Push(AVLTree t)
{
    if(top-base==Stacksize){
        cout << "The stack is full , cancel the push";
        return false;
    }
    *top = t;
    top++;
    return true;
}

AVLTree My_Stack::Top() const
{
    if(If_Empty()){
        return nullptr;
    }
    return *(top-1);
}

bool My_Stack::Pop()
{
    if(If_Empty()){
        return false;
    }
    top--;
    return true;
}

My_Stack::~My_Stack()
{
    delete [] base;
}

root MakeEmpty(ElemType x);
root MakeTree(root T, ElemType x);
root Findposition(root T, ElemType x);
TNode Findposition(root T, ElemType x);
TNode Findmin(root T);
TNode Findmax(root T);
TNode Insert(root T, ElemType x);       //Insert实际上与MakeTree完全相同，故省略
TNode Delete(root T, ElemType key);       //删除节点

TNode Merge(root T1, root T2);     //合并AVL树T1和T2,返回T1
void Split(root T, ElemType key, root &t1, root &t2);  //分裂AVL树T，以key为分界分为t1和t2

//辅助函数
int Height(root T);
ElemType MAX(ElemType a, ElemType b);
//旋转函数
root SingleRotateWithLeft(root T);
root SingleRotateWithRight(root T);
root DoubleRotateWithLeft(root T);
root DoubleRotateWithRight(root T);

void draw_level(AVLTree node, bool left, char* str);
void draw(AVLTree node);

int Height(root T){
    if(T==NULL){
        return -1;
    }
    else{
        return T->height;
    }
}

ElemType MAX(ElemType a, ElemType b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

root MakeEmpty(ElemType x){
    root T;
    T=(root)malloc(sizeof(A_Tree)*1);
    if(T==NULL){
        printf("malloc error");
        exit(0);
    }
    T->data=x;
    T->height=0;
    T->lchild=NULL;
    T->rchild=NULL;
    return T;
}

root MakeTree(root T, ElemType x){
    if(T==NULL){
        T=(root)malloc(sizeof(A_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
        }
        T->data=x;
        T->lchild=NULL;
        T->rchild=NULL;
        T->height=0;
    }
    else{
        if(x<T->data){
            //先按照二叉搜索树相同的方法构造
            T->lchild=MakeTree(T->lchild,x);
            //然后对树进行调整
            if(Height(T->lchild)-Height(T->rchild)==2){
                if(x<(T->lchild)->data){
                    //左-左，只需要单左旋
                    T=SingleRotateWithLeft(T);
                }
                else{
                    //左-右，需要双左旋
                    T=DoubleRotateWithLeft(T);
                }
            }
        }
        else if(x>T->data){
            T->rchild=MakeTree(T->rchild,x);
            if(Height(T->rchild)-Height(T->lchild)==2){
                if(x>(T->rchild)->data){
                    //右-右，只需要单右旋
                    T=SingleRotateWithRight(T);
                }
                else{
                    //右-左，需要双右旋
                    T=DoubleRotateWithRight(T);
                }
            }
        }
        else{
            ;  //节点已存在，do nothing
        }
    }
    //已构造过树且调整过树结构，需要更新该节点的高度
    T->height=MAX(Height(T->lchild),Height(T->rchild))+1;
    return T;
}

root SingleRotateWithLeft(root T){
    TNode Left_of_T;
    Left_of_T=T->lchild;
    T->lchild=Left_of_T->rchild;
    Left_of_T->rchild=T;
    //更新高度
    T->height=MAX(Height(T->lchild),Height(T->rchild))+1;
    Left_of_T->height=MAX(Height(Left_of_T->lchild),Height(Left_of_T->rchild))+1;
    //Left_of_T是新的根节点
    return Left_of_T;
}

root SingleRotateWithRight(root T){
    TNode Right_of_T;
    Right_of_T=T->rchild;
    T->rchild=Right_of_T->lchild;
    Right_of_T->lchild=T;
    //更新高度
    T->height=MAX(Height(T->lchild),Height(T->rchild))+1;
    Right_of_T->height=MAX(Height(Right_of_T->lchild),Height(Right_of_T->rchild))+1;
    //Right_of_T是新的根节点
    return Right_of_T;
}

root DoubleRotateWithLeft(root T){   //左-右
    TNode Left_of_T;
    Left_of_T=T->lchild;
    T->lchild=SingleRotateWithRight(Left_of_T);       //       t1               t1
    //第一步操作对t2和t3进行                           ||    t2      -->      t3      -->     t3
    T=SingleRotateWithLeft(T);                        //       t3          t2              t2  t1
    //第二步操作对t3和t1进行，在调用SingleRotateWithLeft时已更新过高度
    return T;
}

root DoubleRotateWithRight(root T){   //右-左
    TNode Right_of_T;
    Right_of_T=T->rchild;
    T->rchild=SingleRotateWithLeft(Right_of_T);      //       t1               t1
    //第一步操作对t2和t3进行                           ||          t2      -->      t3      -->     t3
    T=SingleRotateWithRight(T);                       //       t3                     t2        t1    t2  
    //第二步操作对t3和t1进行，在调用SingleRotateWithLeft时已更新过高度
    return T;
}

TNode Findposition(root T, ElemType x){
    if(T==NULL){
        //if not found, will return NULL
        return NULL;
    }
    else{
        if(x==T->data){
            return T;
        }
        else if(x<T->data){
            return Findposition(T->lchild,x);
        }
        else{
            return Findposition(T->rchild,x);
        }
    }
}

TNode Findmin(root T){
    if(T==NULL){
        return NULL;
    }
    else{
        if(T->lchild==NULL){
            return T;
        }
        else{
            return Findmin(T->lchild);
        }
    }
}

TNode Findmax(root T){
    if(T==NULL){
        return NULL;
    }
    else{
        if(T->rchild==NULL){
            return T;
        }
        else{
            return Findmax(T->rchild);
        }
    }
}

TNode Insert(root T, ElemType x){
    if(T==NULL){
        T=(root)malloc(sizeof(A_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
        }
        T->data=x;
        T->lchild=NULL;
        T->rchild=NULL;
        T->height=0;
    }
    else{
        if(x<T->data){
            //先按照二叉搜索树相同的方法构造
            T->lchild=Insert(T->lchild,x);
            //然后对树进行调整
            if(Height(T->lchild)-Height(T->rchild)==2){
                if(x<(T->lchild)->data){
                    //左-左，只需要单左旋
                    T=SingleRotateWithLeft(T);
                }
                else{
                    //左-右，需要双左旋
                    T=DoubleRotateWithLeft(T);
                }
            }
        }
        else if(x>T->data){
            T->rchild=Insert(T->rchild,x);
            if(Height(T->rchild)-Height(T->lchild)==2){
                if(x>(T->rchild)->data){
                    //右-右，只需要单右旋
                    T=SingleRotateWithRight(T);
                }
                else{
                    //右-左，需要双右旋
                    T=DoubleRotateWithRight(T);
                }
            }
        }
        else{
            ;  //节点已存在，do nothing
        }
    }
    //已构造过树且调整过树结构，需要更新该节点的高度
    T->height=MAX(Height(T->lchild),Height(T->rchild))+1;
    return T;
}

TNode Delete(root T, ElemType key)
{
    if(T->data==key && T->lchild==nullptr && T->rchild==nullptr){
        //只剩一个根且删除
        return nullptr;   //直接返回空指针
    }

    TNode parent = nullptr;
    TNode p = T;
    TNode q = nullptr;

    My_Stack s(INIT_STACKSIZE);
    while(p){   //先扎到节点，存储轨迹
        if(p->data==key){
            break;
        }
        parent = p;
        s.Push(parent);
        if(key > p->data){
            p = p->rchild;   //大于则走右子
        }
        else{
            p = p->lchild;   //小于则走左子
        }
    }
    //此时除待删除点外，其父节点轨迹都在栈内
    if(p==nullptr){
        cout << "can't find key, cancel delete";
        return nullptr;
    }

    //找到待删除节点
    if(p->lchild && p->rchild){
        parent = p;
        q = p->lchild;
        s.Push(parent);
        //下面的while循环找到左子树最大点
        while(q->rchild){
            parent = q;
            s.Push(parent);
            q = q->rchild;
        }
        //将右子树最小点赋给待删除节点
        p->data = q->data;
        //p指向新的待删除节点
        p = q;
    }

    //此时p,q同时指向待新的删除节点(原删除节点的前驱)
    //parent指向p,q的父母
    if(p->lchild){
        q = p->lchild;
    }
    else{
        q = p->rchild;
    }
    //此时p是删除目标。q是删除目标的子女
    bool Is_Left = false;  //删除的目标是否为左子

    //删除节点
    if(parent==nullptr){  //如果删除目标是整颗AVL树的根节点
        T = q;            //则直接令新根为待删除节点的子女
    }
    else{
        if(parent->lchild==p){
            //删除目标为左孩子，对应的情况是原删除节点的左子树整个无右子，所以新的待删除节点即为原待删除节点的lchild
            parent->lchild = q;
            Is_Left = true;
        }
        else{   //删除目标为右子
            parent->rchild = q;
        }
    }

    //删除后更新高度
    parent->height = MAX(Height(parent->lchild),Height(parent->rchild)) + 1;

    //删除完毕，接下来调整平衡
    bool Is_Balanced = false;
    while(!s.If_Empty()){
        parent = s.Top();
        s.Pop();

        int dist = Height(parent->lchild) - Height(parent->rchild);
        if(dist==1 || dist==-1){   //情况1，原平衡因子为0
            //意味着删除前parent的左右子树等高，删除后仍平衡，且以parent为根的整颗子树高度不变，仍平衡
            break;
        }
        if(dist==0){              //情况2，原平衡因子绝对值为1，且删除的是parent较高的子树
            //结果是以parent为根的整颗子树仍平衡，但是以parent为根的整颗子树高度-1，所以可能影响上层平衡，需要追溯修改更上层的祖先节点
            q = parent;
        }
        if(dist==2 || dist==-2){  //情况3，原平衡因子绝对值为1，且删除的是parent较低的子树
            //首先令q指向较高的树
            if(dist>0){
                q = parent->lchild;
            }
            else{
                q = parent->rchild;
            }

            int dist_q = Height(q->lchild)-Height(q->rchild);

            if(dist_q==0){
                //当q的左右子树等高时，对应情况3.1，此时parent的较低子树高为n-1，较高子树高为n+1且该子树平衡
                //只要进行一次单旋则使整颗parent子树平衡且高度仍为n+2不变，则不需要回溯工作完成
                if(dist==-2){   //parent的右子树高左子树低
                    parent = SingleRotateWithRight(parent);
                }
                else{           //parent的左子树高右子树低
                    parent = SingleRotateWithLeft(parent);
                }
                Is_Balanced = true;    //上面解释过这样调整后达到整体平衡，不需要再回溯
            }
            else{
                if(dist==-2){          //parent的右子树高左子树低,也说明q是parent的右子而删除的是parent的左子
                    if(dist_q<0){      //dist和dist_q同号,对应3.2，需要先单旋，单旋后parent平衡但整体高度-1，需要回溯修改祖先节点
                        parent = SingleRotateWithRight(parent);
                    }
                    else{              //dist和dist_q异号,对应3.3，需要双旋转，双旋后parent平衡但整体高度-1，需要回溯修改祖先节点
                        parent = DoubleRotateWithRight(parent);
                    }
                }
                else{                  //parent的右子树低左子树高,也说明q是parent的左子而删除的是parent的右子
                    if(dist_q>0){      //dist和dist_q同号,对应3.2，需要先单旋，单旋后parent平衡但整体高度-1，需要回溯修改祖先节点
                        parent = SingleRotateWithLeft(parent);
                    }
                    else{              //dist和dist_q异号,对应3.3，需要双旋转，双旋后parent平衡但整体高度-1，需要回溯修改祖先节点
                        parent = DoubleRotateWithLeft(parent);
                    }
                }
            }

            //重新连接,只要进行过单/双旋的，新根均为parent，需要将parent与祖先节点进行连接
            if(s.If_Empty()){
                T = parent;       //parent直接成为整颗树的新根
            }
            else{
                q = s.Top();
                if(parent->data<q->data){
                    q->lchild = parent;
                }
                else{
                    q->rchild = parent;
                }
            }

            if(Is_Balanced){
                break;   //以上存在不需要回溯的情况，直接跳出循环
            }
        }

    //更新高度
    //这里要更新高度的原因是在一次while循环中可能没有发生单/双旋(旋转函数中会更新新根高度)，这意味着没有更新高度，这会影响回溯过程中的判断
    //如果更精细，可以把这里的更新高度放在dist==0和dist==1或dist==-1的情况里，但这里为了好理解，放在整个while的末尾
    parent->height = MAX(Height(parent->lchild),Height(parent->rchild)) + 1;

    }
    return T;
}

TNode Merge(root T1, root T2)
{
    if(T2!=nullptr){
        T1 = Merge(T1,T2->lchild);
        T1 = Insert(T1,T2->data);
        T1 = Merge(T1,T2->rchild); 
    }
    return T1;
}

// T:待分裂的平衡二叉树
// key:分裂的关键字
// t1:关键字全部小于key的平衡二叉树
// t2:关键字全部大于key的平衡二叉树
void Split(root T, ElemType key, root &t1, root &t2)
{
    if(T!=nullptr){
        Split(T->lchild,key,t1,t2);
        if(T->data>key){
            t2 = Insert(t2,T->data);
        }
        else{
            t1 = Insert(t1,T->data);
        }
        Split(T->rchild,key,t1,t2);
    }
}

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

int main(){
    int test[15]={3,5,2,6,7,1,12,14,11,10,9,8,4,13,0};
    AVLTree T;
    T=NULL;
    for(int i=0;i<15;i++){
        T=MakeTree(T,test[i]);
        printf("\n\nThe%dmake:\n\n",i+1);
        draw(T);
    }
    cout << endl << endl;
    for(int i=0;i<15;i++){
        T = Delete(T,test[i]);
        printf("\n\ndelete %d:\n\n",test[i]);
        if(T){
            draw(T);
        }
    }

    int test_m_s[15]={4,2,23,54,21,1,3,9,93,24,52,77,32,45,65};
    AVLTree U;
    U=NULL;
    for(int i=0;i<15;i++){
        U=Insert(U,test_m_s[i]);
    }
    printf("AVLTree U is:\n\n");
    draw(U);
    AVLTree t1,t2;
    t1=t2=NULL;
    printf("\nsplit the tree U (24) get t1 and t2:\n\n");
    Split(U,24,t1,t2);
    printf("\nt1 is:\n");
    draw(t1);
    printf("\nt2 is:\n");
    draw(t2);
    
    printf("\n\nMerge t1 and t2 to get tree M\n");
    AVLTree M;
    M = Merge(t1,t2);
    draw(M);
    return 0;
}