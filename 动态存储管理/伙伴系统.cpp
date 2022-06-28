#include<iostream>  
#include<cmath>

using namespace std;
const int m = 10;
char * buffer = new char[1024];   //申请了一块1KB的buffer

typedef struct WORD_b{
    WORD_b * llink;   //前驱结点 
    int tag;          //块的占用标识
    int kval;         //块的大小(2的幂次)
    char * space;     //具体空间(从buffer中分配)
    int index;        //空间首地址(相对buffer首地址而言)
    WORD_b * rlink;   //后继结点
}WORD_b, head, *Ptr_Word;

typedef struct HeadNode{
    int nodesize;  //空闲块大小(以字节为单位)
    WORD_b  first;  //表头指针,表头不存块
}HeadNode, *Ptr_HeadNode;

int Buddy(int k, int pos);

class Fri_Sys
{
private:
    int size;
    Ptr_HeadNode sys;
public:
    Fri_Sys();   //默认构造函数
    Fri_Sys(int s);   //构造函数
    void Initialize_sys(int s);   //初始化
    void Add_Free_Space(int k);   //从buffer中往伙伴系统里加空块，k为空间大小的幂数
    Ptr_Word AllocBuddy(int n);   //分配空间，n为所需空间大小的字节数
    void Recycle_Space(Ptr_Word p);  //回收内存块
    void Print_FS(void) const;  
    ~Fri_Sys();    //析构函数;
};

Fri_Sys::Fri_Sys() {}

Fri_Sys::Fri_Sys(int s)
{
    size=s;
    sys = new HeadNode[size+1];
    for(int i=0;i<=size;i++){    
        sys[i].first.rlink=nullptr;
        sys[i].nodesize=0;
    }
}

void Fri_Sys::Initialize_sys(int s)
{
    size=s;
    sys = new HeadNode[size+1];
    for(int i=0;i<=size;i++){
        sys[i].first.rlink=nullptr;
        sys[i].nodesize=0;
    }
}

void Fri_Sys::Add_Free_Space(int k)
{
    if(k>m){
        cout << "Exceeds space limits!" << " exit";
    }
    else{
        Ptr_Word s = new WORD_b;
        s->index=0;   //从buffer的首地址开始
        s->kval=k;
        int count=(int)pow(2,k);
        s->space = new (buffer) char[count];   //从buffer分配空间
        s->tag=0;
        s->rlink=s;   //自己指向自己
        sys[k].first.rlink=s;
        s->llink=s;   //自己指向自己
        sys[k].nodesize += pow(2,k);    //更新空间大小
    }
}

Ptr_Word Fri_Sys::AllocBuddy(int n)
{
    //查找满足分配要求的子表
    int k;
    for(k=0;k<=size;k++){
        if(sys[k].nodesize>n){
            break;
        }
    }
    if(k>size){
        cout << "no space including n, alloc error";
        return nullptr;
    }
    Ptr_Word pa,pre,suc,pi;
    pa = sys[k].first.rlink;
    pre = pa->llink;   suc = pa->rlink;
    if(pa==suc){
        //分配后该子表变成空表
        sys[k].first.rlink=nullptr;
        sys[k].nodesize=0;
    }
    else{
        //从子表删去pa节点
        pre->rlink=suc;  suc->llink=pre;
        sys[k].first.rlink=suc;
    }
    //将剩余块插入到相应子表
    int i;
    for(i=1; pow(2,k-i)>=n; i++){
        pi = new WORD_b;
        pi->space=pa->space+(int)pow(2,k-i);  //后半段
        pi->index=pa->index+(int)pow(2,k-i);
        //因为查找空闲块是从小往大查的，所以可以确保这里原本是空的链表
        pi->rlink=pi->llink=pi;
        pi->kval=k-i;
        sys[k-i].first.rlink=pi;
        sys[k-i].nodesize+=pow(2,k-i);
    }
    //返回块
    pa->tag=1;  pa->kval=k-(i-1);
    return pa;
}

void Fri_Sys::Recycle_Space(Ptr_Word p)
{
    int k=p->kval; 
    int pos=p->index;
    int fri=Buddy(k,pos);
    Ptr_Word visit = sys[k].first.rlink;
    bool flag=false;  //true-->有汇集上传，false-->无汇集，插入
    while(visit!=nullptr){
        if(visit->index==fri){
            flag=true;
            if(visit->rlink==visit){  //如果只有visit一项
                sys[k].first.rlink=nullptr;
            }
            else{
                Ptr_Word pre=visit->llink;
                Ptr_Word suc=visit->rlink;
                pre->rlink=suc;
                suc->llink=pre;
                visit->llink=visit->rlink=visit;
                if(sys[k].first.rlink==visit){
                    sys[k].first.rlink=suc;
                }
            }

            //融合p和visit
            if(fri<pos){
                //fri<pos-->visit在前
                visit->kval+=1;
                //visit->index不变
                Fri_Sys::Recycle_Space(visit);
            }
            else{
                //pos>fri-->p在前
                p->kval+=1;
                //p->index不变
                Fri_Sys::Recycle_Space(p);
            }

            break;
        }
        else{
            visit=visit->rlink;
        }
    }
    if(!flag){
        Ptr_Word q=sys[k].first.rlink;
        if(q==nullptr){
            p->rlink=p->llink=p;
            sys[k].first.rlink=p;
        }
        else{
            p->rlink=q;
            p->llink=q->llink;
            q->llink->rlink=p;
            q->llink=p;
            sys[k].first.rlink=p;
        }
    }
}

void Fri_Sys::Print_FS() const
{
    for(int i=0;i<=size;i++){
        cout << i << ": " << sys[i].nodesize << ": ";
        Ptr_Word visit=sys[i].first.rlink;
        if(visit!=nullptr){
            cout << visit->index << "--" << visit->index+(int)pow(2,visit->kval) << "  ";
            while(visit->rlink!=sys[i].first.rlink){
                cout << visit->index << "--" << visit->index+(int)pow(2,visit->kval) << "  ";
            }
        }
        cout << endl;
    }
}

Fri_Sys::~Fri_Sys()
{
    delete [] sys;
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

int main(){
    Fri_Sys fs(15);
    fs.Add_Free_Space(8);
    fs.Print_FS();
    cout << endl << endl << endl;
    Ptr_Word s1 = fs.AllocBuddy(16);
    fs.Print_FS();
    cout << endl << endl << endl;
    Ptr_Word s2 = fs.AllocBuddy(78);
    fs.Print_FS();
    cout << endl << endl << endl;
    fs.Recycle_Space(s1);
    fs.Print_FS(); 
    cout << endl << endl << endl;
    fs.Recycle_Space(s2);
    fs.Print_FS(); 
    delete [] buffer;
    return 0;
}