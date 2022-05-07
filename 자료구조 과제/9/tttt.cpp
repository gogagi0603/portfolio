#define NIL 0
#include <stdio.h>

typedef struct Node_{
    int key;
    struct Node_ *left;
    struct Node_ *right;
    struct Node_ *parent;
}Node;

int compare (Node *x)
{
    printf("%d",x);
}

int distance_bst(Node *x)
{
    x = x->left;
    compare (x);
    return x;
}

