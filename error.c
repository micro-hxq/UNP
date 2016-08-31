#include "unp.h"

#include <stdarg.h>
#include <syslog.h>

int     daemon_proc;

static void err_doit(int ,int, const char*, va_list);

void err_ret(const char* msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    err_doit(1, LOG_INFO, msg, ap);
    va_end(ap);
    return ;
}

void err_msg(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    err_doit(0, LOG_INFO, msg, ap);
    return ;
}

void err_sys(const char* msg, ...)
{
    va_list ap;
    va_start(ap,msg);
    err_doit(1, LOG_ERR, msg, ap);
    va_end(ap);
    exit(1);
}

void err_dump(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    err_doit(1, LOG_ERR, msg, ap);
    va_end(ap);
    abort();
    exit(1);
}

void err_quit(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    err_doit(0, LOG_ERR, msg, ap);
    va_end(ap);
    exit(1);
}

static void err_doit(int errnoflag, int level, const char* msg, va_list ap)
{
    int errno_save, n;
    char buff[MAXLINE + 1];

    errno_save = errno;
    vsnprintf(buff, MAXLINE, msg, ap);

    n = strlen(buff);
    if(errnoflag)
        snprintf(buff + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buff, "\n");
    if(daemon_proc)
    {
        syslog(level, "%s", buff);
    }
    else
    {
        fflush(stdout);
        fputs(buff, stderr);
        fflush(stderr);
    }

    return ;
}
/*
int main(int argc, char const *argv[])
{
    int fd;
    fd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    connect(fd, (SA*) &servaddr, sizeof(servaddr));
    err_sys("err_sys");
    err_ret("err_ret");
    err_quit("err_quit");
    err_msg("err_msg");
    err_dump("err_dump");
    return 0;
}
*/