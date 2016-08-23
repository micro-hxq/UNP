/**
 *  P204
 */
#include "unp.h"

 static void show(const char* ipaddr)
 {
    struct sockaddr_in  servaddr, cliaddr;
    socklen_t           clilen;
    int                 sockfd;

    bzero(&servaddr, sizeof servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Connect(sockfd, (SA*) &servaddr, sizeof servaddr);

    Getsockname(sockfd, (SA*) &cliaddr, &clilen);

    printf("local address: %s\n", Sock_ntop((SA*) &cliaddr, clilen));
 }

 int main(int argc, char const *argv[])
 {
     if(argc != 2)
        err_quit("usage: show_interface <IPaddress>");
    show(argv[1]);

     return 0;
 }