/**
 *  unconnected version
 */
#include "unp.h"

static void udp_cli(const char* host, const char* serv)
{
    int     sockfd;
    char    buf[MAXLINE];
    struct sockaddr *paddr;
    socklen_t len;
    int     n;

    sockfd = udp_unconnect(host, serv, &paddr, &len);

    printf("send to %s...\n", Sock_ntop_host(paddr, len));
    Sendto(sockfd, "", 1, 0, paddr, len);
    n = Recvfrom(sockfd, buf, sizeof buf, 0, NULL, NULL);
    buf[n] = 0;
    fputs(buf, stdout);
}

int main(int argc, char const *argv[])
{
    if(argc != 3)
        err_quit("usage: udp_daytimecli01 <host> <serv or port>");
    udp_cli(argv[1], argv[2]);
    return 0;
}