#include "unp.h"

int udp_connect(const char* host, const char* serv)
{
    struct addrinfo     hints, *res, *ressave;
    int                 sockfd, n;

    bzero(&hints, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if(( n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_connect error for %s, %s: %s",
                 host, serv, gai_strerror(n));

    ressave = res;

    for(; res != NULL; res = res->ai_next)
    {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sockfd < 0)
            continue;
        printf("try connecting %s ...\n",
               Sock_ntop_host(res->ai_addr, res->ai_addrlen));
        fflush(stdout);

        if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        Close(sockfd);
    }
    if(res == NULL)
        err_sys("udp_connect error for %s:%s", host, serv);

    freeaddrinfo(ressave);

    return sockfd;
}