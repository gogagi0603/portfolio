#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int i;
    int offset = 0;
    char buff[100], e[2], o[2];
    int fd;
    off_t size;

    strcpy(e,"e");
    strcpy(o,"o");

    fd = open(argv[1], O_RDWR,S_IRUSR | S_IWUSR);
    size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    if (size % 2 == 0)
    {
        for (i = 0; i<size/2 ; i++)
        {
            lseek(fd,1,SEEK_CUR);
            write(fd,e,1);
        }
    }
    else
    {
        write(fd,o,1);
        for (i=0 ; i <size/2 ; i++)
        {
            lseek(fd,1,SEEK_CUR);
            write(fd,o,1);
        }
    }

}
