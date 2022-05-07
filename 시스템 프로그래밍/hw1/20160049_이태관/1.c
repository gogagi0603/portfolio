#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(void)
 {
     int fd;
     char buf[255];
    fd = open("printf.txt",O_CREAT | O_RDWR ,S_IRUSR | S_IWUSR);
    dup2(fd,STDOUT_FILENO);
    scanf("%s", buf);
    printf("%s",buf);

    }
