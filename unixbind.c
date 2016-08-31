/**
 *  P325
 *  15-2
 */
#include "unp.h"

static void unix_bind(const char* name)
{
    int                 sockfd;
    socklen_t           len;
    struct sockaddr_un  addr1, addr2;

    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(name);

    bzero(&addr1, sizeof addr1);
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, name, sizeof(addr1.sun_path) - 1);
    Bind(sockfd, (SA*) &addr1, SUN_LEN(&addr1));

    len = sizeof addr2;
    Getsockname(sockfd, (SA*) &addr2, &len);
    printf("bound name = %s, return len = %d\n", addr2.sun_path, len);
}

int main(int argc, char const *argv[])
{
    if(argc != 2)
        err_quit("usage: unixbind <filename>");
    unix_bind(argv[1]);
    return 0;
}