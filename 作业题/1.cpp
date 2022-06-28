#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void swap(int *a, int *b);
void quicksort(int *nums, int left, int right);

void swap(int *a, int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

void quicksort(int *nums, int left, int right)
{
    int i,j;
    if(left<right){
        i=left;
        j=right+1;
        while(1){
            do{
                i++;
            }while(i<right && nums[i]>=nums[left]);
            do{
                j--;
            }while(j>left && nums[j]<=nums[left]);
            if(i>=j){
                break;
            }
            else{
                swap(&nums[i],&nums[j]);
            }
        }
        swap(&nums[j],&nums[left]);
        quicksort(nums,left,j-1);
        quicksort(nums,j+1,right);
    }
}

int main(){
    int *nums=(int *)malloc(sizeof(int)*3);
    if(nums==NULL){
        printf("malloc error");
        exit(1);
    }
    char c;
    int i=0;
    printf("please enter the numbers: ");
    do{
        scanf("%d",&nums[i]);
        i++;
    }while((c=getchar())!='\n');
    quicksort(nums,0,2);
    printf("the result is: ");
    for(i=0;i<3;i++){
        printf("%d ",nums[i]);
    }
    return 0;
}