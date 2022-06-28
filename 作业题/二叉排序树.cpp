#include<iostream>

using namespace std;

typedef struct STree{
    int data;
    int left;
    int right;
}STree, *SearchTree;

bool Judge(STree t, SearchTree T){
    if(t.left!=-1){
        if(!(Judge(T[t.left],T) && t.data>T[t.left].data)){
            return false;
        }
    }
    if(t.right!=-1){
        if(!(Judge(T[t.right],T) && t.data<T[t.right].data)){
            return false;
        }
    }
    return true;
}

int main(){
    SearchTree T = new STree[100];
    int index=0;
    char c;
    do{
        cin >> T[index].data;
        c=cin.get();
        cin >> T[index].left;
        c=cin.get();
        cin >> T[index].right;
        index++;
        c=cin.get();
    }while(c!=EOF);
    if(Judge(T[0],T)){
        cout << "1";
    }
    else{
        cout << "0";
    }
    delete [] T;
    return 0;
}