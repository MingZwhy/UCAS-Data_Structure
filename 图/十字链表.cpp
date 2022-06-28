//十字链表，用于有向图
/* 从一个顶点节点的firstout出发，沿标节点的tlink指针遍历直到
tlink指针为空，构成了正临接表的链表结构
从一个顶点的firstin出发，沿表结点的hlink指针构成了逆邻接表的链表结构
也就是说十字链表同时存储了正临接表和逆邻接表
*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
const int MAX_VERTEX_NUM = 30;

typedef char ElemType;
typedef int InfoType;
typedef struct ArcBox{
    int tailvex;   //尾节点位置
    int headvex;   //头结点位置
    InfoType data;  //弧信息
    struct ArcBox *hlink;  //第一个同弧头,指向下一个有相同弧头的点
    struct ArcBox *tlink; //第一个同弧尾，指向下一个有相同弧尾的点
}VexNode, *Ptr_VexNode;

typedef struct HeadVex{
    ElemType data;
    Ptr_VexNode firstin; //指向以该顶点为弧头的第一个弧节点
    Ptr_VexNode firstout; //指向以该顶点为弧尾的第一个弧节点
}HeadVex, *Head;

typedef struct{
    int vexnum;
    int arcnum;
    bool IF_POWER; //是否带权
    HeadVex xlist[MAX_VERTEX_NUM];
}OLGraph, *Ptr_OLGraph;

int LocateVex(Ptr_OLGraph G, ElemType *vp);
void CreateGraph(Ptr_OLGraph G);

//图的顶点定位
int LocateVex(Ptr_OLGraph G, ElemType *vp)
{
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->xlist[k].data==*vp){
            return k;
        }
    }
    return -1;  //not found
}

//十字链表法一般用于有向图
void CreateGraph(Ptr_OLGraph G)
{
    using namespace std;
    cout << "input the number of vex: ";
    cin >> G->vexnum;
    cout << "input the number of arc: ";
    cin >> G->arcnum;
    cout << "if the graph with power? (1->power,0->no)";
    cin >> G->IF_POWER;
    int i,j,k;
    //构造表头
    for(i=0;i<G->vexnum;i++){
        G->xlist[i].data='A'+i;
        G->xlist[i].firstin=NULL;
        G->xlist[i].firstout=NULL;
    }
    char v1,v2;
    for(k=0;k<G->arcnum;k++){
        cout << "input the " << i+1 << " arc";
        cout << "the tail of arc: ";
        cin >> v1;
        cout << "the head of arc: ";
        cin >> v2;
        i=LocateVex(G,&v1);
        j=LocateVex(G,&v2);
        Ptr_VexNode p = new VexNode;
        p->tailvex=i;
        p->headvex=j;
        
        if(G->IF_POWER){
            cout << "input the power value of graph: ";
            cin >> p->data;
        }
        else{
            p->data=1;
        }

        //下一个以弧p的弧头(j点)为弧头的弧节点 为 第一个以j点为弧头的弧节点
        p->hlink=G->xlist[j].firstin;
        //下一个以弧p的弧尾(i点)为弧尾的弧节点 为 第一个以i点为弧尾的弧节点
        p->tlink=G->xlist[i].firstout;
        //采用头插入法,以弧p作为第一个以j点为弧头的弧
        G->xlist[j].firstin=p;
        //采用头插入法,以弧p作为第一个以i点为弧尾的弧
        G->xlist[i].firstout=p;
    }
    return;
}