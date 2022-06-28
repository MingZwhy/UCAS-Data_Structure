#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct SLNode
{
    int data;
    int next;
}SLNode;

int main()
{
    int n,i,j;
    scanf("%d",&n);
    SLNode list[100];
    list[0].data=INT_MAX;
    list[0].next=1;
    for(i=1;i<n+1;i++)
        scanf("%d",&list[i].data);
    list[1].next=0;
    for(i=2;i<n+1;i++){
        for(j=0;list[list[j].next].data<list[i].data;j=list[j].next);
        list[i].next=list[j].next;
        list[j].next=i;
    }
    for(i=list[0].next;i!=0;i=list[i].next){
        printf("%d",list[i].data);
        if(i) printf(" ");
    }
    return 0;
}