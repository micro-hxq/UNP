#include "unp.h"

int udp_unconnect(const char* host, const char* serv, SA** paddr, socklen_t *plen)
{
    struct addrinfo     hints, *res, *ressave;
    int                 sockfd, n;

    bzero(&hints, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if(( n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_unconnect error for %s, %s: %s",
                 host, serv, gai_strerror(n));

    ressave = res;

    for(; res != NULL; res = res->ai_next)
    {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sockfd > 0)
            break;
    }

    if(res == NULL)
        err_sys("udp_unconnect error for %s:%s", host, serv);

    *paddr = Malloc(res->ai_addrlen);
    memcpy(*paddr, res->ai_addr, res->ai_addrlen);
    *plen = res->ai_addrlen;

    freeaddrinfo(ressave);

    return sockfd;
}