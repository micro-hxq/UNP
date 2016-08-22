/**
 *  P188
 *  basic udp server
 */
#include "unp.h"

static void udp_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
    char        buf[MAXLINE];
    int         n;
    socklen_t   len;

    while(1)
    {
        len = clilen;
        n = Recvfrom(sockfd, buf, sizeof buf, 0, pcliaddr, &len);
        Sendto(sockfd, buf, n, 0, pcliaddr, len);
    }
}

static void udp_serv()
{
    struct sockaddr_in      servaddr, cliaddr;
    int                     sockfd;

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Bind(sockfd, (SA*) &servaddr, sizeof servaddr);

    udp_echo(sockfd, (SA*) &cliaddr, sizeof cliaddr);
}

int main(int argc, char const *argv[])
{
    udp_serv();

    return 0;
}