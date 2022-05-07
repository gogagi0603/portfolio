#include <stdio.h>
#include <stdlib.h>

int hybridsort(int A[], int p, int q, int t);
int partition (int *A, int p, int r);
int insertion_sort (int *A, int p , int r);

int main(int argc, char *argv[])
{
    int n=9, t=3;
    int A[9] = {1, 8, 6, 3, 2, 7, 4, 9, 10};
    for ( int i=0; i<9; i++ )
    {
        printf("%d ", A[i]);
    }
    printf("\n");
    int res=hybridsort(A, 0, n-1, t);
    printf("No of insertion-sort calls = %d\n",res);
    for ( int i=0; i<9; i++ ) printf("%d ", A[i]);
    {
    printf("\n");
    }
return 0;
}

