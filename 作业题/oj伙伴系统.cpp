#include<iostream>
#include<cmath>  

using namespace std;

typedef struct WORD_b{
    WORD_b * llink;   //前驱结点 
    int index;        //下标地址
    int tag;          //块的占用标识
    int kval;         //块的大小(2的幂次)
    WORD_b * rlink;   //后继结点
}WORD_b, head, *Ptr_Word;

typedef struct HeadNode{
    int nodesize;  //空闲块大小
    WORD_b * first;  //表头指针
}HeadNode, *Ptr_HeadNode;

int Buddy(int k, int pos);    //计算其伙伴首地址

class Fri_Sys
{
private:
    int size;
    Ptr_HeadNode sys;
public:
    Fri_Sys();   //默认构造函数
    Fri_Sys(int s);   //构造函数
    void Initialize_sys(int s);   //初始化
    void Add_free_Space(int k, int pos);  //插入空闲块
    void Recycle_Space(int k, int pos);   //回收块
    void Print() const;
    ~Fri_Sys();    //析构函数;
};

Fri_Sys::Fri_Sys() {}

Fri_Sys::Fri_Sys(int s)
{
    size=s;
    sys = new HeadNode[size];
    for(int i=0;i<size;i++){
        sys[i].first=NULL;
        sys[i].nodesize=i;
    }
}

void Fri_Sys::Initialize_sys(int s)
{
    size=s;
    sys = new HeadNode[size];
    for(int i=0;i<size;i++){
        sys[i].first=NULL;
        sys[i].nodesize=i;
    }
}

void Fri_Sys::Add_free_Space(int k, int pos)
{
    WORD_b * temp = new WORD_b;
    temp->kval=k;
    temp->index=pos;
    temp->tag=0;
    temp->rlink=NULL;
    if(sys[k].first==NULL){
        sys[k].first=temp;
    }
    else{
        Ptr_Word visit=sys[k].first;
        while(visit->rlink!=NULL){
            visit=visit->rlink;
        }
        visit->rlink=temp;
        temp->llink=visit;
    }
}

void Fri_Sys::Recycle_Space(int k, int pos)
{
    int fri=Buddy(k,pos);
    Ptr_Word visit=sys[k].first;
    Ptr_Word pre;
    bool flag=false;   //true-->有汇集上传. false-->无汇集，插入
    while(visit!=NULL){
        if(visit->index==fri){
            flag=true;

            if(sys[k].first==visit){
                sys[k].first=visit->rlink;
                if(visit->rlink!=nullptr){
                    visit->rlink->llink=sys[k].first;
                }
            }
            else{
                pre->rlink=visit->rlink;
                visit->rlink->llink=pre;
            }
                
            if(fri<pos){
                Fri_Sys::Recycle_Space(k+1,fri);
            }
            else{
                Fri_Sys::Recycle_Space(k+1,pos);
            }
            break;
        }
        else{
            pre=visit;
            visit=visit->rlink;
        }
    }
    if(!flag){
        Ptr_Word p = new WORD_b;
        p->index=pos;
        p->kval=k;
        p->tag=0;
        if(sys[k].first==nullptr || sys[k].first->index>pos){
            if(sys[k].first==nullptr){
                sys[k].first=p;
                p->rlink=nullptr;
            }
            else{
                p->rlink=sys[k].first;
                sys[k].first->llink=p;
                sys[k].first=p;
            }
        }
        else{
            visit=sys[k].first;
            while(visit->rlink!=NULL && visit->rlink->index<pos){
                visit=visit->rlink;
            }
            p->llink=visit;
            p->rlink=visit->rlink;
            if(visit->rlink!=NULL){
                visit->rlink->llink=p;
            }
            visit->rlink=p;
        }
    }
}

void Fri_Sys::Print() const
{
    for(int i=0;i<size;i++){
        if(sys[i].first!=NULL){
            cout << i << " ";
            Ptr_Word visit=sys[i].first;
            while(visit!=NULL){
                cout << visit->index;
                if(visit->rlink!=nullptr){
                    cout << " ";
                }
                visit=visit->rlink;
            }
            cout << endl;
        }
    }
}

int Buddy(int k, int pos)
{
    int x=pow(2,k+1);
    if(pos%x==0){
        return pos + pow(2,k);
    }
    else if(pos%x==pow(2,k)){
        return pos - pow(2,k);
    }
    else{
        return 0;
    }
}

Fri_Sys::~Fri_Sys()
{
    delete [] sys;
}

int main(){
    Fri_Sys s(10);
    int k,pos;
    int c=0;
    while(1){
        c=getchar();
        if(c=='\n'){
            break;
        }
        k=c-'0';
        c=getchar();
        while(c!='\n'){
            cin >> pos;
            s.Add_free_Space(k,pos);
            c=cin.get();
        }
    }
    while(1){
        c=getchar();
        if(c==EOF || c=='\n'){
            break;
        }
        else{
            k=c-'0';
        }
        cin.get();
        cin >> pos;
        s.Recycle_Space(k,pos);
        c=getchar();
        if(c=='\n'){
            continue;
        }
        else if(c==EOF){
            break;
        }
    }
    s.Print();
    return 0;
}