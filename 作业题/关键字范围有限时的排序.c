#include<stdio.h>
#include<stdlib.h>

void swap(int *a,int *b){
	int temp;
	temp=*a;
	*a=*b;
	*b=temp;
} 

void quicksort(int *s,int left,int right){
	int i,j;
	if(left<right){
		i=left;
		j=right+1;
		while(1){
			do{
				i++;
			}while(i<right&&s[i]<=s[left]);
			do{
				j--;
			}while(j>left&&s[j]>=s[left]);
			if(i<j){
				swap(&s[i],&s[j]);
			}
			else{
				break;
			}
		}
		swap(&s[j],&s[left]);
		quicksort(s,j+1,right);
		quicksort(s,left,j-1);
	}
}

int main(){
    int s[1000];
    int i=0;
    char c;
    do{
        scanf("%d",&s[i]);
        i++;
        c=getchar();
    }while(c!='\n');
    quicksort(s,0,999);
    for(i=0;i<1000;i++){
        printf("%d ",s[i]);
    }
    return 0;
}