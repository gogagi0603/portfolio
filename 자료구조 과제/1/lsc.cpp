int lsc (char *A, int p, int r)
{
   int i,j;
   int count_L=1, count_R;

   if(p<r)
   {
      for(i=p;i<=r;i++)
      {
         if(A[i] != A[i+1] )
         {
            break;
         }
         else if(A[i] == A[i+1])
         {
            count_L ++;
         }

      }

      count_R = lsc(A,i+1,r);

      if(count_L>count_R)
      {
         return count_L;
      }
      else if(count_L<count_R)
      {
         return count_R;
      }
      else if(count_L == count_R)
      {
         return count_L;
      }

    }
   return 0;
}

