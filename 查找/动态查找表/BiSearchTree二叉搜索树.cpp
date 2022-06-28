#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define STR_SIZE 1024

typedef struct TreeNode{
    int data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
}S_Tree, *SearchTree;

typedef SearchTree Root;
typedef SearchTree TNode; 

Root MakeEmpty(int x);
Root MakeTree(Root T, int x);
TNode Findposition(Root T, int x);
TNode Findmin(Root T);
TNode Findmax(Root T);
Root Insert(Root T, int x);
Root Delete(Root T, int x);

void Draw_level(SearchTree node, bool left, char *str);
void Draw(SearchTree node);

Root MakeEmpty(int x){
    Root T;
    T=(Root)malloc(sizeof(S_Tree)*1);
    if(T==NULL){
        printf("malloc error");
        exit(0);
    }
    T->data=x;
    T->lchild=NULL;
    T->rchild=NULL;
    return T;
}

Root MakeTree(Root T, int x){
    if(T==NULL){
        T=(TNode)malloc(sizeof(S_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
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

TNode Findposition(Root T, int x){
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

TNode Findmin(Root T){
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

TNode Findmax(Root T){
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

Root Insert(Root T, int x){
    if(T==NULL){
        T=(TNode)malloc(sizeof(S_Tree)*1);
        if(T==NULL){
            printf("malloc error");
            exit(0);
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

Root Delete(Root T, int x){
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
        else{  //find the target
            if(T->lchild && T->rchild){
                temp=Findmin(T->rchild);
                T->data=temp->data;
                T->lchild=T->lchild;
                T->rchild=Delete(T->rchild,temp->data);
            }
            else{     //has one or zeor child
                if(T->lchild==NULL){
                    T=T->rchild;
                }
                else if(T->rchild==NULL){
                    T=T->lchild;
                }
                //这蕴含了如果没有孩子，则T=NULL
            }
        }
    }
    return T;
}

void draw_level(SearchTree node, bool left, char* str){
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

void draw(SearchTree root){
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