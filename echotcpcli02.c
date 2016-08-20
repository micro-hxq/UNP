#include "unp.h"

static void tcp_cli(FILE* fp, const char* ipstr);
static void str_client(FILE* fp, int fd);

int main(int argc, char const *argv[])
{
    FILE* fp;

    if(argc != 3)
        err_quit("usage: echotcpcli01 <IPaddress> <file>");

    if(strcmp("stdin", argv[2]) == 0)
        fp = stdin;
    else
        fp = fopen(argv[2], "r");

    tcp_cli(fp, argv[1]);

    return 0;
}

static void tcp_cli(FILE* fp, const char* ipstr)
{
    int sockfd;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, ipstr, &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(sockfd, (SA*) &servaddr, sizeof servaddr);

    str_client(fp, sockfd);

    Close(sockfd);
}

static void str_client(FILE* fp, int fd)
{
    char sendbuf[MAXLINE];
    char recvbuf[MAXLINE];

    while(fgets(sendbuf, sizeof sendbuf, fp) != NULL)
    {
        Write(fd, sendbuf, 1);
        sleep(1);
        Write(fd, sendbuf + 1, strlen(sendbuf) - 1);
 
        if(Readline(fd, recvbuf, sizeof recvbuf) == 0)
            err_quit("server terminated prematurely");

        fputs(recvbuf, stdout);
    }
}
    