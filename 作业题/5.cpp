#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int digui(int m, int n)
{
    if(m<=0){
        return 0;
    }
    else{
        return digui(m-1,2*n) + n;
    }
}

int main(){
    int m,n;
    scanf("%d,%d",&m,&n);
    printf("%d",digui(m,n));
    return 0;
}