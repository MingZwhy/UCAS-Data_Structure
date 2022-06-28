//目前实现了判断两点之间是否有回路和是否有定长回路两种功能
#include<iostream>
const int MAX_VERTEX_NUM = 100;
const int MAX_ARC_NUM = 100;

typedef int ElemType;
typedef int InfoType;   //一般为权值

//链表中普通节点
typedef struct node{
    int vindex;   //临接点在头结点数组中的位置
    ElemType vex;  //临界点的实际元素
    struct node *next;  //指向下一条弧的指针
}NodeLink, *Ptr_NodeLink;

//邻接表的头结点
typedef struct{
    ElemType vertex;
    Ptr_NodeLink first;
}Head_Node, *Head;

//弧或边的定义
typedef struct ArcType{
    ElemType vex1,vex2;   //弧或边所依附的两个顶点
}ArcType, *Ptr_ArcType;

typedef enum{DG,UDG} Graph_Kind;

typedef struct{
    int vexnum;   //图的顶点数
    int arcnum;  //图的边数
    Graph_Kind kind;  //图的种类
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG);
//默认参数设置为无向图
int LocateVex(Ptr_ALGraph G, ElemType *vp);
int AddVertex(Ptr_ALGraph G, ElemType *vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);
void Print_AdjList(Ptr_ALGraph G);
bool Find_Path_h(Ptr_ALGraph G, ElemType vp1, ElemType vp2);
bool Find_Path(Ptr_ALGraph G, ElemType vp1, ElemType vp2, bool * visited);
bool Find_Path_len_h(Ptr_ALGraph G, ElemType vp1, ElemType vp2, int len);
bool Find_Path_len(Ptr_ALGraph G, ElemType vp1, ElemType vp2, int len, bool * visited);

//图的创建
Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind)
{
    G->kind=kind;
    G->vexnum=0; G->arcnum=0;
    return G;
}

//图的顶点定位
int LocateVex(Ptr_ALGraph G, ElemType *vp)
{
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->head[k].vertex==*vp){
            return k;
        }
    }
    return -1;  //not found
}

//向图中增加顶点
int AddVertex(Ptr_ALGraph G, ElemType *vp)
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
    G->head[G->vexnum].vertex=*vp;
    G->head[G->vexnum].first=NULL;
    G->vexnum++;
    k=G->vexnum;
    return k;  //k为当前顶点数
}

//向图中增加一条弧
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc)
{
    using namespace std;
    int k,j;
    k=LocateVex(G,&arc->vex1);
    j=LocateVex(G,&arc->vex2);
    if(k==-1 || j==-1){
        cout << "Arc's Vertex do not existed!\n";
        return -1;
    }
    Ptr_NodeLink p = new NodeLink;
    Ptr_NodeLink q = new NodeLink;
    //p为边的起始节点
    p->vindex=k;
    p->vex=arc->vex1;
    p->next=NULL;
    //q为边的末尾节点
    q->vindex=j;
    q->vex=arc->vex2; 
    q->next=NULL;

    q->next = G->head[k].first;
    G->head[k].first = q;

    if(G->kind==UDG){
        p->next = G->head[j].first;
        G->head[j].first = p;
    }
    G->arcnum++;
    return 1;
}

void Print_AdjList(Ptr_ALGraph G){
    using namespace std;
    int i;
    Ptr_NodeLink visit;
    for(i=0;i<G->vexnum;i++){
        cout << G->head[i].vertex;
        visit=G->head[i].first;
        if(visit!=NULL){
            cout << " ";
        }
        while(visit!=NULL){
            cout << visit->vindex;
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

bool Find_Path_len_h(Ptr_ALGraph G, ElemType vp1, ElemType vp2, int len)
{
    using namespace std;
    cout << "Try to find path from" << vp1 << "to" << vp2 << "in Graph G" << "in length" << len << endl;
    bool * visited = new bool[G->vexnum];
    for(int i=0;i<G->vexnum;i++){
        visited[i]=false;
    }
    if(Find_Path_len(G,vp1,vp2,len,visited)){
        cout << "Find it!" << endl;
    }
    else{
        cout << "no such path" << endl;
    }
}

bool Find_Path_len(Ptr_ALGraph G, ElemType vp1, ElemType vp2, int len, bool * visited)
{
    if(vp1==vp2 && len==0){
        return true;
    }
    else if(len>0){
        int pos1=LocateVex(G,&vp1);
        visited[pos1]=true;
        Ptr_NodeLink visit;
        visit=G->head[pos1].first;
        while(visit!=NULL){
            if(visited[visit->vindex]==false && Find_Path_len(G,visit->vex,vp2,len-1,visited)){
                return true;
            }
            visit=visit->next;
        }
        //恢复可能性
        visited[pos1]=false;
    }
    return false;
}