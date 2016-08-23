#include "unp.h"

static void udp_echo(FILE *fp, int sockfd, const SA* pserv, socklen_t servlen)
{
    int                 n;
    char                buf[MAXLINE];
    socklen_t           len;
    struct sockaddr*    preply;

    preply = Malloc(servlen);

    while(fgets(buf, sizeof buf, fp) != NULL)
    {
        Sendto(sockfd, buf, strlen(buf), 0, pserv, servlen);
        len = servlen;
        n = Recvfrom(sockfd, buf, sizeof buf, 0, preply, &len);
        if(len != servlen || memcmp(pserv, preply, servlen) != 0)
        {
            printf("reply from %s (ignored)\n", Sock_ntop(preply, len));
            continue;
        }
        buf[n] = 0;
        fputs(buf, stdout);
    }
}

static void udp_echo_connect(FILE *fp, int sockfd, const SA* pserv, socklen_t servlen)
{
    int                 n;
    char                buf[MAXLINE];

    Connect(sockfd, pserv, servlen);

    while(fgets(buf, sizeof buf, fp) != NULL)
    {
        Write(sockfd, buf, strlen(buf));
        n = Read(sockfd, buf, sizeof buf);
        buf[n] = 0;
        fputs(buf, stdout);
    }
}

static void udp_cli(const char* ipaddr)
{
    struct sockaddr_in  servaddr;
    int                 sockfd;

    bzero(&servaddr, sizeof servaddr);
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);

    // udp_echo(stdin, sockfd, (SA*) &servaddr, sizeof servaddr);
    udp_echo_connect(stdin, sockfd, (SA*) &servaddr, sizeof servaddr);
}

int main(int argc, char const *argv[])
{
    if(argc != 2)
        err_quit("usage: echoudpcli01 <IPaddress>");
    udp_cli(argv[1]);
    return 0;
}