/**
 *  P328
 *  15-3
 */
#include "unp.h"

static void unix_serv();
static void str_serv(int fd);
static void sig_child(int signo)
{
    int     state;
    pid_t   pid;

    while((pid = waitpid(-1, &state, WNOHANG)) > 0)
    {
        printf("child %d terminated\n", pid);
        fflush(stdout);
    }
}

int main(int argc, char const *argv[])
{
    unix_serv();

    return 0;
}

static void unix_serv()
{
    int         sockfd, listenfd;
    pid_t       pid;
    socklen_t   clilen;
    struct sockaddr_un servaddr, cliaddr;

    listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof servaddr);

    unlink(UNIXSTR_PATH);
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    Bind(listenfd, (SA*) &servaddr, sizeof servaddr);
    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_child);
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

