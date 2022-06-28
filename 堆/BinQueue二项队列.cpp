//虽然左式堆和斜堆每次操作花费O(log N)时间，这有效地支持了合并、插入和DeleteMin，但还是有改进的余地
//二项队列支持所有这三种操作，每次操作的最坏清醒运行时间为O(log N)时间，而插入操作平均花费常数时间。
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define Infinity 100000
#define MaxTrees 25
#define CAPACITY ((1<<MaxTrees)-1)  //容量是2^0+2^1+2^2+2^3+……+2^(MaxTrees-1)

typedef int ElementType;
typedef struct BinNode *BinTree;
typedef struct BinNode *Position;
typedef struct Collection *BinQueue;

struct BinNode
{
    ElementType Element;
    Position LeftChild;
    Position NextSibling;
};

struct Collection
{
    int CurrentSize;
    BinTree TheTrees[MaxTrees];
};

BinQueue Initialize(void);
ElementType FindMin(BinQueue H);
bool IsEmpty(BinQueue H);
BinTree CombineTrees(BinTree T1, BinTree T2);
BinQueue Merge(BinQueue H1, BinQueue H2);
void Insert(ElementType X, BinQueue H);
void DeleteMIn(BinQueue H);


BinQueue Initialize(void)
{
    BinQueue H=(BinQueue)malloc(sizeof(struct Collection)*1);
    if(H==NULL){
        printf("malloc error");
        exit(0);
    }
    int i;
    for(i=0;i<MaxTrees;i++){
        H->TheTrees[i]=NULL;
    }
    H->CurrentSize=0;
    return H;
}

ElementType FindMin(BinQueue H)
{
    if(IsEmpty(H)){
        printf("The BinQueue is empty");
        return 0;
    }
    ElementType MinElem=Infinity;
    int i;
    for(i=0;i<MaxTrees;i++){
        if(H->TheTrees[i]!=NULL && H->TheTrees[i]->Element<MinElem){
            MinElem=H->TheTrees[i]->Element;
            break;
        }
    }
    return MinElem;
}

bool IsEmpty(BinQueue H){
    return H->CurrentSize==0;
}

//Return the result of merging equal-sized T1 and T2
BinTree CombineTrees(BinTree T1, BinTree T2)
{
    if(T1->Element>T2->Element){
        return CombineTrees(T2,T1);
    }
    T2->NextSibling=T1->LeftChild;
    T1->LeftChild=T2;
    return T1;
}

//Merge two binomial queues
//H1 contains merged result
BinQueue Merge(BinQueue H1, BinQueue H2)
{
    BinTree T1,T2,Carry=NULL;
    int i,j;

    if(H1->CurrentSize+H2->CurrentSize>CAPACITY){
        printf("Merge would exceed Capacity");
        return NULL;
    }
    H1->CurrentSize+=H2->CurrentSize;
    for(i=0,j=1;j<=H1->CurrentSize;i++,j*=2){
        //j能保证停留在合适的位置
        T1=H1->TheTrees[i];
        T2=H2->TheTrees[i];

        switch( !!T1 + 2 * !!T2 + 4 * !!Carry)
        {
            case 0: break;  //No tree
            case 1: break;  //Only H1 
            case 2: //Only H2
                H1->TheTrees[i]=T2;
                H2->TheTrees[i]=NULL;
                break;
            case 4: //Only Carry
                H1->TheTrees[i]=Carry;
                Carry=NULL;
                break;
            case 3: //H2 and H1
                Carry=CombineTrees(T1,T2);
                H1->TheTrees[i]=H2->TheTrees[i]=NULL;
                break;
            case 5: //Carry and H1
                Carry=CombineTrees(T1,Carry);
                H1->TheTrees[i]=NULL;
                break;
            case 6: //Carry and H2
                Carry=CombineTrees(T2,Carry);
                H2->TheTrees[i]=NULL;
                break;
            case 7: //All trees
                H1->TheTrees[i]=Carry;
                Carry=CombineTrees(T1,T2);
                H2->TheTrees[i]=NULL;
                break;
        }
    }
    return H1;
}

ElementType DeleteMin(BinQueue H)
{
    int i,j;
    int MinTree;    //The tree with the minimum item
    BinQueue DeletedQueue;
    Position DeletedTree,OldRoot;
    ElementType MinItem;

    if(IsEmpty(H)){
        printf("Empty binomial queue");
        return -Infinity;
    }

    MinItem=Infinity;
    for(i=0;i<MaxTrees;i++){
        if(H->TheTrees[i] && H->TheTrees[i]->Element < MinItem){
            //update minimum
            MinItem=H->TheTrees[i]->Element;
            MinTree=i;
        }

        DeletedTree=H->TheTrees[MinTree];
        OldRoot=DeletedTree;
        DeletedTree=DeletedTree->LeftChild;
        free(OldRoot);

        DeletedQueue=Initialize();
        DeletedQueue->CurrentSize=(1<<MinTree)-1;
        for(j=MinTree-1;j>=0;j--){
            DeletedQueue->TheTrees[j]=DeletedTree;
            DeletedTree=DeletedTree->NextSibling;
            DeletedQueue->TheTrees[j]->NextSibling=NULL;
        }

        H->TheTrees[MinTree]=NULL;
        H->CurrentSize -= (DeletedQueue->CurrentSize+1);
        //+1为删去的那个结点
        Merge(H,DeletedQueue);
        free(DeletedQueue);
        return MinItem;
    }
}

void Insert(ElementType X, BinQueue H)
{
    BinQueue temp=Initialize();
    temp->CurrentSize=1;
    temp->TheTrees[0]=(BinTree)malloc(sizeof(struct BinNode)*1);
    if(temp->TheTrees[0]==NULL){
        printf("malloc error");
        exit(0);
    }
    temp->TheTrees[0]->Element=X;
    temp->TheTrees[0]->LeftChild=NULL;
    temp->TheTrees[0]->NextSibling=NULL;
    Merge(H,temp);
    free(temp);
}