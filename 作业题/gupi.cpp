#include<stdio.h>
#include<stdlib.h>


int main(){
    char **s;
    s=(char **)malloc(sizeof(char *)*100);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    int i;
    for(i=0;i<100;i++){
        s[i]=(char *)malloc(sizeof(char)*10);
    }
    for(i=0;i<5;i++){
        s[0][i]=i+'0';
    }
    s[0][i]='\0';
    printf("%s\n",s[0]);
    s[0][0]='s';
    printf("%s",s[0]);
    return 0;
}