#include<iostream>
#include<cstring>

using namespace std;
const int MAXKEYLEN = 20;
typedef int KeyType;
typedef enum{LEAF,BRANCH} NodeKind;

typedef struct{
    char ch[MAXKEYLEN]; //关键字
    int num;            //关键字长度
}KeysType;              //关键字类型

typedef struct TrieNode{
    NodeKind kind;   //节点类型
    union{
        struct{KeysType K;} leaf;
        //叶子节点(关键字和整个路径)
        struct{TrieNode *ptr[27]; int num;} branch;
        //分支节点
        //0指向在上一层就结束的单词
        //假设每个电词以@结尾，则@-@=0刚好指向0节点
    };
}TrieNode, *TrieTree;

//ord求字符在字母表中序号
int ord(char c){return c-'`';}

bool SearchTrie(TrieTree T, KeysType K)
{
    TrieTree visit=T;
    int deep=0;
    while(deep<K.num){
        if(visit==nullptr){
            return false;
        }
        int pos = ord(K.ch[deep]);
        visit = visit->branch.ptr[pos];
        deep++;
    }
    if(visit!=nullptr && visit->kind==LEAF && strcmp(visit->leaf.K.ch,K.ch)==0){
        return true;
    }
    else{
        return false;
    }
}

void InitTrieTree(TrieTree &T)
{
    T->kind=BRANCH;
    T->branch.num=0;
    for(int i=0;i<27;i++){
        T->branch.ptr[i]=nullptr;
    }
}

void AddKey(TrieTree T, KeysType K)
{
    TrieTree visit; int deep;
    visit=T; deep=0;
    while(deep<K.num){
        int pos = ord(K.ch[deep]);

        if(visit->branch.ptr[pos]==nullptr){
            visit->branch.ptr[pos] = new TrieNode;
            if(deep==K.num-1){
                //K.ch[K.num-1]='@',pos=0
                visit->branch.ptr[pos]->kind=LEAF;
                visit->branch.ptr[pos]->leaf.K = K;
                break;
            }
            else{
                visit->branch.num++;   //该层多了一个节点
                InitTrieTree(visit->branch.ptr[pos]);
            }
            visit=visit->branch.ptr[pos];
        }
        else{
            visit=visit->branch.ptr[pos];
        }

        deep++;
    }
}

void DeleteKey(TrieTree T, KeysType K)
{
    TrieTree visit=T;
    TrieTree pre;
    int deep=0;
    int pos_of_pre;
    while(deep<K.num-1){
        if(visit==nullptr){
            return;
        }
        int pos = ord(K.ch[deep]);
        pre = visit;
        pos_of_pre = pos;
        visit = visit->branch.ptr[pos];
        deep++;
    }
    if(visit!=nullptr && visit->kind==BRANCH && visit->branch.ptr[0]!=nullptr && visit->branch.ptr[0]->kind==LEAF 
                      && strcmp(visit->branch.ptr[0]->leaf.K.ch,K.ch)==0)
    {
        //说明待删除节点存在
        visit->branch.ptr[0]=nullptr;
        if(visit->branch.num==0){
            //该层不存在节点，直接删去该层
            pre->branch.ptr[pos_of_pre]=nullptr;
            pre->branch.num--;
        }
    }
}

void My_Strcpy(KeysType &k , const char * s)
{
    int len = strlen(s);
    for(int i=0;i<len;i++){
        k.ch[i]=s[i];
    }
    k.ch[len]='`';
    k.ch[len+1]='\0';
    k.num=len+1;
}

int main(){
    TrieTree T = new TrieNode;
    InitTrieTree(T);
    char choice;
    char c;
    bool first=true;
    do{
        if(first){
            cin >> choice;
            first=false;
        }
        else{
            choice=c;
        }
        c=cin.get();
        KeysType k;
        k.num=1;    //for `
        int index=0;
        while((c=cin.get())!='\n'){
            k.ch[index++]=c;
            k.num++;
        }
        k.ch[index++]='`';
        k.ch[index]='\0';
        if(choice=='1'){
            AddKey(T,k);
        }
        else if(choice=='2'){
            DeleteKey(T,k);
        }
        else{
            if(SearchTrie(T,k)){
                cout << "True" << endl;
            }
            else{
                cout << "False" << endl;
            }
        }
        c=cin.get();
        if(c==EOF || c=='\n'){
            break;
        }
    }while(c!=EOF);
    return 0;
}