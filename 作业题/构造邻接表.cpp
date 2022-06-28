#include<iostream>
#include<algorithm>
const int MAX_VERTEX_NUM = 100;

typedef int ElemType;
typedef int InfoType;   //一般为权值

//链表中普通节点
typedef struct node{
    ElemType vindex;   //临接点在头结点数组中的位置
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

//图的创建
Ptr_ALGraph Create_Graph(Ptr_ALGraph G)
{
    G->kind=DG;
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
    Ptr_NodeLink q = new NodeLink;
    //q为边的末尾节点
    q->vindex=arc->vex2; 
    q->next=NULL;

    //有向图，对arc为vex1->vex2
    //按从大到小排序
    if(G->head[k].first==NULL || q->vindex>=G->head[k].first->vindex){
        q->next = G->head[k].first;
        G->head[k].first = q;
    }
    else{
        Ptr_NodeLink visit = G->head[k].first;
        while(visit->next!=NULL && q->vindex<visit->next->vindex){
            visit=visit->next;
        }
        q->next=visit->next;
        visit->next=q;
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
    int Vexnum,Arcnum;
    cin >> Vexnum;
    if(Vexnum==0){
        return 0;
    }
    cin.get();
    cin >> Arcnum;
    Ptr_ArcType ARC = new ArcType[Arcnum];   //动态创建弧数组
    ElemType * VEX = new ElemType[Vexnum];   //动态创建顶点数组 
    char c;
    int i=0;
    int index=0;
    do{
        int j;
        cin >> ARC[i].vex1;
        for(j=0;j<index;j++){
            if(VEX[j]==ARC[i].vex1){
                break;
            }
        }
        if(j==index){
            VEX[index++]=ARC[i].vex1;
        }

        cin.get();

        cin >> ARC[i].vex2;
        for(j=0;j<index;j++){
            if(VEX[j]==ARC[i].vex2){
                break;
            }
        }
        if(j==index){
            VEX[index++]=ARC[i].vex2;
        }
        i++;
        c=cin.get();
    }while(c!='\n');

    sort(VEX,VEX+Vexnum);    //按将顶点数组升序进行排序
    for(int k=0;k<Vexnum;k++){
        AddVertex(G,&VEX[k]);
    }
    for(int k=0;k<Arcnum;k++){
        AddArc(G,&ARC[k]);
    }
    delete [] VEX;
    delete [] ARC;
    Print_AdjList(G);
    delete  G;
    return 0;
}