#include "unp.h"

int udp_server(const char* host, const char* serv, socklen_t* plen)
{
    struct addrinfo     hints, *res, *ressave;
    int                 listenfd, n;


    bzero(&hints, sizeof hints);
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_server error for %s, %s: %s",
                 host, serv, gai_strerror(n));
    ressave = res;

    for(; res != NULL; res = res->ai_next)
    {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(listenfd < 0)
            continue;
        if(bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        Close(listenfd);
    }

    if(res == NULL)
        err_sys("udp_server error for %s: %s", host, serv);

    if(plen)
        *plen = res->ai_addrlen;
    freeaddrinfo(ressave);
    return listenfd;
}