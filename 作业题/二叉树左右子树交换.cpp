#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int left[1000];
    int right[1000];
    char c=' ';
    int i=0;
    while(c!='\n'){
        scanf("%d",&left[i]);
        i++;
        c=getchar();
    }
    i=0;
    c=' ';
    int temp;
    while(c!='\n'){
        scanf("%d",&right[i]);
        temp=right[i];
        right[i]=left[i];
        left[i]=temp;
        i++;
        c=getchar();
    }
    int j;
    for(j=0;j<i;j++){
        if(j<i-1){
            printf("%d ",left[j]);
        }
        else{
            printf("%d\n",left[j]);
        }
    }
    for(j=0;j<i;j++){
        printf("%d ",right[j]);
    }
    return 0;
}