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
    int         sockfd, listenfd;
    pid_t       pid;
    socklen_t   clilen;
    struct sockaddr_in servaddr, cliaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*) &servaddr, sizeof servaddr);
    Listen(listenfd, LISTENQ);

    for( ; ; ) {
        sockfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
        if((pid = Fork()) == 0)
        {
            Close(listenfd);
            str_serv(sockfd);
            Close(sockfd);
            exit(0);
        }
        Close(sockfd);
    }
}

static void str_serv(int fd)
{
    ssize_t n;
    char    buf[MAXLINE];

    while(1) {
        while((n = read(fd, buf, sizeof buf)) > 0)
            Write(fd, buf, n);

        if(n == 0)
            break;
        else if(n <0 && errno != EINTR)
            err_sys("read error");
    }
}