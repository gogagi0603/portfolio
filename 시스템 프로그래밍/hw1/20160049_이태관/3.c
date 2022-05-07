#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void search (DIR *dp, int *fp_count, int *dp_count);

int main(int argc, char *argv[])
{
        int  file_count = 0;
        int dir_count = 0;
        int *fp_count = &file_count;
        int *dp_count = &dir_count;
        struct dirent *dirp;
        struct stat buf;
        DIR  *dp;

        char path[255];

        if (argc != 2) // error about argv[]
        {
           fprintf(stderr,"a\n");
            exit(1);
         }
         if ((dp = opendir(argv[1])) == NULL) // error about open dir)
         {
            fprintf(stderr,"b\n");
             exit(1);
         }
    
	         chdir(argv[1]); // Path change to argv[]

         search(dp, fp_count, dp_count); // start

         printf("File Count = %d\n", *fp_count);
         printf("DIR Count = %d\n", *dp_count);
         closedir(dp);

         return 0;
}


void search (DIR *dp, int *fp_count, int *dp_count)
{
    DIR *dp2;
    DIR *rdp;
    int dircount = 0;
    struct dirent *dirp;
    struct dirent *dirp2;
    struct dirent *rdirp;
    struct stat buf;
    struct stat rbuf;
    char pathbuf[200];
    char path[255];
    char nowpath[255];
    int i=0;

    getcwd(nowpath,255); // get path in nowpath
    dp = opendir(nowpath); // opendir about nowpath


    while ((dirp = readdir(dp))!= NULL) // searching for nowpath 
    {

        if (strcmp (dirp -> d_name , ".")!=0 && strcmp(dirp -> d_name, "..") != 0) // . and .. exception
        {
            stat(dirp -> d_name,&buf);

            if (buf.st_mode & S_IFREG) // if reguler file
            {
            	
                *fp_count += 1; // counting
            }
            else if (buf.st_mode & S_IFDIR) // if dir
            {
            	
                *dp_count += 1; // counting
                dircount ++;
            }
        }
    }

	
	closedir(dp);


    if (dircount > 0) // if count dir 1 >= 0
    {


        rdp = opendir(nowpath);
        while ((rdirp = readdir(rdp)) != NULL) // searching for nowpath again
        {
            if (strcmp (rdirp -> d_name , ".") != 0 && strcmp (rdirp -> d_name, "..") != 0)
            {

                stat(rdirp->d_name, &rbuf);



             if (rbuf.st_mode & S_IFDIR) // if dir file
                {
                    strcpy(pathbuf,"");
                    getcwd(pathbuf,200); // save this path
                    chdir(rdirp -> d_name); // change path
                    strcpy(path,"");
                    getcwd(path,255);

                     if(( dp2 = opendir(path))== NULL)
                    {
                        fprintf(stderr,"error\n");
                        exit(1);
                     }
                    search(dp2, fp_count, dp_count); // recursive call this dirname
                    closedir(dp2);
                    chdir("..");
                }

            }


        }


    }


}
