#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char AtomType;
typedef enum{ATOM,LIST} ElemTag;  //ATOM=0,LIST=1

typedef struct GLNode
{
    ElemTag tag;   //公共部分，用于区分
    union 
    {
        AtomType atom;
        struct 
        {
            struct GLNode *hp, *tp;
        }ptr;    
    };    
}GLnode,*Glist;

//求广义表的长度
int GListLength(Glist L)
{
    if(!L){
        return 0;
    }
    return 1 + GListLength(L->ptr.tp);
    //L的表头要么是原子要么是表，无论哪种长度都为1
}

//求广义表的深度
int GListDepth(Glist L)
{
    if(!L){
        return 1;
    }
    if(L->tag==0){ //如果时原子节点
        return 0;
    }
    int max;
    Glist visit=L;
    for(max=0;visit!=NULL;visit=visit->ptr.tp){
        int dep=GListDepth(visit->ptr.hp);
        if(dep>max){
            max = dep;
        }
    }
    return max+1;
}

Glist Creat_Glist(char *s)
{
    Glist node=(Glist)malloc(sizeof(GLnode)*1);
    if(node==NULL)  exit(1);
    int len=strlen(s);
    int i;
    if(s[0]=='(' && s[len-1]==')'){
        //如果有外层括号，先去掉外层括号
        for(i=0;i<len;i++){
            s[i]=s[i+1];
        }
        s[len-2]='\0';
        len=len-2;
    }
    if(len==1){   //只剩一个单原子
        node->tag=ATOM;
        node->atom=s[0];
    }
    else{
        int count=0;
        int index=0;
        while(s[index]!='\0'){
            if(s[index]==',' && count==0){
                break;         //遇见中断分割
            }
            else if(s[index]=='('){
                count++;
            }
            else if(s[index]==')'){
                count--;
            }
            index++;
        }

        if(s[index]=='\0'){
            node->tag=LIST;
            node->ptr.hp=Creat_Glist(s);
            node->ptr.tp=NULL;
        }
        else{
            char *head=(char *)malloc(sizeof(char)*(index+1));
            char *tail=(char *)malloc(sizeof(char)*(len-index+2));
            int pos1;
            for(pos1=0;pos1<index;pos1++){
                head[pos1]=s[pos1];
            }
            head[pos1]='\0';
            tail[0]='(';
            int pos2;
            for(pos1=index+1,pos2=1;pos1<len;pos1++,pos2++){
                tail[pos2]=s[pos1];
            }
            tail[pos2]=')';
            tail[pos2+1]='\0';

            node->tag=LIST;
            node->ptr.hp=Creat_Glist(head);
            node->ptr.tp=Creat_Glist(tail);
        }
    }
    return node;
}

Glist Del_Glist_1(Glist L, char tg)
{
    if(L==NULL){
        return NULL;
    }
    if(L->tag==ATOM){
        if(L->atom==tg){
            return NULL;
        }
        else{
            return L;
        }
    }
    else{
        L->ptr.hp=Del_Glist_1(L->ptr.hp,tg);
        L->ptr.tp=Del_Glist_1(L->ptr.tp,tg);
        if(L->ptr.hp==NULL && L->ptr.tp==NULL){
            return NULL;
        }
        else if(L->ptr.tp==NULL){
            if(L->ptr.hp->tag==ATOM){
                L->tag=ATOM;
                L->atom=L->ptr.hp->atom;
            }
        }
        else if(L->ptr.hp==NULL){
            if(L->ptr.tp->tag==ATOM){
                L->tag=ATOM;
                L->atom=L->ptr.tp->atom;
            }
            else{
                L->ptr.hp=L->ptr.tp;
                L->ptr.tp=NULL;
            }
        }
    }
    return L;
}

Glist Del_Glist_2(Glist L, char tg)
{
    if(L==NULL){
        return NULL;
    }
    if(L->tag==ATOM){
        if(L->atom==tg){
            return NULL;
        }
        else{
            return L;
        }
    }
    else{
        L->ptr.hp=Del_Glist_2(L->ptr.hp,tg);
        L->ptr.tp=Del_Glist_2(L->ptr.tp,tg);
        if(L->ptr.hp==NULL && L->ptr.tp==NULL){
            return NULL;
        }
    }
    return L;
}

void Print_GList(Glist L, int flag)
{
    if(L==NULL){
        return;
    }
    if(L->tag==ATOM){
        printf("%c",L->atom);
    }
    else{
        if(flag)    printf("(");
        Print_GList(L->ptr.hp,1);
        if(L->ptr.hp!=NULL && L->ptr.tp!=NULL){
            printf(",");
        }
        Print_GList(L->ptr.tp,0);
        if(flag)    printf(")");
    }
}

int main(){
    char *s=(char *)malloc(sizeof(char)*300);
    int i=0;
    char c;
    while((c=getchar())!=' '){
        s[i++]=c;
    }
    s[i]='\0';
    Glist L=Creat_Glist(s);
    c=getchar();
    L=Del_Glist_2(L,c);
    if(L==NULL){
        printf("-1");
    }
    else{
        Print_GList(L,1);
    }
    return 0;
}
