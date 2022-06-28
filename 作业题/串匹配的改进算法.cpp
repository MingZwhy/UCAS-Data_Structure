#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Lnode{
    char data;
    struct Lnode *next;
    struct Lnode *pre;
}LNode, *Ptr_To_LNode;

typedef Ptr_To_LNode List;
typedef Ptr_To_LNode Position;

List Create_List(void)
{
    List L=(List)malloc(sizeof(LNode)*1);
    if(L==NULL){
        printf("malloc error");
        exit(1);
    }
    L->next=NULL;
    Position visit=L;
    char c=getchar();
    while(c!='\n' && c!=EOF){
        Position node=(Position)malloc(sizeof(LNode)*1);
        if(node==NULL){
            printf("malloc error");
            exit(1);
        }
        node->data=c;
        node->next=visit->next;
        visit->next=node;
        visit=node;
        c=getchar();
    }
    return L;
}

void GetNext(List L)
{
    Position Cur=L->next;
    Position Pre=L;
    if(Cur!=NULL){
        Cur->pre=Pre;
    }
    while(Cur!=NULL && Cur->next!=NULL)
    {
        if(Pre==L || Cur->data==Pre->data){
            Cur=Cur->next;
            Pre=Pre->next;
            Cur->pre=Pre;
        }
        else{
            Pre=Pre->pre;
        }
    }
}

int KMP_List(List S, List T)
{
    int place=0;
    GetNext(T);
    Position visit_s=S;
    Position visit_t=T;
    while(visit_s!=NULL && visit_t!=NULL)
    {
        if(visit_t==T || visit_s->data==visit_t->data){
            visit_s=visit_s->next;
            place++;
            visit_t=visit_t->next;
        }
        else{
            visit_t=visit_t->pre;
        }
    }
    if(visit_t==NULL){
        return place-1;
    }
    else{
        return 0;
    }
}

int main(){
    int m,n;
    scanf("%d %d\n",&m,&n);
    List S=Create_List();
    List T=Create_List();
    int index=KMP_List(S,T);
    if(index==0){
        printf("-1");
    }
    else{
        printf("%d",index-n);
    }
    return 0;
}