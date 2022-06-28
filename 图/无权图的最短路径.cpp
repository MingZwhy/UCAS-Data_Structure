
#include<iostream>
const int MAX_VERTEX_NUM = 100;
const int MAX_ARC_NUM = 100;

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
    Head_Node head[MAX_VERTEX_NUM];
}ALGraph, *Ptr_ALGraph;

//链队列节点
typedef struct ListNode{
    ElemType data;
    struct ListNode *prior;
    struct ListNode *next;
}ListNode, *Ptr_ListNode;

//双向链表
typedef struct DulList{
    Ptr_ListNode front;
    Ptr_ListNode rear;
    Ptr_ListNode head;
    int Node_num;
}DulList, *Ptr_DulList;

class DList
{
private:
    DulList L;
public:
    DList();
    bool If_Empty(void) const;
    bool EnQueue(ElemType e);
    bool DeQueue(ElemType &e);
    int Print(void) const;
    ~DList();
};

DList::DList()
{
    L.head = new ListNode;
    L.front=L.rear=L.head;
    L.Node_num = 0;
}

DList::~DList()
{
    Ptr_ListNode visit1,visit2;
    visit1=L.head;
    while(visit1!=nullptr){
        visit2=visit1->next;
        delete visit1;
        visit1=visit2;
    }
}

bool DList::If_Empty(void) const
{
    return (L.Node_num==0);
}

bool DList::EnQueue(ElemType e)
{
    Ptr_ListNode p = new ListNode;
    p->data=e;
    p->next=NULL;
    p->prior=L.front;
    L.rear->next=p;
    L.rear=p;
    L.Node_num++;
    return true;
}

bool DList::DeQueue(ElemType &e)
{
    L.front=L.front->next;
    e=L.front->data;
    return true;
}

int DList::Print(void) const
{
    using namespace std;
    Ptr_ListNode visit=L.rear;
    int len=0;
    while(visit!=L.head){
        cout << visit->data;
        len++;
        visit=visit->prior;
        if(visit!=L.head){
            cout << "<--";
        }
    }
    return len;
}

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG);
//默认参数设置为无向图
int LocateVex(Ptr_ALGraph G, ElemType &vp);
int AddVertex(Ptr_ALGraph G, ElemType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);
void Print_AdjList(Ptr_ALGraph G);
bool Find_shortest_path(Ptr_ALGraph G, ElemType v1, ElemType v2);

bool Find_shortest_path(Ptr_ALGraph G, ElemType v1, ElemType v2)
{
    using namespace std;
    bool * visited = new bool [G->vexnum];
    for(int i=0;i<G->vexnum;i++){
        visited[i]=false;
    }
    DList L = DList();
    visited[LocateVex(G,v1)]=true;
    L.EnQueue(v1);
    ElemType e;
    while(!L.If_Empty()){
        L.DeQueue(e);
        int pos=LocateVex(G,e);
        Ptr_NodeLink visit=G->head[pos].first;
        while(visit!=NULL){
            if(!visited[LocateVex(G,visit->vex)]){
                L.EnQueue(visit->vex);
                visited[LocateVex(G,visit->vex)]=true;
            }
            if(visit->vex==v2){
                int len=L.Print();
                cout << "\nlen is:" << len;
                return true;
            }
            visit=visit->next;
        }
    }
    return false;
}

//图的创建
Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind)
{
    G->kind=kind;
    G->vexnum=0; G->arcnum=0;
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

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G);
    for(int i=1;i<=9;i++){
        AddVertex(G,'0'+i);
    }
    Ptr_ArcType arc = new ArcType[12];
    arc[0].vex1='1'; arc[0].vex2='2'; 
    arc[1].vex1='1'; arc[1].vex2='3'; 
    arc[2].vex1='2'; arc[2].vex2='3'; 
    arc[3].vex1='1'; arc[3].vex2='4'; 
    arc[4].vex1='1'; arc[4].vex2='7'; 
    arc[5].vex1='4'; arc[5].vex2='5'; 
    arc[6].vex1='4'; arc[6].vex2='6'; 
    arc[7].vex1='5'; arc[7].vex2='6'; 
    arc[8].vex1='6'; arc[8].vex2='8'; 
    arc[9].vex1='7'; arc[9].vex2='8'; 
    arc[10].vex1='7'; arc[10].vex2='9'; 
    arc[11].vex1='8'; arc[11].vex2='9'; 

    for(int i=0;i<12;i++){
        AddArc(G,arc+i);
    }
    if(Find_shortest_path(G,'3','5'))
        cout << "  find";
    else
        cout << "no find";
    delete G;
    return 0;
}