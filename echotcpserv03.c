/**
 *  select version tcp server
 */

#include "unp.h"

static void tcp_serv();
static void str_serv(int fd);


int main(int argc, char const *argv[])
{
    tcp_serv();

    return 0;
}

static void tcp_serv()
{
    int         listenfd;
    struct sockaddr_in servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*) &servaddr, sizeof servaddr);
    Listen(listenfd, LISTENQ);

    str_serv(listenfd);
}

static void str_serv(int listenfd)
{
    ssize_t             n;
    char                buf[MAXLINE];
    int                 connfd, sockfd;
    struct sockaddr_in  cliaddr;
    socklen_t           clilen;

    fd_set              rset, allset;
    int                 clients[FD_SETSIZE];
    int                 maxfd, maxi, i, nready;

    maxfd = listenfd;
    for(i = 0; i < FD_SETSIZE; ++i)
        clients[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while(1)
    {
        rset = allset;
        nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(listenfd, &rset))
        {
            connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
            for(i = 0; i < FD_SETSIZE; ++i)
            {
                if(clients[i] < 0)
                {
                    clients[i] = connfd;
                    break;
                }
            }

            if(i == FD_SETSIZE)
                err_quit("too many clients");

            FD_SET(connfd, &allset);

            if(maxfd < connfd)
                maxfd = connfd;
            if(maxi < i)
                maxi = i;
            if(--nready <= 0)
                continue;
        }

        for(i = 0; i <= maxi; ++i)
        {
            if((sockfd = clients[i]) < 0)
                continue;
            if(FD_ISSET(clients[i], &rset))
            {
                if((n = Read(sockfd, buf, sizeof buf)) == 0)
                {
                    Close(sockfd);
                    clients[i] = -1;
                    FD_CLR(sockfd, &allset);
                }
                else
                    Write(sockfd, buf, n);

                if(--nready <= 0)
                    break;
            }
        }
    }
}