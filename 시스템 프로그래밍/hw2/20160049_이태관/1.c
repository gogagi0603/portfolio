#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    struct stat statbuf;

    stat(argv[0],&statbuf);

    if ((strcmp(argv[1],"0")) == 0)
    {
        chown(argv[0], getuid(), getgid());
        chmod(argv[0], 00777);
    }
    else if ((strcmp(argv[1],"1")) == 0)
    {
        chown(argv[0], 0,0);
        chmod(argv[0],statbuf.st_mode | S_ISUID);
    }
     return 0;
}

