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

int LocateVex(Ptr_To_MGraph G, VertexType vp);
Ptr_To_MGraph CreateGraph(Ptr_To_MGraph G, int vexnum, VertexType * vexs, int arcnum, Ptr_ArcType arcs, GraphKind kind=UDG, bool IF_Power=false, bool IF_Info=false);   //默认为无附加信息无权无向图
Ptr_To_MGraph CreateDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs);
Ptr_To_MGraph CreateUDG(Ptr_To_MGraph G, VertexType * vexs, Ptr_ArcType arcs);
int AddVertex(Ptr_To_MGraph G, VertexType vp);

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
        for(k=0;k<G->arcnum;k++){
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
        for(k=0;k<G->arcnum;k++){
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
        for(k=0;k<G->arcnum;k++){
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
        for(k=0;k<G->arcnum;k++){
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