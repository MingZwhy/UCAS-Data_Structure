#include<iostream>
const int MAX_VERTEX_NUM = 30;

typedef char VexType;
typedef int InfoType;

//链表节点
typedef struct node{
    int Vex_index;
    VexType elem;
    InfoType info;
    struct node *next;
}NodeLink, *Ptr_NodeLink;

//邻接表的头结点
typedef struct{
    VexType vex;
    int degree;   //对正邻接表，记录顶点出度
    Ptr_NodeLink first;
}Head_Node, *Head;

//弧或边的定义
typedef struct ArcType{
    VexType vex1,vex2;   //弧或边所依附的两个顶点
    InfoType info;    //与弧或边相关信息，如权值
}ArcType, *Ptr_ArcType;

typedef struct{
    int vexnum;   //图的顶点数
    int arcnum;  //图的边数
    bool IF_POWER;   //是否带权值
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

//广度优先搜索需要用到队列
typedef struct Queue{
    int front;
    int rear;
    int data[MAX_VERTEX_NUM];
}Queue, *Ptr_Queue;

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, bool if_power);
int LocateVex(Ptr_ALGraph G, VexType vp);
int AddVertex(Ptr_ALGraph G, VexType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);
void DFS(Ptr_ALGraph G);
void DFS_DG(Ptr_ALGraph G, bool *visited, int vp);

Ptr_Queue Init_Queue(Ptr_Queue Q);
bool IF_Empty(Ptr_Queue Q);
bool EnQueue(Ptr_Queue Q, int vex);
int DeQueue(Ptr_Queue Q);
void BFS(Ptr_ALGraph G);
void BFS_DG(Ptr_ALGraph G, bool *visited);

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, bool if_power)
{
    if(if_power)
        G->IF_POWER=true;
    else
        G->IF_POWER=false;
    G->vexnum=0; G->arcnum=0;
    return G;
}

//图的顶点定位
int LocateVex(Ptr_ALGraph G, VexType vp)
{
    using namespace std;
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->head[k].vex==vp){
            return k;
        }
    }
    return -1;  //not found
}

//向图中增加顶点
int AddVertex(Ptr_ALGraph G, VexType vp)
{
    int k,j;
    if(G->vexnum>=MAX_VERTEX_NUM){
        printf("Vertex overflow !\n");
        return -1;
    }
    if(LocateVex(G,vp)!=-1){
        printf("This Vertex has existed !\n");
        return -1;
    }
    G->head[G->vexnum].vex=vp;
    G->head[G->vexnum].degree=0;
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
        printf("Arc's Vertex do not existed!\n");
        return -1;
    }
    Ptr_NodeLink p = new NodeLink;
    Ptr_NodeLink q = new NodeLink;
    //p为边的起始节点
    p->Vex_index=k;
    p->elem=arc->vex1;
    p->info=arc->info;
    p->next=NULL;
    //q为边的末尾节点
    q->Vex_index=j;
    q->elem=arc->vex2;
    q->info=arc->info;
    q->next=NULL;

    //无向图，需要用头插入法插入到两个单链表
    q->next = G->head[k].first;
    G->head[k].first = q;
    G->head[k].degree++;
    p->next = G->head[j].first;
    G->head[j].first = p;
    G->head[j].degree++;

    G->arcnum++;
    return 1;
}

void DFS(Ptr_ALGraph G)
{
    using namespace std;
    bool * visited = new bool[G->vexnum];
    for(int i=0;i<G->vexnum;i++){
        visited[i]=false;
    }
    for(int i=0;i<G->vexnum;i++){
        if(!visited[i]){
            DFS_DG(G,visited,i);
        }
    }
}

void DFS_DG(Ptr_ALGraph G, bool *visited, int vp)
{
    using namespace std;
    visited[vp]=true;
    cout << G->head[vp].vex << ' ';
    Ptr_NodeLink visit;
    visit=G->head[vp].first;
    while(visit!=NULL){
        if(!visited[visit->Vex_index]){
            DFS_DG(G,visited,visit->Vex_index);
        }
        visit=visit->next;
    }
}

Ptr_Queue Init_Queue(Ptr_Queue Q)
{
    Q->front=Q->rear=0;
    return Q;
}

bool IF_Empty(Ptr_Queue Q)
{
    return (Q->rear==Q->front);
}

bool EnQueue(Ptr_Queue Q, int vex)
{
    using namespace std;
    if(Q->rear==MAX_VERTEX_NUM){
        cout << "queue no space";
        return false;
    }
    Q->data[Q->rear]=vex;
    Q->rear++;
    return true;
}

int DeQueue(Ptr_Queue Q)
{
    using namespace std;
    if(IF_Empty(Q)){
        cout << "queue is empty";
        return -1;
    }
    return Q->data[Q->front++];
}

void BFS(Ptr_ALGraph G)
{
    using namespace std;
    bool * visited = new bool[G->vexnum];
    for(int i=0;i<G->vexnum;i++){
        visited[i]=false;
    }
    BFS_DG(G,visited);
}

void BFS_DG(Ptr_ALGraph G, bool *visited)
{
    using namespace std;
    Ptr_Queue Q = new Queue;
    Q=Init_Queue(Q);

    for(int v=0;v<G->vexnum;v++){
        if(!visited[v]){
            cout << G->head[v].vex << ' ';
            visited[v]=true;
            EnQueue(Q,v);
        }
        while(!IF_Empty(Q)){
            int vex=DeQueue(Q);
            Ptr_NodeLink visit;
            visit=G->head[vex].first;
            while(visit!=NULL){
                if(!visited[visit->Vex_index]){
                    cout << visit->elem << ' ';
                    visited[visit->Vex_index]=true;
                    EnQueue(Q,visit->Vex_index);
                }
                visit=visit->next;
            }
        }
    }
    delete Q;
}

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G,false);
    for(int i=0;i<8;i++){
        AddVertex(G,'1'+i);
    }
    Ptr_ArcType arc = new ArcType[9];
    arc[0].vex1='1'; arc[0].vex2='2'; arc[0].info=1;
    arc[1].vex1='1'; arc[1].vex2='3'; arc[1].info=1;
    arc[2].vex1='2'; arc[2].vex2='4'; arc[2].info=1;
    arc[3].vex1='2'; arc[3].vex2='5'; arc[3].info=1;
    arc[4].vex1='4'; arc[4].vex2='8'; arc[4].info=1;
    arc[5].vex1='5'; arc[5].vex2='8'; arc[5].info=1;
    arc[6].vex1='3'; arc[6].vex2='6'; arc[6].info=1;
    arc[7].vex1='3'; arc[7].vex2='7'; arc[7].info=1;
    arc[8].vex1='6'; arc[8].vex2='7'; arc[8].info=1;

    for(int i=0;i<9;i++){
        AddArc(G,arc+i);
    }
    cout << "DFS result is: ";
    DFS(G);
    cout << "\nBFS result is: ";
    BFS(G);
    delete [] arc;
    delete G;
    return 0;
}