//优先队列(左式堆)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MinPQSize 10
#define MinData -100000
#define MaxData 100000

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef int ElementType;

PriorityQueue Initialize(int MaxElements);
PriorityQueue BuildHeap(PriorityQueue H); //将一颗无序的完全二叉树转换为二叉堆
void PrecolateDown(int P, PriorityQueue H);     //从位置i开始下滤
void Destroy(PriorityQueue H);
void MakeEmpty(PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
void DecreaseKey(int P, int Value, PriorityQueue H);
void IncreaseKey(int P, int Value, PriorityQueue H);
void Delete(int X, PriorityQueue H);
ElementType FindMin(PriorityQueue H);
bool IsEmpty(PriorityQueue H);
bool IsFull(PriorityQueue H);
void Print(PriorityQueue H);

struct HeapStruct
{
    int Capacity;
    int Size;
    ElementType *Elements;
};

PriorityQueue Initialize(int MaxElements){
    PriorityQueue H;

    if(MaxElements<MinPQSize){
        printf("The size is too small");
        return NULL;
    }
    H=(PriorityQueue)malloc(sizeof(struct HeapStruct)*1);
    if(H==NULL){
        printf("malloc error");
        exit(0);
    }
    H->Elements=(ElementType *)malloc(sizeof(ElementType)*(MaxElements+1));
    if(H->Elements==NULL){
        printf("malloc error");
        exit(0);
    }
    H->Capacity=MaxElements;
    H->Size=0;
    H->Elements[0]=MinData; //0号位存储负无穷，方便上滤终止

    return H;
}

bool IsFull(PriorityQueue H){
    if(H->Capacity==H->Size){
        return true;
    }
    else{
        return false;
    }
}

bool IsEmpty(PriorityQueue H){
    if(H->Size==0){
        return true;
    }
    else{
        return false;
    }
}

ElementType FindMin(PriorityQueue H){
    if(H->Size==0){
        printf("PriorityQueue is empty");
        return MinData;
    }
    else{
        return H->Elements[1];     //堆的根节点一定为最小元
    }
}

void Insert(ElementType X, PriorityQueue H){
    int i;

    if(IsFull(H)){
        printf("PriorityQueue is full, can't insert");
        return;
    }

    //Insert的原理是自下而上，条件是insert前整个堆是有序的
    for(i=++H->Size;H->Elements[i/2]>X;i/=2){
        H->Elements[i]=H->Elements[i/2];
    }
    //因为H->Elements[0]是极小值，所以循环一定会终止
    H->Elements[i]=X;
}

ElementType DeleteMin(PriorityQueue H){
    int i,Child;
    ElementType MinElement,LastElement;

    if(IsEmpty(H)){
        printf("PriorityQueue is empty, can't delete");
        return H->Elements[0];
    }
    MinElement=H->Elements[1];
    LastElement=H->Elements[H->Size];
    H->Size--; //H->Elements[H->Size]处的数据还在，但通过H->Size--的方式懒惰删除了它，并且不影响后面操作

    for(i=1;i*2<=H->Size;i=Child){
        //Find smaller child
        Child=i*2;
        if(Child!=H->Size && H->Elements[Child+1]<H->Elements[Child]){
            //child!=H->size是考虑了父节点只有左子一个儿子的情况
            Child++;
        }

        if(LastElement > H->Elements[Child]){
            H->Elements[i]=H->Elements[Child];
            //若无法用最后一个元素填充该空穴，则将i的小儿子填到i（空穴）去
        }
        else{
            //说明待插入节点小于当前所在空穴的两个儿子中小的那个，可以直接填充在该空穴
            break;
        }
    }
    //将LastElement填充到最后的空穴处
    H->Elements[i]=LastElement;
    return MinElement;
}

void DecreaseKey(int P, int Value, PriorityQueue H){
    if(P<=0 || P>H->Size){
        printf("wrong position");
        return;
    }
    else{
        H->Elements[P]-=Value;
    }
    int i;
    int temp=H->Elements[P];
    //减小P后P可能比其子小，从P往上进行上滤
    for(i=P;H->Elements[i/2]>temp;i/=2){
        H->Elements[i]=H->Elements[i/2];
        H->Elements[i/2]=temp;
    }
}

void IncreaseKey(int P, int Value, PriorityQueue H){
    if(P<=0 || P>H->Size){
        printf("wrong position");
        return;
    }
    else{
        H->Elements[P]+=Value;
    }
    //增大P后P可能比其子大，从P往下进行下滤
    //与DeleteMin相似，只不过这里的“最后一个元素”就是P位置的元素本身，这并不冲突, 还正好省略了H->Size--的步骤
    /*!!!!!!!!!!!!!!!!!!
    以下过程直接调用PrecolateDown(P)也可，作用是从P开始往下下滤
    */
    int i,Child;
    ElementType LastElement=H->Elements[P];
    for(i=P;i*2<=H->Size;i=Child){
        Child=i*2;
        if(Child!=H->Size && H->Elements[Child+1]<H->Elements[Child]){
            Child++;
        }

        if(LastElement>H->Elements[Child]){
            H->Elements[i]=H->Elements[Child];
        }
        else{
            break;
        }
    }
    H->Elements[i]=LastElement;
}

PriorityQueue BuildHeap(PriorityQueue H){
    //从最年轻的父亲节点开始调整，也即叶子节点的父亲
    //H->Size/2 恰好为第一个可能需要调整的最年轻父亲节点, 原理可以参考完全二叉树的性质
    //用自上而下过滤而非自下而上过滤的原因是后者的条件是原堆是有序的
    int i;
    for(i=H->Size/2;i>0;i--){
        PrecolateDown(i,H);
    }
    return H;
}

void PrecolateDown(int P, PriorityQueue H){
    //下滤的条件是位置P以下（从树来看是以下，从数组来看是向右）原本的结构是有序的
    int i,Child;
    ElementType LastElement=H->Elements[P];
    for(i=P;i*2<=H->Size;i=Child){
        Child=i*2;
        if(Child!=H->Size && H->Elements[Child+1]<H->Elements[Child]){
            Child++;
        }

        if(LastElement>H->Elements[Child]){
            H->Elements[i]=H->Elements[Child];
        }
        else{
            break;
        }
    }
    H->Elements[i]=LastElement;
}

void Delete(int X, PriorityQueue H){
    DecreaseKey(X,MaxData,H);
    DeleteMin(H);
}

void Print(PriorityQueue H){
    int i;
    for(i=1;i<=H->Size;i++){
        printf("%d  ",H->Elements[i]);
    }
    printf("\n");
}

int main(){
    PriorityQueue H=Initialize(11);
    int s[11]={13,14,16,19,21,19,68,65,26,32,31};
    int i;
    for(i=0;i<11;i++){
        Insert(s[i],H);
    }
    Print(H);
    printf("\nAfter DeleteMin: \n");
    DeleteMin(H);
    Print(H);
    printf("\nAfter Delete(26): \n");
    Delete(4,H);
    Print(H);
    printf("\nAfter IncreaseKey(3,10,H): \n");
    IncreaseKey(3,10,H);
    Print(H);
    printf("\nAfter it, we exam if BuildHeap can adjust noorder tree to PriorQueue\n");
    PriorityQueue W=Initialize(15);
    W->Size=15;
    int r[15]={150,80,40,30,10,70,110,100,20,90,60,50,120,140,130};
    printf("refer to example in P148\n");
    for(i=0;i<15;i++){
        W->Elements[i+1]=r[i];
    }
    Print(W);
    W=BuildHeap(W);
    printf("\nAfter build:\n");
    Print(W);
    return 0;
}