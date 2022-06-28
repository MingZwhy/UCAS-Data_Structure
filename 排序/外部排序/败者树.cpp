//利用败者树完成k路平衡归并排序算法
#include<iostream>
#include<stdlib.h>
using namespace std;

const int MaxValue = 10000;
const int MAX_WAY = 10;

typedef struct Element{
    int index;
    int data[100];
    int size;
}Element;

typedef struct ELem_Group{
    Element son[MAX_WAY];
    int k;   //实际归并路数
    int All_Size;  //k个归并段的总元素个数
    int Cur_Size;
    int index_k;   //0<=index_k<k，决定下一个元素从哪路取
    bool IF_ALL_IN;  //=1时代表所有元素已送入叶子
}ELem_Group, *EG;

void adjust(int * key, int * loser, int k, int q);
void InitRecord(Element * s, int * data, int len);
void Init_EG(EG eg, int k, int **data, int * size);
int  InputRecord(EG eg);
void OutputRecord(Element * r, int data);
Element * kwaymerge(EG eg, int k);

void InitRecord(Element * s, int * data, int len)
{
    for(int i=0; i<len; i++){
        s->data[i] = data[i];
    }
    s->index=0;
    s->size=len;
}

void Init_EG(EG eg, int k, int **data, int * size)
{
    eg->k = k;
    for(int i=0;i<k;i++){
        InitRecord(&eg->son[i],data[i],size[i]);
    }
    int i=0;
    for(eg->All_Size=0;i<k;i++){
        eg->All_Size+=size[i];
    }
    eg->Cur_Size=0;
    eg->index_k=0;
    eg->IF_ALL_IN=false;
}

int  InputRecord(EG eg)
{
    int temp;
    if(eg->IF_ALL_IN){
        return MaxValue;
    }
    else{
        while(1){
            if(eg->son[eg->index_k].index==eg->son[eg->index_k].size){
                if(eg->index_k==eg->k-1){
                    eg->index_k=0;
                }
                else{
                    eg->index_k++;
                }
            }
            else{
                break;
            }
        }
        temp = eg->son[eg->index_k].data[eg->son[eg->index_k].index++];
        eg->Cur_Size++;
        if(eg->index_k==eg->k-1){
            eg->index_k=0;
        }
        else{
            eg->index_k++;
        }
    }
    if(eg->Cur_Size==eg->All_Size){
        eg->IF_ALL_IN=true;
    }
    return temp;
}

void OutputRecord(Element * r, int data)
{
    r->data[r->index++] = data;
}

Element * kwaymerge(EG eg, int k)
{
    //k为归并数， len为待排序序列s的长度
    int i,q;

    Element * r = new Element; //输出归并段
    r->index=0; r->size=eg->All_Size;

    int * key = new int[k+1];  //记录的排序码   key[k]存无限小值
    int * loser = new int[k];  //存放败者树

    //先将数组读入记录的排序码
    for(i=0;i<k;i++){
        key[i]=InputRecord(eg);
    }
    key[k] = -MaxValue;

    //loser存放败者树，初始时全部初始化为k(组号k对应key中无限小元素)
    //初始填充元素进叶子时，每填充一个叶子最多打通一个loser
    //这样保证了在初始填满叶子前不可能得出总冠军(打通loser[0])
    for(i=0;i<k;i++){
        loser[i] = k;
    }

    //从key[k-1]到key[0]调整成败者树
    for(i=k-1;i>=0;i--){
        adjust(key,loser,k,i);
    }

    //依次输出最小值(总冠军)
    //每输出一个最小值后在原最小值在key中位置读入
    //一个新的记录，记录的末位标志是MaxValue，也就
    //是说最终会读入MaxValue
    while(r->index!=r->size){
        //当前最小记录的下表
        q = loser[0];   
        //将最小记录输出到r
        OutputRecord(r,key[q]);
        cout << "output " << key[q] << endl;
        //从s读入下一个记录
        key[q] = InputRecord(eg);
        //从key[q]其调整
        adjust(key,loser,k,q);
    }
    delete []key;
    delete []loser;
    return r;
}

void adjust(int * key, int * loser, int k, int q)
{
    int num=key[q];
    //从败者树某叶节点key[q]起到根进行比较，将最小key记录所在归并段的段号计入loser[0]
    int t = (k+q) / 2;  //t是q的双亲(q一定是叶子节点)
    //注意整颗败者树是由上层的loser和叶子层的key合成的，他们的下标分开算(没法直接q/2)
    //loser的顺序是确定的(按正常完全二叉树的顺序)
    //当t = (k+q) / 2确定后，叶子层key的排列顺序随之确定
    for(;t>0;t=t/2){
        //进入loser部分正常完全二叉树(非叶子层)后，父节点可以直接通过t=t/2计算
        int dads_group_index = loser[t];
        if(key[dads_group_index]<key[q]){
            //败者(大值)记入loser[t]父节点，胜者(小值)记入q
            loser[t] = q;
            q = dads_group_index;
        }
        loser[0] = q;
        //败者树能方便的根本原因是我们不关心路径上的每一个胜者
        //究竟是怎样的，我们只要求两点：
        //1-->能得到最终的胜者(最小值)
        //2-->路径上保存的信息保证新加入元素时能顺着路径向上更新新的总胜者
    }
}

int main(){
    EG eg = new ELem_Group;
    int raw[6][3]={{1,21,43},{4,14,45},{2,70,81},{3,24,86},{8,17,87},{30,46,64}};
    int ** data;
    data = (int **)malloc(sizeof(int *)*6);
    for(int i=0;i<6;i++){
        data[i]=(int *)malloc(sizeof(int)*3);
        for(int j=0;j<3;j++){
            data[i][j]=raw[i][j];
        }
    }
    int size[6]={3,3,3,3,3,3};
    Init_EG(eg,6,data,size);
    Element * result = kwaymerge(eg,6);
    for(int i=0;i<result->index;i++){
        cout << result->data[i] << " ";
    }
    free(data);
    return 0;
}