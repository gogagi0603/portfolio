#define NIL -1
#include <stdio.h>
#include <stdlib.h>

int linear_probing_insert (int *T, int M, int k)
{

    int i = 0;
    int h;
    int coll = 0;
    for (h = (k + i) %(M) ; i !=  M ; )
    {
        h = (k + i) %(M);
        if (T[h] == NIL)
        {

            T[h] = k;
            return coll;
        }
        else
        {
            i++;
            coll ++;

        }
    }
}
int quadratic_probing_insert(int *T, int M, int k, int C1, int C2)
{

    int i = 0;
    int h;
    int coll = 0;
    for (h = (k + C1 * i + C2 * i * i) % (M) ; i != M ;)
    {
        h = (k + C1 * i + C2 * i * i) % (M);
        if (T[h] == NIL)
        {
            T[h] = k;
            return coll;
        }

        else
        {
            i++;
            coll ++;
        }
    }
}

int compare_hash(int M, int *key, int N, int C1, int C2)
{
    int *T;
    T = (int *)malloc(sizeof(int)*M);
    int i,j;
    int lincoll = 0, quacoll = 0;

    for (i=0 ; i<M ; i++)
    {
        T[i] = NIL;
    }
    for (i = 0 ; i<N ; i++)
 {

        lincoll = lincoll + linear_probing_insert(T,M,key[i]);
    }
    for (i=0 ; i<M ; i++)
    {
        T[i] = NIL;
    }
    for (i=0;  i<N ; i++)
    {
        quacoll = quacoll + quadratic_probing_insert(T,M,key[i],C1,C2);
    }
    return lincoll - quacoll;
}


