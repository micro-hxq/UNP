/**
 *  connection version
 */

#include "unp.h"

static void udp_cli(const char* host, const char* serv)
{
    int         sockfd, n;
    char        buf[MAXLINE];

    sockfd = udp_connect(host, serv);
    Write(sockfd, "", 1);
    n = Read(sockfd, buf, sizeof buf);
    buf[n] = 0;
    fputs(buf, stdout);
}

int main(int argc, char const *argv[])
{
    if(argc != 3)
        err_quit("usage: tcp_daytimecli <host> <serv or port>");
    udp_cli(argv[1], argv[2]);
    return 0;
}