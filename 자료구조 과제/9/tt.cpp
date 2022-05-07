#define NIL 0
#include <stdio.h>
#include <stdlib.h>

int ccount = 0;
int *count = &ccount;

typedef struct Node_{
    int key;
    struct Node_ *left;
    struct Node_ *right;
    struct Node_ *parent;
}Node;

int distance_bst(Node *x)
{
    if ( x != NIL)
    {
        Node *y;
        Node *l;
        Node *r;
        Node *p;

        y = x;

        l = y->left;
        r = y->right;
        p = y->parent;

        if ((y!=NIL)&&(l!=NIL)&&(y->key) <(l->key))
        {
            *count +=1;
        }

        distance_bst(l);

        if ((y!=NIL)&&(r!=NIL)&&(y->key) >(r->key))
        {
            *count +=1;
        }

        distance_bst(r);

     }
    return *count;
 }

