#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MAXLEN 30
#define MinTableSize 5

struct ListNode;
typedef struct ListNode *Position;
typedef struct ListNode *List;
struct HashTbl;
typedef struct HashTbl *HashTable;

struct ListNode
{
    char data[MAXLEN];
    Position next;
};

struct HashTbl
{
    int TableSize;
    List *Thelist;
};

typedef unsigned int Index;
Index Hash_1(const char *key, int TableSize);
Index Hash_2(const char *key, int TableSize);
Index Hash_3(const char *key, int TableSize);
//Use the prime as the size of table is better
int NextPrime(int TableSize);  //find the min prime bigger than TableSize
HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(char *key, HashTable H);
void Insert(char *key, HashTable H);
void Delete(char *key, HashTable H);
char *Retrieve(Position P);

void Draw_HashTable(HashTable H);

//f(Hash_1)
Index Hash_1(const char *key, int TableSize){
    unsigned int HashVal=0;
    while((*key)!='\0'){
        HashVal+=(*key);   //add the value of ASCII of *key
        key++;  //visit all of array key
    }
    return HashVal % TableSize;
}

//f(Hash_2)
Index Hash_2(const char *key, int TableSize){
    unsigned int HashVal=0;
    HashVal=key[0]+27*key[1]+729*key[2];
    return HashVal % TableSize;
}

//f(Hash_3)
Index Hash_3(const char *key, int TableSize){
    unsigned int HashVal=0;
    while((*key)!='\0'){
        HashVal=(HashVal<<5) + (*key);  //HashVal<<5 == HashVal*32
    }
    return HashVal % TableSize;
}

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

//initialize the HashTable
HashTable InitializeTable(int TableSize){
    HashTable H;
    int i;

    if(TableSize < MinTableSize){
        printf("The TableSize is too small");
        return NULL;
    }

    //Allocate the HashTbl
    H=(HashTable)malloc(sizeof(struct HashTbl)*1);
    if(H==NULL){
        printf("malloc error");
        exit(0);
    }

    H->TableSize=NextPrime(TableSize);

    //Alloocate the array of lists
    H->Thelist=(List *)malloc(sizeof(List)*H->TableSize);
    if(H->Thelist==NULL){
        printf("malloc error");
        exit(0);
    }

    //Allocate the head of each list
    for(i=0;i<H->TableSize;i++){
        H->Thelist[i]=(List)malloc(sizeof(struct ListNode));
        if(H->Thelist[i]==NULL){
            printf("malloc error");
            exit(0);
        }
        else{
            H->Thelist[i]->next=NULL;
        }
    }

    return H;
}

//find the PTR to the node containing char array key
Position Find(char *key, HashTable H){
    int target=Hash_1(key,H->TableSize);
    Position p;
    List L;

    L=H->Thelist[target];
    p=L->next;
    while(p!=NULL && strcmp(key,p->data)!=0){
        p=p->next;
    }
    return p;
    //if not found, will return NULL
}

//Insert the node containing char array key
void Insert(char *key, HashTable H){
    Position Pos, NewCell;
    List L;
    Pos=Find(key,H);

    if(Pos==NULL){  //not fount
        NewCell=(Position)malloc(sizeof(struct ListNode));
        if(NewCell==NULL){
            printf("malloc error");
            exit(0);
        }
        int target=Hash_1(key,H->TableSize);
        L=H->Thelist[target];
        strcpy(NewCell->data,key);
        NewCell->next=L->next;
        L->next=NewCell;  //insert newcell into the front of List
    }
    else{
        printf("this node has existed in the HashTable");
        return;
    }
}

//Delete the node containing char array key
void Delete(char *key, HashTable H){
    Position Pos,Visit_pre;
    List L;

    Pos=Find(key,H);
    if(Pos==NULL){
        printf("this node doesn't exist");
        return;
    }
    else{
        int target=Hash_1(key,H->TableSize);
        L=H->Thelist[target];
        Visit_pre=L;
        while(Visit_pre->next!=NULL && strcmp(Visit_pre->next->data,key)!=0){
            Visit_pre=Visit_pre->next;
        }
        Position temp=Visit_pre->next;
        Visit_pre->next=temp->next;
        free(temp);
    }
}

//read the array
char *Retrieve(Position P){
    return P->data;
}

//Destroy the HashTable
void Destroy(HashTable H){
    int i;
    for(i=0;i<H->TableSize;i++){
        Position visit=H->Thelist[i];
        Position temp=visit->next;
        while(temp!=NULL){
            visit=temp->next;
            free(temp);
            temp=visit;
        }
        free(H->Thelist[i]);
    }
    H->Thelist=NULL;
    H=NULL;
}

void Draw_HashTable(HashTable H){
    int i;
    for(i=0;i<H->TableSize;i++){
        List L=H->Thelist[i];
        Position p=L->next;
        printf("List(%d): L--->HEAD--->",i);
        while(p!=NULL){
            printf("%s--->",p->data);
            p=p->next;
        }
        printf("NULL\n");
    }
}