#include<iostream>

using namespace std;

typedef struct ListNode{
    int data;
    struct ListNode * next;
}ListNode, *List;

ListNode L;

void Merge(int * SR, int * TR, int low, int mid, int high)
{
    //left: low---mid
    //right: mid+1---high
    int j,k;
    for(j=mid+1,k=low;low<=mid&&j<=high;k++){
        //将SR中记录由小到大地并入TR
        if(SR[low]<=SR[j]){
            TR[k] = SR[low];
            low++;
        }
        else{
            TR[k] = SR[j];
            j++;
        }
    }
    if(low<=mid){
        for(;k<=high;k++,low++){
            TR[k] = SR[low];
        }
    }
    else{
        for(;k<=high;k++,j++){
            TR[k] = SR[j];
        }
    }
}

void Msort(int * s, int * r, int low, int high)
{
    int * r_temp = new int[high-low+1];
    if(low==high){
        r[low]=s[low];
    }
    else{
        int mid = (low+high)/2;
        Msort(s,r_temp,low,mid);
        Msort(s,r_temp,mid+1,high);
        Merge(r_temp,r,low,mid,high);
    }
}

int main(){
    int num;
    cin >> num;
    if(num<=0){
        return 0;
    }
    int * s = new int[num+1];
    int * r = new int[num+1];
    int n=1;
    char c = getchar();
    do{
        cin >> s[n++];
        c = getchar();
    }while(c!='\n');
    Msort(s,r,1,num);
    for(int i=1;i<num;i++){
        cout << r[i] << " ";
    }
    cout << r[num];
    delete s;
    delete r;
    return 0;
}