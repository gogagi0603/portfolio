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
        Node *l;
        Node *r;

        l = x->left;
        r = x->right;

        if ((x!=NIL)&&(l!=NIL)&&(x->key) <(l->key))
        {
            *count +=1;
        }

        distance_bst(l);

        if ((x!=NIL)&&(r!=NIL)&&(x->key) >(r->key))
        {
            *count +=1;
        }

        distance_bst(r);

     }
    return *count;
 }


