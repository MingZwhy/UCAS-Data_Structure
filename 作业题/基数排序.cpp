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
    head h[10];
    List result;
}Num_List, *CList;

void Init_Clist(CList cl)
{
    for(int i=0;i<10;i++){
        cl->h[i] = new ListNode;
        cl->h[i]->next=nullptr;
    }
    cl->result = new ListNode;
    cl->result->next=nullptr;
}

void Insert(CList cl, int pos, int elem){
    node temp = new ListNode;
    temp->data=elem;
    head visit = cl->h[pos];
    //插入到链表末端
    while(visit->next!=nullptr){
        visit=visit->next;
    }
    temp->next=visit->next;
    visit->next=temp;
}

void Reset(CList cl)
{
    for(int i=0;i<10;i++){
        cl->h[i]->next=nullptr;
    }
}

CList Basic_Num_Sort(int * s, int n)
{
    int wei=0;
    int h=s[1];
    while(h>0){
        h = h / 10;
        wei++;
    }
    CList cl = new Num_List;
    Init_Clist(cl);
    for(int i=1;i<=n;i++){
        int pos = s[i] % 10;
        Insert(cl,pos,s[i]);
    }
    List visit;
    List write = cl->result;
    for(int i=0;i<10;i++){
        visit = cl->h[i]->next;
        while(visit!=nullptr){
            List newnode = new ListNode;
            newnode->data=visit->data;
            newnode->next=write->next;
            write->next=newnode;
            write=newnode;
            visit=visit->next;
        }
    }

    int time = 1;
    while(time<wei){
        Reset(cl);
        visit = cl->result->next;
        while(visit!=nullptr){
            int pos; int i=0;
            int num = visit->data;
            while(i<time){
                num = num / 10;
                i++;
            }
            pos = num % 10;
            Insert(cl,pos,visit->data);
            visit=visit->next;
        }
        cl->result->next=nullptr;
        write = cl->result;
        for(int i=0;i<10;i++){
            visit = cl->h[i]->next;
            while(visit!=nullptr){
                List newnode = new ListNode;
                newnode->data=visit->data;
                newnode->next=write->next;
                write->next=newnode;
                write=newnode;
                visit=visit->next;
            }
        }
        time++;
    }
    return cl;
}

int main(){
    int s[1000];
    int n=1;
    char c;
    do{
        cin >> s[n++];
        c = getchar();
    }while(c!='\n');
    CList cl = Basic_Num_Sort(s,n-1);
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
    return 0;
}