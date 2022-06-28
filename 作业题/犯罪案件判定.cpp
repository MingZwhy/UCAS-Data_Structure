#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define bool int
#define true 1
#define false 0

typedef struct{
    int *ANJ;
    int num;
}gang;

void judge(gang A, gang B, int index1, int index2)
{
    bool f_in_A, f_in_B, s_in_A, s_in_B;
    f_in_A=false; f_in_B=false; s_in_A=false; s_in_B=false;
    int i;
    for(i=1;i<=A.num;i++){
        if(A.ANJ[i]==index1){
            f_in_A=true;
        }
        if(A.ANJ[i]==index2){
            s_in_A=true;
        }
    }
    for(i=1;i<=B.num;i++){
        if(B.ANJ[i]==index1){
            f_in_B=true;
        }
        if(B.ANJ[i]==index2){
            s_in_B=true;
        }
    }
    if((f_in_A && s_in_A) || (f_in_B && s_in_B)){
        printf("same\n");
    }
    else if((f_in_A && s_in_B) || (f_in_B && s_in_A)){
        printf("not same\n");
    }
    else{
        printf("not sure\n");
    }
}

void check(gang *A, gang *B, int index1, int index2)
{
    bool f_in_A, f_in_B, s_in_A, s_in_B;
    f_in_A=false; f_in_B=false; s_in_A=false; s_in_B=false;
    int i;
    for(i=1;i<=A->num;i++){
        if(A->ANJ[i]==index1){
            f_in_A=true;
        }
        if(A->ANJ[i]==index2){
            s_in_A=true;
        }
    }
    for(i=1;i<=B->num;i++){
        if(B->ANJ[i]==index1){
            f_in_B=true;
        }
        if(B->ANJ[i]==index2){
            s_in_B=true;
        }
    }
    if(f_in_A){
        B->ANJ[B->num]=index2;
        B->num++;
    }
    else if(f_in_B){
        A->ANJ[A->num]=index2;
        A->num++;
    }
    else if(s_in_A){
        B->ANJ[B->num]=index1;
        B->num++;
    }
    else if(s_in_B){
        A->ANJ[A->num]=index1;
        A->num++;
    }
    else{  //两个案件都没出现过线索
        A->ANJ[A->num]=index1;
        A->num++;
        B->ANJ[B->num]=index2;
        B->num++;
    }
}

int main(){
    int T;
    scanf("%d\n",&T);
    int i;
    for(i=1;i<=T;i++){
        int N,M;
        scanf("%d %d\n",&N,&M);
        gang A;
        A.ANJ=(int *)malloc(sizeof(int)*(N+1)); //0弃用
        A.num=1;
        gang B;
        B.ANJ=(int *)malloc(sizeof(int)*(N+1));
        B.num=1;
        int j;
        for(j=1;j<=M;j++){
            char c;
            int index1,index2;
            scanf("%c %d %d",&c,&index1,&index2);
            char eat=getchar();
            if(c=='A'){
                judge(A,B,index1,index2);
            }
            else{
                check(&A,&B,index1,index2);
            }
        }
    }
    return 0;
}