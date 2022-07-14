#include<iostream>
#include<ctime>
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

void Init_Board(void);                      //初始化棋盘  ROW x COL
int Get_Possib(int x, int y, int count);    //得到可能的下一步可能的选点数
void Get_Next(Point &p, int count);         //得到当前点8个方向的下一个落点信息
int Next_Point(Point &p);                   //通过比较得到最小值作为最终选点
void Print_Board(void);                     //打印棋盘
void DFS_Visit(int x=0, int y=0, int target_num=100, bool IF_Print=true);               //深度优先搜索棋盘

//栈类及其方法——构造，判断是否为空，入栈，读栈，出栈以及析构
/*------------------------------------------------------------------------------------*/
class My_Stack
{
private:
    Ptr_Point base;
    Ptr_Point top;
    int Stacksize;
public:
    My_Stack();          //默认构造函数
    My_Stack(int size = INIT_STACK_SIZE);  //构造函数，参数为栈的大小，默认为1000
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

/*--------------------------------------------------------------------------------*/

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
    int possib = ALL_Direction;     //最多8个选点
    if(x-2<0 || y>=COL || Chess_Board[x-2][y+1]!=0)  possib--;  //每有一个方向不合格，则可能-1
    if(x-1<0 || y+2>=COL || Chess_Board[x-1][y+2]!=0)  possib--;
    if(x+1>=ROW || y+2>=COL || Chess_Board[x+1][y+2]!=0)  possib--;
    if(x+2>=ROW || y+1>=COL || Chess_Board[x+2][y+1]!=0)  possib--;
    if(x+2>=ROW || y-1<0 || Chess_Board[x+2][y-1]!=0)  possib--;
    if(x+1>=ROW || y-2<0 || Chess_Board[x+1][y-2]!=0)  possib--;
    if(x-1<0 || y-2<0 || Chess_Board[x-1][y-2]!=0)  possib--;
    if(x-2<0 || y-1<0 || Chess_Board[x-2][y-1]!=0)  possib--;
    
    if(possib==0 && count!=FINISH-1){   //非终点且可能选点为0
        return INF;    //返回一个极大值，因为我们选最小值，所以不可能选到该点
    }
    else{
        return possib;  //正常返回选点可能数
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
    return index;   //如果所有可能点都探索过了则会返回-1
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

void DFS_Visit(int x, int y, int target_num, bool IF_Print)
{
    int count = 1;
    My_Stack s(INIT_STACK_SIZE);
    Point init;
    init.x = x; init.y = y; init.IF_Init=false;
    s.Push(init);
    Chess_Board[init.x][init.y] = count;

    while(!s.If_Empty())
    {
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
                if(IF_Print){
                    cout << "result " << num << endl;
                    Print_Board();
                }
                if(num==target_num){
                    cout << "finish " << target_num << " results successfully!" << endl;
                    return;
                }
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
    int row,col;
    int target_num;
    cout << "Please enter the X-axis of the starting point:";
    cin >> row;
    cout << "Please enter the Y-axis of the starting point:";
    cin >> col;
    cout << "How many results do you want to get:";
    cin >> target_num;
    bool IF_Print;
    char temp;
    cout << "If you want to print each kind of results? (y/n)";
    cin >> temp;
    if(temp=='y' || temp=='Y'){
        IF_Print = true;
    }
    else{
        IF_Print = false;
    }
    clock_t start,end;
    start = clock();
    DFS_Visit(row,col,target_num,IF_Print);
    end = clock();
    double time=(double)(end-start)/CLOCKS_PER_SEC;
    if(!IF_Print){
        cout << "using time " << time;
    }
    return 0;
}