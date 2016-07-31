#include "unp.h"

int main(int argc, char const *argv[])
{
    int                 sockfd,n;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in  servaddr;

    if(argc != 2)
    {
        printf("usage: a.out <IPaddress>\n");
        exit(1);
    }

    sockfd = Socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(9999);

    Inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

    Connect(sockfd,(SA*)&servaddr,sizeof(servaddr));

    while((n = Read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF)
        {
            printf("fputs error\n");
            exit(1);
        }
    }

    if(n < 0)
    {
        printf("read error\n");
        exit(1);
    }

    exit(0);
}

