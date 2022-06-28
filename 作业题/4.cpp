#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(){
    char c;
    int x,n;
    int a;
    scanf("%d %d",&x,&n);
    c=getchar();
    int r=0;
    int i=0;
    while(i<=n)
    {
        scanf("%d",&a);
        r+=a*pow(x,i);
        i++;
        if((c=getchar())==EOF){
            break;
        }
    }
    printf("%d",r);
    return 0;
}