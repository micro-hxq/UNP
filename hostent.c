#include "unp.h"

static void query_host(const char *hostname)
{
    char    **pptr;
    char    buf[INET_ADDRSTRLEN];

    struct hostent *hptr;

    if((hptr = gethostbyname(hostname)) == NULL)
    {
        err_msg("gethostbyname error for host: %s : %s",
                 hostname, hstrerror(h_errno));
        return;
    }

    printf("official name: %s\n", hptr->h_name);

    for(pptr = hptr->h_aliases; *pptr != NULL; ++pptr)
        printf("\talias: %s\n", *pptr);

    switch(hptr->h_addrtype)
    {
    case AF_INET:
        pptr = hptr->h_addr_list;
        for(; *pptr != NULL; ++pptr)
        {
            printf("\taddress: %s\n", Inet_ntop(AF_INET, *pptr, buf, sizeof buf));
        }
        break;

    default:
        err_ret("unknown address type");
        break;
    }
}

int main(int argc, char const *argv[])
{
    if(argc < 2)
        err_quit("usage: hostent <IPaddress ...>");
    for(int i = 1; i < argc; ++i)
        query_host(argv[i]);
    return 0;
}
