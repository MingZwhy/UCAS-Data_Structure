//邻接多重表，用于无向图
#include<iostream>
const int MAX_VERTEX_NUM=30;
typedef enum{unvisited,visited} IF_Visit;
typedef int InfoType;
typedef char VertexType;
typedef struct Ebox{
    IF_Visit mark;       //访问标记
    int ivex,jvex;  //弧依附的两个顶点
    struct Ebox *ilink, *jlink;
    InfoType info;    //与边有关信息如权值
}Ebox, *Ptr_Ebox;

typedef struct HeadVex{
    VertexType data;   //顶点信息
    Ebox *firstedge;   //指向依附于该顶点的第一条边
}HeadVex;

//弧或边的定义
typedef struct ArcType{
    VertexType vex1,vex2;   //弧或边所依附的两个顶点
    InfoType info;    //与弧或边相关信息，如权值
}ArcType, *Ptr_ArcType;

typedef struct{
    int vexnum;
    int edgenum;
    HeadVex adjmulist[MAX_VERTEX_NUM];
}AMGraph, *Ptr_AMGraph;

//创建图
Ptr_AMGraph Creat_AMGrapgh(Ptr_AMGraph G)
{
    G->vexnum=0;
    G->edgenum=0;
    return G;
}

//图的顶点定位
int LocateVex(Ptr_AMGraph G, VertexType vp)
{
    int k;
    for(k=0;k<G->vexnum;k++){
        if(G->adjmulist[k].data==vp){
            return k;
        }
    }
    return -1;  //not found
}

//向图中增加顶点
int AddVertex(Ptr_AMGraph G, VertexType vp)
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
    G->adjmulist[G->vexnum].data=vp;
    G->adjmulist[G->vexnum].firstedge=NULL;
    G->vexnum++;
    k=G->vexnum;
    return k;  //k为当前顶点数
}

//向图中增加一条弧
int AddArc(Ptr_AMGraph G, Ptr_ArcType arc)
{
    using namespace std;
    int k,j;
    k=LocateVex(G,arc->vex1);
    j=LocateVex(G,arc->vex2);
    if(k==-1 || j==-1){
        printf("Arc's Vertex do not existed!\n");
        return -1;
    }
    Ptr_Ebox p = new Ebox;
    //p为边的起始节点
    p->ivex=k;
    p->jvex=j;
    p->info=arc->info;
    p->mark=unvisited;
    p->ilink=G->adjmulist[k].firstedge;
    p->jlink=G->adjmulist[j].firstedge;
    
    G->adjmulist[k].firstedge=p;
    G->adjmulist[j].firstedge=p;
    return 1;
}

//向图中删除一条弧
int DelArc(Ptr_AMGraph G, Ptr_ArcType arc)
{
    using namespace std;
    int i,j;
    i=LocateVex(G,arc->vex1);
    j=LocateVex(G,arc->vex2);
    if(i==-1 || j==-1){
        cout << "Arc's Vertex do not existed!\n";
        return -1;
    }
    //分别从i和j出发，找到欲删除弧的前弧，然后删除它
    bool headi,headj;
    headi=false; headj=false;
    Ptr_Ebox visiti,visitj;
    if(G->adjmulist[i].firstedge->ivex==j || G->adjmulist[i].firstedge->jvex==j){
        headi=true;
    }
    else{
        visiti=G->adjmulist[i].firstedge;
        while(1){
            if(i==visiti->ivex){
                if(visiti->ilink!=NULL && visiti->ilink->ivex!=j && visiti->ilink->jvex!=j){
                    visiti=visiti->ilink;
                }
                else{
                    if(visiti->ilink==NULL){
                        cout << "arc not found";
                        return -1;
                    }
                    break;
                }
            }
            else if(i==visiti->jvex){
                if(visiti->jlink!=NULL && visiti->jlink->ivex!=j && visiti->jlink->jvex!=j){
                    visiti=visiti->jlink;
                }
                else{
                    if(visiti->jlink==NULL){
                        cout << "arc not found";
                        return -1;
                    }
                    break;
                }
            }
        }
    }

    if(G->adjmulist[j].firstedge->ivex==i || G->adjmulist[j].firstedge->jvex==i){
        headj=true;
    }
    else{
        visitj=G->adjmulist[j].firstedge;
        while(1){
            if(j==visitj->ivex){
                if(visitj->ilink!=NULL && visitj->ilink->ivex!=i && visitj->ilink->jvex!=i){
                    visitj=visitj->ilink;
                }
                else{
                    if(visitj->ilink==NULL){
                        cout << "arc not found";
                        return -1;
                    }
                    break;
                }
            }
            else if(j==visitj->jvex){
                if(visitj->jlink!=NULL && visitj->jlink->ivex!=i && visiti->jlink->jvex!=i){
                    visitj=visitj->jlink;
                }
                else{
                    if(visitj->jlink==NULL){
                        cout << "arc not found";
                        return -1;
                    }
                    break;
                }
            }
        }
    }

    //删除弧
    if(headi){
        if(i==G->adjmulist[i].firstedge->ivex){
            G->adjmulist[i].firstedge=G->adjmulist[i].firstedge->ilink;
        }
        else{
            G->adjmulist[i].firstedge=G->adjmulist[i].firstedge->jlink;
        }
    }
    else{
        if(i==visiti->ilink->ivex){
            visiti->ilink=visiti->ilink->ilink;
        }
        else if(i==visiti->ilink->jvex){
            visiti->ilink=visiti->ilink->jlink;
        }
        else if(i==visiti->jlink->ivex){
            visiti->jlink=visiti->jlink->ilink;
        }
        else if(i==visiti->jlink->jvex){
            visiti->jlink=visiti->jlink->jlink;
        }
    }

    if(headj){
        if(j==G->adjmulist[i].firstedge->ivex){
            G->adjmulist[j].firstedge=G->adjmulist[j].firstedge->ilink;
        }
        else{
            G->adjmulist[j].firstedge=G->adjmulist[j].firstedge->jlink;
        }
    }
    else{
        if(j==visiti->ilink->ivex){
            visitj->ilink=visiti->ilink->ilink;
        }
        else if(i==visiti->ilink->jvex){
            visitj->ilink=visiti->ilink->jlink;
        }
        else if(i==visiti->jlink->ivex){
            visitj->jlink=visiti->jlink->ilink;
        }
        else if(i==visiti->jlink->jvex){
            visitj->jlink=visiti->jlink->jlink;
        }
    }
    return 1;
}

void Print_AMGraph(Ptr_AMGraph G)
{
    using namespace std;
    cout << G->vexnum << "Vex";
    for(int i=0;i<G->vexnum;i++){
        cout << G->adjmulist[i].data << " ";
    } 
    cout << endl;
    for(int k=0;k<G->vexnum;k++){
        cout << G->adjmulist[k].data << ": ";
        Ptr_Ebox visit=G->adjmulist[k].firstedge;
        while(visit!=NULL){
            if(k==visit->ivex){
                cout << "(" << visit->ivex << "," << visit->jvex << ")" ;
                visit=visit->ilink;
                if(visit){
                    cout << "-->";
                }
            }
            else if(k==visit->jvex){
                cout << "(" << visit->ivex << "," << visit->jvex << ")";
                visit=visit->jlink;
                if(visit){
                    cout << "-->";
                }
            }
        }
        cout << endl;
    }
}

int main(){
    using namespace std;
    Ptr_AMGraph G = new AMGraph;
    G=Creat_AMGrapgh(G);
    AddVertex(G,'A'); AddVertex(G,'B');
    AddVertex(G,'C'); AddVertex(G,'D');
    AddVertex(G,'E');
    Ptr_ArcType arc = new ArcType[6];
    arc[0].vex1='A'; arc[0].vex2='B'; arc[0].info=1;
    arc[1].vex1='A'; arc[1].vex2='D'; arc[1].info=1;
    arc[2].vex1='C'; arc[2].vex2='B'; arc[2].info=1;
    arc[3].vex1='C'; arc[3].vex2='D'; arc[3].info=1;
    arc[4].vex1='E'; arc[4].vex2='B'; arc[4].info=1;
    arc[5].vex1='C'; arc[5].vex2='E'; arc[5].info=1;
    for(int i=0;i<6;i++){
        AddArc(G,arc+i);
    }
    Print_AMGraph(G);
    cout << endl << "delete the arc arc[3] CD:" << endl;
    DelArc(G,arc+3);
    Print_AMGraph(G);
    delete G;
}