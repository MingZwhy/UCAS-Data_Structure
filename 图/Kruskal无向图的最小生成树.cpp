//图的边表表示+Krustal
#include<iostream>
#include<algorithm>
const int MAX_VEX = 30; //最大顶点数  
const int MAX_EDGE = 100; //最大边数

typedef int VRType;       //关系类型——无权则为0或1，有权则为实数
typedef char VertexType;  //顶点类型，可为int或char

typedef struct ENode{
    VertexType i,j;  //边所依附的两个顶点
    int ivex,jvex;   //边所依附的两个顶点的下标
    VRType weight;   //边的权值
}ENode, *Ptr_ENode;

typedef struct{
    int vexnum;     //顶点数
    int arcnum;    //边数
    VertexType vexs[MAX_VEX];
    ENode arcs[MAX_EDGE];
}ELGraph, *Ptr_ELGraph;

int Locate_Vex(Ptr_ELGraph G, VertexType v);
Ptr_ELGraph Create_ELGraph(Ptr_ELGraph G);
Ptr_ELGraph Add_Vex(Ptr_ELGraph G, VertexType v);
Ptr_ELGraph Add_arc(Ptr_ELGraph G, ENode arc);
Ptr_ENode Kruskal_MST(Ptr_ELGraph G);
bool cmp(ENode x, ENode y);

int Locate_Vex(Ptr_ELGraph G, VertexType v)
{
    using namespace std;
    for(int i=0;i<G->vexnum;i++){
        if(G->vexs[i]==v){
            return i;
        }
    }
    return -1;
}

Ptr_ELGraph Create_ELGraph(Ptr_ELGraph G)
{
    G = new ELGraph;
    G->vexnum=0;
    G->arcnum=0;
    return G;
}

Ptr_ELGraph Add_Vex(Ptr_ELGraph G, VertexType v)
{
    using namespace std;
    if(Locate_Vex(G,v)!=-1){
        cout << "vex " << v << " has existed!";
    }
    G->vexs[G->vexnum]=v;
    G->vexnum++;
    return G;
}

Ptr_ELGraph Add_arc(Ptr_ELGraph G, ENode arc)
{
    G->arcs[G->arcnum].i=arc.i;
    G->arcs[G->arcnum].j=arc.j;
    G->arcs[G->arcnum].ivex=Locate_Vex(G,arc.i);
    G->arcs[G->arcnum].jvex=Locate_Vex(G,arc.j);
    G->arcs[G->arcnum].weight=arc.weight;
    G->arcnum++;
    return G;
}

Ptr_ENode Kruskal_MST(Ptr_ELGraph G)
{
    using namespace std;
    Ptr_ENode TE = new ENode[G->vexnum-1]; 
    //一维数组Vset存放图G中每个顶点所在的连通分量的编号
    int * Vset = new int[G->vexnum];
    
    //初始化Vset，初始时每个顶点所在连通分量就是它自己
    for(int j=0;j<G->vexnum;j++){
        Vset[j]=j;
    }
    //对G->arcs从小到大排序
    sort(G->arcs,G->arcs+G->arcnum,cmp);

    int j=0; int k=0;
    int s1,s2;
    while(k<G->vexnum-1 && j<G->arcnum){
        s1=Vset[G->arcs[j].ivex];
        s2=Vset[G->arcs[j].jvex];
        //如果边的两个顶点所属的连通分量的编号不同，则该边加入TE内
        if(s1!=s2){
            TE[k].i=G->arcs[j].i;
            TE[k].j=G->arcs[j].j;
            TE[k].weight=G->arcs[j].weight;

            k++;

            //更新顶点所属连通分量的编号
            for(int v=0;v<G->vexnum;v++){
                if(Vset[v]==s2){
                    Vset[v]=s1;
                }
            }
        }
        j++;
    }
    delete []Vset;
    return TE;
}

bool cmp(ENode x, ENode y)
{
    return (x.weight<y.weight);
}

int main(){
    using namespace std;
    Ptr_ELGraph G = Create_ELGraph(G);
    for(int i=0;i<8;i++){
        G = Add_Vex(G,'a'+i);
    }
    Ptr_ENode arcs = new ENode[13];
    arcs[0].i='a'; arcs[0].j='c'; arcs[0].weight=2;
    arcs[1].i='f'; arcs[1].j='g'; arcs[1].weight=2;
    arcs[2].i='b'; arcs[2].j='c'; arcs[2].weight=3;
    arcs[3].i='e'; arcs[3].j='f'; arcs[3].weight=3;
    arcs[4].i='a'; arcs[4].j='b'; arcs[4].weight=4;
    arcs[5].i='d'; arcs[5].j='h'; arcs[5].weight=4;
    arcs[6].i='b'; arcs[6].j='d'; arcs[6].weight=5;
    arcs[7].i='c'; arcs[7].j='d'; arcs[7].weight=5;
    arcs[8].i='c'; arcs[8].j='h'; arcs[8].weight=5;
    arcs[9].i='d'; arcs[9].j='g'; arcs[9].weight=5;
    arcs[10].i='h'; arcs[10].j='g'; arcs[10].weight=6;
    arcs[11].i='d'; arcs[11].j='e'; arcs[11].weight=7;
    arcs[12].i='b'; arcs[12].j='e'; arcs[12].weight=9;
    for(int i=0;i<13;i++){
        G=Add_arc(G,arcs[i]);
    }
    Ptr_ENode MST = Kruskal_MST(G);
    for(int j=0;j<G->vexnum-1;j++){
        cout << MST[j].i << "---" << MST[j].j << ": " << MST[j].weight << endl;
    }
    delete G;
    delete MST;
    return 0;
}