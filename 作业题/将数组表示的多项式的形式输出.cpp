#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int m,n;
    scanf("%d %d\n",&m,&n);
    char c;
    int **num;
    num=(int **)malloc(sizeof(int *)*m);
    if(num==NULL)   exit(1);
    int i,j;
    for(i=0;i<m;i++){
        num[i]=(int *)malloc(sizeof(int)*n);
        for(j=0;j<n;j++){
            scanf("%d",&num[i][j]);
        }
        c=getchar();
    }
    for(i=m-1;i>=0;i--){
        for(j=n-1;j>=0;j--){
            if(num[i][j]!=0){
                if(num[i][j]!=1){
                    printf("%d",num[i][j]);
                }
                if(i>0){
                    printf("xE%d",i);
                }
                if(j>0){
                    printf("yE%d",j);
                }
                if(i+j!=0){
                    printf("+");
                }
            }
        }
    }
    return 0;
}