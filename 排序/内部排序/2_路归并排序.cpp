#include<iostream>

using namespace std;

//归并排序
//其中SR[low...mid] 和 SR[mid+1...high]都是排序好的
//SR[low...high]整体排序好的结果送入TR
void Merge(int * SR, int * TR, int low, int mid, int high)
{
    int right,left,index;
    right = mid + 1;
    left = low;
    index = low;
    for(;left<=mid && right<=high;index++)
    {
        //一对一对比较，哪边小
        if(SR[left]<=SR[right]){
            TR[index] = SR[left];
            left++;
        }
        else{
            TR[index] = SR[right];
            right++;
        }
    }
    if(left<=mid){
        for(;index<=high;index++,left++){
            TR[index] = SR[left];
        }
    }
    else{
        for(;index<=high;index++,right++){
            TR[index] = SR[right];
        }
    }
}

//递归的2路归并排序，输入为s[low...high]，排序完毕结果输出进r
void Msort(int * s, int * r, int low, int high)
{
    int * r_temp = new int[high-low+1];
    if(low==high){
        r[low] = s[low];
    }
    else{
        int mid = (low+high)/2;
        Msort(s,r_temp,low,mid);    //左边排序
        Msort(s,r_temp,mid+1,high); //右边排序
        Merge(r_temp,r,low,mid,high);  //归并排序，使整体有序
    }
}

int main(){
    int s[10]={23,1,4,5,2,443,54,24,55,12};
    for(int i=0;i<10;i++){
        cout << s[i] << " ";
    }
    cout << endl;
    int t[10];
    Msort(s,t,0,9);
    for(int i=0;i<10;i++){
        cout << t[i] << " ";
    }
    return 0;
}