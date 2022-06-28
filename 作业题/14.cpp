#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Status int

typedef struct LNode{
    int data;
    int freq;
    struct LNode *prior;
    struct LNode *next;
}LNode, *PTR_To_List;

typedef PTR_To_List List;
typedef PTR_To_List Position;
int record[10000];

int Compare(int a, int b)
{
    int i,j;
    i=0;
    j=0;
    while(i<10000){
        if(record[i]==a){
            break;
        }
        i++;
    }
    while(j<10000){
        if(record[j]==b){
            break;
        }
        j++;
    }
    if(i<j){
        return 0;
    }
    else{
        return 1;
    }
}

void Locate(List L, int i);
List Create_List(void);
int IsEmpty(List L);
void Print_list(List L);

List Create_List(void)
{
    List L=(List)malloc(sizeof(LNode)*1);
    if(L==NULL){
        printf("malloc error");
        exit(0);
    }
    L->next=NULL;
    L->prior=NULL;
    Position visit=L;
    char c=' ';
    while(c!='\n'){
        int elem;
        scanf("%d",&elem);
        Position node=(Position)malloc(sizeof(LNode)*1);
        if(node==NULL){
            printf("malloc error");
            exit(0);
        }
        node->data=elem;
        node->freq=0;
        node->next=NULL;
        node->prior=visit;
        visit->next=node;
        visit=node;
        c=getchar();
    }
    visit->next=L;
    L->prior=visit;
    return L;
}

void Locate(List L, int i)
{
    Position visit=L->next;
    while(visit!=L){
        if(visit->data==i){
            visit->freq+=1;
            break;
        }
        else{
            visit=visit->next;
        }
    }
    if(visit!=L->next){
        Position change=L->next;
        while(change!=visit && (change->freq>visit->freq || (change->freq==visit->freq && Compare(visit->data,change->data)))){
            change=change->next;
        }
        if(change!=visit){
            visit->prior->next=visit->next;
            visit->next->prior=visit->prior;

            visit->prior=change->prior;
            change->prior->next=visit;
            change->prior=visit;
            visit->next=change;
        }
    }
}

int IsEmpty(List L){
    if(L->next==NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void Print_list(List L){
	Position visit=L->next;
  	if(visit==NULL)
      	printf("NULL");
	while(visit!=L){
		if(visit->next!=NULL){
            printf("%d ",visit->data);
        }
        else{
            printf("%d",visit->data);
        }
		visit=visit->next;
	}
}

int main(){
    int n;
    scanf("%d",&n);
    List L=Create_List();
    int i;
    char c;
    int index=0;
    do{
        scanf("%d",&i);
        record[index++]=i;
        Locate(L,i);
        c=getchar();
    }while(c!='\n');
    Print_list(L);
    return 0;
}