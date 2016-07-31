#include "unp.h"

int Socket(int family, int type, int protocol)
{
    int sockfd;
    if((sockfd = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return sockfd;
}

void Connect(int fd, const struct sockaddr* sa, socklen_t salen)
{
    if(connect(fd, sa, salen) < 0)
        err_sys("connect error");
}

void Bind(int fd, const struct sockaddr* sa, socklen_t salen)
{
    if(bind(fd, sa, salen) < 0)
        err_sys("bind error");
}

void Listen(int fd, int backlog)
{
    char* ptr;
    if((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    if(listen(fd, backlog) < 0)
        err_sys("listen error");
}

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr)
{
    int sockfd;
    while((sockfd = accept(fd, sa, salenptr)) < 0)
    {
        if(!(errno == EPROTO || errno == ECONNABORTED))
            err_sys("accept");
    }
    return sockfd;
}

void Close(int fd)
{
    if(close(fd) == -1)
        err_sys("close error");
}

void Getsockname(int sockfd, struct sockaddr* sa, socklen_t* salenptr)
{
    if(getsockname(sockfd, sa, salenptr) < 0)
        err_sys("getsockname error");
}

void Getpeername(int sockfd, struct sockaddr* sa, socklen_t* salenptr)
{
    if(getpeername(sockfd, sa, salenptr) < 0)
        err_sys("getpeername error");
}

void Setsockopt(int sockfd, int level, int optname,
                const void* optval, socklen_t optlen)
{
    if(setsockopt(sockfd, level, optname, optval, optlen) < 0)
        err_sys("setsockopt error");
}

void Getsockopt(int sockfd, int level, int optname,
                void* optval, socklen_t* optlen)
{
    if(getsockopt(sockfd, level, optname, optval, optlen) < 0)
        err_sys("getsockopt error");
}

void Shutdown(int fd, int how)
{
    if(shutdown(fd, how) < 0)
        err_sys("shutdown error");
}

const char* Inet_ntop(int family, const void* src, char* dst, socklen_t size)
{
    const char* ptr;
    if(dst == NULL)
        err_quit("NULL 3th argument to inet_ntop");
    if((ptr = inet_ntop(family, src, dst, size)) == NULL)
        err_sys("inet_ntop error");
    return ptr;
}

void Inet_pton(int family, const char* src, void* dst)
{
    int n;
    if((n = inet_pton(family, src, dst)) < 0)
        err_sys("inet_pton error for %s, invalid address family", src);
    else if(n == 0)
        err_quit("inet_pton error for %s, invalid address", src);
}

/**
 *  unix c function wrapper
 */

void* Calloc(size_t n, size_t size)
{
    void *ptr;
    if((ptr = calloc(n, size)) == NULL)
        err_sys("calloc error");
    return ptr;
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    int n;
    if((n = read(fd, ptr, nbytes)) == -1)
        err_sys("read error");
    return n;
}

void Write(int fd, void *ptr, size_t nbytes)
{
    if(write(fd, ptr, nbytes) != nbytes)
        err_sys("write error");
}