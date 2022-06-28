#include<iostream>

using namespace std;

int * Count_Sort(int * s, int n)
{
    int * count = new int[n+1];
    for(int i=1;i<=n;i++){
        count[i]=0;
    }
    int i;
    for(i=1;i<=n;i++){
        int j;
        for(j=1;j<=n;j++){
            if(s[j]<s[i]){
                count[i]++;
            }
        }
    }
    return count;
}

int main(){
    int s[1000];
    int i=1;
    char c;
    do{
        cin >> s[i++];
        c=getchar();
    }while(c!='\n');
    int * count = Count_Sort(s,i-1);
    for(int j=1;j<=i-1;j++){
        cout << count[j] << " ";
    }
    return 0;
}