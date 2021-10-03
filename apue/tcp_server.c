#include <sys/socket.h>
#include <errno.h>
int initserver(int type, const struct sockaddr*addr, socklen_t addrlen, int qlen)
{
    int fd;
    int err = 0;

    fd = socket(addr->sa_family, type, 0);
    if (fd < 0)
    {
        return -1;
    }
    if(bind(fd, addr, addrlen) < 0)
        goto quit;

    if(type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if(listen(fd, qlen) < 0)
            goto quit;
    }

    return fd;    
quit:
    err = errno;
    close(fd);
    errno = err;
    return -1;
}

int main(){

    struct sockaddr sock;
    int fd = initserver(AF_INET, &sock, sizeof(sock), );

}