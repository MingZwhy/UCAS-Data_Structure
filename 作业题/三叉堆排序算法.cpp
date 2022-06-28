#include<iostream>
using namespace std;
#define LeftChild(i) (3*(i)+1)
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
        if(child!=N-1){
            //找到空穴较大的儿子，child!=N-1是考虑了空穴只有一个儿子的情况
            if(A[child+1]>A[child]){
				child++;
				if(child!=N-1){
					if(A[child+1]>A[child]){
						child++;
					}
				}
			}
			else{
				if(child!=N-2){
					if(A[child+2]>A[child]){
						child = child + 2;
					}
				}
			}
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
    for(i=N/3;i>=0;i--){
        //从最右边的父亲开始，下滤
        PercDown(A,i,N);
    }
    //依次取堆根也就是最大值
    for(i=N-1;i>0;i--){
        Swap(&A[0],&A[i]);  //实质上与deletMax的道理相同
        PercDown(A,0,i);    //0——从根开始下滤   i——新的队的大小
    }
}

int main(){
    ElementType s[1000];
    char c;
	int index=0;
	do{
		cin >> s[index++];
		c = getchar();
	}while(c!='\n');
    Heapsort(s,index);
    for(int i=0;i<index;i++){
        cout << s[i] << " ";
    }
    return 0;
}