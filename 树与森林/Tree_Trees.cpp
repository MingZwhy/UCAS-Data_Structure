//程序待修改.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 4

typedef struct bitree{
    char data;
    struct bitree *lchild;
    struct bitree *rchild;
}Bi_Tree, *Ptr_To_BiTree;

typedef Ptr_To_BiTree Root_of_B;
typedef Ptr_To_BiTree Position_of_B;

typedef struct SStree{   //son-sibling tree
    char data;
    struct SStree *firstson;
    struct SStree *sibling;
}SS_Tree, *Ptr_To_SSTree;

typedef Ptr_To_SSTree Root_of_S;
typedef Ptr_To_SSTree Position_of_S;

//N叉树
typedef struct N_tree{
    char data;
    struct N_tree **Sons;
}N_Tree, *Ptr_To_NTree;

typedef Ptr_To_NTree Root_of_NT;
typedef Ptr_To_NTree Position_of_NT;

//森林(必须说明由几棵树合成的才知道怎么拆分)
typedef struct Trees{
    int number;
    Root_of_S Ts;
}Trees, *Ptr_to_Trees;

typedef Ptr_to_Trees Root_of_Trees;

Root_of_S Trans_StoB(Root_of_B B);
Root_of_B Trans_BtoS(Root_of_S S);
Root_of_NT Trans_StoNT(Root_of_S S);
Root_of_S Trans_NTtoS(Root_of_NT NT);
Root_of_Trees Merge_S_to_Trees(Root_of_S *S, int n);
Root_of_S * Trees_divide_to_Strees(Root_of_Trees T);
Root_of_Trees Merge_NT_to_Trees(Root_of_NT *NT, int n);
Root_of_B Create_Bitree(char *s, int *index, int n);

//画S树
void draw_level_S(Root_of_S node, bool left, char* str);
void draw_S(Root_of_S root);
//画B树
void draw_level_B(Root_of_B node, bool left, char* str);
void draw_B(Root_of_B root);

//二叉树转为son-sibling树
Root_of_S Trans_BtoS(Root_of_B B)
{
    if(B==NULL){
        return NULL;
    }
    else{
        Root_of_S S;
        S=(Root_of_S)malloc(sizeof(SS_Tree)*1);
        if(S==NULL){
            printf("malloc error");
            exit(1);
        }
        S->data=B->data;
        S->firstson=Trans_BtoS(B->lchild);
        S->sibling=Trans_BtoS(B->rchild);
        return S;
    }
}

//son-sibling树转为二叉树
Root_of_B Trans_StoB(Root_of_S S)
{
    if(S==NULL){
        return NULL;
    }
    else{
        Root_of_B B;
        B=(Root_of_B)malloc(sizeof(Bi_Tree)*1);
        if(B==NULL){
            printf("malloc error");
            exit(1);
        }
        B->data=S->data;
        B->lchild=Trans_StoB(S->firstson);
        B->rchild=Trans_StoB(S->sibling);
        return B;
    }
}

//son-sibling树转N叉树
Root_of_NT Trans_StoNT(Root_of_S S)
{
    if(S==NULL){
        return NULL;
    }
    else{
        Root_of_NT NT;
        NT=(Root_of_NT)malloc(sizeof(N_Tree)*1);
        if(NT==NULL){
            printf("malloc error");
            exit(1);
        }
        NT->data=S->data;
        NT->Sons=(Position_of_NT *)malloc(sizeof(Position_of_NT)*N);
        int i;
        Position_of_S visit=S->firstson;
        for(i=0;i<N;i++){
            if(visit!=NULL){
                NT->Sons[i]=Trans_StoNT(visit);
                visit=visit->sibling;
            }
            else{
                for(;i<N;i++){
                    NT->Sons[i]=NULL;
                }
                break;
            }
        }
        return NT;
    }
}

//N叉树转son-sibling树
Root_of_S Trans_NTtoS(Root_of_NT NT)
{
    if(NT==NULL){
        return NULL;
    }
    else{
        Root_of_S S;
        S=(Root_of_S)malloc(sizeof(Position_of_S)*1);
        if(S==NULL){
            printf("malloc error");
            exit(1);
        }
        S->data=NT->data;
        S->firstson=Trans_NTtoS(NT->Sons[0]);
        S->sibling=NULL;
        if(S->firstson!=NULL){
            int i;
            Position_of_S visit=S->firstson;
            for(i=1;i<N;i++){
                visit->sibling=Trans_NTtoS(NT->Sons[i]);
                if(visit->sibling!=NULL){
                    visit=visit->sibling;
                }
                else{
                    break;
                }
            }
        }
        return S;
    }
}

//S树合成森林
Root_of_Trees Merge_S_to_Trees(Root_of_S *S, int n)
{
    Root_of_Trees T=(Root_of_Trees)malloc(sizeof(Trees)*1);
    if(T==NULL){
        printf("malloc error");
        exit(1);
    }
    T->number=n;
    int i=0;
    Position_of_S visit=S[i];
    while(i<n-1){
        visit=S[i+1];
        S[i]->sibling=visit;
        i++;
    }
    T->Ts=S[0];
    return T;
}

//森林拆分为S树
Root_of_S * Trees_divide_to_Strees(Root_of_Trees T)
{
    int n=T->number;
    Root_of_S *S;
    S=(Root_of_S *)malloc(sizeof(Root_of_S)*n);
    if(S==NULL){
        printf("malloc error");
        exit(1);
    }
    int i=0;
    Root_of_S visit=T->Ts;
    Root_of_S head=visit->sibling;
    while(head!=NULL){
        visit->sibling=NULL;
        S[i]=visit;
        i++;
        visit=head;
        head=head->sibling;
    }
    return S;
}

//n个N叉树合成森林
Root_of_Trees Merge_NT_to_Trees(Root_of_NT *NT, int n)
{
    Root_of_S *S;
    S=(Root_of_S *)malloc(sizeof(Root_of_S)*n);
    if(S==NULL){
        printf("malloc error");
        exit(1);
    }
    int i;
    for(i=0;i<n;i++){
        S[i]=Trans_NTtoS(NT[i]);
    }
    return Merge_S_to_Trees(S,n);
}

void draw_level_S(Root_of_S node, bool left, char* str){
	if(node->sibling){
		draw_level_S(node->sibling, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->data);
	
	if(node->firstson){
		draw_level_S(node->firstson,true,strcat(str, (left ? "      " : "|     ")));
	}
	//长度都为6
	str[strlen(str)-6] = '\0'; 
} 

void draw_S(Root_of_S root){
	char str[30];
	memset(str, '\0', 30);
	
	if(root->sibling){
		draw_level_S(root->sibling, false, str);
	}
	printf("%c\n",root->data);  //打印根节点的数据
	if(root->firstson){
		draw_level_S(root->firstson, true, str);
	} 
}

void draw_level_B(Root_of_B node, bool left, char* str){
	if(node->rchild){
		draw_level_B(node->rchild, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->data);
	
	if(node->lchild){
		draw_level_B(node->lchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//长度都为6
	str[strlen(str)-6] = '\0'; 
} 

void draw_B(Root_of_B root){
	char str[30];
	memset(str, '\0', 30);
	
	if(root->rchild){
		draw_level_B(root->rchild, false, str);
	}
	printf("%c\n",root->data);  //打印根节点的数据
	if(root->lchild){
		draw_level_B(root->lchild, true, str);
	} 
}

Root_of_B Create_Bitree(char *s, int *index, int n)
{
    if(*index<n){
        if(s[*index]=='*'){
            (*index)++;
            return NULL;
        }
        else{
            Position_of_B node;
            node=(Position_of_B)malloc(sizeof(Bi_Tree)*1);
            if(node==NULL){
                printf("malloc error");
                exit(1);
            }
            node->data=s[*index];
            (*index)++;
            node->lchild=Create_Bitree(s,index,n);
            node->rchild=Create_Bitree(s,index,n);
            return node;
        }
    }
    else{
        printf("the char *s is wrong");
        return NULL;
    }
}

int main(){
    Root_of_B B;
    char b[]="ABE*F**C*DG*****";
    int index=0;
    B=Create_Bitree(b,&index,16);
    draw_B(B);

    Root_of_S S;
    S=Trans_BtoS(B);
    printf("traned S is \n\n");
    draw_S(S);

    Root_of_B New_B;
    New_B=Trans_StoB(S);
    printf("traned B is \n\n");
    draw_B(New_B);

    Root_of_NT NT;
    NT=Trans_StoNT(S);
    Root_of_S New_S;
    New_S=Trans_NTtoS(NT);
    printf("through S->NT, NT->New_S, New_S is \n\n");
    draw_S(New_S);

    printf("After that, we try to check trees and s-tree: \n\n");
    char b1[]="AB*C*D***";
    char b2[]="EF***";
    char b3[]="GHJ**I***";
    Root_of_B B1,B2,B3;
    index=0;
    B1=Create_Bitree(b1,&index,9);
    index=0;
    B2=Create_Bitree(b2,&index,5);
    index=0;
    B3=Create_Bitree(b3,&index,9);

    Root_of_S *Strees=(Root_of_S *)malloc(sizeof(Root_of_S)*3);
    Strees[0]=Trans_BtoS(B1);
    Strees[1]=Trans_BtoS(B2);
    Strees[2]=Trans_BtoS(B3);
    Root_of_Trees Whole_Tree;
    Whole_Tree=Merge_S_to_Trees(Strees,3);
    printf("The whole tree consists of %d s-trees\n\n", Whole_Tree->number);
    draw_S(Whole_Tree->Ts);
    return 0;
}