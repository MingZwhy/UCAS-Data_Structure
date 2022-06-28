#include<iostream>

using namespace std;

typedef struct ListNode{
    int data;
    struct ListNode * next;
}ListNode, *List;

typedef List head;
typedef List tail;
typedef List node;

typedef struct Num_List{
    int wei;     //数的位数
    head h[10];
    tail t[10];
    List result;
}Num_List, *CList;

void Init_Clist(CList cl, int wei)
{
    cl->wei = wei;
    for(int i=0;i<10;i++){
        cl->h[i] = new ListNode;
        cl->h[i]->next = nullptr;
        cl->t[i] = cl->h[i];
    }
    cl->result = new ListNode;
    cl->result->next = nullptr;
}

void Insert(CList cl, int pos, int elem)
{
    node temp = new ListNode;
    temp->data = elem;
    head visit = cl->h[pos];
    //插入到链表末端
    while(visit->next!=nullptr){
        visit=visit->next;
    }  
    temp->next=visit->next;
    visit->next=temp;
    //修改尾指针，使其指向刚插入点
    cl->t[pos] = temp;
}

void Reset_Result(CList cl)
{
    for(int i=0;i<10;i++){
        cl->h[i]->next = nullptr;
        cl->t[i] = cl->h[i];
    }
}

void First_Cycle(CList &cl, int *s, int n)
{
    for(int i=1;i<=n;i++){
        int pos = s[i] % 10;
        Insert(cl,pos,s[i]);
    }
    List visit = cl->result;
    //将其尾首连接起来
    for(int i=0;i<10;i++){
        if(cl->h[i]->next!=nullptr){
            visit->next = cl->h[i]->next;
            visit = cl->t[i];
        }
    }
}

void Next_Cycle(CList &cl, int *s, int time)
{
    List visit = cl->result->next;
    while(visit!=nullptr){
        int pos; int i=0;
        int temp_num = visit->data;
        while(i<time){
            temp_num = temp_num / 10;
            i++;
        }
        pos = temp_num % 10;
        Insert(cl,pos,visit->data);
        visit = visit->next;
    }
    visit = cl->result;
    //将其尾首连接起来
    for(int i=0;i<10;i++){
        if(cl->h[i]->next!=nullptr){
            visit->next = cl->h[i]->next;
            visit = cl->t[i];
        }
    }
}

CList Basic_Num_Sort(int * s, int n)
{
    //先计算出关键字位数
    int wei = 0;
    int temp_num = s[1];
    while(temp_num>0){
        temp_num = temp_num / 10;
        wei++;
    }

    CList cl = new Num_List;
    Init_Clist(cl,wei);

    First_Cycle(cl,s,n);

    int time = 1;   //记录/10次数
    while(time<cl->wei){
        Reset_Result(cl);
        Next_Cycle(cl,s,time);
        time++;
    }
    return cl;
}

void Print_CList(CList cl)
{
    List visit = cl->result->next;
    while(visit!=nullptr){
        if(visit->next!=nullptr){
            cout << visit->data << " ";
        }
        else{
            cout << visit->data;
        }
        visit = visit->next;
    }
}

int main(){
    int s[1000];
    int n=1;
    char c;
    cout << "input the key list" << endl;
    do{
        cin >> s[n++];
        c = getchar();
    }while(c!='\n');
    CList cl = Basic_Num_Sort(s,n-1);
    Print_CList(cl);
    return 0;
}