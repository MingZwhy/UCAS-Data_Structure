#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int s[5][2];
    int i,j;
    for(i=0;i<5;i++){
        for(j=0;j<2;j++){
            s[i][j]=0;
        }
    }
    char c;
    while(1){
        c=getchar();
        c=getchar();
        int xb;
        if((c=getchar())=='M'){
            xb=0;
        }
        else{
            xb=1;
        }
        c=getchar();
        int xx;
        xx=(c=getchar())-'A';
        c=getchar();
        int point;
        scanf("%d",&point);
        s[xx][xb]+=point;
        if((c=getchar())!='\n'){
        	break;
		}
    }
    
    for(i=0;i<5;i++){
        if(s[i][0]!=0){
            printf("%c M %d\n",'A'+i,s[i][0]);
        }
        if(s[i][1]!=0){
            printf("%c F %d\n",'A'+i,s[i][1]);
        }
        if(s[i][0]!=0 || s[i][1]!=0){
            printf("%c %d\n",'A'+i,s[i][0]+s[i][1]);
        }
    }
    return 0;
}