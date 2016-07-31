#include "unp.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in server, client;
    socklen_t len = 0;
    char strbuff[32];
    char addrbuff[128];

    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(9999);

    Bind(sockfd, (SA*) &server, sizeof server);
    Listen(sockfd, LISTENQ);

    int fd;
    time_t ticks;
    while(1)
    {
        fd = Accept(sockfd, (SA*) &client, &len);
        printf("connected from: %s, port: %d\n", 
                Inet_ntop(AF_INET, &client.sin_addr, addrbuff, sizeof addrbuff),
                ntohs(client.sin_port));
        fflush(stdout);
        ticks = time(NULL);
        snprintf(strbuff, sizeof strbuff, "%.24s\r\n", ctime(&ticks));
        Write(fd, strbuff, strlen(strbuff));
        Close(fd);
    }
    return 0;
}