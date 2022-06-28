#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Lnode{
    int data;
    struct Lnode *next;
}LNode, *List;

typedef List Position;

List Create_List(Position *tail)
{
    List L=(List)malloc(sizeof(LNode)*1);
    if(L==NULL){
        printf("malloc error");
        exit(1);
    }
    L->next=NULL;
    Position visit=L;
    char c=' ';
    int n;
    while(c!='\n' && c!=EOF){
        scanf("%d",&n);
        Position temp=(Position)malloc(sizeof(LNode)*1);
        if(temp==NULL){
            printf("malloc error");
            exit(1);
        }
        temp->data=n;
        temp->next=NULL;
        visit->next=temp;
        visit=temp;
        c=getchar();
    }
    *tail=visit;
    return L;
}

int main(){
    int m,n;
    scanf("%d %d\n",&m,&n);
    List A,B;
    Position a,b;
    if(m==0 && n==0){
        printf(" ");
    }
    else if(n==0){
        A=Create_List(&a);
        Position v=A->next;
        while(v!=NULL){
            printf("%d ",v->data);
            v=v->next;
        }
    }
    else if(m==0){
        A=Create_List(&a);
        Position v=A->next;
        while(v!=NULL){
            printf("%d ",v->data);
            v=v->next;
        }
    }
    else{
        A=Create_List(&a);
        B=Create_List(&b);
        b->next=A->next;
        Position visit=B->next;
        while(visit!=NULL){
            printf("%d ",visit->data);
            visit=visit->next;
        }
    }
    return 0;
}
