#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int compare(char *a, char *b){
    while(*a!='\0' && *b!='\0'){
        if(*a>*b){
            return 1;
        }
        else if(*a<*b){
            return 0;
        }
        else{
            a++;
            b++;
        }
    }
    if(*a=='\0' && *b!='\0'){
        return 0;
    }
    else{
        return 1;
    }
}

void swap(char *a, char *b)
{
    while(*a!='\0' && *b!='\0'){
        char c=*a;
        *a=*b;
        *b=c;
        a++; b++;
    }
    if(*a=='\0' && *b!='\0'){
        char *pos=b;
        while(*b!='\0'){
            *a=*b;
            a++; b++;
        }
        *a='\0';
        *pos='\0';
    }
    else if(*a!='\0' && *b=='\0'){
        char *pos=a;
        while(*a!='\0'){
            *b=*a;
            a++; b++;
        }
        *b='\0';
        *pos='\0';
    }
    else{
        ;
    }
}

void quick_sort(char **s, int left, int right)
{
    int i;
    int j;
    if(left<right){
        i=left;
        j=right+1;
        while(1){
            do{
                i++;
            }while(i<right && compare(s[left],s[i]));
            do{
                j--;
            }while(j>left && compare(s[j],s[left]));
            if(i<j){
                swap(s[i],s[j]);
            }
            else{
                break;
            }
        }
        swap(s[left],s[j]);
        quick_sort(s,left,j-1);
        quick_sort(s,j+1,right);
    }
}

char* Max_Subchar(char *s, int *pos)
{
    int len=strlen(s);
    char **post;
    post=(char **)malloc(sizeof(char *)*len);
    if(post==NULL)  exit(1);
    int i;
    for(i=0;i<len;i++){
        post[i]=(char *)malloc(sizeof(char)*(len+1));
        int pos=len-i-1;
        int j;
        for(j=0;j<i+2;j++){
            post[i][j]=s[pos];
            pos++;
        }
    }
    quick_sort(post,0,len-1);
    char *result=(char *)malloc(sizeof(char)*(len+1));
    int lenr=0;
    char *temp=(char *)malloc(sizeof(char)*(len+1));
    char *target;
    for(i=0;i<len-1;i++){
        int j=0;
        while(post[i][j]!='\0' && post[i+1][j]!='\0'){
            if(post[i][j]==post[i+1][j]){
                temp[j]=post[i][j];
                j++;
            }
            else{
                break;
            }
        }
        temp[j]='\0';
        if(j>lenr){
            int index=0;
            while(index<=j){
                result[index]=temp[index];
                index++;
            }
            lenr=j;
            if(strlen(post[i])>strlen(post[i+1])){
                target=post[i];
            }
            else{
                target=post[i+1];
            }
        }
    }
    if(lenr==0){
        printf("-1");
        *pos=-1;
        return NULL;
    }
    else{
        *pos=strlen(s)-strlen(target);
        printf("%s %d",result,*pos);
    }
    return result;
}

int main(){
    char s[300];
    scanf("%s",s);
    int pos;
    Max_Subchar(s,&pos);
    return 0;
}