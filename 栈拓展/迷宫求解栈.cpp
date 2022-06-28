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
    //走不通的块: '!'
}MazeType;

//类型定义迷宫坐标
typedef struct{
    int row;
    int col;
}PosType;

//类型定义路径中的一个通道块
typedef struct{
    int order;   //表示该通道块在路径上序号
    PosType seat; //表示通道块在迷宫的坐标位置
    int direction;   //表示走向下一个通道块的方向
}ElemType, *Ptr_To_Block;   //起名ElemType因为将它作为栈中元素

//定义栈类型
typedef struct{
    Ptr_To_Block base;
    Ptr_To_Block top;
    int Stack_size;
}Maze_stack, *Stack;

//迷宫函数
bool If_Pass(MazeType MyMaze, PosType CurPos);
void FootPrint(MazeType *MyMaze, PosType CurPos);
void MarkPrint(MazeType *MyMaze, PosType CurPos);
PosType NextPos(PosType CurPos, int Dir);

//栈函数
Stack Init_Maze_stack(void);
bool Is_Empty(Stack s);
void Push(Stack s, ElemType b);
ElemType Top(Stack s);
ElemType Pop(Stack s);

//初始化栈
Stack Init_Maze_stack(void)
{
    Stack s;
    s=(Stack)malloc(sizeof(Maze_stack)*1);
    if(s==NULL){
        printf("malloc error");
        exit(1);
    }
    s->base=(Ptr_To_Block)malloc(sizeof(ElemType)*INIT_STACKSIZE);
    if(s->base==NULL)   exit(1);
    s->top=s->base;
    s->Stack_size = INIT_STACKSIZE;
    return s;
}

//是否为空
bool Is_Empty(Stack s)
{
    return (s->top==s->base)? true : false;
}

//入栈
void Push(Stack s, ElemType b)
{
    if(s->top-s->base>=s->Stack_size){
        s->base=(Ptr_To_Block)realloc(s->base,sizeof(ElemType)*(s->Stack_size+ADDSIZE));
        if(s->base==NULL)   exit(1);
        s->top = s->base + s->Stack_size;
        s->Stack_size += ADDSIZE;
    }
    *s->top = b;
    (s->top)++;
}

//读栈顶
ElemType Top(Stack s)
{
    if(Is_Empty(s)){
        printf("stack is empty");
        ElemType none;
        return none;
    }
    else{
        return *(s->top-1);
    }
}

//出栈
ElemType Pop(Stack s)
{
    if(Is_Empty(s)){
        printf("stack is empty");
        ElemType none;
        return none;
    }
    else{
        s->top--;
        return *s->top;
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
        //可能是墙X、走过的*、走不通的！
    }
}

//在迷宫的当前位置留下走过标记*
void FootPrint(MazeType *MyMaze, PosType CurPos)
{
    MyMaze->MiGong[CurPos.row][CurPos.col]='*';
}

//在迷宫的当前位置留下走不通标记!
void MarkPrint(MazeType *MyMaze, PosType CurPos)
{
    MyMaze->MiGong[CurPos.row][CurPos.col]='!';
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

//若迷宫Maze中存在从入口 start到出口 end的通道，则求得一条存放在栈S中
bool MazePath(MazeType *maze, PosType start, PosType end, Stack S)
{
    PosType curpos;  //当前位置
    int curstep;     //步数
    ElemType e;      //块

    curpos = start;  //设定当前位置为入口位置
    curstep = 1;     

    do{
        if(If_Pass(*maze,curpos)){   
            FootPrint(maze,curpos);  //当前位置可走，留下zuji
            e.order = curstep;
            e.seat = curpos;
            e.direction = 1;  //从右边开始
            Push(S,e);   //将当前通道块加入路径
            if(curpos.row==end.row && curpos.col==end.col){
                return true;  //到达出口
            }
            //准备探索下一通道块
            //设置当前通道块为当前通道块的东边
            curpos = NextPos(curpos,1);
            curstep++;
        }
        else{  //当前位置不能通过
            if(!Is_Empty(S)){
                e=Pop(S);
                while(e.direction==4 && !Is_Empty(S)){
                    //e.direction==4 意味着可能性被探索完了，需要出栈
                    MarkPrint(maze,e.seat);  //此路不通
                    e=Pop(S);  //退栈直到该块还有可能性
                    curstep--;  //步数减一
                }
                //当前块e还有可能性或者e即为开始块且不确定是否探索完
                if(e.direction<4){
                    e.direction++;
                    Push(S,e);  //重新入栈且向下一个方向探索
                    //更新当前块
                    curpos = NextPos(e.seat,e.direction);
                }
            }
        }
    }while(!Is_Empty(S));
    
    return false;
}

int main(){
    Stack S=Init_Maze_stack();
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
    if(flag){
        //打印路径图
        for(i=0;i<ROW;i++){
            for(j=0;j<COL;j++){
                printf("%c",M.MiGong[i][j]);
            }
            printf("\n");
        }
        //打印路径数据
        printf("The length of path is %d\n",S->top-S->base);
        printf("The path is:\n");
        while(S->base<S->top){
            printf("%d,%d\n",S->base->seat.row,S->base->seat.col);
            S->base++;
        }
    }
    else{
        printf("no path");
    }

    return 0;
}