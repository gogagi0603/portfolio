int insertion_count = 0;
int hybridsort(int *A,int p,int r, int t)
{
    if (p<r)
    {

        int q = partition(A,p,r);

        if ((q-1)-p+1 > t)
        {
            hybridsort(A, p , q-1, t);
        }
        else
        {
            insertion_count ++;// 인서션 소트
            insertion_sort(A,p,q-1);

        }

        if (r - (q+1)+1 > t)
        {
            hybridsort (A,q+1,r, t);
        }
        else
        {
            insertion_count ++;
            insertion_sort(A,q+1,r);

        }
    }
    return insertion_count;
}

int partition (int *A, int p, int r)
{
    int j, temp;
    int x = A[r];
    int i = p-1;
    for (j=p; j<=r-1; j++)
    {
        if (A[j]<=x)
        {
            i ++;
            temp = A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    temp = A[r];
    A[r] = A[i+1];
    A[i+1]=temp;



    return i+1;
}

int insertion_sort (int *A, int p , int r)
{
    int i,j;
    int key;

    for(j=p+1;j<=r;j++)
    {
        key = A[j];
        i=j-1;

        while(A[i] > key && i >= 0)
        {
            A[i+1] = A[i];
            i--;
        }
        A[i+1] = key;
    }
}

