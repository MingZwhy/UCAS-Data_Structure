#include<iostream>

using namespace std;
typedef char ElemType;

typedef struct BiTree{
    ElemType data;
    struct BiTree * leftchild, * rightchild;
}BiTree, *Ptr_BiTree;

typedef struct Point{
    ElemType data;
    int Power;
}Point, *Ptr_Point;

typedef Ptr_BiTree Root;
typedef Ptr_BiTree Position;

Position Mk_Tree(Position T, ElemType * R, int * SW, int * Power, int low, int high);

class SecondOptimal
{
private:
    Root T;
    int Data_Num;
    ElemType * R;    //下标0不存数据
    int * Power;     //权重值(正比于访问频率)
public:
    SecondOptimal();   //默认构造函数
    SecondOptimal(int num, Ptr_Point p);   //构造函数
    void Make_Second_Tree();
    ~SecondOptimal();
};

SecondOptimal::SecondOptimal()
{
    T=nullptr;
    Data_Num=0;
    R=nullptr; Power=nullptr;
}

SecondOptimal::SecondOptimal(int num, Ptr_Point p)
{
    T=nullptr;
    Data_Num = num;
    R = new ElemType[num+1];   //下标0不存数据
    Power = new int[num+1];    
    for(int i=1;i<=Data_Num;i++){
        R[i] = p[i-1].data;
        Power[i] = p[i-1].Power;
    }
    R[0] = '0';  Power[0] = 0;
}

SecondOptimal::~SecondOptimal()
{
    delete T;
    delete []R;
    delete []Power;
}

void SecondOptimal::Make_Second_Tree()
{
    //initialize the SW累积权值表
    int * SW = new int[Data_Num+1];
    SW[0]=0;
    for(int i=1;i<=Data_Num;i++){
        SW[i] = SW[i-1] + Power[i];
    }
    //选择最小的△p
    int low=1; 
    int high=Data_Num;
    int min = SW[high] - SW[low];
    int pos = low;
    int dw = SW[high] - SW[low-1];

    for(int j=low+1;j<=high;j++){
        if((dw-SW[j]-SW[j-1]) < min){
            pos = j;
            min = dw-SW[j]-SW[j-1];
        }
    }
    T = new BiTree;
    T->data = R[pos];
    if(pos==low){
        T->leftchild=nullptr;
    }
    else{
        T->leftchild=Mk_Tree(T->leftchild,R,SW,Power,low,pos-1);
    }
    if(pos==high){
        T->rightchild=nullptr;
    }
    else{
        T->rightchild=Mk_Tree(T->rightchild,R,SW,Power,pos+1,high);
    }
}

Position Mk_Tree(Position T, ElemType * R, int * SW, int * Power, int low, int high)
{
    SW[low-1]=0;
    for(int i=low;i<=high;i++){
        SW[i] = SW[i-1] + Power[i];
    }
    //选择最小的△p
    int min = SW[high] - SW[low];
    int pos = low;
    int dw = SW[high] - SW[low-1];

    for(int j=low+1;j<=high;j++){
        if((dw-SW[j]-SW[j-1]) < min){
            pos = j;
            min = dw-SW[j]-SW[j-1];
        }
    }
    T = new BiTree;
    T->data = R[pos];
    if(pos==low){
        T->leftchild=nullptr;
    }
    else{
        T->leftchild=Mk_Tree(T->leftchild,R,SW,Power,low,pos-1);
    }
    if(pos==high){
        T->rightchild=nullptr;
    }
    else{
        T->rightchild=Mk_Tree(T->rightchild,R,SW,Power,pos+1,high);
    }
    return T;
}

int main(){
    Point p[7];
    p[0].data='A'; p[0].Power=2;
    p[1].data='A'; p[1].Power=2;
    p[2].data='A'; p[2].Power=2;
    p[3].data='A'; p[3].Power=2;
    p[4].data='A'; p[4].Power=2;
    p[5].data='A'; p[5].Power=2;
    p[6].data='A'; p[6].Power=2;

    SecondOptimal st=SecondOptimal(7,p);
    st.Make_Second_Tree();
    st.~SecondOptimal();
    return 0;
}