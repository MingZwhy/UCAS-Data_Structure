#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INIT_SIZE 30
#define ADD_SIZE 10
#define STR_SIZE 100

typedef struct TNode{
    char data;
    struct TNode* lchild;
    struct TNode* rchild;
}TreeNode, *BiTree;

typedef struct stack{
    int stacksize;
    BiTree* base;
    BiTree* top; 
}treestack;

void draw_level(BiTree node, bool left, char* str);
void draw(BiTree root);

void initialstack(treestack* s){
    if(s==NULL){
        printf("malloc error!");
        exit(0);
    }
    s->base=(BiTree*)malloc(sizeof(BiTree)*INIT_SIZE);
    if(s->base==NULL){
        printf("malloc error!");
        exit(0);
    }
    s->top=s->base;
    s->stacksize=INIT_SIZE;
}

bool push(treestack* s, BiTree t){
    if((s->top-s->base)==s->stacksize){
        s->base=(BiTree*)realloc(s,sizeof(BiTree)*(s->stacksize+ADD_SIZE));
        if(s->base==NULL){
            printf("realloc error!");
            exit(0);
        }
        s->top=s->base+s->stacksize;
        s->stacksize+=ADD_SIZE;
    }
    *(s->top)=t;
    (s->top)++;
    return true;
}

bool pop(treestack* s, BiTree* t){
    if(s->base==s->top){
        printf("the stack is empty\n");
        return false;
    }
    (s->top)--;
    *t=*(s->top);   //BiTree=BiTree
    return true;
}

void destroystack(treestack* s){
    free(s->base);
    s->top=NULL;
    free(s);
}

void transform(char* test, treestack* t){
    int length=strlen(test);
    for(int i=0;i<length;i++){
        char c=test[i];
        printf("\n%c\n",test[i]);
        if(c!='+' && c!='-' && c!='*' && c!='/'){
            BiTree leaf;
            leaf=(BiTree)malloc(sizeof(TreeNode)*1);
            if(leaf==NULL){
                printf("malloc error");
                exit(0);
            }
            leaf->data=c;
            leaf->lchild=NULL;
            leaf->rchild=NULL;
            push(t,leaf);
        }
        else{
            BiTree root;
            root=(BiTree)malloc(sizeof(TreeNode)*1);
            root->data=c;
            BiTree left,right;
            pop(t,&right);
            pop(t,&left);
            root->lchild=left;
            root->rchild=right;
            push(t,root);
        }
    }
}

void preorder(BiTree t, char* s, int* index){
    if(t==NULL){
        return;
    }
    else{
        s[*index]=t->data;
        (*index)++;
        preorder(t->lchild,s,index);
        preorder(t->rchild,s,index);
    }
}

void midorder(BiTree t, char* s, int* index){
    if(t==NULL){
        return;
    }
    else{
    	if(t->data=='+' || t->data=='-'){
    		s[*index]='(';
        	(*index)++;
        	midorder(t->lchild,s,index);
        	s[*index]=t->data;
        	(*index)++;
        	midorder(t->rchild,s,index);
    		s[*index]=')';
        	(*index)++;
		}
		else{
	        midorder(t->lchild,s,index);
        	s[*index]=t->data;
        	(*index)++;
        	midorder(t->rchild,s,index);
		}
    }
}

void postorder(BiTree t, char* s, int* index){
    if(t==NULL){
        return;
    }
    else{
        postorder(t->lchild,s,index);
        postorder(t->rchild,s,index);
        s[*index]=t->data;
        (*index)++;
    }
}

//水平画树
void draw_level(BiTree node, bool left, char* str){
	if(node->rchild){
		draw_level(node->rchild, false, strcat(str,(left ? "|     " : "      ")));
	}
	
	printf("%s",str);
	printf("%c",(left ? '\\' : '/'));
	printf("-----");
	printf("%c\n", node->data);
	
	if(node->lchild){
		draw_level(node->lchild,true,strcat(str, (left ? "      " : "|     ")));
	}
	//长度都为6
	str[strlen(str)-6] = '\0'; 
} 

void draw(BiTree root){
	char str[STR_SIZE];
	memset(str, '\0', STR_SIZE);
	
	if(root->rchild){
		draw_level(root->rchild, false, str);
	}
	printf("%c\n",root->data);  //打印根节点的数据
	if(root->lchild){
		draw_level(root->lchild, true, str);
	} 
}

int main(){
    char test[]="ab+cde+**";
    treestack* t;
    t=(treestack*)malloc(sizeof(treestack)*1);
    initialstack(t);
    transform(test,t);
    printf("\nsuccess transform\n");
    BiTree result;
    result=(BiTree)malloc(sizeof(TreeNode)*1);
    pop(t,&result);
    char* pre;
    char* mid;
    char* post;
    pre=(char*)malloc(sizeof(char)*20);
    mid=(char*)malloc(sizeof(char)*20);
    post=(char*)malloc(sizeof(char)*20);
    int index=0;
    preorder(result,pre,&index);
    pre[index]='\0';
    
    index=0;
    midorder(result,mid,&index);
    mid[index]='\0';

    index=0;
    postorder(result,post,&index);
    post[index]='\0';

    destroystack(t);
    printf("\n前序遍历的结果为");
    for(int i=0;i<strlen(pre);i++){
        printf("%c",pre[i]);
    }
    printf("\n中序遍历的结果为");
    for(int i=0;i<strlen(mid);i++){
        printf("%c",mid[i]);
    }
    printf("\n后序遍历的结果为");
    for(int i=0;i<strlen(post);i++){
        printf("%c",post[i]);
    }
    printf("\n\n\n");
    draw(result);
    free(result);
    return 0;
}
