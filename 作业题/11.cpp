#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Status int

typedef struct LNode{
    char data;
    struct LNode *next;
}LNode, *PTR_To_List;

typedef PTR_To_List List;
typedef PTR_To_List Position;

Status Reverse_List(List L);
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
        c=getchar();
        Position node=(Position)malloc(sizeof(LNode)*1);
        if(node==NULL){
            printf("malloc error");
            exit(0);
        }
        node->data=c;
        node->next=NULL;
        visit->next=node;
        visit=node;
        c=getchar();
    }
    return L;
}

int IsEmpty(List L){
    if(L->next==NULL){
        return 1;
    }
    else{
        return 0;
    }
}

Status Reverse_List(List L)
{
    Position pre=NULL;
    Position cur=L->next;
    while(cur!=NULL){
        Position nexttemp=cur->next;
        cur->next=pre;
        pre=cur;
        cur=nexttemp;
    }
    L->next=pre;
    return 1;
}

void Print_list(List L){
	Position visit=L->next;
  	if(visit==NULL)
      	printf("NULL");
	while(visit!=NULL){
		if(visit->next!=NULL){
            printf("%c,",visit->data);
        }
        else{
            printf("%c",visit->data);
        }
		visit=visit->next;
	}
}

int main(){
    List L=Create_List();
    int flag=Reverse_List(L);
    Print_list(L);
    return 0;
}