/**
 *  select version
 */


#include "unp.h"

static void tcp_cli(FILE* fp, const char* ipstr);
static void str_client(FILE* fp, int fd);

int main(int argc, char const *argv[])
{
    FILE* fp;

    if(argc != 3)
        err_quit("usage: echotcpcli01 <IPaddress> <file>");

    if(strcmp("stdin", argv[2]) == 0)
        fp = stdin;
    else
        fp = fopen(argv[2], "r");

    tcp_cli(fp, argv[1]);

    return 0;
}

static void tcp_cli(FILE* fp, const char* ipstr)
{
    int sockfd;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, ipstr, &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(sockfd, (SA*) &servaddr, sizeof servaddr);

    str_client(fp, sockfd);

    Close(sockfd);
}

static void str_client(FILE* fp, int fd)
{
    char    buf[MAXLINE];
    int     maxfd, stdineof;
    fd_set  rset;
    int     n;

    stdineof = 0;
    FD_ZERO(&rset);

    while(1)
    {
        if(stdineof == 0)   
            FD_SET(fileno(fp), &rset);
        FD_SET(fd, &rset);

        maxfd = max(fd, fileno(fp)) + 1;
        Select(maxfd, &rset, NULL, NULL, NULL);

        if(FD_ISSET(fileno(fp), &rset))
        {
            if((n = Read(fileno(fp), buf, sizeof buf)) == 0)
            {
                stdineof = 1;
                Shutdown(fd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Write(fd, buf, n);
        }

        if(FD_ISSET(fd, &rset))
        {
            if((n = Read(fd, buf, sizeof buf)) == 0)
            {
                if(stdineof == 1)
                    return ;
                else
                    err_quit("server terminated prematurely");
            }
            Write(fileno(stdout), buf, n);
        }
    }
}
    