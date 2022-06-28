#include<iostream>
const int MAX_VERTEX_NUM = 100;
const int MAX_ARC_NUM = 100;
const int POSITIVE = 1; //正邻接表
const int NEGATIVE = 0; //逆邻接表

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
    bool Pos_Neg;     //正临接表或逆邻接表
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG, bool Pos_Neg=true);
//默认参数设置为无向图,正临接表
int LocateVex(Ptr_ALGraph G, ElemType &vp);
int AddVertex(Ptr_ALGraph G, ElemType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);

bool Find_Path_h(Ptr_ALGraph G, ElemType vp1, ElemType vp2);
bool Find_Path(Ptr_ALGraph G, ElemType vp1, ElemType vp2, bool * visited, bool self);

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

bool Find_Path_h(Ptr_ALGraph G, ElemType vp1, ElemType vp2)
{
    using namespace std;
    bool * visited = new bool[G->vexnum];
    for(int i=0;i<G->vexnum;i++){
        visited[i]=false;
    }
    if(Find_Path(G,vp1,vp2,visited,true)){
        delete []visited;
        return true;
    }
    else{
        delete []visited;
        return false;
    }
}

bool Find_Path(Ptr_ALGraph G, ElemType vp1, ElemType vp2, bool * visited, bool self)
{
    if(vp1==vp2 && !self){
        return true;
    }
    else{
        int pos1=LocateVex(G,vp1);
        if(!self){
            visited[pos1]=true;
        }
        Ptr_NodeLink visit;
        visit=G->head[pos1].first;
        while(visit!=NULL){
            if(visited[visit->vindex]==false && Find_Path(G,visit->vex,vp2,visited,false)){
                return true;
            }
            visit=visit->next;
        }
    }
    return false;
}

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G,DG,true);
    int vexnum;
    cin >> vexnum;
    cin.get();
    for(int i=1;i<=vexnum;i++){
        AddVertex(G,i);
    }
    int * s = new int[MAX_VERTEX_NUM];
    int * fst = new int[MAX_VERTEX_NUM];
    int * lst = new int[MAX_VERTEX_NUM];
    char c=' ';
    int len_s;
    int index=0;
    while(c!='\n'){
        cin >> s[index++];
        c=cin.get();
    }
    len_s=index;
    c=' ';
    index=0;
    while(c!='\n'){
        cin >> fst[index++];
        c=cin.get();
    }
    c=' ';
    index=0;
    while(c!='\n'){
        cin >> lst[index++];
        c=cin.get();
    }

    //按照s，fst，lst增加弧
    for(int j=1;j<=vexnum;j++){
        int pos=fst[j];
        while(pos<lst[j]){
            ArcType arc;
            arc.vex1=j; arc.vex2=s[pos];
            AddArc(G,&arc);
            pos++;
        }
    }
#if 0
    Print_AdjList(G);
#endif
    for(int j=1;j<vexnum;j++){
        if(Find_Path_h(G,j,j)){
            cout << "yes";
            return 0;
        }
    }
    cout << "no";
    return 0;
}