#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct PolyNode{
  int xishu;
  int cishu; 
  struct PolyNode *next;
} PolyNode, *PolyLink;

typedef PolyLink LinkedPoly;
typedef PolyLink Position;

int Trans(char *s, int begin, int end);
void Print_list(LinkedPoly L);
LinkedPoly Create_Link(char *s);
int mygetline(char *s ,int lim);

int Trans(char *s, int begin, int end)
{
    int sum=0;
    int i=begin;
    while(i<=end){
        sum=sum*10+(s[i]-'0');
        i++;
    }
    return sum;
}

LinkedPoly Create_Link(char *s)
{
    LinkedPoly L=(LinkedPoly)malloc(sizeof(PolyNode)*1);
    L->next=NULL;
    Position visit=L;
    if(L==NULL){
        printf("malloc error");
        exit(1);
    }
    int len=strlen(s);
    int index=0;
    while(index<len && s[index]!='\0')
    {
        if(s[index]==' '){
            index++;
        }
        int xishu;
        int cishu;
        int begin;
        int end;
        int flag;
        if(s[index]=='+' || s[index]=='-' || (index==0 && s[index]>='0' && s[index]<='9')){
            if(index==0 && s[index]>='0' && s[index]<='9'){
                flag=1;
            }
            else{
                flag=(s[index]=='+')? 1 : -1;
                index+=2;
            }
            begin=index;
            while(s[index]>='0' && s[index]<='9'){
                index++;
            }
            end=index-1;
            xishu=flag*Trans(s,begin,end);
            
            if(s[index]=='x'){
                if(s[index+1]=='^'){
                    index+=2;
                    begin=index;
                    while(s[index]>='0' && s[index]<='9'){
                        index++;
                    }
                    end=index-1;
                    cishu=Trans(s,begin,end);
                }
                else{  //s[index+1]==' ' || s[index+1]=='\0'
                    index+=1;
                    cishu=1;
                }
            }
            else{           //s[index]==' ' || s[index]=='\0'
                cishu=0;
            }
        }

        //build the node
        Position node=(LinkedPoly)malloc(sizeof(PolyNode)*1);
        if(node==NULL){
            printf("malloc error");
            exit(1);
        }
        node->xishu=xishu;
        node->cishu=cishu;
        node->next=NULL;
        visit=L; //重置至头结点
        while(visit->next!=NULL && cishu<visit->next->cishu){
            visit=visit->next;
        }
        if(visit->next!=NULL && cishu==visit->next->cishu){
            visit->next->xishu+=xishu;
            if(visit->next->xishu==0){
                Position temp=visit->next;
                visit->next=temp->next;
                free(temp);
            }
        }
        else{
            node->next=visit->next;
            visit->next=node;
        }
    }
    visit=L;
    while(visit->next!=NULL){
        visit=visit->next;
    }
    visit->next=L;  //循环链表
    return L;
}

void Qiudao(LinkedPoly L)
{
    Position visit=L;
    while(visit->next!=L){
        Position temp=visit->next;
        if(temp->cishu==0){
            visit->next=temp->next;
            free(temp);
        }
        else{
            temp->xishu=temp->xishu*temp->cishu;
            temp->cishu--;
            visit=visit->next;
        }
    }
}

void Print_list(LinkedPoly L){
	Position visit=L->next;
  	if(visit==L)
      	printf("0");
	while(visit!=L){
        if(visit==L->next){
            if(visit->xishu<0){
                printf("- ");
            }
        }
        else{
            if(visit->xishu<0){
                printf(" - ");
            }
            else{
                printf(" + ");
            }
        }
        if(visit->xishu<0){
            printf("%d",-visit->xishu);
        }
        else{
            printf("%d",visit->xishu);
        }
        if(visit->cishu==0){
            ;
        }
        else if(visit->cishu==1){
            printf("x");
        }
        else{
            printf("x^%d",visit->cishu);
        }
		visit=visit->next;
	}
}

int mygetline(char *s ,int lim)
{
    int i,c;
    i=0;
    while (--lim>0 &&(c=getchar())!=EOF&&c!='\n')
        s[i++]=c;
    s[i]='\0';
    return i;
}

int main(){
    char *s=(char *)malloc(sizeof(char)*10000);
    int len=mygetline(s,10000);
    LinkedPoly L=Create_Link(s);
    Print_list(L);
    printf("\n");
    Qiudao(L);
    Print_list(L);
    return 0;
}