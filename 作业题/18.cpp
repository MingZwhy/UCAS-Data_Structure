#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define InitStackSize 100
#define AddSize 10

typedef struct{
    char *base;
    char *top;
    int Stacksize;
}Stack, *PTR_To_Stack;

typedef char Operator;

PTR_To_Stack Create_Stack(void);
bool Push(PTR_To_Stack S, char elem);
bool IsEmpty(PTR_To_Stack S);
char Top(PTR_To_Stack S);
char Pop(PTR_To_Stack S);
void DestroyStack(PTR_To_Stack S);
bool Priority(Operator o1, Operator o2);

PTR_To_Stack Create_Stack(void)
{
    PTR_To_Stack S=(PTR_To_Stack)malloc(sizeof(Stack)*1);
    if(S==NULL){
        printf("malloc error");
        exit(0);
    }
    S->base=(char *)malloc(sizeof(char)*InitStackSize);
    if(S->base==NULL){
        printf("malloc error");
        exit(0);
    }
    S->top=S->base;
    S->Stacksize=InitStackSize;
    return S;
}

bool Push(PTR_To_Stack S, char elem)
{
    if(S->top-S->base>=S->Stacksize){
        S->base=(char *)realloc(S->base,sizeof(char)*(S->Stacksize+AddSize));
        if(S->base==NULL){
            printf("malloc error");
            exit(0);
        }
        else{
            S->top=S->base+S->Stacksize;
            S->Stacksize+=AddSize;
        }
    }
    *(S->top)=elem;
    S->top++;
    return true;
}

bool IsEmpty(PTR_To_Stack S)
{
    if(S->top==S->base){
        return true;
    }
    else{
        return false;
    }
}

char Top(PTR_To_Stack S)
{
    if(IsEmpty(S)){
        printf("top error, stack is empty");
        return '?';
    }
    else{
        return *(S->top-1);
    }
}

char Pop(PTR_To_Stack S)
{
    if(IsEmpty(S)){
        printf("pop error, stack is empty");
        return '?';
    }
    else{
        S->top--;
        return *(S->top);
    }
}

void DestroyStack(PTR_To_Stack S)
{
    free(S->base);
    S->top=NULL;
    free(S);
}

bool Priority(Operator o1, Operator o2)
{
    if(o1=='#' || o1=='('){
        return true;
    }
    else if((o1=='+' || o1=='-') && (o2=='*' || o2=='/')){
        return true;
    }
    else{
        return false;
    }
}

int main(){
    char s[100];
    printf("please enter an expression: ");
    scanf("%s",s);
    int length=strlen(s);
    if(s[length-1]!='#'){
        s[length]='#';
        s[length+1]='\0';
    }
    printf("The midorder expression is: %s\n",s);
    PTR_To_Stack Number_Stack=Create_Stack();
    PTR_To_Stack Operator_Stack=Create_Stack();
    Push(Operator_Stack,'#');
    printf("The postorder expression is: ");

    int i=0;
    while(s[i]!='#'){
        if((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z')){
            Push(Number_Stack,s[i]);
            printf("%c",s[i]);
        }
        else{
            if(s[i]==')'){
                char c;
                while( (c=Pop(Operator_Stack)) != '('){
                    Push(Number_Stack,c);
                    printf("%c",c);
                }
            }
            else{
                if(s[i]=='(' || Priority(Top(Operator_Stack),s[i])){
                    Push(Operator_Stack,s[i]);
                }
                else{
                	while(Top(Operator_Stack)!='#' && !Priority(Top(Operator_Stack),s[i])){
                		char temp=Pop(Operator_Stack);
                		Push(Number_Stack,temp);
                		printf("%c",temp);
					}
                    Push(Operator_Stack,s[i]);
                }
            }
        }
        i++;
    }
    char c;
    while((c=Pop(Operator_Stack)) != '#'){
        Push(Number_Stack,c);
        printf("%c",c);
    }
    return 0;
}