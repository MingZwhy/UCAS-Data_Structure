#include<iostream>
using namespace std;

int main(){
    int color[3]={0,0,0};
    char c;
    int flag;
    do{
        cin >> flag;
        color[flag-1]++;
        c=cin.get();
    }while(c!='\n');
    for(int i=0;i<3;i++){
        while(color[i]>0){
            cout << i+1 << " ";
            color[i]--;
        }
    }
    return 0;
}