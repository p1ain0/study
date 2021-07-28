#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

char buf[0x10] = "123213213";
int main()
{
    int fd = open("/dev/fd/0", O_RDWR);
    write(fd, buf, 10);
    printf("%s", buf);
    close(fd);
    return 0;
}

