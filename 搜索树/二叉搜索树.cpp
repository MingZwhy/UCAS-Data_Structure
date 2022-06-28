#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define STR_SIZE 1024

typedef struct TreeNode{
    int data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
}S_Tree, *Search_Tree;

typedef Search_Tree Root;
typedef Search_Tree TNode;

Root MakeEmpty(int x);
Root MakeTree(Root T, int x);
TNode Findposition(Root T, int x);
TNode Findposition_Nodigui(Root T, int x);
TNode Findmin(Root T);
TNode Findmin_Nodigui(Root T);
TNode Findmax(Root T);
TNode Findmax_Nodigui(Root T);
Root Insert(Root T, int x);
Root Insert_Nodigui(Root T, int x);
Root Delete(Root T, int x);
Root Delete_Nodigui(Root T, int x);

void Draw_level(Search_Tree node, bool left, char *str);
void Draw(Search_Tree node);

Root MakeEmpty(int x)
{
    Root T;
    T=(Root)malloc(sizeof(S_Tree)*1);
    if(T==NULL){
        printf("malloc error");
        exit(1);
    }
    T->data=x;
    T->lchild=NULL;
    T->rchild=NULL;
    return T;
}

Root MakeTree(Root T, int x)
{
    if(T==NULL){
        T=(TNode)malloc(sizeof(S_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(1);
        }
        T->data=x;
        T->lchild=NULL;
        T->rchild=NULL;
    }
    else{
        if(x<T->data){
            T->lchild=MakeTree(T->lchild,x);
        }
        else if(x>T->data){
            T->rchild=MakeTree(T->rchild,x);
        }
        else{
            printf("x has existed");
        }
    }
    return T;
}

TNode Findposition(Root T, int x)
{
    if(T==NULL){
        //if not found, will return NULL
        return NULL;
    }
    else{
        if(x==T->data){
            return T;
        }
        else if(x<T->data){
            return Findposition(T->lchild,x);
        }
        else{
            return Findposition(T->rchild,x);
        }
    }
}

TNode Findposition_Nodigui(Root T, int x)
{
    if(T==NULL){
        return NULL;
    }
    else{
        TNode visit=T;
        while(visit!=NULL && visit->data!=x){
            while(visit!=NULL && x<visit->data){
                visit=visit->lchild;
            }
            while(visit!=NULL && x>visit->data){
                visit=visit->rchild;
            }
        }
        return visit;
    }
}

TNode Findmin(Root T)
{
    if(T==NULL){
        return NULL;
    }
    else{
        if(T->lchild==NULL){
            return T;
        }
        else{
            return Findmin(T->lchild);
        }
    }
}

TNode Findmin_Nodigui(Root T)
{
    if(T==NULL){
        return NULL;
    }
    else{
        TNode visit=T;
        while(visit->lchild!=NULL){
            visit=visit->lchild;
        }
        return visit;
    }
}

TNode Findmax(Root T)
{
    if(T==NULL){
        return NULL;
    }
    else{
        if(T->rchild==NULL){
            return T;
        }
        else{
            return Findmax(T->rchild);
        }
    }
}

TNode Findmax_Nodigui(Root T)
{
    if(T==NULL){
        return NULL;
    }
    else{
        TNode visit=T;
        while(visit->rchild!=NULL){
            visit=visit->rchild;
        }
        return visit;
    }
}

Root Insert(Root T, int x)
{
    if(T==NULL){
        T=(TNode)malloc(sizeof(S_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(1);
        }
        T->data=x;
        T->lchild=NULL;
        T->rchild=NULL;
    }
    else{
        if(x<T->data){
            T->lchild=Insert(T->lchild,x);
        }
        else if(x>T->data){
            T->rchild=Insert(T->rchild,x);
        }
        else{
            printf("node x has existed");
        }
    }
    return T;
}

Root Insert_Nodigui(Root T, int x)
{
    TNode visit=T;
    TNode pre=NULL;
    int tag;  //tag=0表示visit是pre的左子树，反之为右子树
    while(visit!=NULL && visit->data!=x){
        while(visit!=NULL && x<visit->data){
            pre=visit;
            visit=visit->lchild;
            tag=0;
        }
        while(visit!=NULL && x>visit->data){
            pre=visit;
            visit=visit->rchild;
            tag=1;
        }
    }
    if(visit!=NULL){
        printf("x has existed");
    }
    else{
        if(T==NULL){
            T=(TNode)malloc(sizeof(S_Tree)*1);
            T->data=x;
            T->lchild=NULL;
            T->rchild=NULL;
        }
        else{
            visit=(TNode)malloc(sizeof(S_Tree)*1);
            visit->data=x;
            visit->lchild=NULL;
            visit->rchild=NULL;
            if(tag==0){
                pre->lchild=visit;
            }
            else{
                pre->rchild=visit;
            }
        }
    }
    return T;
}

Root Delete(Root T, int x)
{
    TNode temp;
    if(T==NULL){
        return NULL;
    }
    else{
        if(x<T->data){
            T->lchild=Delete(T->lchild,x);
        }
        else if(x>T->data){
            T->rchild=Delete(T->rchild,x);
        }
        else{
            if(T->lchild && T->rchild){
                temp=Findmin(T->rchild);
                T->data=temp->data;
                T->lchild=T->lchild;
                T->rchild=Delete(T->rchild,temp->data);
            }
            else{
                if(T->lchild==NULL){
                    T=T->rchild;
                }
                else if(T->rchild==NULL){
                    T=T->lchild;
                }
                else{
                    T=NULL;
                }
            }
        }
        return T;
    }
}

Root Delete_Nodigui(Root T, int x)
{
    TNode visit=T;
    TNode pre;
    int tag;
    while(visit!=NULL && visit->data!=x){
        while(visit!=NULL && x<visit->data){
            pre=visit;
            visit=visit->lchild;
            tag=0;
        }
        while(visit!=NULL && x>visit->data){
            pre=visit;
            visit=visit->rchild;
            tag=1;
        }
    }
    if(visit==NULL){
        return T;   //can't find node x
    }
    else{   //visit->data==x
        if(visit->lchild && visit->rchild){
            TNode visit2=visit->rchild;
            TNode pre_forvisit2=visit;
            while(visit2->lchild!=NULL){
                pre_forvisit2=visit2;
                visit2=visit2->lchild;
            }
            int data=visit2->data;
            visit->data=data;
            if(visit2==visit->rchild){ 
                visit->rchild=visit2->rchild;
                /*此为visit的右子树无左子树，即要删除的为
                visit的右子树本身的情况*/
            }
            else{
                pre_forvisit2->lchild=NULL;
            }
            free(visit2);
        }
        else{
            if(pre==NULL){  //要删除的就是根节点
                if(visit->lchild==NULL){
                    return visit->rchild;
                }
                else if(visit->rchild==NULL){
                    return visit->lchild;
                }
                else{
                    return NULL;
                }
            }
            else{
                if(visit->lchild==NULL){
                    if(tag==0){
                        pre->lchild=visit->rchild;
                    }
                    else{
                        pre->rchild=visit->rchild;
                    }
                }
                else if(T->rchild==NULL){
                    if(tag==0){
                        pre->lchild=visit->lchild;
                    }
                    else{
                        pre->rchild=visit->lchild;
                    }
                }
                else{
                    if(tag==0){
                        pre->lchild=NULL;
                    }
                    else{
                        pre->rchild=NULL;
                    }
                }
            }
        }
    }
    return T;
}

void draw_level(Search_Tree node, bool left, char* str){
	if(node->rchild){
		draw_level(node->rchild, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%d\n", node->data);
	
	if(node->lchild){
		draw_level(node->lchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//闀垮害閮戒负6
	str[strlen(str)-6] = '\0'; 
} 

void draw(Search_Tree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->rchild){
		draw_level(root->rchild, false, str);
	}
	printf("%d\n",root->data);  //鎵撳嵃鏍硅妭鐐圭殑鏁版嵁
	if(root->lchild){
		draw_level(root->lchild, true, str);
	} 
}

int main(){
    int node[11]={2,4,23,45,49,54,89,123,234,345,542};
    int i;
    Root T=NULL;
    for(i=0;i<11;i++){
        T=MakeTree(T,node[i]);
    }
    printf("The tree is like: \n");
    draw(T);
    if(Findposition(T,65)==Findposition_Nodigui(T,65)){
        printf("\nFindposition is equal to Findposition_Nodigui\n");
    }
    printf("Findmin -> %d\t\tFindmin_Nodigui -> %d\n",Findmin(T)->data,Findmin_Nodigui(T)->data);
    printf("Findmax -> %d\t\tFindmax_Nodigui -> %d\n",Findmax(T)->data,Findmax_Nodigui(T)->data);
    printf("Delete 11 get result is: \n");
    Delete(T,11);
    draw(T);

    printf("Delete_Nodigui 23 get result is \n");
    Delete_Nodigui(T,23);
    draw(T);

    printf("Insert 51 get result is \n");
    Insert(T,51);
    draw(T);

    printf("Insert_Nodigui 82 get result is \n");
    Insert_Nodigui(T,82);
    draw(T);

    return 0;
}