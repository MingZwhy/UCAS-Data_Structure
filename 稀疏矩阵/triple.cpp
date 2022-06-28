//采用三元组的方法压缩矩阵
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAXSIZE 12500
#define ElemType int

typedef struct{
    int i,j;     //非零元的行列坐标
    ElemType e;  //非零元素的值
}Triple, *Ptr_To_Triple;    //三元组类型

typedef struct{
    Ptr_To_Triple data;
    int *rpos;   //矩阵每一行第一个非零元在data中的位置
    int hang;    //矩阵行数
    int lie;     //矩阵列数
    int n;       //矩阵非零元个数
}TSMatrix, *Ptr_To_TSMatrix;

Ptr_To_TSMatrix Init_TSMatrix(int **n, int hang, int lie);
Ptr_To_TSMatrix TransposeSmatrix(Ptr_To_TSMatrix M);
Ptr_To_TSMatrix MultSMatrix(Ptr_To_TSMatrix M, Ptr_To_TSMatrix N);
void Print_TSMatrix(Ptr_To_TSMatrix T);

Ptr_To_TSMatrix Init_TSMatrix(int **n, int hang, int lie)
{
    Ptr_To_TSMatrix T;
    T=(Ptr_To_TSMatrix)malloc(sizeof(TSMatrix)*1);
    if(T==NULL){
        printf("malloc error");
        exit(1);
    }
    T->hang=hang;
    T->lie=lie;
    T->data=(Ptr_To_Triple)malloc(sizeof(Triple)*(hang*lie));
    T->rpos=(int *)malloc(sizeof(int)*hang);
    if(T->data==NULL || T->rpos==NULL){
        printf("malloc error");
        exit(1);
    }
    int i,j; 
    int count=0;
    for(i=0;i<hang;i++){
        int flag=true;
        for(j=0;j<lie;j++){
            if(n[i][j]!=0){
                if(flag){
                    T->rpos[i]=count;
                    flag=false;
                }
                Triple temp;
                temp.i=i; temp.j=j;
                temp.e=n[i][j];
                T->data[count++]=temp;
            }
        }
        if(flag){
            T->rpos[i]=-1;  //-1表示这一行无非零元
        }
    }
    T->n=count;
    return T;
}

//矩阵的转置，返回指向转置后矩阵的指针
Ptr_To_TSMatrix TransposeSmatrix(Ptr_To_TSMatrix M)
{
    Ptr_To_TSMatrix T=(Ptr_To_TSMatrix)malloc(sizeof(TSMatrix)*1);
    if(T==NULL) exit(1);
    T->hang=M->lie; T->lie=M->hang;
    T->n=M->n;
    T->data=(Ptr_To_Triple)malloc(sizeof(Triple)*T->n);
    T->rpos=(int *)malloc(sizeof(int)*T->hang);
    if(T->data==NULL || T->rpos==NULL)   exit(1);

    int *Num;
    Num=(int *)malloc(sizeof(int)*M->lie);
    if(Num==NULL)   exit(1);
    int i,j;
    for(j=0;j<M->lie;j++){
        Num[j]=0;
    }
    for(i=0;i<M->n;i++){
        Num[M->data[i].j]++;
    }

    int *Cpot,*flag;
    Cpot=(int *)malloc(sizeof(int)*M->lie);
    flag=(int *)malloc(sizeof(int)*M->lie);
    if(Cpot==NULL)  exit(1);
    Cpot[0]=flag[0]=1;
    for(j=1;j<M->lie;j++){
        Cpot[j] = Cpot[j-1] + Num[j-1];
        flag[j]=1;
        T->rpos[j]=-1; //-1表示这一行无非零元，这里只是做一个初始化
    }
    for(i=0;i<M->n;i++){
        j=M->data[i].j;
        int pos=Cpot[j];
        T->data[pos].i=j;
        T->data[pos].j=M->data[i].i;
        T->data[pos].e=M->data[i].e;
        if(flag[j]){
            T->rpos[j]=i;
            flag[j]=false;
        }
        Cpot[j]++;  //应对原矩阵同一列有多个非零元的情况
    }

    return T;
}

Ptr_To_TSMatrix MultSMatrix(Ptr_To_TSMatrix M, Ptr_To_TSMatrix N)
{
    int **temp;
    temp=(int **)malloc(sizeof(int *)*M->hang);
    int i,j;
    for(i=0;i<M->hang;i++){
        temp[i]=(int *)malloc(sizeof(int)*N->lie);
        for(j=0;j<N->lie;j++){
            temp[i][j]=0;
        }
    }

    int p,q;
    for(p=0;p<M->n;p++){
        int hang=M->data[p].i;
        int lie;
        int Nhang=M->data[p].j;
        int q=N->rpos[Nhang];
        if(q!=-1){
            while(q<M->n && N->data[q].i==Nhang){
                lie=N->data[q].j;
                temp[hang][lie]+=M->data[p].e*N->data[q].e;
                q++;
            }
        }
    }
    Ptr_To_TSMatrix T=Init_TSMatrix(temp,M->hang,N->lie);
    return T;
}

void Print_TSMatrix(Ptr_To_TSMatrix T)
{
    int i;
    for(i=0;i<T->n;i++){
        printf("%d  %d  %d\n",T->data[i].i+1,T->data[i].j+1,T->data[i].e);
    }
    printf("\n");
}

int main(){
    int i;
    int **m,**n;
    m=(int **)malloc(sizeof(int *)*4);
    for(i=0;i<4;i++){
        m[i]=(int *)malloc(sizeof(int)*5);
    }
    m[0][0]=0; m[0][1]=2; m[0][2]=0; m[0][3]=0; m[0][4]=3;
    m[1][0]=0; m[1][1]=-1; m[1][2]=5; m[1][3]=0; m[1][4]=0;
    m[2][0]=4; m[2][1]=0; m[2][2]=0; m[2][3]=7; m[2][4]=6;
    m[3][0]=0; m[3][1]=0; m[3][2]=-3; m[3][3]=0; m[3][4]=0;

    n=(int **)malloc(sizeof(int *)*5);
    for(i=0;i<5;i++){
        n[i]=(int *)malloc(sizeof(int)*2);
    }
    n[0][0]=0; n[0][1]=3;
    n[1][0]=2; n[1][1]=4;
    n[2][0]=1; n[2][1]=0;
    n[3][0]=0; n[3][1]=0;
    n[4][0]=0; n[4][1]=-2;

    Ptr_To_TSMatrix M=Init_TSMatrix(m,4,5);
    printf("The Matrix M is: \n");
    Print_TSMatrix(M);
    Ptr_To_TSMatrix N=Init_TSMatrix(n,5,2);
    printf("The Matrix N is: \n");
    Print_TSMatrix(N);
    Ptr_To_TSMatrix T=MultSMatrix(M,N);
    printf("The Matrix T is: \n");
    Print_TSMatrix(T);
    return 0; 
}
