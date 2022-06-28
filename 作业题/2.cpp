#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int Value(int k, int m)
{
    if(m<=k-2){
        return 0;
    }
    else if(m==k-1){
        return 1;
    }
    else{
        int i=1;
        int sum=0;
        for(;i<=k;i++){
            sum+=Value(k,m-i);
        }
        return sum;
    }
}

int main(){
    int k,m;
    printf("enter k: ");
    scanf("%d",&k);
    char eat=getchar();
    printf("enter m: ");
    scanf("%d",&m);
    printf("the result is :");
    printf("%d",Value(k,m));
    return 0;
}