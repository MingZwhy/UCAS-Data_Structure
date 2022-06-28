//用十字链表法存储稀疏矩阵
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define ElemType int

typedef struct node{
    int i,j;
    ElemType e;
    struct node *right, *down;
}OLNode, *Olink;
typedef Olink Position;

typedef struct{
    Olink *rhead, *chead;
    int hang,lie,n;  //行数，列数，非零元个数
}CrossList, *Ptr_To_CrossList;

Ptr_To_CrossList CreatSMatrix(int **n, int hang, int lie);
void AddSMatrix(Ptr_To_CrossList A, Ptr_To_CrossList B);
void Print_CrossList(Ptr_To_CrossList L);

Ptr_To_CrossList CreatSMatrix(int **n, int hang, int lie)
{
    Ptr_To_CrossList L;
    L=(Ptr_To_CrossList)malloc(sizeof(CrossList)*1);
    if(L==NULL) exit(1);
    L->hang=hang;
    L->lie=lie;
    L->rhead=(Olink *)malloc(sizeof(Olink)*hang);
    L->chead=(Olink *)malloc(sizeof(Olink)*lie);
    if(L->rhead==NULL || L->chead==NULL){
        printf("malloc error");
        exit(1);
    }
    int i,j;
    for(i=0;i<hang;i++){
        L->rhead[i]=(Olink)malloc(sizeof(OLNode)*1);
        if(L->rhead[i]==NULL)   exit(1);
        L->rhead[i]->right=NULL;
    }
    for(i=0;i<lie;i++){
        L->chead[i]=(Olink)malloc(sizeof(OLNode)*1);
        if(L->chead[i]==NULL)   exit(1);
        L->chead[i]->down=NULL;
    }
    Position visit_r;
    Position *visit_c=(Position *)malloc(sizeof(Position)*lie);
    for(i=0;i<lie;i++){
        visit_c[i]=L->chead[i];
    }

    int count=0;    //记录非零节点个数
    for(i=0;i<hang;i++){
        visit_r=L->rhead[i];
        for(j=0;j<lie;j++){
            if(n[i][j]!=0){
                count++;
                Position temp=(Position)malloc(sizeof(OLNode)*1);
                if(temp==NULL)  exit(1);
                temp->e=n[i][j];
                temp->i=i; temp->j=j;

                temp->right=visit_r->right;
                visit_r->right=temp;
                visit_r=temp;
                
                temp->down=visit_c[j]->down;
                visit_c[j]->down=temp;
                visit_c[j]=temp;
            }
        }
    }
    L->n=count;
    return L;
}

void AddSMatrix(Ptr_To_CrossList A, Ptr_To_CrossList B)
{
    if(A->hang!=B->hang || A->lie!=B->lie){
        printf("illegal add");
        return;
    }
    int hang=A->hang;
    int lie=A->lie;
    int i;
    Position visit_a_r,visit_b_r;
    Position *visit_c=(Position *)malloc(sizeof(Position)*lie);
    for(i=0;i<lie;i++){
        visit_c[i]=A->chead[i];
    }
    for(i=0;i<hang;i++){
        //移动列指针包括对节点做了改动包括插入删除和改变值以及仅仅是遍历到了该节点，改变值或仅仅遍历时需要将列指针移到改变值的节点上
        visit_a_r=A->rhead[i];
        visit_b_r=B->rhead[i]->right;
        while(visit_b_r!=NULL){
            if(visit_a_r->right==NULL || visit_a_r->right->j > visit_b_r->j){
                Position temp=(Position)malloc(sizeof(OLNode)*1);
                if(temp==NULL)  exit(1);
                temp->i=i;
                temp->j=visit_b_r->j;
                temp->e=visit_b_r->e;
                temp->right=visit_a_r->right;
                visit_a_r->right=temp;
                visit_a_r=temp;

                temp->down=visit_c[temp->j]->down;
                visit_c[temp->j]->down=temp;
                visit_c[temp->j]=temp;
            }
            else if(visit_a_r->right->j < visit_b_r->j){
                visit_a_r=visit_a_r->right;
                visit_c[visit_a_r->j]=visit_a_r;
                continue;    //只右移a指针，b指针不动;下移c指针
            }
            else if(visit_a_r->right->j == visit_b_r->j){
                if(visit_a_r->right->e+visit_b_r->e==0){
                    Position temp=visit_a_r->right;
                    visit_a_r->right=temp->right;
                    visit_c[temp->j]->down=temp->down;
                    free(temp);
                }
                else{
                    visit_a_r->right->e+=visit_b_r->e;
                    visit_a_r=visit_a_r->right;
                    visit_c[visit_a_r->j]=visit_a_r;
                }
            }
            else{
                ;
            }
            visit_b_r=visit_b_r->right;
        }
        //须确保对每一行当b已经遍历完而a还未到尽头时，未遍历的a的节点对应的列的c指针指向该节点
        while(visit_a_r!=NULL){
            visit_c[visit_a_r->j]=visit_a_r;
            visit_a_r=visit_a_r->right;
        }
    }
}

void Print_CrossList(Ptr_To_CrossList L)
{
    Position visit;
    int hang=L->hang;
    int lie=L->lie;
    int i;
    printf("row visit result is:\n");
    for(i=0;i<hang;i++){
        visit=L->rhead[i]->right;
        while(visit!=NULL){
            printf("%d  ",visit->e);
            visit=visit->right;
        }
        printf("\n");
    }
    printf("col visit result is:\n");
    for(i=0;i<lie;i++){
        visit=L->chead[i]->down;
        while(visit!=NULL){
            printf("%d  ",visit->e);
            visit=visit->down;
        }
        printf("\n");
    }
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

    n=(int **)malloc(sizeof(int *)*4);
    for(i=0;i<4;i++){
        n[i]=(int *)malloc(sizeof(int)*5);
    }
    n[0][0]=0; n[0][1]=-2; n[0][2]=3; n[0][3]=1; n[0][4]=0;
    n[1][0]=1; n[1][1]=0; n[1][2]=0; n[1][3]=0; n[1][4]=2;
    n[2][0]=-4; n[2][1]=0; n[2][2]=1; n[2][3]=0; n[2][4]=0;
    n[3][0]=0; n[3][1]=2; n[3][2]=2; n[3][3]=0; n[3][4]=0;

    Ptr_To_CrossList M=CreatSMatrix(m,4,5);
    Print_CrossList(M);
    Ptr_To_CrossList N=CreatSMatrix(n,4,5);
    Print_CrossList(N);
    AddSMatrix(M,N);
    Print_CrossList(M);
    return 0;
}