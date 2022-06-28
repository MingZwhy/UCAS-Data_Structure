#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
//根据表达式形成的二叉树所有叶子节点都是数字的特点
//当给出逆波兰表达式（表达式的后序）时，（无论是正常表达式还是逆波兰表达式都对应同一棵二叉树）
//正常表达式对应该树的中序遍历，逆波兰表达式对应该树的后序遍历
//所以此时我们有该树的后序遍历，利用上面已提到的叶子特性，我们修正后序遍历字符数组，在
//每个数字后面加上两个字符N，意味标记该数字的两个子节点都为空，利用该修正过的数组
//以根节点--右子节点--左子节点的顺序就可以建立该二叉树（这一顺序刚好是后序遍历的逆顺序）说明建立与遍历刚好是相反的 
//再中序遍历该二叉树可以得到正常表达式（不足之处在于没有括号） 
//实际上没有括号的原因根本上在于只由后序遍历是无法确定唯一一颗二叉树的，必须知道两种遍历才可唯一确定一棵树。 

typedef struct TreeNode{
	char elem;
	struct TreeNode *left,*right;
}Bitree, *tree;

void transform(char *s,char *t){
	int len=strlen(s);
	int i,j;
	for(i=0,j=0;i<len;i++,j++){
		if(isdigit(s[i])){
			t[j++]='N';
			t[j++]='N';
			t[j]=s[i];
		}
		else if(s[i]==' '){
			j--;
		}
		else if(s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/'){
			t[j]=s[i];
		}
		else{
			printf("error!");
		}
	}
	t[j]='\0';
}

void self_getline(char *s){
	int i=0;
	char c;
	while((c=getchar())!='\n' && i<100-2){
		s[i++]=c;
	}
	s[i]='\0';
}

void backinittree(tree *T,char *t,int *index){
	if(t[*index]=='N'){
		*T=NULL;
		(*index)++;
	}
	else{
		*T=(tree)malloc(sizeof(Bitree)*1);
		(*T)->elem=t[*index];
		(*index)++;
		backinittree(&((*T)->right),t,index);
		backinittree(&((*T)->left),t,index);
	}
}

void prereadtree(tree T,char *result,int *index){
	if(T==NULL){
		;
	}
	else{
		prereadtree(T->left,result,index);
		result[*index]=T->elem;
		(*index)++;
		prereadtree(T->right,result,index);
	}
}

void reverse(char *t){
	int len=strlen(t);
	for(int i=0,j=len-1;i<j;i++,j--){
		char temp=t[i];
		t[i]=t[j];
		t[j]=temp;
	}
}

int main(){
	char *s,*t;
	s=(char *)malloc(sizeof(char)*100);
	t=(char *)malloc(sizeof(char)*300);
	printf("请输入逆波兰表达式：");
	self_getline(s);
	printf("原表达式为：%s\n",s);
	transform(s,t);
	printf("%s\n",t);
	reverse(t);
	printf("%s\n",t);
	
	tree a=NULL;
	int index=0;
	backinittree(&a,t,&index);
	char *result;
	result=(char *)malloc(sizeof(char)*300);
	index=0;
	prereadtree(a,result,&index);
	result[index]='\0';
	printf("\n转换后为：%s",result);
	return 0;
}
