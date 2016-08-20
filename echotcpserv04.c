/**
 *  poll version tcp server
 */
#include "unp.h"
// #include <limits.h>

static void tcp_serv();
static void str_serv(int fd);


int main(int argc, char const *argv[])
{
    tcp_serv();

    return 0;
}

static void tcp_serv()
{
    int                 listenfd;
    struct sockaddr_in  servaddr;

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

    long open_max = sysconf(_SC_OPEN_MAX);
    if(open_max == -1)
    {
        if(errno == EINVAL)
            err_sys("sysconf error");
        else
            err_quit("OPEN_MAX is no definite limit");
    }

    // printf("open_max: %ld\n", open_max);
    // fflush(stdout);

    struct pollfd       clients[open_max];
    int                 maxi, i, nready;

    maxi = 0;
    clients[0].fd = listenfd;
    clients[0].events = POLLRDNORM;

    for(i = 1; i < open_max; ++i)
        clients[i].fd = -1;

    while(1)
    {
        nready = Poll(clients, maxi + 1, INFTIM);

        if(clients[0].revents & POLLRDNORM)
        {
            connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
            for(i = 1; i < open_max; ++i)
            {
                if(clients[i].fd < 0)
                {
                    clients[i].fd = connfd;
                    break;
                }
            }

            if(i == open_max)
                err_quit("too many clients");

            clients[i].events = POLLRDNORM;

            if(maxi < i)
                maxi = i;
            if(--nready <= 0)
                continue;
        }

        for(i = 0; i <= maxi; ++i)
        {
            if((sockfd = clients[i].fd) < 0)
                continue;
            if(clients[i].revents & (POLLRDNORM | POLLERR))
            {
                if((n = read(sockfd, buf, sizeof buf)) == -1)
                {
                    if(errno == ECONNRESET)
                    {
                        Close(sockfd);
                        clients[i].fd = -1;
                    }
                    else
                        err_sys("read error");
                }
                else if(n == 0)
                {
                    Close(sockfd);
                    clients[i].fd = -1;
                }
                else
                    Write(sockfd, buf, n);

                if(--nready <= 0)
                    break;
            }
        }
    }
}