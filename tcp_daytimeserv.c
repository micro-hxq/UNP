#include "unp.h"
#include <time.h>


static void tcp_serv(const char* host, const char* serv)
{
    int         listenfd, connfd;
    char        buf[MAXLINE];
    socklen_t   len;
    struct sockaddr_storage cliaddr;
    time_t      ticks;

    listenfd = tcp_listen(host, serv, &len);

    while(1)
    {
        connfd = Accept(listenfd, (SA*) &cliaddr, &len);
        printf("connecting from %s\n", Sock_ntop((SA*) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof buf, "%.24s\r\n", ctime(&ticks));
        Write(connfd, buf, strlen(buf));
        Close(connfd);
    }
}

int main(int argc, char const *argv[])
{
    if(argc == 2)
        tcp_serv(NULL, argv[1]);
    else if(argc == 3)
        tcp_serv(argv[1], argv[2]);
    else
        err_quit("usage: tcp_daytimeserv [<host>] <serv or port>");
    return 0;
}