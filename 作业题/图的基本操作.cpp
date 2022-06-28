#include<iostream>
const int MAX_VERTEX_NUM=50;

typedef int VertexType;

typedef struct{
    int vexnum;
    bool arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
}MGraph, *Ptr_To_MGraph;

typedef struct{
    VertexType vex1,vex2;
}ArcType;

Ptr_To_MGraph CreateGraph(Ptr_To_MGraph G);
Ptr_To_MGraph InsertVex(Ptr_To_MGraph G, VertexType pos);
Ptr_To_MGraph DeleteVex(Ptr_To_MGraph G, VertexType pos);
Ptr_To_MGraph InsertArc(Ptr_To_MGraph G, ArcType arc);
Ptr_To_MGraph DeleteArc(Ptr_To_MGraph G, ArcType arc);
void Print_Graph(Ptr_To_MGraph G);

Ptr_To_MGraph CreateGraph(Ptr_To_MGraph G)
{
    using namespace std;
    char c;
    cin >> G->vexnum;
    cin.get();
    for(int i=0;i<G->vexnum;i++){
        for(int j=0;j<G->vexnum;j++){
            cin >> G->arcs[i][j];
            cin.get();
        }
    }
    return G;
}

Ptr_To_MGraph InsertVex(Ptr_To_MGraph G, VertexType pos)
{
    //向右下移动
    for(int i=G->vexnum-1;i>=pos;i--){
        for(int j=G->vexnum-1;j>=pos;j--){
            G->arcs[i+1][j+1] = G->arcs[i][j];
        }
    }
    //向右移动
    for(int i=0;i<pos;i++){
        for(int j=G->vexnum-1;j>=pos;j--){
            G->arcs[i][j+1] = G->arcs[i][j];
        }
    }
    //向下移动
    for(int j=0;j<pos;j++){
        for(int i=G->vexnum-1;i>=pos;i--){
            G->arcs[i+1][j] = G->arcs[i][j];        }
    }
    //填充0
    for(int i=pos;i<=G->vexnum;i++){
        G->arcs[i][pos]=0;
    }
    for(int j=pos;j<=G->vexnum;j++){
        G->arcs[pos][j]=0;
    }
    //修改顶点数
    G->vexnum++;
    return G;
}

Ptr_To_MGraph DeleteVex(Ptr_To_MGraph G, VertexType pos)
{
    //向左上移动
    for(int i=pos;i<G->vexnum-1;i++){
        for(int j=pos;j<G->vexnum-1;j++){
            G->arcs[i][j] = G->arcs[i+1][j+1];
        }
    }
    //向左移动
    for(int i=0;i<pos;i++){
        for(int j=pos;j<G->vexnum-1;j++){
            G->arcs[i][j] = G->arcs[i][j+1];
        }
    }
    //向上移动
    for(int j=0;j<pos;j++){
        for(int i=pos;i<G->vexnum-1;i++){
            G->arcs[i][j] = G->arcs[i+1][j];
        }
    }
    //修改顶点数
    G->vexnum--;
    return G;
}

Ptr_To_MGraph InsertArc(Ptr_To_MGraph G, ArcType arc)
{
    G->arcs[arc.vex1][arc.vex2] = 1;
    G->arcs[arc.vex2][arc.vex1] = 1;
    return G;
}

Ptr_To_MGraph DeleteArc(Ptr_To_MGraph G, ArcType arc)
{
    G->arcs[arc.vex1][arc.vex2] = 0;
    G->arcs[arc.vex2][arc.vex1] = 0;
    return G;
}

void Print_Graph(Ptr_To_MGraph G)
{
    using namespace std;
    for(int i=0;i<G->vexnum;i++){
        for(int j=0;j<G->vexnum;j++){
            cout << G->arcs[i][j];
            if(j<G->vexnum-1){
                cout << " ";
            }
        }
        if(i<G->vexnum-1){
            cout << endl;
        }
    }
}

int main(){
    using namespace std;
    Ptr_To_MGraph G = new MGraph;
    G=CreateGraph(G);
    char c=cin.get();
    char a,b;
    VertexType pos;
    ArcType arc;
    while(c!=EOF){
        a = c;
        cin >> b;
        if(a=='I' && b=='V'){
            cin.get();
            cin >> pos;
            InsertVex(G,pos);
            c=cin.get();
            c=cin.get();
        }
        else if(a=='I' && b=='A'){
            cin.get();
            cin >> arc.vex1;
            cin.get();
            cin >> arc.vex2;
            InsertArc(G,arc);
            c=cin.get();
            c=cin.get();
        }
        else if(a=='D' && b=='V'){
            cin.get();
            cin >> pos;
            DeleteVex(G,pos);
            c=cin.get();
            c=cin.get();
        }
        else if(a=='D' && b=='A'){
            cin.get();
            cin >> arc.vex1;
            cin.get();
            cin >> arc.vex2;
            DeleteArc(G,arc);
            c=cin.get();
            c=cin.get();
        }
    }
    Print_Graph(G);
    delete G;
    return 0;
}