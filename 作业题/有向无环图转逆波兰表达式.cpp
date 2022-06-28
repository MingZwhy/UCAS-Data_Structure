#include<iostream>
const int MAX_VERTEX_NUM = 1000;
const int MAX_ARC_NUM = 1000;
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
    int indegree;
    bool tag;      //true--->运算符号， false--->普通符号
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
void Turn_revebolan_h(Ptr_ALGraph G);
void Turn_revebolan(Ptr_ALGraph G, int pos);

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
    if(vp=='+' || vp=='-' || vp=='*' || vp=='/'){
        G->head[G->vexnum].tag=true;
    }
    else{
        G->head[G->vexnum].tag=false;
    }
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
    }
    G->arcnum++;
    return 1;
}

void Print_AdjList(Ptr_ALGraph G){
    using namespace std;
    int i;
    Ptr_NodeLink visit;
    for(i=0;i<G->vexnum;i++){
        cout << G->head[i].tag << " ";
        cout << "indegree: " << G->head[i].indegree;
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

void Turn_revebolan_h(Ptr_ALGraph G)
{
    int begin;
    for(int i=0;i<G->vexnum;i++){
        if(G->head[i].tag==true && G->head[i].indegree==0){
            begin=i;
            break;
        }
    }
    Turn_revebolan(G, begin);
}

void Turn_revebolan(Ptr_ALGraph G, int pos)
{
    using namespace std;
    int sub1,sub2;
    if(G->head[pos].tag==false){
        cout << G->head[pos].vertex;
    }
    else{
        sub1=G->head[pos].first->vindex;
        sub2=G->head[pos].first->next->vindex;
        Turn_revebolan(G,sub1);
        Turn_revebolan(G,sub2);
        cout << G->head[pos].vertex;
    }
}

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G,DG,true);
    int vexnum;
    cin >> vexnum;
    cin.get();
    ElemType * vex = new ElemType[MAX_VERTEX_NUM];
    Ptr_ArcType arc = new ArcType[MAX_ARC_NUM];
    int * arc_help = new int[MAX_ARC_NUM];
    char c=' ';
    ElemType v1,v2;
    int index_vex=0; 
    int index_arc=0;
    for(int i=0;i<vexnum;i++){
        cin >> vex[index_vex++];
        c=cin.get();
        if(c==EOF){
            break;
        }
        if(c!='\n'){
            arc[index_arc].vex1=vex[index_vex-1];
            cin >> arc_help[index_arc];
            index_arc++;
            c=cin.get();
            arc[index_arc].vex1=vex[index_vex-1];
            cin >> arc_help[index_arc];
            index_arc++;
            c=cin.get();
        }
    }

    for(int i=0;i<vexnum;i++){
        AddVertex(G,vex[i]);
    }

    for(int i=0;i<index_arc;i++){
        arc[i].vex2=G->head[arc_help[i]].vertex;
        AddArc(G,arc+i);
    }

    Turn_revebolan_h(G);
    delete []vex;
    delete []arc;
    delete []arc_help;
    delete G;
    return 0;
}