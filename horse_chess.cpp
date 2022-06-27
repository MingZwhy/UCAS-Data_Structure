#include<iostream>
using namespace std;

const int ROW = 8;
const int COL = 8;
const int FINISH = ROW * COL;
const int ALL_Direction = 8;
const int INF = 10000;

typedef struct Next_point{
    int x;
    int y;
    int Possib;
}Next_point;

typedef struct Point{
    int x;
    int y;
    bool IF_Init;  //是否已初始化过信息
    Next_point next[ALL_Direction];  //可能的选点
}Point, *Ptr_Point;

//ROW x COL 的棋盘
int Chess_Board[ROW][COL];
//记录成功结果的次数
int num=0;
//默认栈的初始大小
const int INIT_STACK_SIZE = 1000;

void Init_Board(void);
int Get_Possib(int x, int y, int count);
void Get_Next(Point &p, int count);
int Next_Point(Point &p);
void Print_Board(void);
void DFS_Visit(int x, int y);

//栈类
class My_Stack
{
private:
    Ptr_Point base;
    Ptr_Point top;
    int Stacksize;
public:
    My_Stack();          //默认构造函数
    My_Stack(int size = INIT_STACK_SIZE);  //构造函数，参数为栈的大小
    bool If_Empty() const;     //判断栈是否为空
    bool Push(Point p);         //入栈，返回true代表入栈成功
    bool Top(Point &p) const;  //读取栈顶元素
    bool Pop();          //出栈
    ~My_Stack();         //析构函数
};

My_Stack::My_Stack()
{
    base = new Point[INIT_STACK_SIZE];
    top = base;
    Stacksize = INIT_STACK_SIZE;
}

My_Stack::My_Stack(int size)
{
    base = new Point[size];
    top = base;
    Stacksize = size;  
}

My_Stack::~My_Stack()
{
    delete [] base;
}

bool My_Stack::If_Empty() const
{
    return (base==top);
}

bool My_Stack::Push(Point p)
{
    if(top-base==Stacksize){
        cout << "The stack is full , cancel push";
        return false;
    }
    *top = p;
    top++;
    return true;
}

bool My_Stack::Top(Point &p) const
{
    if(If_Empty()){
        cout << "The stack is empty , can't get top";
        return false;
    }
    p = *(top-1);
    return true;
}

bool My_Stack::Pop()
{
    if(If_Empty()){
        cout << "The stack is empty, cancel the pop";
        return false;
    }
    top--;
    return true;
}

void Init_Board(void){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            Chess_Board[i][j]=0;
        }
    }
}

int Get_Possib(int x, int y, int count)
{
    if(x<0 || x>=ROW || y<0 || y>=COL || Chess_Board[x][y]!=0){
        return INF;
    }
    int possib = ALL_Direction;
    if(x-2<0 || y>=COL || Chess_Board[x-2][y+1]!=0)  possib--;
    if(x-1<0 || y+2>=COL || Chess_Board[x-1][y+2]!=0)  possib--;
    if(x+1>=ROW || y+2>=COL || Chess_Board[x+1][y+2]!=0)  possib--;
    if(x+2>=ROW || y+1>=COL || Chess_Board[x+2][y+1]!=0)  possib--;
    if(x+2>=ROW || y-1<0 || Chess_Board[x+2][y-1]!=0)  possib--;
    if(x+1>=ROW || y-2<0 || Chess_Board[x+1][y-2]!=0)  possib--;
    if(x-1<0 || y-2<0 || Chess_Board[x-1][y-2]!=0)  possib--;
    if(x-2<0 || y-1<0 || Chess_Board[x-2][y-1]!=0)  possib--;
    
    if(possib==0 && count!=FINISH-1){
        return INF;
    }
    else{
        return possib;
    }
}

void Get_Next(Point &p, int count)
{
    for(int i=0;i<ALL_Direction;i++)
    {
        switch (i+1)
        {
        case 1:
            p.next[i].x = p.x-2;
            p.next[i].y = p.y+1;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 2:
            p.next[i].x = p.x-1;
            p.next[i].y = p.y+2;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 3:
            p.next[i].x = p.x+1;
            p.next[i].y = p.y+2;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 4:
            p.next[i].x = p.x+2;
            p.next[i].y = p.y+1;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 5:
            p.next[i].x = p.x+2;
            p.next[i].y = p.y-1;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 6:
            p.next[i].x = p.x+1;
            p.next[i].y = p.y-2;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 7:
            p.next[i].x = p.x-1;
            p.next[i].y = p.y-2;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        case 8:
            p.next[i].x = p.x-2;
            p.next[i].y = p.y-1;
            p.next[i].Possib = Get_Possib(p.next[i].x,p.next[i].y,count);
            break;
        default:
            break;
        }
    }
    p.IF_Init=true;
}
int Next_Point(Point &p)
{
    int less = INF;
    int index = -1;
    for(int i=0;i<ALL_Direction;i++){
        if(p.next[i].Possib < less){
            index = i;
            less = p.next[i].Possib;
        }
    }
    if(index!=-1){
        p.next[index].Possib = INF;
    }
    return index;
}

void Print_Board(void){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            if(Chess_Board[i][j]<10){
                cout << Chess_Board[i][j] << "  ";
            }
            else{
                cout << Chess_Board[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void DFS_Visit(int x, int y)
{
    int count = 1;
    My_Stack s(INIT_STACK_SIZE);
    Point init;
    init.x = x; init.y = y; init.IF_Init=false;
    s.Push(init);
    Chess_Board[init.x][init.y] = count;

    while(!s.If_Empty())
    {
        
#if 0
        cout << "count is" << count << endl;
        Print_Board();
#endif
        Point p;
        s.Top(p);
        s.Pop();
        if(!p.IF_Init){
            Get_Next(p,count);
        }
        int index = Next_Point(p);
        s.Push(p);
        if(index==-1){  //所有可能都已探索，需要出栈该点
            s.Pop();
            Chess_Board[p.x][p.y] = 0;
            count--;
        }
        else{
            Point next;
            next.x=p.next[index].x; next.y=p.next[index].y;
            next.IF_Init=false;
            s.Push(next);
            count++;
            Chess_Board[next.x][next.y] = count;
            if(count==FINISH){
                num++;
                cout << "result " << num << endl;
                Print_Board();
                count--;
                Chess_Board[next.x][next.y]=0;
                s.Pop();
            }
        }
        
        if(count==FINISH){
            Print_Board();
        }
    }
}

int main(){
    Init_Board();
    DFS_Visit(2,0);
    cout << "end";
    return 0;
}