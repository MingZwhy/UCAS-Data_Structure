//运用队列——广度优先搜索的方法可以在不穷举的情况下求出最短路径
//先被访问的顶点的邻接点先于后被访问的顶点的邻接
//因为广度搜索遍历是一层一层的去搜索从入口到出口的遍历过程，所以在找到出口的时候，一定是当前最短路径
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 10
#define COL 10
#define INIT_STACKSIZE 100
#define ADDSIZE 10

char migong[ROW+2][COL+2] = {{'X','X','X','X','X','X','X','X','X','X'},
                             {'X',' ',' ','X',' ',' ',' ','X',' ','X'},
                             {'X',' ',' ','X',' ',' ',' ','X',' ','X'},
                             {'X',' ',' ',' ',' ','X','X',' ',' ','X'},
                             {'X',' ','X','X','X',' ',' ',' ',' ','X'},
                             {'X',' ',' ',' ','X',' ',' ',' ',' ','X'},
                             {'X',' ','X',' ',' ',' ','X',' ',' ','X'},
                             {'X',' ','X','X','X',' ','X','X',' ','X'},
                             {'X','X',' ',' ',' ',' ',' ',' ',' ','X'},
                             {'X','X','X','X','X','X','X','X','X','X'}};

//类型定义迷宫
typedef struct Maze{
    char MiGong[ROW][COL];
    //迷宫的墙: 'X'
    //没有走过的通道块: ' '
    //走过标记: '*'
}MazeType;

//类型定义迷宫坐标
typedef struct{
    int row;
    int col;
}PosType;

//类型定义路径中的一个通道块
typedef struct elem{
    PosType seat; //表示通道块在迷宫的坐标位置
    int former;  //表示通道块的上一个位置(溯源)
}ElemType, *Ptr_To_Block;   //起名ElemType因为将它作为栈中元素

//定义栈类型
typedef struct{
    Ptr_To_Block head;
    Ptr_To_Block tail;
    int number;
    int Stack_size;
}Maze_Queue, *Queue;

//记录轨迹
ElemType path[1000];

//迷宫函数
bool If_Pass(MazeType MyMaze, PosType CurPos);
bool If_Final(PosType final, PosType CurPos);
void FootPrint(MazeType *MyMaze, PosType CurPos);
PosType NextPos(PosType CurPos, int Dir);
void Print_Path(ElemType f);

//栈函数
Queue Init_Maze_queue(void);
bool Is_Empty(Queue s);
void Push(Queue s, ElemType b);
ElemType Top(Queue s);
ElemType Pop(Queue s);

//初始化栈
Queue Init_Maze_queue(void)
{
    Queue s;
    s=(Queue)malloc(sizeof(Maze_Queue)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->head=(Ptr_To_Block)malloc(sizeof(ElemType)*INIT_STACKSIZE);
    if(s->head==NULL)   exit(1);
    s->tail=s->head;
    s->Stack_size = INIT_STACKSIZE;
    s->number=0;
    return s;
}

//是否为空
bool Is_Empty(Queue s)
{
    return (s->number==0)? true : false;
}

//入栈
void Push(Queue s, ElemType b)
{
    *s->tail = b;
    (s->tail)++;
    s->number++;
}

//判断是否到终点
bool If_Final(PosType final, PosType CurPos)
{
    return (final.row==CurPos.row && final.col==CurPos.col);
}

//读栈顶
ElemType Top(Queue s)
{
    if(Is_Empty(s)){
        printf("Queue is empty");
        ElemType none;
        return none;
    }
    else{
        return *(s->head);
    }
}

//出栈
ElemType Pop(Queue s)
{
    if(Is_Empty(s)){
        printf("Queue is empty");
        ElemType none;
        return none;
    }
    else{
        s->head++;
        s->number--;
        return *(s->head-1);
    }
}

//判定迷宫的当前位置是否可通过
bool If_Pass(MazeType MyMaze, PosType CurPos)
{
    if(MyMaze.MiGong[CurPos.row][CurPos.col]==' '){
        return true;
    }
    else{
        return false;
        //可能是墙X、走过的*
    }
}

//在迷宫的当前位置留下走过标记*
void FootPrint(MazeType *MyMaze, PosType CurPos)
{
    MyMaze->MiGong[CurPos.row][CurPos.col]='*';
}

//返回当前位置的direction所指示的位置
PosType NextPos(PosType CurPos, int Dir)
{
    PosType Return_Pos;
    switch(Dir){
        case 1: Return_Pos.row = CurPos.row;
                Return_Pos.col = CurPos.col + 1;
                break;
        case 2: Return_Pos.row = CurPos.row + 1;
                Return_Pos.col = CurPos.col;
                break;
        case 3: Return_Pos.row = CurPos.row;
                Return_Pos.col = CurPos.col - 1;
                break;
        case 4: Return_Pos.row = CurPos.row - 1;
                Return_Pos.col = CurPos.col;
                break;
        default: Return_Pos.row = 0;
                 Return_Pos.col = 0;
                 break;
    }
    return Return_Pos;
}

void Print_Path(ElemType f)
{
    PosType *record=(PosType *)malloc(sizeof(PosType)*1000);
    if(record==NULL){
        printf("malloc error");
        exit(1);
    }
    int index=0;
    record[index++]=f.seat;
    int visit=f.former;
    while(visit!=-1){
        record[index++]=path[visit].seat;
        visit=path[visit].former;
    }
    for(--index;index>=0;index--){
        printf("(%d,%d) ",record[index].row,record[index].col);
    }
}

//若迷宫Maze中存在从入口 start到出口 end的通道，则求得一条存放在栈S中
bool MazePath(MazeType *maze, PosType start, PosType end, Queue S)
{
    int index=0;
    PosType curpos;  //当前位置
    int curstep;     //步数
    ElemType e;      //块

    curpos = start;  //设定当前位置为入口位置 
    e.seat=curpos;
    e.former=-1; //启始节点无前溯
    Push(S,e);
    FootPrint(maze,e.seat);
    while(!Is_Empty(S)){
        e=Pop(S);
        if(If_Final(end,e.seat)){
            Print_Path(e);
            return true;
        }
        int i;
        path[index++]=e;
        for(i=1;i<=4;i++){
            curpos=NextPos(e.seat,i);
            if(If_Pass(*maze,curpos)){
                ElemType temp;
                temp.seat=curpos;
                temp.former=index-1;
                Push(S,temp);
                FootPrint(maze,temp.seat);
            }
        }
    }  
    return false;
}

int main(){
    Queue S=Init_Maze_queue();
    MazeType M;
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            M.MiGong[i][j]=migong[i][j];
        }
    }
    PosType start;
    start.row=1; start.col=1;
    PosType end;
    end.row=8; end.col=8;
    bool flag=MazePath(&M, start, end, S);
    if(!flag){
        printf("no pass");
    }
    return 0;
}