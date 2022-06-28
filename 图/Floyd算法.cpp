//Floyd算法求每一对顶点之间最短路径
//Floyd算法允许图中有带负值边，但不允许有包含带负权值得边组成的回路
//求每一对顶点之间最短距离时，时间复杂度仍为O(n^3),但比Dijkstra更为简洁

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
void FLOYD_ShortestPath(Ptr_To_MGraph G);

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

void FLOYD_ShortestPath(Ptr_To_MGraph G)
{
    using namespace std;
    int u,w,v,i;
    bool PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int DistancMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

    //对各节点之间初始化已知路径及距离
    for(v=0;v<G->vexnum;v++){
        for(w=0;w<G->vexnum;w++){
            DistancMatrix[v][w]=G->arcs[v][w].adj;
            for(u=0;u<G->vexnum;u++){
                PathMatrix[v][w][u]=false;
            }
            if(DistancMatrix[v][w]<_INFINITY){  //如果从v到w有直接路径
                PathMatrix[v][w][v] = PathMatrix[v][w][w]=true;
            }
        }
    }

    for(u=0;u<G->vexnum;u++){
        for(v=0;v<G->vexnum;v++){
            for(w=0;w<G->vexnum;w++){
                if(DistancMatrix[v][u]+DistancMatrix[u][w]<DistancMatrix[v][w]){
                    //如果从v经u到w的一条路径更短
                    DistancMatrix[v][w]=DistancMatrix[v][u]+DistancMatrix[u][w];
                    //更新路径
                    for(i=0;i<G->vexnum;i++){
                        //因为在if之前v到w之间可能是没有路径的，所以要更新所有v和w之间可能新有的路径
                        PathMatrix[v][w][i] = (PathMatrix[v][u][i] || PathMatrix[u][w][i]);
                    }
                }
            }
        }
    }
    
    //输出最短路径
    cout << "   ";
    for(i=0;i<G->vexnum;i++){
        cout << G->vexs[i] << " ";
    }
    cout << endl;
    for(u=0;u<G->vexnum;u++){
        cout << G->vexs[u] << ": ";
        for(v=0;v<G->vexnum;v++){
            if(u==v || DistancMatrix[u][v]==_INFINITY){   //在有回路时，会出现顶点到自的D不为0的情况，所以当u==v时直接输出0
                cout << "0" << " ";
            }
            else{
                cout << DistancMatrix[u][v] << " ";
            }
        }
        cout << endl;
    }
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
        vexs[i]='A'+i;
    }
    Ptr_ArcType arcs = new ArcType[arcnum]; 
    arcs[0].vex1='A'; arcs[0].vex2='B'; arcs[0].Arcval=1;
    arcs[1].vex1='A'; arcs[1].vex2='D'; arcs[1].Arcval=3;
    arcs[2].vex1='B'; arcs[2].vex2='C'; arcs[2].Arcval=1;
    arcs[3].vex1='C'; arcs[3].vex2='D'; arcs[3].Arcval=2;
    arcs[4].vex1='D'; arcs[4].vex2='B'; arcs[4].Arcval=4;
    arcs[5].vex1='C'; arcs[5].vex2='A'; arcs[5].Arcval=5;

    G=CreateGraph(G,vexnum,vexs,arcnum,arcs,DG,true,false);
    FLOYD_ShortestPath(G);
    return 0;
}