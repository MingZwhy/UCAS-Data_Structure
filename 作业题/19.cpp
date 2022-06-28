#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int mygetline(char *s ,int lim)
{
    int i,c;
    i=0;
    while (--lim>0 &&(c=getchar())!='@'&&c!='\n')
        s[i++]=c;
    s[i]='\0';
    return i;
}

int main(){
    char *s=(char *)malloc(sizeof(char)*1000);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    int len=mygetline(s,10000);
    int i=0;
    int j=len-1;
    while(i<j){
        if(s[i]==s[j]){
            i++;
            j--;
        }
        else{
            printf("0");
            return 0;
        }
    }
    printf("1");
    return 0;
}