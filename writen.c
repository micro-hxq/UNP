#include "unp.h"

ssize_t writen(int fd, const void* vptr, size_t nbytes)
{
    size_t nleft = nbytes;
    ssize_t nwritten;
    const char* ptr = vptr;

    while(nleft > 0)
    {
        if((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return nbytes;
}


/*
int main(int argc, char const *argv[])
{
    char buff[20] = "hello,world\n";
    ssize_t n = writen(fileno(stdout), buff, sizeof(buff));
    printf("string: %s\n", buff);
    printf("buff size: %ld \n", sizeof(buff));
    printf("string length: %ld \n", strlen(buff));
    printf("write %ld btyes\n", n);

    return 0;
}
*/
