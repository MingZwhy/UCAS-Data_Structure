#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int mygetline(char *s ,int lim)
{
    int i,c;
    i=0;
    while (--lim>0 &&(c=getchar())!=EOF&&c!='\n')
        s[i++]=c;
    s[i]='\0';
    return i;
}

char *Ditui(char *s, int left, int right)
{
    if(left<right){
        char temp=s[left];
        s[left]=s[right];
        s[right]=temp;
        return Ditui(s,left+1,right-1);
    }
    else{
        return s;
    }
}

int main(){
    char *s;
    s=(char *)malloc(sizeof(char)*1000);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    int len=mygetline(s,1000);
    int i,j;
    i=0; j=len-1;
    s=Ditui(s,i,j);
    printf("%s",s);
    return 0;
}