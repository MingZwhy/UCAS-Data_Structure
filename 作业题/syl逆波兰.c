#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 100
typedef struct n1
{
    char data;
    struct n1 *left;
    struct n1 *right;
} node;

node *create()
{
    node *n = (node *)malloc(sizeof(node));
    n->data = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}
void printlast(node *n)
{

    if (n->left != NULL)
    {
        printlast(n->left);
    }
    if (n->right != NULL)
    {
        printlast(n->right);
    }
    printf("%c", n->data);
}
int main()
{
    int n; //矩阵的大小
    scanf("%d", &n);
    node * Tree[MAXLEN] = {NULL};
    for (int i = 0; i < n; i++)
    {
        Tree[i] = create();
    }
    char c;
    while ((c = getchar()) != '\n')
        ;
    for (int i = 0; i < n; i++)
    {
        char data;
        int a, b;
        data = getchar();
        if (data == EOF)
            break;
        Tree[i]->data = data;
        c = getchar();
        if (c == '\n')
            continue;
        if (c == EOF)
            break;
        scanf("%d %d", &a, &b);
        Tree[i]->left = Tree[b];
        Tree[i]->right = Tree[a];
        c = getchar();
        if (c == EOF)
            break;
    }
    printlast(Tree[0]);
    return 0;
}