#include<iostream>

using namespace std;
const int MAXKEYLEN = 20;

typedef enum{LEAF,BRANCH} NodeKind;
//两种节点{叶子和分支}

typedef struct DLTNode{
    char symbol;
    struct DLTNode * next;   //指向兄弟节点的指针
    NodeKind kind;
    union{
        char * s;   //路径记录指针
        struct DLTNode * first;   //分支节点内的孩子链指针
    };
}DLTNode, *DLTree;

typedef struct{
    char ch[MAXKEYLEN]; //关键字
    int num;            //关键字长度
}KeysType;              //关键字类型

bool SearchDLTree(DLTree T, KeysType K)
{
    DLTree p;
    int i;
    p=T->first; i=0;  //初始化
    while(p!=nullptr && i<K.num){
        //查找关键字的第i位
        while(p!=nullptr && p->symbol!=K.ch[i]){
            p=p->next;
        }
        if(p!=nullptr && i<K.num-1){
            p = p->first;      //准备查找下一位
        }
        i++;
    }
    return (p!=nullptr);
}
