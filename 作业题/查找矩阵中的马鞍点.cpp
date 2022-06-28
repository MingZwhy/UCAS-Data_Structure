#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int m,n;
    scanf("%d %d\n",&m,&n);
    m++;
    n++;
    int **z;
    z=(int **)malloc(sizeof(int *)*m);
    int index;
    for(index=0;index<m;index++){
        z[index]=(int *)malloc(sizeof(int)*n);
    }
    int i,j;
    char c;
    for(i=0;i<m-1;i++){
        for(j=0;j<n-1;j++){
            int n;
            scanf("%d",&n);
            z[i][j]=n;
            c=getchar();
        }
    }

    for(i=0;i<m-1;i++){
        for(j=0;j<n;j++){
            int min;
            if(j==0){
                min=z[i][j];
            }
            else if(j<n-1){
                if(z[i][j]<min){
                    min=z[i][j];
                }
            }
            else{
                z[i][j]=min;
            }
        }
    }
    for(j=0;j<n-1;j++){
        for(i=0;i<m;i++){
            int max;
            if(i==0){
                max=z[i][j];
            }
            else if(i<m-1){
                if(z[i][j]>max){
                    max=z[i][j];
                }
            }
            else{
                z[i][j]=max;
            }
        }
    }

    for(i=0;i<m-1;i++){
        for(j=0;j<n-1;j++){
            if(z[i][j]==z[i][n-1] && z[i][j]==z[m-1][j]){
                printf("%d ",z[i][j]);
            }
        }
    }
    return 0;
}