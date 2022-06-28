#include<iostream>
const int MAX_VERTEX_NUM = 100;
const int MAX_ARC_NUM = 100;
const int POSITIVE = 1; //正邻接表
const int NEGATIVE = 0; //逆邻接表

typedef char ElemType;
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
    bool Pos_Neg;     //正临接表或逆邻接表
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG, bool Pos_Neg=true);
//默认参数设置为无向图,正临接表
int LocateVex(Ptr_ALGraph G, ElemType &vp);
int AddVertex(Ptr_ALGraph G, ElemType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);

void Connected_DG(Ptr_ALGraph G_pos, Ptr_ALGraph G_neg);
void Rev_DFS_DG(Ptr_ALGraph G, int * stack, int &count, bool *visited, int vp);
void DFS_DG(Ptr_ALGraph G, bool *visited, int vp);

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
    p->next=NULL;
    //q为边的末尾节点
    q->vindex=j;
    q->vex=arc->vex2; 
    q->next=NULL;

    if(G->kind==DG){
        if(G->Pos_Neg){
            q->next = G->head[k].first;
            G->head[k].first = q;
        }
        else{
            p->next = G->head[j].first;
            G->head[j].first = p;
        }
    }
    else{
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

void Connected_DG(Ptr_ALGraph G_pos, Ptr_ALGraph G_neg)
{
    using namespace std;
    bool * visited = new bool[G_pos->vexnum];
    for(int i=0;i<G_pos->vexnum;i++){
        visited[i]=false;
    }
    //对反向图进行逆后序遍历时需要栈，由于此栈先一直进再一直出，用数组代替即可
    //数组中存储的数时访问到的顶点在head中的下标
    int * stack = new int[G_pos->vexnum];
    int top=0;   //作为stack的下标
    for(int v=0;v<G_pos->vexnum;v++){
        if(!visited[v]){
            Rev_DFS_DG(G_neg,stack,top,visited,v);   //注意这里逆后序遍历的是反向图，即反向邻接表
        }
    }
    //现在我们得到了stack，按照从栈顶到栈底的顺序对正向图(正邻接表)进行正常遍历即可
    //在此之前，我们先重置visited数组
    for(int i=0;i<G_pos->vexnum;i++){
        visited[i]=false;
    }
    int vp;
    int index=1;   //记录的是强连通分量的个数
    for(int v=G_pos->vexnum-1;v>=0;v--){
        vp=stack[v];
        if(!visited[v]){
            cout << "visit the " << index++ << " great connected part of DG: ";
            DFS_DG(G_pos,visited,vp);
            cout << endl;
        }
    }
    delete []visited;
    delete []stack;
}

void Rev_DFS_DG(Ptr_ALGraph G, int * stack, int &count, bool *visited, int vp)
{
    using namespace std;
    visited[vp]=true;
    Ptr_NodeLink visit;
    visit=G->head[vp].first;
    while(visit!=NULL){
        if(!visited[visit->vindex]){
            Rev_DFS_DG(G,stack,count,visited,visit->vindex);
        }
        visit=visit->next;
    }
    stack[count++]=vp;
}

void DFS_DG(Ptr_ALGraph G, bool *visited, int vp)
{
    using namespace std;
    visited[vp]=true;
    cout << G->head[vp].vertex << ' ';
    Ptr_NodeLink visit;
    visit=G->head[vp].first;
    while(visit!=NULL){
        if(!visited[visit->vindex]){
            DFS_DG(G,visited,visit->vindex);
        }
        visit=visit->next;
    }
}

int main(){
    using namespace std;
    Ptr_ALGraph G_pos = new ALGraph;
    G_pos=Create_Graph(G_pos,DG,true);
    Ptr_ALGraph G_neg = new ALGraph;
    G_neg=Create_Graph(G_neg,DG,false);
    for(int i=0;i<8;i++){
        AddVertex(G_pos,'A'+i);
        AddVertex(G_neg,'A'+i);
    }
    Ptr_ArcType arc = new ArcType[14];
    arc[0].vex1='A'; arc[0].vex2='B'; 
    arc[1].vex1='E'; arc[1].vex2='A'; 
    arc[2].vex1='B'; arc[2].vex2='E'; 
    arc[3].vex1='B'; arc[3].vex2='F'; 
    arc[4].vex1='E'; arc[4].vex2='F'; 
    arc[5].vex1='B'; arc[5].vex2='C'; 
    arc[6].vex1='C'; arc[6].vex2='G'; 
    arc[7].vex1='F'; arc[7].vex2='G'; 
    arc[8].vex1='G'; arc[8].vex2='F'; 
    arc[9].vex1='C'; arc[9].vex2='D'; 
    arc[10].vex1='D'; arc[10].vex2='C'; 
    arc[11].vex1='D'; arc[11].vex2='H'; 
    arc[12].vex1='H'; arc[12].vex2='D';
    arc[13].vex1='H'; arc[13].vex2='G';

    for(int i=0;i<14;i++){
        AddArc(G_pos,arc+i);
        AddArc(G_neg,arc+i);
    }
    cout << "Positive AdjList is: " << endl;
    Print_AdjList(G_pos);
    cout << endl << endl;
    cout << "Negative AdjList is: " << endl;
    Print_AdjList(G_neg);
    cout << endl << endl;
    Connected_DG(G_pos,G_neg);
    delete G_pos;
    delete G_neg;
    return 0;
}