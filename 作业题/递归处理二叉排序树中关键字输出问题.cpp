#include<iostream>

using namespace std;

typedef struct TreeNode{
    int data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
}S_Tree, *SearchTree;

typedef SearchTree Root;
typedef SearchTree TNode; 

Root Insert(Root T, int x){
    if(T==NULL){
        T=(TNode)malloc(sizeof(S_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
        }
        T->data=x;
        T->lchild=NULL;
        T->rchild=NULL;
    }
    else{
        if(x<T->data){
            T->lchild=Insert(T->lchild,x);
        }
        else if(x>T->data){
            T->rchild=Insert(T->rchild,x);
        }
        else{
            printf("node x has existed");
        }
    }
    return T;
}

void Just_Visit(Root T)
{
    if(T!=nullptr){
        Just_Visit(T->rchild);
        cout << T->data << " ";
        Just_Visit(T->lchild);
    }
}

void Visit(Root T, int x)
{
    if(T!=nullptr){
        if(x>T->data){
            Visit(T->rchild,x);
        }
        else if(x==T->data){
            Just_Visit(T->rchild);
            cout << T->data << " ";
        }
        else{
            Just_Visit(T->rchild);
            cout << T->data << " ";
            Visit(T->lchild,x);
        }
    }
}

int main(){
    Root T=nullptr;
    char c;
    int elem;
    do{
        cin >> elem;
        T = Insert(T,elem);
        c = cin.get();
    }while(c!='\n');
    int x;
    cin >> x;
    Visit(T,x);
    return 0;
}