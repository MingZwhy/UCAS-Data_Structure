#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Status int

typedef struct LNode{
    int data;
    struct LNode *next;
}LNode, *PTR_To_List;

typedef PTR_To_List List;
typedef PTR_To_List Position;

Status Delete_Node(List L, int j);
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
        int n;
        scanf("%d",&n);
        Position node=(Position)malloc(sizeof(LNode)*1);
        if(node==NULL){
            printf("malloc error");
            exit(0);
        }
        node->data=n;
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

Status Delete_Node(List L, int j)
{
    if(IsEmpty(L)){
        printf("The list is empty");
        return 0;
    }
    else{
        Position visit=L;
        while(j>0){
            if(visit->next==NULL){
                printf("i is wrong, delete error");
                return 0;
            }
            else{
                visit=visit->next;
            }
            j--;
        }
	    Position temp=visit->next;
        visit->next=temp->next;
        free(temp);
    }
    return 1;
}

void Print_list(List L){
	Position visit=L->next;
  	if(visit==NULL)
      	printf("NULL");
	printf("The list now is: ");
	while(visit!=NULL){
		printf("%d ",visit->data);
		visit=visit->next;
	}
}

int main(){
    List L=Create_List();
    int n;
    scanf("%d",&n);
    int flag=Delete_Node(L,n);
    Print_list(L);
    return 0;
}