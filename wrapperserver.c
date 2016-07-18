#include "unp.h"
#include <ctype.h>

int main(int argc, char const *argv[])
{
    int listenfd, connfd;
    char recvline[MAXLINE],clientinfo[MAXLINE];
    int  n;
    struct sockaddr_in servaddr, clientaddr;

    memset(&clientaddr, 0, sizeof(clientaddr));
    memset(&servaddr, 0, sizeof(servaddr));
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    while(1) 
    {
        int len = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA*) &clientaddr, &len);
        Inet_ntop(AF_INET, (SA*) &clientaddr, clientinfo, MAXLINE);
        printf("connecting from: %s", clientinfo);
        while((n = read(connfd, recvline, sizeof(recvline))) != 0)
        {

            if(n < 0)
                err_quit("read error");
            recvline[n] = 0;
            for(int i=0; i<n; ++i)
                recvline[i] = toupper(recvline[i]);
            write(connfd, recvline, n);
        }
        printf("\nclose connection.\n");
        close(connfd);
    }
    return 0;
}