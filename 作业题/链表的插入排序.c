#include<stdio.h>
#include<stdlib.h>

#define SIZE 100000
#define MAXINT 1000000;
typedef int RcdType;

typedef struct{
	RcdType key;
	int next;
}SLNode;

typedef struct{
	SLNode data[SIZE];
	int length;
}SLinkList;

void LInserionSort(SLinkList *SL)
{
	SL->data[0].key = MAXINT;
	SL->data[0].next = 1;
	SL->data[1].next = 0;

	int i,j,k;
	for(i=2;i<=SL->length;i++){
		j = 0;
		k = SL->data[0].next;
		while(SL->data[k].key<=SL->data[i].key){
			//当k循环回0位置时，因为0位置存入的是MAXINT，所以一定会退出while
			j = k;
			k = SL->data[k].next;
		}
		//应插入在j和k之间
		SL->data[j].next=i;
		SL->data[i].next=k;
	}
}

void Visit(SLinkList SL)
{
	int visit=SL.data[0].next;
	while(1){
		if(SL.data[visit].next!=0){
			printf("%d ",SL.data[visit].key);
			visit = SL.data[visit].next;
		}
		else{
			printf("%d",SL.data[visit].key);
			break;
		}
	}
}

int main(){
	SLinkList SL;
	scanf("%d",&SL.length);
	char c=getchar();
	int index=1;
	do{
		scanf("%d",&SL.data[index++].key);
		c=getchar();
	}while(c!='\n');
	LInserionSort(&SL);
	Visit(SL);
    return 0;
}