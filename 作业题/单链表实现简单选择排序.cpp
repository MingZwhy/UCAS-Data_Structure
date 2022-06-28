#include<iostream>

using namespace std;

typedef struct ListNode{
    int data;
    struct ListNode * next;
}ListNode, *List;

typedef List head;
typedef List node;

void Insert(head L, int x)
{
    node visit = L;
    while(visit->next!=nullptr && visit->next->data < x){
        visit = visit->next;
    }
    node temp = new ListNode;
    temp->data = x;
    temp->next = visit->next;
    visit->next = temp;
}

void VisitList(head L)
{
    node visit = L->next;
    while(visit!=nullptr){
        cout << visit->data << " ";
        visit = visit->next;
    }
}

int main(){
    head L = new ListNode;
    L->next=nullptr;
    int num;
    char c;
    int elem;
    do{
        cin >> elem;
        Insert(L,elem);
        c=cin.get();
    }while(c!='\n');
    VisitList(L);
    return 0;
}