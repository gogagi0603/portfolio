int char_counting_sort(char *A, char *B, int n)
{
    int C[51];
    int i,j;
    int key = A[0];


    for (i= 0 ; i<51; i++) // C �ʱ�ȭ
    {
        C[i] =0;
    }

    for (i=0 ; i<n; i++) //�빮�ڶ� �ҹ��ڶ� ������ ��� ����...
    {
        if (A[i] >= 97)
        {
            C[(A[i]-97)*2+1] ++;
        }
        else if (A[i] <= 90)
        {
            C[(A[i]-65)*2] ++;
        }
    }



    for (i=1 ; i<51 ; i++)
    {
        C[i] = C[i] + C[i-1];
    }



    for (j = n-1 ; j>=0 ; j--) //B���ٰ� ����
    {
        if (A[j] >= 97)
        {
            B[C[(A[j]-97)*2+1]-1] = A[j];
            C[(A[j]-97)*2+1] -- ;
            if ( j == 0) // A[0]�� ��𰬴��� �ε����� key���ٰ� ����
            {
                key = C[(A[j]-97)*2+1];
            }
        }
        else if (A[j] <= 90)
        {
            B[C[(A[j] - 65) *2]-1] = A[j];
            C[(A[j]-65) *2] -- ;
            if (j==0) // �̰ŵ� �Ȱ��� key�ٰ� ����
            {
                key = C[(A[j] - 65) *2];
            }
        }
    }
    return key;
}

