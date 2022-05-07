#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define buf_index 100

int main(int argc, char **argv)
{
    FILE *fp, *fp2;
    fp = fopen("2.c","r+");
    int i=0,j;
    int count = 0;
    char buf[buf_index];
    char buff[buf_index];
    char name[255];
    char a[255];
    strcpy(buf,"");
    strcpy(buff,"");

    while (feof(fp) == 0)// EOF??    
    {
        strcpy(name,argv[0]);
        fread(buff, sizeof(char), buf_index, fp); // read (\n exception);


        count ++;
        sprintf(a,"_%d.c",count);

        strcat(name,a);

        fp2 = fopen(name,"w+");

        snprintf(buf,sizeof(buff)-1,"%s",buff);
        fprintf(fp2,"%s",buf);
        strcpy(buff,"");
        strcpy(buf,"");
        strcpy(name,"");
        fclose(fp2);
    }

}

