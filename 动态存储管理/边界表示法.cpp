#include<iostream>

using namespace std;

typedef struct word{
    union{
        struct word *llink;   //若是头部域，则用llink指向上一块的头部
        struct word *uplink;  //若是尾部域，则用uplink指向本块的头部
    };
    int tag;  // 0-->空闲；1-->占用
    int size; //头部域，块大小
    int fake_index;   //虚拟地址
    struct word * rlink;  //头部域，指向后继节点
}WORD,head,foot,*Space;

//定义内联函数由head指针计算foot指针
inline Space Goto_foot(Space p) {return p + p->size - 1;}

//分配一整块内存
WORD * Buffer = new WORD[150000];

class BoundTag
{
private:
    Space pav;
    WORD * ground;    //整块内存
    int WholeSize; 
    int e;            //约定的最小舍弃数，即如果空闲块的大小比欲分配的空间大不超过e，则整块分配
public:
    BoundTag(int size, int min_e, int fake_index);
    void Link_Space(BoundTag & next);      //构造一个离散的链式空闲块，连接不同的空闲块
    Space Find_Empty(int n) const;
    Space AllocBoundTag(int n);  //n为申请的空间，返回的是分配的空间的首地址
    void Recycle_Space(Space p);  //回收空间,当从一整块内存中连续alloc并recycle时用此方法
    void Print_Space(void) const;
    ~BoundTag();
};

void Init_Buffer(void);

BoundTag::BoundTag(int size, int min_e, int fake_index)
{
    ground = new (Buffer+fake_index)  WORD[size];
    WholeSize=size;
    e=min_e;
    ground[0].tag=0; ground[0].llink=&ground[0]; ground[0].rlink=&ground[size-1];
    ground[0].size=WholeSize;
    ground[0].fake_index=fake_index;
    ground[size-1].tag=0; ground[size-1].uplink=&ground[0]; ground[size-1].rlink=&ground[0];
    pav=&ground[0];
}

Space BoundTag::Find_Empty(int n) const
{
    Space p;
    for(p=pav; (p && p->size<n && p->rlink!=pav); p=p->rlink);
    if(p==nullptr || p->size<n){
        return nullptr;
    }
    else{
        return p;        //返回满足条件的空闲块的头部域指针(首地址)
    }
}

Space BoundTag::AllocBoundTag(int n)
{
    Space p = BoundTag::Find_Empty(n);
    if(p==nullptr){
        return nullptr;
    }
    Space f=Goto_foot(p);   //f指向空闲块的尾部
    //!!!!!!!!!!
    pav = p->rlink;         //pav指向p结点的后继节点,为了提高查找速度(约定)
    //!!!!!!!!!!

    if(p->size-n<=e){       //整块分配，不保留剩余量
        if(pav==p){
            pav=NULL;       //可利用空间变为空表
        }
        else{
            pav->llink=p->llink;   
            p->llink->rlink=pav;
        }
        //修改分配结点的头部和底部标志
        p->tag=1; f->tag=1;
    }
    else{   //分配该块的后n个字(高地址)
        f->tag=1;      //修改分配块的底部标志
        p->size-=n;    //置剩余块大小
        f=Goto_foot(p);   //f重新指向剩余块的底部
        f->tag = 0; f->uplink=p;   //设置剩余块底部
        p=f+1;      //f指向分配块头部
        p->tag=1;   p->size=n;     //设置分配块头部
    }
    WholeSize -= n;
    return p;
}

void BoundTag::Link_Space(BoundTag & next)
{
    Space temp = next.pav->llink;  //指向next的链尾

    Space visit=pav->llink;  //调用对象的链尾
    visit->rlink=next.pav;
    next.pav->llink=visit;
    temp->rlink=pav;
    pav->llink=temp;
    WholeSize += next.WholeSize;
}

void BoundTag::Print_Space(void) const
{
    Space visit=pav;
    cout << "Left space size is: " << WholeSize << endl;
    do{
        cout << "address" << visit->fake_index << ": size: " << visit->size;
        if(visit->rlink!=pav){
            cout << "---> ";
        }
        visit=visit->rlink;
    }while(visit!=pav);
    cout << endl;
}

void BoundTag::Recycle_Space(Space p)
{
    if((p->fake_index==0 || (p-1)->tag==1) && (p+p->size)->tag==1){    //左右均占用
        p->tag=0;
        Goto_foot(p)->uplink=p;
        Goto_foot(p)->tag=0;
        if(pav==nullptr){
            pav=p->llink=p->rlink=p;  //第一个空闲块
        }
        else{    //依据升序插入
            Space q=pav;
            if(p->fake_index<q->fake_index){
                p->rlink=q;
                p->llink=q->llink;
                q->llink->rlink=p;
                q->llink=p;
                pav=p;
            }
            else{
                while(p->fake_index>q->fake_index){
                    q=q->rlink;
                    if(q==pav){
                        break;
                    }
                }
                p->rlink=q;
                q->llink->rlink=p;
                p->llink=q->llink;
                q->llink=p;
            }
        }
    }
    else if((p->fake_index!=0 && (p-1)->tag==0) && (p+p->size)->tag==1){  //左空右占用
        //和左邻块合并成一个大的空闲块节点，改变左邻块的size域并重新设置合并后节点的底部
        Space q=(p-1)->uplink;
        q->size+=p->size;
        Space f=Goto_foot(p);
        f->uplink=q;
        f->tag=0;
    }
    else if((p->fake_index==0 || (p-1)->tag==1) && (p+p->size)->tag==0){  //左占用右空
        //和右邻块合并成一个大的空闲块节点，改变右邻块的size域并重新设置合并后节点的底部
        Space t=p+p->size;    //t指向右邻块的头部
        if(pav==t){
            pav=p;
        }
        Space q=t->llink;     //q指向左邻块的头部
        p->tag=0;  
        p->llink=q; q->rlink=p;   //设置空闲块的前驱
        p->size+=t->size;         //设置新空闲块的大小
        p->rlink = t->rlink;
        t->rlink->llink=p;
        Space f=Goto_foot(t);     //f指向右邻块的尾部
        f->uplink=p;              //尾部指针指向新节点的头部
        f->tag=0;
    }
    else{        //左右均空
        Space s=(p-1)->uplink;    //s指向左邻块头部
        Space t=(p+p->size);      //t指向右邻块头部
        s->size = s->size + p->size + t->size;
        //在空闲链表中，删除右邻块
        Space q1=t->rlink;  //右邻的右
        Space q=t->llink;   //右邻的左
        q->rlink=q1;  q1->llink=q;
        //新节点底部指针指向其头部(原s的头部)
        Goto_foot(t)->uplink=s;
    }
    WholeSize += p->size;
}

BoundTag::~BoundTag(){}

void Init_Buffer(void)
{
    for(int i=0;i<150000;i++){
        Buffer[i].tag=1;      //为了让离散的空闲块之间的空间默认为占用的
    }
}

int main(){
    Init_Buffer();
    BoundTag s(5000,100,0);
    BoundTag add1(8000,100,31000);
    BoundTag add2(41000,100,59000);
    s.Link_Space(add1);
    s.Link_Space(add2);
    cout << "discrete free space list:" << endl;
    s.Print_Space();
    Space aloc1=s.AllocBoundTag(1000);
    Space aloc2=s.AllocBoundTag(6000);
    Space aloc3=s.AllocBoundTag(20000);
    cout << "After 3 malloc: " << endl;
    s.Print_Space();
    s.Recycle_Space(aloc1);
    s.Recycle_Space(aloc2);
    s.Recycle_Space(aloc3);
    cout << "After recycle: " << endl;
    s.Print_Space();
    delete [] Buffer;
    return 0; 
}