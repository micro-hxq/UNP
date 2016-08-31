#include "unp.h"

int tcp_listen(const char* host, const char* serv, socklen_t* plen)
{
    struct addrinfo     hints, *res, *ressave;
    int                 listenfd, n;
    const int           on = 1;

    bzero(&hints, sizeof hints);
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("getaddrinfo error for %s, %s: %s",
                 host, serv, gai_strerror(n));
    ressave = res;

    for(; res != NULL; res = res->ai_next)
    {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(listenfd < 0)
            continue;
        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
        if(bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        Close(listenfd);
    }

    if(res == NULL)
        err_sys("tcp_listen error for %s: %s", host, serv);

    Listen(listenfd, LISTENQ);

    if(plen)
        *plen = res->ai_addrlen;
    freeaddrinfo(ressave);
    return listenfd;
}