#include "unp.h"


int main(int argc, char const *argv[])
{
    char sendline[MAXLINE],recvline[MAXLINE];
    char *ptr;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);

    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    Connect(fd, (SA*) &servaddr, sizeof(servaddr));
    Getsockname(fd, (SA*) &cliaddr, &len);

    while((ptr = fgets(sendline, sizeof(sendline), stdin)) != NULL)
    {

        write(fd, sendline, strlen(sendline));
        int n = read(fd, recvline, sizeof(recvline));
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    
    return 0;
}