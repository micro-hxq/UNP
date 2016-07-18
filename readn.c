#include "unp.h"

ssize_t readn(int fd, void* vptr, size_t nbytes)
{
    size_t nleft = nbytes;
    ssize_t nread;
    char *ptr = vptr;

    while(nleft > 0)
    {
        if((nread = read(fd, ptr, nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)
        {
            break;
        }

        nleft -= nread;
        ptr   += nread;
    }

    return (nbytes - nleft);
}

/*
int main(int argc, char const *argv[])
{
    char buff[MAXLINE];
    ssize_t n = readn(fileno(stdin), buff, sizeof(buff));
    printf("input context: %s size: %ld\n", buff, sizeof(buff));
    printf("read: %ld bytes\n", n);
    return 0;
}
*/