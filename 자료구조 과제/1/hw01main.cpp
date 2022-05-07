#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lsc(char *A,int p,int r);


int main(int argc, char* argv[])
{

   int l = strlen(argv[1]);
    printf("%d",lsc(argv[1],0,l-1));

   return 0;
}

