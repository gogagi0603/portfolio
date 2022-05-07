void max_heap_append(int A[], int p , int q)
{

    int temp;
    int par;
    int i,j,k;

    for(k=p;k<=q;k++)
    {


        if(k%2==0)
        {
            par = k/2-1;
        }
        else
        {
            par=k/2;
        }


        while(k>0 && A[par] < A[k])
        {
            temp = A[k];
            A[k] = A[par];
            A[par] = temp;
            k=par;

            if(k%2==0)
            {
                par = k/2-1;
            }
            else
            {
                par=k/2;
            }
        }
    }


}

