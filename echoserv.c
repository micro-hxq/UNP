/**
 *  P205  
 *  tcp udp server
 */

#include "unp.h"
#include <errno.h>

static void sig_child(int signo)
{
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("child pid = %ld\n", (long)pid);
        fflush(stdout);
    }
}

static void tcp_udp_serv()
{
    int     tcpfd, udpfd, listenfd;
    struct sockaddr_in  servaddr, cliaddr;
    socklen_t   clilen;
    fd_set  rset;
    int     maxfd, nready, n;
    char    buf[MAXLINE];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*) &servaddr, sizeof servaddr);
    Listen(listenfd, LISTENQ);


    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(udpfd, (SA*) &servaddr, sizeof servaddr);

    Signal(SIGCHLD, sig_child);
    FD_ZERO(&rset);
    maxfd = max(listenfd, udpfd) + 1;
    while(1)
    {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        if((nready = select(maxfd, &rset, NULL, NULL, NULL)) == -1)
        {
            if(errno == EINTR)
                continue;
            else
                err_sys("select error");
        }

        if(FD_ISSET(listenfd, &rset))
        {
            tcpfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
            if(Fork() == 0)
            {
                Close(listenfd);
                while((n = Read(tcpfd, buf, sizeof buf)))
                    Write(tcpfd, buf, n);
                Close(tcpfd);
                exit(0);
            }
            Close(tcpfd);
        }

        if(FD_ISSET(udpfd, &rset))
        {
            n = Recvfrom(udpfd, buf, sizeof buf, 0, (SA*) &cliaddr, &clilen);
            Sendto(udpfd, buf, n, 0, (SA*) &cliaddr, clilen);
        }
    }
}

int main(int argc, char const *argv[])
{
    tcp_udp_serv();
    return 0;
}