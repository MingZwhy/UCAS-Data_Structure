//临接矩阵+Prim
#include<iostream>
const int _INFINITY = 10000;   //最大值正无穷
const int MAX_VERTEX_NUM = 20;  //最大顶点个数
const int MAX_ARC_NUM = 100;    //最大边个数

typedef int VRType;       //关系类型——无权则为0或1，有权则为实数
typedef char VertexType;  //顶点类型，可为int或char

typedef enum {DG,DN,UDG,UDN} GraphKind;  //{有向图，有向网，无向图，无向网}
typedef struct ArcCell{
    VRType adj;
    int *info;  //可能记录的弧的其他信息
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct{
    VertexType vexs[MAX_VERTEX_NUM];   //顶点向量
    AdjMatrix   arcs;                  //邻接矩阵
    int vexnum, arcnum;                //图的顶点数和弧数
    GraphKind kind;                    //图的种类标志
    bool IF_Info;      //true时表示记录弧的其他信息，反之则不
    bool IF_Power;     //IF_Power记录图是否带权
}MGraph, *Ptr_To_MGraph;

typedef struct ArcType{
    VertexType vex1,vex2;     //弧或边所依附的两个顶点
    VRType Arcval;            //弧或边的权值
    //结构中可能还有弧或边的结构定义
}ArcType, *Ptr_ArcType;

//一维数组closedge[n]用来保存V-U中各顶点到U中顶点具有的权值最小的边
struct{
    int adjvex;   //边所依附于U中的顶点在vexs中的下标
    int lowcost;  //该边的权值
}closedge[MAX_VERTEX_NUM];

int LocateVex(Ptr_To_MGraph G, VertexType vp);
Ptr_To_MGraph CreateGraph(Ptr_To_MGraph G, int vexnum, VertexType * vexs, int arcnum, Ptr_ArcType arcs, GraphKind kind=UDG, bool IF_Power=false, bool IF_Info=false);   //默认为无附加信息无权无向图
Ptr_To_MGraph CreateDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs);
Ptr_To_MGraph CreateUDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs);
int AddVertex(Ptr_To_MGraph G, VertexType vp);

void MiniSpanTree_PRIM(Ptr_To_MGraph G, VertexType u);   //u作为起始点
int minimum(Ptr_To_MGraph G, int &power);    //找closedge数组中的非零最小值(0表示该点已在U中，不能选择),返回该最小边所依附的U中顶点的下标

//图的顶点定位
int LocateVex(Ptr_To_MGraph G, VertexType vp)
{
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->vexs[k]==vp){
            return k;
        }
    }
    return -1;   //not found
}

//图的创建
Ptr_To_MGraph CreateGraph(Ptr_To_MGraph G, int vexnum, VertexType * vexs, int arcnum, Ptr_ArcType arcs, GraphKind kind, bool IF_Power, bool IF_Info)
{
    G->kind=kind;
    G->IF_Power=IF_Power;
    G->IF_Info=IF_Info;
    G->vexnum=vexnum;
    G->arcnum=arcnum;

    switch(G->kind){
        case DG: return CreateDG(G,vexs,arcs);
        case UDG: return CreateUDG(G,vexs,arcs);
        default: return G;
    }
}

//UDG无向图的创建
Ptr_To_MGraph CreateUDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs)
{
    using namespace std;
    //构建顶点向量
    int i;
    for(i=0;i<G->vexnum;i++){
        G->vexs[i]=vexs[i];
    }
    //初始化临接矩阵
    int j;
    if(G->IF_Power){
        for(i=0;i<G->vexnum;i++){
            for(j=0;j<G->vexnum;j++){
                G->arcs[i][j]={_INFINITY,NULL};
            }
        }
    }
    else{
        for(i=0;i<G->vexnum;i++){
            for(j=0;j<G->vexnum;j++){
                G->arcs[i][j]={0,NULL};
            }
        }
    }
    //构造临接矩阵
    int k;
    if(G->IF_Power){
        for(k=0;k<=G->arcnum;k++){
            i=LocateVex(G,arcs[k].vex1);
            j=LocateVex(G,arcs[k].vex2);
            G->arcs[i][j].adj=arcs[k].Arcval;
            if(G->IF_Info){
                ;   //根据需要
            }
            //置与<v1,v2>对称的弧<v2,v1>
            G->arcs[j][i]=G->arcs[i][j];
        }
    }
    else{
        for(k=1;k<=G->arcnum;k++){
            i=LocateVex(G,arcs[k].vex1);
            j=LocateVex(G,arcs[k].vex2);
            G->arcs[i][j].adj=1;  //无权图直接赋0即可
            if(G->IF_Info){
                ;   //根据需要
            }
            //置与<v1,v2>对称的弧<v2,v1>
            G->arcs[j][i]=G->arcs[i][j];
        }
    }
    return G;
}

//DG有向图的创建
Ptr_To_MGraph CreateDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs)
{
    //构建顶点向量
    int i;
    for(i=0;i<G->vexnum;i++){
        G->vexs[i]=vexs[i];
    }
    //初始化临接矩阵
    int j;
    if(G->IF_Power){
        for(i=0;i<G->vexnum;i++){
            for(j=0;j<G->vexnum;j++){
                G->arcs[i][j]={_INFINITY,NULL};
            }
        }
    }
    else{
        for(i=0;i<G->vexnum;i++){
            for(j=0;j<G->vexnum;j++){
                G->arcs[i][j]={0,NULL};
            }
        }
    }
    //构造临接矩阵
    int k;
    if(G->IF_Power){
        for(k=0;k<=G->arcnum;k++){
            i=LocateVex(G,arcs[k].vex1);
            j=LocateVex(G,arcs[k].vex2);
            G->arcs[i][j].adj=arcs[k].Arcval;
            if(G->IF_Info){
                ;
                //具体要存储什么关于弧的信息并不确定，可根据需要更改info的类型
            }
        }
    }
    else{
        for(k=1;k<=G->arcnum;k++){
            i=LocateVex(G,arcs[k].vex1);
            j=LocateVex(G,arcs[k].vex2);
            G->arcs[i][j].adj=1;
            if(G->IF_Info){
                ;
                //具体要存储什么关于弧的信息并不确定，可根据需要更改info的类型
            }
        }
    }
    return G;
}

int AddVertex(Ptr_To_MGraph G, VertexType vp)
{
    int k,j;
    if(G->vexnum>=MAX_VERTEX_NUM){
        printf("Vertex Overflow !\n");
        return -1;
    }
    if(LocateVex(G,vp)!=-1){
        printf("Vertex has existed !\n");
        return -1;
    }
    G->vexs[G->vexnum]=vp;
    G->vexnum++;
    k=G->vexnum-1;   //此时k为新加入点的下标

    if(G->IF_Power){
        for(j=0;j<G->vexnum;j++){
            G->arcs[j][k].adj=_INFINITY;
            G->arcs[k][j].adj=_INFINITY;
        }
    }
    else{
        for(j=0;j<G->vexnum;j++){
            G->arcs[j][k].adj=0;
            G->arcs[k][j].adj=0;
        }
    }
    return k;
}

void MiniSpanTree_PRIM(Ptr_To_MGraph G, VertexType u)
{
    using namespace std;
    int k=LocateVex(G,u);
    //辅助数组closedge初始化
    for(int i=0;i<G->vexnum;i++){
        if(i!=k){
            closedge[i].adjvex=k;
            closedge[i].lowcost=G->arcs[k][i].adj;
        }
    }
    //将起始点u加入集合U(已加入的顶点)
    closedge[k].lowcost=0;   //初始: U={u}

    //选择其余G->vexnum-1个顶点添加到生成树上
    int power;
    for(int i=1;i<G->vexnum;i++){
        //新加入的顶点在G数组中的下标为k，power为新加入边的权值
        k = minimum(G,power);
        //打印信息
        cout << G->vexs[closedge[k].adjvex] << "---" << G->vexs[k] << ": " << power << endl;
        //第k顶点并入U集
        closedge[k].lowcost=0;
        //更新其他顶点的最小值
        for(int j=0;j<G->vexnum;j++){
            if(closedge[j].lowcost!=0){   //首先确保是V-U集合中的顶点
                if(G->arcs[k][j].adj<closedge[j].lowcost){
                    closedge[j].adjvex=k;
                    closedge[j].lowcost=G->arcs[k][j].adj;
                }
            }
        }
    }
}

int minimum(Ptr_To_MGraph G, int &power)
{
    int min=_INFINITY;
    int vindex;
    for(int i=0;i<G->vexnum;i++){
        if(closedge[i].lowcost!=0){  //首先确保选的点i属于V-U
            if(closedge[closedge[i].adjvex].lowcost==0 && closedge[i].lowcost<min){   //点i相连的点要属于U且连接的弧比min小
                min = closedge[i].lowcost;
                vindex = i;          //待选点赋为i
            }
        }
    }
    power = min;
    return vindex;
}

int main(){
    using namespace std;
    Ptr_To_MGraph G = new MGraph;
    int vexnum,arcnum;
    cout << "input the vexnum: ";
    cin >> vexnum;
    cout << "input the arcnum: ";
    cin >> arcnum;
    VertexType * vexs = new VertexType[vexnum];
    for(int i=0;i<vexnum;i++){
        vexs[i]='a'+i;
    }
    Ptr_ArcType arcs = new ArcType[arcnum]; 
    arcs[0].vex1='a'; arcs[0].vex2='b'; arcs[0].Arcval=19;
    arcs[1].vex1='a'; arcs[1].vex2='g'; arcs[1].Arcval=18;
    arcs[2].vex1='a'; arcs[2].vex2='e'; arcs[2].Arcval=14;
    arcs[3].vex1='b'; arcs[3].vex2='e'; arcs[3].Arcval=12;
    arcs[4].vex1='b'; arcs[4].vex2='c'; arcs[4].Arcval=5;
    arcs[5].vex1='b'; arcs[5].vex2='d'; arcs[5].Arcval=7;
    arcs[6].vex1='c'; arcs[6].vex2='d'; arcs[6].Arcval=3;
    arcs[7].vex1='d'; arcs[7].vex2='e'; arcs[7].Arcval=8;
    arcs[8].vex1='d'; arcs[8].vex2='f'; arcs[8].Arcval=21;
    arcs[9].vex1='f'; arcs[9].vex2='g'; arcs[9].Arcval=27;
    arcs[10].vex1='g'; arcs[10].vex2='e'; arcs[10].Arcval=16;

    G=CreateGraph(G,vexnum,vexs,arcnum,arcs,UDG,true,false);
    cout << "create ok ";
    MiniSpanTree_PRIM(G,'a');
    return 0;
}