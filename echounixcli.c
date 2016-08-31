/**
 *  P329
 */
#include "unp.h"

static void tcp_cli(FILE* fp);
static void str_client(FILE* fp, int fd);

int main(int argc, char const *argv[])
{
    FILE* fp;

    if(argc != 2)
        err_quit("usage: echotcpcli01 <file>");

    if(strcmp("stdin", argv[1]) == 0)
        fp = stdin;
    else
        fp = fopen(argv[1], "r");

    tcp_cli(fp);

    return 0;
}

static void tcp_cli(FILE* fp)
{
    int sockfd;
    struct sockaddr_un servaddr;

    bzero(&servaddr, sizeof servaddr);
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);
    
    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    Connect(sockfd, (SA*) &servaddr, sizeof servaddr);

    str_client(fp, sockfd);

    //  finish with reset
    // struct linger lin;
    // lin.l_onoff = 1;
    // lin.l_linger = 0;
    // Setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &lin, sizeof lin);

    Close(sockfd);
}

static void str_client(FILE* fp, int fd)
{
    char sendbuf[MAXLINE];
    char recvbuf[MAXLINE];

    while(fgets(sendbuf, sizeof sendbuf, fp) != NULL)
    {
        Write(fd, sendbuf, strlen(sendbuf));

        if(Readline(fd, recvbuf, sizeof recvbuf) == 0)
            err_quit("server terminated prematurely");

        fputs(recvbuf, stdout);
    }
}
    