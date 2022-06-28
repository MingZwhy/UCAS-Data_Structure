#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define bool int
#define true 1
#define false 0

typedef struct{
    int x;
    int y;
}Point, *Ptr_To_Point;

typedef struct{
    Point path[10000];
    int length;
}Path_Record, *Ptr_To_Path_Record;

typedef struct{
    Ptr_To_Path_Record All_Choice;
    int min_length;              //最短路径长度
    Ptr_To_Path_Record min_path; //指向最短路径
}All_Path;

typedef struct{
    Point path_temp[10000];
    bool IF_KEY;
}Temp_Record;

int count=0;   //path数量

void Init_MiGong(void);
void Find_Path(Point p, int index, char **MiGong, All_Path *top, int ROW, int COL, Temp_Record *temp, Point OUT, Point KEY, bool *tag);

void Find_Path(Point p, int index, char **MiGong, All_Path *top, int ROW, int COL, Temp_Record *temp, Point OUT, Point KEY, bool *tag)
{
    if(p.x==KEY.x && p.y==KEY.y){
        temp->IF_KEY=true;
        *tag=true;
    } 
#if 0
    int u,v;
    for(u=0;u<ROW+2;u++){
        for(v=0;v<COL;v++){
            printf("%c",MiGong[u][v]);
        }
        printf("\n");
    }
    printf("\n\n");
#endif
    if(p.x==OUT.x && p.y==OUT.y && temp->IF_KEY==true){
        //到达终点
        //记录入record
        top->All_Choice[count].length=index+1;
        int i,j;
        for(i=0;i<=index;i++){
            top->All_Choice[count].path[i].x=temp->path_temp[i].x;
            top->All_Choice[count].path[i].y=temp->path_temp[i].y;
        }
        if(index<top->min_length){
            top->min_length = index;
            top->min_path = &top->All_Choice[count];
        }
        count++;
        return;
    }
    int i;
    for(i=1;i<=4;i++){
        if(i==1){   //向右
            if(MiGong[p.x][p.y+1]=='0'){
                p.y+=1;
                MiGong[p.x][p.y]='x';
                temp->path_temp[index].x=p.x;
                temp->path_temp[index].y=p.y;
                Find_Path(p,index+1,MiGong,top,ROW,COL,temp,OUT,KEY,tag);
                MiGong[p.x][p.y]='0';
                if(*tag==true){
                    temp->IF_KEY=false;    //因为捡到钥匙的路径被撤销了，所以标记也撤销
                    *tag=false;
                }
                p.y-=1;
            }
        }
        else if(i==2){    //向上
            if(MiGong[p.x-1][p.y]=='0'){
                p.x-=1;
                MiGong[p.x][p.y]='x';
                temp->path_temp[index].x=p.x;
                temp->path_temp[index].y=p.y;
                Find_Path(p,index+1,MiGong,top,ROW,COL,temp,OUT,KEY,tag);
                MiGong[p.x][p.y]='0';
                if(*tag==true){
                    temp->IF_KEY=false;    //因为捡到钥匙的路径被撤销了，所以标记也撤销
                    *tag=false;
                }
                p.x+=1;
            }
        }
        else if(i==3){   //向左
            if(MiGong[p.x][p.y-1]=='0'){
                p.y-=1;
                MiGong[p.x][p.y]='x';
                temp->path_temp[index].x=p.x;
                temp->path_temp[index].y=p.y;
                Find_Path(p,index+1,MiGong,top,ROW,COL,temp,OUT,KEY,tag);
                MiGong[p.x][p.y]='0';
                if(*tag==true){
                    temp->IF_KEY=false;    //因为捡到钥匙的路径被撤销了，所以标记也撤销
                    *tag=false;
                }
                p.y+=1;
            }
        }
        else if(i==4){    //向下
            if(MiGong[p.x+1][p.y]=='0'){
                p.x+=1;
                MiGong[p.x][p.y]='x';
                temp->path_temp[index].x=p.x;
                temp->path_temp[index].y=p.y;
                Find_Path(p,index+1,MiGong,top,ROW,COL,temp,OUT,KEY,tag);
                MiGong[p.x][p.y]='0';
                if(*tag==true){
                    temp->IF_KEY=false;    //因为捡到钥匙的路径被撤销了，所以标记也撤销
                    *tag=false;
                }
                p.x-=1;
            }
        }
    }
}

int main()
{
    int ROW,COL;
    scanf("%d %d\n",&ROW,&COL);
    char **MiGong;
    MiGong=(char **)malloc(sizeof(char *)*(ROW+2));
    int i,j;
    MiGong[0]=(char *)malloc(sizeof(char)*COL);
    MiGong[ROW+1]=(char *)malloc(sizeof(char)*COL);
    for(i=0;i<COL;i++){
        MiGong[0][i]='1';
        MiGong[ROW+1][i]='1';
    }
    char c;
    Point IN,OUT,KEY;
    for(i=1;i<=ROW;i++){
        MiGong[i]=(char *)malloc(sizeof(char)*COL);
        for(j=0;j<COL;j++){
            c=getchar();
            if(c=='R'){
                IN.x=i; IN.y=j;
            }
            if(c=='C'){
                OUT.x=i; OUT.y=j;
                c='0';
            }
            if(c=='Y'){
                KEY.x=i; KEY.y=j;
                c='0';
            }
            MiGong[i][j]=c;
        }
        c=getchar();  //\n
    }
    All_Path top;
    top.All_Choice=(Ptr_To_Path_Record)malloc(sizeof(Path_Record)*1000);
    top.min_length=100000;
    if(top.All_Choice==NULL){
        printf("malloc error");
        exit(1);
    }
    Point p;
    p=IN;
    MiGong[p.x][p.y]='x';
    Temp_Record temp;
    temp.IF_KEY=false;
    temp.path_temp[0].x=IN.x;
    temp.path_temp[0].y=IN.y;
    bool tag=false;
    Find_Path(p,1,MiGong,&top,ROW,COL,&temp,OUT,KEY,&tag);
    for(i=0;i<top.min_length;i++){
        printf("%d %d\n",top.min_path->path[i].x,(top.min_path->path[i].y+1));
    }
    return 0;
}