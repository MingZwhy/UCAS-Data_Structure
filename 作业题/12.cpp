#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(){
    int n,arrsize;
    scanf("%d %d",&n,&arrsize);
    long long int a[22];
    int index=0;
    if(n>arrsize){
        printf("-1");
        return 0;
    }
    else{
        long long int max=pow(2,32)-1;
        int i=0;
        long long int sum=0;
        while(i<n){
            if(i==0){
                sum=1;
                a[index++]=sum;
            }
            else{
                if(max/i/2 < sum){
                    printf("-1");
                    return 0;
                }
                else{
                    sum=sum*i*2;
                    a[index++]=sum;
                }
            }
            i++;
        }
    }
    int i=0;
    while(i<n){
        printf("%lld ",a[i]);
        i++;
    }
    return 0;
}