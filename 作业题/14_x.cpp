#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Status int

typedef struct LNode{
    int data;
    int freq;
    struct LNode *next;
}LNode, *PTR_To_List;

typedef PTR_To_List List;
typedef PTR_To_List Position;
int record[10000];

void Locate(List L, int i);
int Compare(int a, int b);
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
        visit->next=node;
        visit=node;
        c=getchar();
    }
    return L;
}

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

void Locate(List L, int i)
{
    Position visit=L;
    while(visit->next!=NULL){
        if(visit->next->data==i){
            visit->next->freq+=1;
            break;
        }
        else{
            visit=visit->next;
        }
    }
    if(visit!=L){
        Position temp=visit->next;
        visit->next=temp->next;
        temp->next=NULL;
        //adjust the position
        visit=L;
        while(visit->next!=NULL && (temp->freq<visit->next->freq || (temp->freq==visit->next->freq && Compare(temp->data,visit->next->data)))){
            visit=visit->next;
        }
        temp->next=visit->next;
        visit->next=temp;
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
	while(visit!=NULL){
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
    int i=0;
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