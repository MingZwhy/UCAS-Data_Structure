#include<iostream>

using namespace std;

const int MAX_VERTEX_NUM = 100;
const int MAX_ARC_NUM = 100;
const int POSITIVE = 1; //正邻接表
const int NEGATIVE = 0; //逆邻接表
const int infinity = 10000;

typedef char ElemType;
typedef int InfoType;   //一般为权值

//链表中普通节点
typedef struct node{
    int vindex;   //临接点在头结点数组中的位置
    ElemType vex;  //临界点的实际元素
    int power;     //记录权值
    struct node *next;  //指向下一条弧的指针
}NodeLink, *Ptr_NodeLink;

//邻接表的头结点
typedef struct{
    ElemType vertex;
    Ptr_NodeLink first;
    int indegree;
    int MPL;
}Head_Node, *Head;

//弧或边的定义
typedef struct ArcType{
    ElemType vex1,vex2;   //弧或边所依附的两个顶点
    int power;   //弧的权值
}ArcType, *Ptr_ArcType;

typedef enum{DG,UDG} Graph_Kind;

typedef struct{
    int vexnum;   //图的顶点数
    int arcnum;  //图的边数
    Graph_Kind kind;  //图的种类
    bool Pos_Neg;     //正临接表或逆邻接表
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

//拓扑排序需要借助栈存储入度为0的顶点，存储的是其在head中的下标
class SqStack
{
private:
    int * base;
    int * top;
    int Stacknum;
public:
    SqStack();
    bool If_Empty() const;
    bool Push(int index);
    bool Pop(int & index);
    friend ostream & printstack(ostream & os, SqStack & s);
    ~SqStack();
};

SqStack::SqStack()
{
    base = new int [MAX_VERTEX_NUM];
    top = base;
    Stacknum = 0;
}

SqStack::~SqStack()
{
    delete [] base;
}

bool SqStack::If_Empty() const
{
    return (Stacknum==0);
}

bool SqStack::Push(int index)
{
    if(Stacknum<MAX_VERTEX_NUM){
        (*top)=index;
        top++;
        Stacknum++;
        return true;
    }
    else{
        cout << "full stack";
        return false;
    }
}

bool SqStack::Pop(int & index)
{
    if(Stacknum==0){
        cout << "empty stack";
        return false;
    }
    else{
        top--;
        index=*top;
        Stacknum--;
        return true;
    }
}

ostream & printstack(ostream & os, SqStack & s)
{
    int * visit=s.base;
    while(visit!=s.top){
        os << *visit << " ";
        visit++; 
    }
    return os;
}

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG, bool Pos_Neg=true);
//默认参数设置为无向图,正临接表
int LocateVex(Ptr_ALGraph G, ElemType &vp);
int AddVertex(Ptr_ALGraph G, ElemType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);
int * TopologicalSort(Ptr_ALGraph G);
void GetKeyPath(Ptr_ALGraph G, int * Topo);
bool FindArc(Ptr_ALGraph G, int v1, int v2, int & path_len);

void Print_AdjList(Ptr_ALGraph G);

//图的创建
Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind, bool Pos_Neg)
{
    G->kind=kind;
    G->vexnum=0; G->arcnum=0;
    G->Pos_Neg=Pos_Neg;
    return G;
}

//图的顶点定位
int LocateVex(Ptr_ALGraph G, ElemType &vp)
{
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->head[k].vertex==vp){
            return k;
        }
    }
    return -1;  //not found
}

//向图中增加顶点
int AddVertex(Ptr_ALGraph G, ElemType vp)
{
    using namespace std;
    int k,j;
    if(G->vexnum>=MAX_VERTEX_NUM){
        cout << "Vertex overflow !\n";
        return -1;
    }
    if(LocateVex(G,vp)!=-1){
        cout << "This Vertex has existed !\n";
        return -1;
    }
    G->head[G->vexnum].vertex=vp;
    G->head[G->vexnum].first=NULL;
    G->head[G->vexnum].indegree=0;
    G->head[G->vexnum].MPL=0;
    G->vexnum++;
    k=G->vexnum;
    return k;  //k为当前顶点数
}

//向图中增加一条弧
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc)
{
    using namespace std;
    int k,j;
    k=LocateVex(G,arc->vex1);
    j=LocateVex(G,arc->vex2);
    if(k==-1 || j==-1){
        cout << "Arc's Vertex do not existed!\n";
        return -1;
    }
    Ptr_NodeLink p = new NodeLink;
    Ptr_NodeLink q = new NodeLink;
    //p为边的起始节点
    p->vindex=k;
    p->vex=arc->vex1;
    p->power=arc->power;
    p->next=NULL;
    //q为边的末尾节点
    q->vindex=j;
    q->vex=arc->vex2; 
    q->power=arc->power;
    q->next=NULL;

    if(G->kind==DG){
        if(G->Pos_Neg){
            q->next = G->head[k].first;
            G->head[k].first = q;
            G->head[j].indegree++;
        }
        else{
            p->next = G->head[j].first;
            G->head[j].first = p;
            G->head[k].indegree++;
        }
    }
    else{
        p->next = G->head[j].first;
        G->head[j].first = p;
        q->next = G->head[k].first;
        G->head[k].first = q;
    }
    G->arcnum++;
    return 1;
}

void Print_AdjList(Ptr_ALGraph G){
    using namespace std;
    int i;
    Ptr_NodeLink visit;
    for(i=0;i<G->vexnum;i++){
        cout << G->head[i].vertex << ":";
        visit=G->head[i].first;
        if(visit!=NULL){
            cout << " ";
        }
        while(visit!=NULL){
            cout << visit->vex;
            if(visit->next!=NULL){
                cout << ",";
            }
            visit=visit->next;
        }
        if(i<G->vexnum-1){
            cout << endl;
        }
    }
}

//拓扑排序
int * TopologicalSort(Ptr_ALGraph G)
{
    int * Topo = new int[G->vexnum];
    SqStack S = SqStack();
    int * indegree = new int[G->vexnum];
    //初始化入度数组,同时使入度为0的顶点入栈
    for(int i=0;i<G->vexnum;i++){
        indegree[i]=G->head[i].indegree;
        if(indegree[i]==0){
            S.Push(i);
        }
    }

    int count=0; //对输出的顶点计数，若最终m=G->vexnum,则无环而有拓扑排序
    int index;
    Ptr_NodeLink visit;
    while(!S.If_Empty()){
        S.Pop(index);  //一个入度为0的顶点出栈，其下标赋给index
        Topo[count]=index;
        count++;
        for(visit=G->head[index].first;visit!=nullptr;visit=visit->next){
            int temp_index=visit->vindex;
            indegree[temp_index]--;   //index顶点的每个邻接点的入度减1
            if(indegree[temp_index]==0){
                S.Push(temp_index);    //若过入度减为0，则入栈
            }
        }
    }
    return Topo;
}

void GetKeyPath(Ptr_ALGraph G, int * Topo)
{
    int * ve = new int[G->vexnum];     //事件最早发生时间
    int * vl = new int[G->vexnum];     //事件最晚发生时间
    for(int i=0;i<G->vexnum;i++){
        ve[i]=0;
        vl[i]=infinity;
    }
    int v1,v2;
    int path_len;

    //先求到每个点的最长路径(最早发生时间)
    for(int j=0;j<G->vexnum;j++){
        v2=Topo[j];  //v2为目标点
        for(int k=0;k<j;k++){ 
            v1=Topo[k];   //v1为出发点，必须是v2的前驱
            if(FindArc(G,v1,v2,path_len)){
                if(ve[v1]+path_len>ve[v2]){
                    ve[v2]=ve[v1]+path_len;
                }
            }
        }
    }

    //再求顶点到汇点的最短路径(最晚发生时间)
    vl[G->vexnum-1]=ve[G->vexnum-1];  //汇点为关键点，最早发生时间=最晚发生时间
    for(int j=G->vexnum-2;j>=0;j--){
        v1=Topo[j];   //v1为要求的当前顶点，也为出发点
        for(int k=j+1;k<G->vexnum;k++){
            v2=Topo[k];   //v2为目标点
            if(FindArc(G,v1,v2,path_len)){
                if(vl[v2]-path_len<vl[v1]){
                    vl[v1]=vl[v2]-path_len;
                }
            }
        }
    }
    ElemType * KeyPath = new ElemType[G->vexnum];
    int index=0;
    cout << "vex: ";
    for(int i=0;i<G->vexnum;i++){
        cout << G->head[Topo[i]].vertex << " ";
        if(ve[i]==vl[i]){
            KeyPath[index++]=G->head[i].vertex;
        }
    }
    cout << endl << "ve:  ";
    for(int i=0;i<G->vexnum;i++){
        cout << ve[i] << " ";
    }
    cout << endl << "vl:  ";
    for(int i=0;i<G->vexnum;i++){
        cout << vl[i] << " ";
    }
    cout << endl << "Keypath is: ";
    for(int i=0;i<index;i++){
        cout << KeyPath[i];
        if(i<index-1){
            cout << "-->";
        }
    }
    delete [] Topo;
    delete [] ve;
    delete [] vl;
    delete [] KeyPath;
}

bool FindArc(Ptr_ALGraph G, int v1, int v2, int & path_len)
{
    Ptr_NodeLink visit=G->head[v1].first;
    while(visit!=nullptr){
        if(visit->vindex==v2){
            path_len=visit->power;
            return true;
        }
        visit=visit->next;
    }
    return false;
}

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G,DG,true);
    for(int i=0;i<8;i++){
        AddVertex(G,'a'+i);
    }
    AddVertex(G,'k');   //k没跟前面接上.....
    Ptr_ArcType arc = new ArcType[11];
    arc[0].vex1='a'; arc[0].vex2='b'; arc[0].power=6;
    arc[1].vex1='a'; arc[1].vex2='c'; arc[1].power=4;
    arc[2].vex1='a'; arc[2].vex2='d'; arc[2].power=5;  
    arc[3].vex1='b'; arc[3].vex2='e'; arc[3].power=1;
    arc[4].vex1='c'; arc[4].vex2='e'; arc[4].power=1;
    arc[5].vex1='e'; arc[5].vex2='g'; arc[5].power=8;
    arc[6].vex1='e'; arc[6].vex2='h'; arc[6].power=7;
    arc[7].vex1='g'; arc[7].vex2='k'; arc[7].power=2;
    arc[8].vex1='h'; arc[8].vex2='k'; arc[8].power=4;
    arc[9].vex1='d'; arc[9].vex2='f'; arc[9].power=2;
    arc[10].vex1='f'; arc[10].vex2='h'; arc[10].power=4;

    for(int i=0;i<11;i++){
        AddArc(G,arc+i);
    }
    GetKeyPath(G,TopologicalSort(G));
    delete G;
    return 0;
}