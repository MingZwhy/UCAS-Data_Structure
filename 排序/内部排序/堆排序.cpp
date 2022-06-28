//参考数据结构与算法分析 p181 堆排序
//这里因为只用到下滤而没有用到上滤，因此不用0位置存储最大最小值，与普通二叉堆略有不同
//这里用了大堆，得到递增序列；如果用小堆，很容易得到递减序列
#include<iostream>
using namespace std;
#define LeftChild(i) (2*(i)+1)
typedef int ElementType;

void Swap(int * a, int * b);
void PercDown(ElementType A[], int i, int N);
void Heapsort(ElementType A[], int N);

void Swap(int * a, int * b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void PercDown(ElementType A[], int i, int N)
{
    int child;
    ElementType Tmp;

    for(Tmp=A[i]; LeftChild(i) < N; i=child)
    {
        child = LeftChild(i);
        if(child!=N-1 && A[child+1] > A[child]){
            //找到空穴较大的儿子，child!=N-1是考虑了空穴只有一个儿子的情况
            child++;
        }
        if(Tmp<A[child]){
            A[i]=A[child];
        }
        else{
            //说明空穴适合填充，退出循环
            break;
        }
    }
    A[i] = Tmp;
}

void Heapsort(ElementType A[], int N)
{
    int i;
    //构建大堆
    for(i=N/2;i>=0;i--){
        //从最右边的父亲开始，下滤
        PercDown(A,i,N);
    }
    for(int i=0;i<10;i++){
        cout << A[i] << " ";
    }
    cout << endl;
    //依次取堆根也就是最大值
    for(i=N-1;i>0;i--){
        Swap(&A[0],&A[i]);  //实质上与deletMax的道理相同
        PercDown(A,0,i);    //0——从根开始下滤   i——新的队的大小
    }
}

int main(){
    ElementType s[10]={503,87,512,61,908,170,897,275,653,426};
    for(int i=0;i<10;i++){
        cout << s[i] << " ";
    }
    cout << endl;
    Heapsort(s,10);
    for(int i=0;i<10;i++){
        cout << s[i] << " ";
    }
    return 0;
}