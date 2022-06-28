#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 8
#define COL 8
#define bool int
#define true 1
#define false 0

typedef struct{
    int x;
    int y;
}Point, *Ptr_To_Point;

typedef struct{
    char MG[ROW+2][COL+2];
    int length;
}Path_Record, *Ptr_To_Path_Record;

typedef struct{
    Ptr_To_Path_Record All_Choice;
    int min_length;              //最短路径长度
    Ptr_To_Path_Record min_path; //指向最短路径
}All_Path;

All_Path top;
int count=0;   //path数量

char MiGong[ROW+2][COL+2] = {{'#','#','#','#','#','#','#','#','#','#'},
                             {'#',' ',' ','#',' ',' ',' ','#',' ','#'},
                             {'#',' ',' ','#',' ',' ',' ','#',' ','#'},
                             {'#',' ',' ',' ',' ','#','#',' ',' ','#'},
                             {'#',' ','#','#','#',' ',' ',' ',' ','#'},
                             {'#',' ',' ',' ','#',' ',' ',' ',' ','#'},
                             {'#',' ','#',' ',' ',' ','#',' ',' ','#'},
                             {'#',' ','#','#','#',' ','#','#',' ','#'},
                             {'#','#',' ',' ',' ',' ',' ',' ','*','#'},
                             {'#','#','#','#','#','#','#','#','#','#'}};

void Init_MiGong(void);
void Print_Board(void);
bool Find_Path(Point p, int index);

void Print_Board(void)
{
    int i,j;
    for(i=0;i<(ROW+2);i++){
        for(j=0;j<COL+2;j++){
            printf("%c",MiGong[i][j]);
        }
        printf("\n");
    }
}

bool Find_Path(Point p, int index)
{
    if(MiGong[p.x][p.y+1]=='*' || MiGong[p.x-1][p.y]=='*' 
        || MiGong[p.x][p.y-1]=='*' || MiGong[p.x+1][p.y]=='*' ){
        //到达终点
        //记录入record
        top.All_Choice[count].length=index+1;
        int i,j;
        for(i=0;i<ROW+2;i++){
            for(j=0;j<COL+2;j++){
                top.All_Choice[count].MG[i][j]=MiGong[i][j];
            }
        }
        if(index+1<top.min_length){
            top.min_length = index+1;
            top.min_path = &top.All_Choice[count];
        }
        count++;
        return true;
    }
    bool flag_return=false;
    int i;
    for(i=1;i<=4;i++){
        if(i==1){   //向右
            if(MiGong[p.x][p.y+1]==' '){
                p.y+=1;
                MiGong[p.x][p.y]='x';
                bool flag=Find_Path(p,index+1);
                if(flag){
                    flag_return=true;
                }
                MiGong[p.x][p.y]=' ';
                p.y-=1;
            }
        }
        else if(i==2){    //向上
            if(MiGong[p.x-1][p.y]==' '){ 
                p.x-=1;
                MiGong[p.x][p.y]='x';
                bool flag=Find_Path(p,index+1);
                if(flag){
                    flag_return=true;
                }
                MiGong[p.x][p.y]=' ';
                p.x+=1;
            }
        }
        else if(i==3){   //向左
            if(MiGong[p.x][p.y-1]==' '){
                p.y-=1;
                MiGong[p.x][p.y]='x';
                bool flag=Find_Path(p,index+1);
                if(flag){
                    flag_return=true;
                }
                MiGong[p.x][p.y]=' ';
                p.y+=1;
            }
        }
        else if(i==4){    //向下
            if(MiGong[p.x+1][p.y]==' '){
                p.x+=1;
                MiGong[p.x][p.y]='x';
                bool flag=Find_Path(p,index+1);
                if(flag){
                    flag_return=true;
                }
                MiGong[p.x][p.y]=' ';
                p.x-=1;
            }
        }
    }
    return flag_return;
}

int main()
{
    top.All_Choice=(Ptr_To_Path_Record)malloc(sizeof(Path_Record)*1000);
    top.min_length=100000;
    if(top.All_Choice==NULL){
        printf("malloc error");
        exit(1);
    }
    Point p;
    p.x=1; p.y=1;
    MiGong[p.x][p.y]='x';
    bool flag=Find_Path(p,0);
    if(flag){
        int i,j;
        for(i=0;i<ROW+2;i++){
            for(j=0;j<COL+2;j++){
                printf("%c",top.min_path->MG[i][j]);
            }
            printf("\n");
        }
        printf("min length is %d",top.min_length);
        printf("\nThere is %d kinds of choices",count);
    }
    else{
        printf("no path");
    }
    return 0;
}