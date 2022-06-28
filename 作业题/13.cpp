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

void Delete_Same(List A, List B)
{
    Position visita=A;
    Position visitb=B->next;
    while(visita->next!=NULL){
        char c=visita->next->data;
        int flag=0;
        while(visitb!=NULL){
            if(visitb->data==c){
                flag=1;
                break;
            }
            else{
                visitb=visitb->next;
            }
        }
        visitb=B->next;
        if(flag){
            Position temp=visita->next;
            visita->next=temp->next;
            free(temp);
        }
        else{
            visita=visita->next;
        }
    }
}

void Save_Same(List A, List B)
{
    Position visita=A;
    Position visitb=B->next;
    while(visita->next!=NULL){
        char c=visita->next->data;
        int flag=1;
        while(visitb!=NULL){
            if(visitb->data==c){
                flag=0;
                break;
            }
            else{
                visitb=visitb->next;
            }
        }
        visitb=B->next;
        if(flag){
            Position temp=visita->next;
            visita->next=temp->next;
            free(temp);
        }
        else{
            visita=visita->next;
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
    List A=Create_List();
    List B=Create_List();
    List C=Create_List();
    Save_Same(B,C);
    Delete_Same(A,B);
    Print_list(A);
    return 0;
}