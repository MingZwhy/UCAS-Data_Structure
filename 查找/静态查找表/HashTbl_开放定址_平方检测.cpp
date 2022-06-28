#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MinTableSize 5
#define MAXLEN 50

typedef char * ElementType; 
typedef unsigned int Index;
typedef Index Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

enum KindOfEntry{Legitimate, Empty, Deleted};

struct HashEntry
{
    ElementType Element;
    enum KindOfEntry Info;
};
typedef struct HashEntry Cell;

struct HashTbl
{
    int TableSize;
    Cell *TheCells;
};

HashTable InitializeTable(int TableSize);
Index Hash(const char *key, int TableSize);
int NextPrime(int TableSize);  //find the min prime bigger than TableSize
void DestroyTable(HashTable H);
Position Find_Empty(ElementType key, HashTable H);
Position Find_Existed(ElementType key, HashTable H);
void Insert(ElementType key, HashTable H);
void Delete(ElementType key, HashTable H);
ElementType Retrieve(Position P, HashTable H);
HashTable Rehash(HashTable H);
void Draw_HashTbl(HashTable H);

//find the min prime bigger than TableSize
int NextPrime(int TableSize){
    int n=TableSize;
    while(1){
        int i=2;
        bool prime=true;
        for(;i<n;i++){
            if(n%i==0){
                prime=false;
                break;
            }
        }
        if(prime){
            break;
        }
        else{
            n++;
        }
    }
    return n;
}

Index Hash(const char *key, int TableSize){
    unsigned int HashVal=0;
    while((*key)!='\0'){
        HashVal+=(*key);   //add the value of ASCII of *key
        key++;  //visit all of array key
    }
    return HashVal % TableSize;
}

HashTable InitializeTable(int TableSize){
    HashTable H;
    int i;

    if(TableSize < MinTableSize){
        printf("size is too small");
        return NULL;
    }
    //Allocate table
    H=(HashTable)malloc(sizeof(struct HashTbl)*1);
    if(H==NULL){
        printf("malloc error");
        exit(0);
    }
    H->TableSize=NextPrime(TableSize);
    //Allocate array of cells
    H->TheCells=(Cell *)malloc(sizeof(Cell)*H->TableSize);
    if(H->TheCells==NULL){
        printf("malloc error");
        exit(0);
    }
    
    for(i=0;i<H->TableSize;i++){
        H->TheCells[i].Element=(char *)malloc(sizeof(char)*MAXLEN);
        if(H->TheCells[i].Element==NULL){
            printf("malloc error");
            exit(0);
        }
        H->TheCells[i].Info=Empty;
    }
    return H;
}

//找到表中可以插入新元素key的地方
//假设散列表的大小至少为表中元素个数的两倍，因此平方探测法总能够find成功
Position Find_Empty(ElementType key, HashTable H){
    Position CurrentPos;
    Index CollisionNum;

    CollisionNum = 0;
    CurrentPos=Hash(key,H->TableSize);
    while(H->TheCells[CurrentPos].Info==Legitimate && strcmp(H->TheCells[CurrentPos].Element,key)!=0)
    {
        CurrentPos+=2*(++CollisionNum)-1;   //这实际上是实现了F(i)， 因为F(i)=F(i-1)+2i-1
        //hi(X)=(Hash(X)+F(i)) mod TableSize,  F(i)=i*i 平方探测法
        if(CurrentPos>=H->TableSize){
            CurrentPos-=H->TableSize;
        }
    }
    return CurrentPos;
    //如果待插入元素已经存在，将返回已存在该元素的地址
}

//找到表中已存在的元素key的位置
Position Find_Existed(ElementType key, HashTable H){
    Position CurrentPos;
    Index CollisionNum;

    CollisionNum = 0;
    CurrentPos=Hash(key,H->TableSize);
    while(H->TheCells[CurrentPos].Info!=Empty && strcmp(H->TheCells[CurrentPos].Element,key)!=0)
    {
        CurrentPos+=2*(++CollisionNum)-1;   //这实际上是实现了F(i)， 因为F(i)=F(i-1)+2i-1
        //hi(X)=(Hash(X)+F(i)) mod TableSize,  F(i)=i*i 平方探测法
        if(CurrentPos>=H->TableSize){
            CurrentPos-=H->TableSize;
        }
    }
    if(H->TheCells[CurrentPos].Info==Empty){
        printf("key is not found");
    }
    return CurrentPos;
    //如果未找到，则会返回对应第一个空的位置
}

void Insert(ElementType key, HashTable H){
    Position Pos;

    Pos=Find_Empty(key,H);
    if(H->TheCells[Pos].Info!=Legitimate){
        //ok to insert here
        H->TheCells[Pos].Info=Legitimate;
        strcpy(H->TheCells[Pos].Element,key);
    }
    //clearly, if H->TheCells[Pos].Info==Legitimate which means key has existed, then we do nothing
}

/*Delete只会将目标元素的Info改为Deleted，因为如果改为Empty的话，其以下的F[i]找到
的元素会无法被Find_Existed找到.
*/
void Delete(ElementType key, HashTable H){
    Position Pos;

    Pos=Find_Existed(key,H);
    if(H->TheCells[Pos].Info==Legitimate){
        //ok to delete
        H->TheCells[Pos].Info=Deleted;
    }
}

ElementType Retrieve(Position P, HashTable H){
    if(H->TheCells[P].Info==Legitimate){
        return H->TheCells[P].Element;
    }
    else{
        return NULL;
    }
}


//何时调用Rehash在文件中并未规定好，一般来说当表填满一半时再散列，或到达某个装填因子时再散列
HashTable Rehash(HashTable H){
    int i,OldSize;
    Cell *OldCells;

    OldCells=H->TheCells; //继承
    OldSize=H->TableSize;
    //get a new, empty table
    H=InitializeTable(2*OldSize);
    //scan through old table, reinserting into new
    for(i=0;i<OldSize;i++){
        if(OldCells[i].Info==Legitimate){
            Insert(OldCells[i].Element,H);
        }
    }
    free(OldCells);
    return H;
}

void Draw_HashTbl(HashTable H){
    int i;
    for(i=0;i<H->TableSize;i++){
        if(H->TheCells[i].Info==Legitimate){
            printf("%d: %s\n",i,H->TheCells[i].Element);
        }
        else{
            printf("%d\n",i);
        }
    }
}

int main(){
    HashTable H;
    H=InitializeTable(30);
    Draw_HashTbl(H);
    char s1[]="Gucunkuailewu";
    char s2[]="GuPi";
    Insert(s1,H);
    Draw_HashTbl(H);
    Insert(s2,H);
    Draw_HashTbl(H);
    printf("\n%s\n",Retrieve(Find_Existed(s1,H),H));
    Delete(s1,H);
    Draw_HashTbl(H);
    return 0;
}