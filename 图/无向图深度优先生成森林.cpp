#include<iostream>
#include<string.h>
#include<stdlib.h>
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

//孩子兄弟表示法示树
typedef struct TreeNode{
    ElemType data;
    struct TreeNode *firstchild;
    struct TreeNode *nextsibling;
}TreeNode, *CSTree;

Ptr_ALGraph Create_Graph(Ptr_ALGraph G, Graph_Kind kind=UDG);
//默认参数设置为无向图
int LocateVex(Ptr_ALGraph G, ElemType &vp);
int AddVertex(Ptr_ALGraph G, ElemType vp);
int AddArc(Ptr_ALGraph G, Ptr_ArcType arc);
void Print_AdjList(Ptr_ALGraph G);
void DFSFrorest(Ptr_ALGraph G, CSTree &T);
void DFSTree(Ptr_ALGraph G, int v, CSTree &T, bool * visited);
void draw_level_S(CSTree node, bool left, char* str);
void draw_S(CSTree root);

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

void DFSFrorest(Ptr_ALGraph G, CSTree &T)
{
    using namespace std;
    int v; int j=0;
    CSTree p,q;
    T=NULL;
    bool * visited = new bool[G->vexnum];
    for(v=0;v<G->vexnum;v++){
        visited[v]=false;
    }
    for(v=0;v<G->vexnum;v++){     //该循环以每颗子树为单位，之间以nextsibling连接
        if(!visited[v]){
            //第v顶点为新的生成树的根节点
            p = new TreeNode;
            p->data=G->head[v].vertex;
            p->firstchild=NULL;
            p->nextsibling=NULL;

            if(T==NULL){
                T=p;  //p是第一课生成树的根
            }
            else{
                q->nextsibling=p;  //当前生成树的根为上一课生成树的根的兄弟
            }

            q=p;  //q指向当前最新生成树的根

            DFSTree(G,v,p,visited);   //建立以p为根的生成树
        }
    }
}

void DFSTree(Ptr_ALGraph G, int v, CSTree &T, bool * visited)
{
    using namespace std;
    //从第v个顶点出发递归地深度优先遍历图或子图(非连通)
    int w;
    CSTree p,q,tag;
    bool first=true;
    /*first是标记，对于一个节点来说，他的第一个临接点要作为他的firstchild，
    而他的其他邻接点将作为他的firstchild的nextsibling，第一个和其他通过
    bool first来区分，遍历过第一个之后将first置为false即可*/

    visited[v]=true;

    Ptr_NodeLink visit=G->head[v].first;
    while(visit!=NULL){
        w=visit->vindex;   //当前节点在图中顶点数组的下标

        if(!visited[w]){
            p = new TreeNode;
            p->data=visit->vex;
            p->firstchild=NULL;
            p->nextsibling=NULL;

            if(first){   //如果w是v第一个访问的邻接点
                T->firstchild=p;
                first=false;  //第一次没了
            }
            else{        //w是v第2,3..访问的节点，将作为上一临接顶点的右兄弟
                q->nextsibling=p;
            }

            q=p;  //q置为当前访问的节点
            DFSTree(G,w,q,visited);    //递归调用,w为当前节点在图中顶点数组的下标,q指向当前节点
        }

        visit=visit->next;
    }
}

void draw_level_S(CSTree node, bool left, char* str){
	if(node->nextsibling){
		draw_level_S(node->nextsibling, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->data);
	
	if(node->firstchild){
		draw_level_S(node->firstchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//长度都为6
	str[strlen(str)-6] = '\0'; 
} 

void draw_S(CSTree root){
	char str[30];
	memset(str, '\0', 30);
	
	if(root->nextsibling){
		draw_level_S(root->nextsibling, false, str);
	}
	printf("%c\n",root->data);  //打印根节点的数据
	if(root->firstchild){
		draw_level_S(root->firstchild, true, str);
	} 
}

int main(){
    using namespace std;
    Ptr_ALGraph G = new ALGraph;
    G=Create_Graph(G);
    for(int i=0;i<13;i++){
        AddVertex(G,'A'+i);
    }
    Ptr_ArcType arc = new ArcType[13];
    arc[0].vex1='A'; arc[0].vex2='C'; 
    arc[1].vex1='A'; arc[1].vex2='F'; 
    arc[2].vex1='A'; arc[2].vex2='B'; 
    arc[3].vex1='A'; arc[3].vex2='L'; 
    arc[4].vex1='B'; arc[4].vex2='M'; 
    arc[5].vex1='L'; arc[5].vex2='M'; 
    arc[6].vex1='L'; arc[6].vex2='J'; 
    arc[7].vex1='M'; arc[7].vex2='J'; 
    arc[8].vex1='D'; arc[8].vex2='E'; 
    arc[9].vex1='I'; arc[9].vex2='G'; 
    arc[10].vex1='G'; arc[10].vex2='H'; 
    arc[11].vex1='H'; arc[11].vex2='K'; 
    arc[12].vex1='G'; arc[12].vex2='K'; 

    for(int i=0;i<13;i++){
        AddArc(G,arc+i);
    }
    Print_AdjList(G);
    cout << endl << endl;
    CSTree T;
    DFSFrorest(G,T);
    draw_S(T);
    delete G;
    return 0;
}