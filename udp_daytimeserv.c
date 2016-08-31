#include "unp.h"
#include <time.h>


static void udp_serv(const char* host, const char* serv)
{
    int         sockfd, connfd, n;
    char        buf[MAXLINE];
    socklen_t   len;
    struct sockaddr_storage cliaddr;
    time_t      ticks;

    sockfd = udp_server(host, serv, &len);

    while(1)
    {
        Recvfrom(sockfd, buf, sizeof buf, 0, (SA*) &cliaddr, &len);
        printf("receiving from %s\n", Sock_ntop((SA*) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof buf, "%.24s\r\n", ctime(&ticks));
        Sendto(sockfd, buf, strlen(buf), 0, (SA*) &cliaddr, len);
    }
}

int main(int argc, char const *argv[])
{
    if(argc == 2)
        udp_serv(NULL, argv[1]);
    else if(argc == 3)
        udp_serv(argv[1], argv[2]);
    else
        err_quit("usage: tcp_daytimeserv [<host>] <serv or port>");
    return 0;
}