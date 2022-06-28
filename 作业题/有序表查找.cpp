#include<iostream>
using namespace std;

typedef struct ListNode{
    int data;
    struct ListNode * next;
}ListNode, *List;

typedef struct OrderList{
    List h;
    List t;
}OrderList, *Olist;

void Insert(Olist L, int x)
{
    if(L->h==nullptr){
        L->h = new ListNode;
        L->h->data = x;
        L->h->next=L->h;
        L->t = L->h;
        return;
    }
    List visit = L->h;
    if(x<=visit->data){
        while(visit->next!=L->h){
            visit = visit->next;
        }
        List temp = new ListNode;
        temp->data = x;
        temp->next = L->h;
        visit->next = temp;
        L->h = temp; 
    }
    else{
        while(visit->next!=L->h && x>visit->next->data){
            visit=visit->next;
        }
        List temp = new ListNode;
        temp->data = x;
        temp->next = visit->next;
        visit->next = temp;
    }
}

bool Search(Olist L, int x)
{
    if(L->t->data==x){
        return true;
    }
    List visit;
    if(x<L->t->data){
        visit = L->h;
        if(visit->data==x){
            L->t=visit;
            return true;
        }
        while(visit->next!=L->t){
            visit=visit->next;
            if(visit->data==x){
                L->t=visit;
                return true;
            }
        }
    }
    else{
        visit=L->t;
        while(visit->next!=L->h){
            visit=visit->next;
            if(visit->data==x){
                L->t=visit;
                return true;
            }
        }
    }
    return false;
}

int main(){
    Olist L = new OrderList;
    L->h = L->t = nullptr;
    int elem;
    char c;
    do{
        cin >> elem;
        Insert(L,elem);
        c = cin.get();
    }while(c!='\n');
    do{
        cin >> elem;
        if(Search(L,elem)){
            cout << "1" << " ";
        }
        else{
            cout << "0" << " ";
        }
        c = cin.get();
    }while(c!='\n');
    delete L;
    return 0;
}