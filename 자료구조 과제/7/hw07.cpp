#define NIL -1
#include <stdio.h>

void insert_node_head(int *next, int *key, int *prev, int n, int *L, int *free , int k);
void delete_node_head(int *next, int *key, int *prev, int n, int *L, int *free);
void insert_node_tail(int *next, int *key, int *prev, int n, int *L, int *free , int k);
void delete_node_tail(int *next, int *key, int *prev, int n, int *L, int *free);


void insert_node_head(int *next, int *key, int *prev, int n, int *L, int *free , int k)
{
    int temp;
    prev[next[*free]] = NIL;
    temp = next[*free];
    key[*free] = k;
    next[*free] = *L;
    prev[*free] = NIL;
    prev[*L] =  *free;
    *L = *free;
    *free = temp;
}


void delete_node_head(int *next, int *key, int *prev, int n, int *L, int *free)
{
    int temp;
    temp =next[*L];
    next[*L] = *free;
    prev[*free] = *L;
    prev[*L] = NIL;
    *free = *L;
    *L = temp;
    prev[*L] = NIL;
}

void insert_node_tail(int *next, int *key, int *prev, int n, int *L, int *free , int k)
{
    int temp;
    int tail_index = *L;
    while (next[tail_index] != NIL)
    {
       tail_index = next[tail_index];
    }
    temp = next[*free];
    next[tail_index] = *free;
    key[*free] = k;
    next[*free] = NIL;
    prev[*free] = tail_index;
    *free = temp;

}
void delete_node_tail(int *next, int *key, int *prev, int n, int *L, int *free)
{
    int tail_index = *L;
    while (next[tail_index] != NIL)
    {
        tail_index = next[tail_index];
    }
    next[prev[tail_index]] = NIL;
    next[tail_index] = *free;
    prev[tail_index] = NIL;

    prev[*free] = tail_index;
}

