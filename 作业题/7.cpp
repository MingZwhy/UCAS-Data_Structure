#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int mygetline(char *s ,int lim)
{
    int i,c;
    i=0;
    while (--lim>0 &&(c=getchar())!=EOF&&c!='\n')
        s[i++]=c;
    if(c=='\n')
        s[i++]='\n';
    s[i]='\0';
    return i;
}


int main(){
    char *A=(char *)malloc(sizeof(char)*210);
    char *B=(char *)malloc(sizeof(char)*210);
    if(A==NULL || B==NULL){
        printf("malloc error");
        exit(1);
    }
    mygetline(A,210);
    mygetline(B,210);
    int i=0;
    while(A[i]!='\0' && B[i]!='\0')
    {
        if(A[i]==B[i]){
            i++;
        }
        else{
            if(A[i]<B[i]){
                printf("1");
                return 0;
            }
            else{
                printf("2");
                return 0;
            }   
        }
    }
    if(A[i]!='\0' && B[i]=='\0'){
        printf("2");
    }
    else if(A[i]=='\0' && B[i]!='\0'){
        printf("1");
    }
    else{
        printf("0");
    }
    return 0;
}