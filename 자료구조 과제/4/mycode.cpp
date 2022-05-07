int char_counting_sort(char *A, char *B, int n)
{
    int C[51];
    int i,j;
    int key = A[0];


    for (i= 0 ; i<51; i++) // C 초기화
    {
        C[i] =0;
    }

    for (i=0 ; i<n; i++) //대문자랑 소문자랑 구분을 어떻게 하지...
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



    for (j = n-1 ; j>=0 ; j--) //B에다가 정렬
    {
        if (A[j] >= 97)
        {
            B[C[(A[j]-97)*2+1]-1] = A[j];
            C[(A[j]-97)*2+1] -- ;
            if ( j == 0) // A[0]이 어디갔는지 인덱스를 key에다가 저장
            {
                key = C[(A[j]-97)*2+1];
            }
        }
        else if (A[j] <= 90)
        {
            B[C[(A[j] - 65) *2]-1] = A[j];
            C[(A[j]-65) *2] -- ;
            if (j==0) // 이거도 똑같이 key다가 저장
            {
                key = C[(A[j] - 65) *2];
            }
        }
    }
    return key;
}

