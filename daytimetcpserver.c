#include "unp.h"
#include <time.h>

int main(int argc, char const *argv[])
{
    int                 listenfd,connfd;
    struct sockaddr_in  servaddr;
    char                buff[MAXLINE];
    time_t              ticks;

    listenfd = socket(AF_INET,SOCK_STREAM,0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);

    if(bind(listenfd,(SA*) &servaddr,sizeof(servaddr)) < 0)
    {
        printf("bind error\n");
        exit(1);
    }

    if(listen(listenfd, LISTENQ) < 0)
    {
        printf("listen error\n");
        exit(1);
    }

    while(1)
    {
        if((connfd = accept(listenfd,(SA*) NULL, NULL)) < 0)
        {
            printf("accept error\n");
            exit(1);
        }

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "Now is %.24s\n", ctime(&ticks));
        printf("Server : %s\n", buff);
        
        if(write(connfd, buff, strlen(buff)) < strlen(buff))
        {
            printf("write error\n");
            exit(1);
        }

        if(close(connfd) == -1)
        {
            printf("close error\n");
            exit(1);
        }
    }

    return 0;
}