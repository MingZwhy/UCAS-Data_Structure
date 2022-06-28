#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(){
    int n;
    scanf("%d",&n);
    int *f=(int *)malloc(sizeof(int)*(n+1));
    if(f==NULL){
        printf("malloc error");
        exit(1);
    }
    f[0]=1;
    int i=1;
    for(;i<=n;i++){
        f[i]=i*f[i/2];
    }
    printf("%d",f[n]);
    return 0;
}