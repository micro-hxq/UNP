#include "unp.h"

static void tcp_cli(const char* host, const char* serv)
{
    int         connfd, n;
    char        buf[MAXLINE];

    connfd = tcp_connect(host, serv);
    while((n = Read(connfd, buf, sizeof buf)) > 0)
    {
        buf[n] = 0;
        fputs(buf, stdout);
    }
}

int main(int argc, char const *argv[])
{
    if(argc != 3)
        err_quit("usage: tcp_daytimecli <host> <serv or port>");
    tcp_cli(argv[1], argv[2]);
    return 0;
}