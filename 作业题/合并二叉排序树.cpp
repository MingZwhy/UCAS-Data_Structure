#include<iostream>
#include<cstdlib>

using namespace std;

typedef struct BiTree{
    int data;
    struct BiTree * left;
    struct BiTree * right;
}BiTree, *BiNode;

typedef BiNode root;
typedef BiNode node;

root MakeTree(int * elem, int &index)
{
    if(elem[index]==-1){
        index++;
        return nullptr;
    }
    else{
        node t = new BiTree;
        t->data=elem[index++];
        t->left = MakeTree(elem,index);
        t->right = MakeTree(elem,index);
        return t;
    }
}

root Insert(root T, int x)
{
    if(T==nullptr){
        T = new BiTree;
        T->data = x;
        T->left = nullptr;
        T->right = nullptr;
    }
    else{
        if(x<T->data){
            T->left=Insert(T->left,x);
        }
        else if(x>T->data){
            T->right=Insert(T->right,x);
        }
        else{
            cout << "this node has existed";
        }
    }
    return T;
}

root Merge(root t1, root t2)
{
    if(t2!=nullptr){
        t1 = Merge(t1,t2->left);
        t1 = Insert(t1,t2->data);
        t1 = Merge(t1,t2->right);
    }
    return t1;
}

void Mid_Visit(root T)
{
    if(T!=nullptr){
        Mid_Visit(T->left);
        cout << T->data << " ";
        Mid_Visit(T->right);
    }
}

int main(){
    int t1[100];
    int t2[100];
    int index=0;
    char c;
    do{
        cin >> t1[index++];
        c=cin.get();
    }while(c!='\n');
    index=0;
    do{
        cin >> t2[index++];
        c=cin.get();
    }while(c!='\n');
    index=0;
    BiNode T1 = MakeTree(t1,index);
    index=0;
    BiNode T2 = MakeTree(t2,index);
    BiNode merged = Merge(T1,T2);
    Mid_Visit(merged);
    return 0;
}