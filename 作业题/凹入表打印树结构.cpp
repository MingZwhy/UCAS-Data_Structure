#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INITSIZE 100
#define ADDSIZE 10
#define bool int
#define true 1
#define false 0

typedef struct{
    char data;
    int firstchild;
    int sibling;
}node, *ptr_node;

void Visit(ptr_node tnode, int index, int level)
{
    int i=1;
    for(;i<=level;i++){
        printf("-");
    }
    printf("%c\n",tnode[index].data);
    if( tnode[index].firstchild != -1){
        Visit(tnode,tnode[index].firstchild-1,level+1);
    }
    if( tnode[index].sibling != -1){
        Visit(tnode,tnode[index].sibling-1,level);
    }
}

int main(){
    ptr_node tnode=(ptr_node)malloc(sizeof(node)*1000);
    if(tnode==NULL) exit(1);
    char c=' ';
    int i=0;
    while(c!=EOF){
        scanf("%c ",&tnode[i].data);
        scanf("%d ",&tnode[i].firstchild);
        scanf("%d",&tnode[i].sibling);
        i++;
        c=getchar();
    }
    Visit(tnode,0,0);
    return 0;
}