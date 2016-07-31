#include "unp.h"

#ifdef HAVE_SOCKADDR_DL_STRUCT
#include <net/if_dl.h>
#endif


char* sock_ntop(const struct sockaddr *sa, socklen_t addrlen)
{
    char        port_str[8];
    static char str[128];
    switch(sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in *sin = (struct sockaddr_in*) sa;
            if(inet_ntop(AF_INET, &sin->sin_addr,
                         str, (socklen_t)sizeof str) == NULL)
                return NULL;
            if(ntohs(sin->sin_port) != 0 )
            {
                snprintf(port_str, sizeof port_str, ":%d", ntohs(sin->sin_port));
                strcat(str, port_str);
            }
            return str;
        }

#ifdef IPV6 
        case AF_INET6: {
            struct sockaddr_in6 *sin6 = (struct sockaddr_in6*) sa;
            str[0] = '[';
            if(inet_ntop(AF_INET6, &sin6->sin6_addr,
                         str + 1, (socklen_t)sizeof (str - 1)) == NULL)
                return NULL;
            if(ntohs(sin6->sin6_port) != 0)
            {
                snprintf(port_str, sizeof port_str, "]:%d", ntohs(sin6->sin6_port));
                strcat(str, port_str);
                return str;
            }
            return str + 1;
        }
#endif

#ifdef AF_UNIX        
        case AF_UNIX: {
            struct sockaddr_un *sun = (struct sockaddr_un*) sa;
            if(sun->sun_path[0] == 0)
                strcpy(str, "(no pathname bound)");
            else
                snprintf(str, sizeof str, "%s", sun->sun_path);
            return str;
        }        
#endif

#ifdef HAVE_SOCKADDR_DL_STRUCT
        case AF_LINK: {
            struct sockaddr_dl *sdl = (struct sockaddr_dl*) sa;
            if(sdl->sdl_nlen > 0)
                snprintf(str, sizeof str, "%*s (index %d)", sdl->sdl_nlen,
                         &sdl->sdl_data[0], sdl->sdl_index);
            else
                snprintf(str, sizeof str, "AF_LINK, index=%d", sdl->sdl_index);
            return str;
        }
#endif        
        default:
            snprintf(str, sizeof str, "sock_ntop: unknown AF_xxx: %d, len %d",
                     sa->sa_family, addrlen);
            return str;
    } // end switch
    return NULL;
}

char* Sock_ntop(const struct sockaddr *sa, socklen_t addrlen)
{
    char *ptr;
    if((ptr = sock_ntop(sa, addrlen)) == NULL)
        err_sys("sock_ntop error");
    return ptr;
}

// #include <ctype.h>
// int main()
// {
//     int listenfd, connfd;
//     char recvline[MAXLINE],*clientinfo;
//     int  n;
//     struct sockaddr_in servaddr, clientaddr;

//     memset(&servaddr, 0, sizeof(servaddr));
//     listenfd = Socket(AF_INET, SOCK_STREAM, 0);
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     servaddr.sin_port = htons(SERV_PORT);

//     Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
//     Listen(listenfd, LISTENQ);

//     while(1) 
//     {
//         int len = sizeof(clientaddr);
//         connfd = Accept(listenfd, (SA*) &clientaddr, &len);
//         clientinfo = Sock_ntop((SA*) &clientaddr, len);
//         printf("connecting from: %s", clientinfo);
//         fflush(NULL);
//         while((n = read(connfd, recvline, sizeof(recvline))) != 0)
//         {

//             if(n < 0)
//                 err_quit("read error");
//             recvline[n] = 0;
//             for(int i=0; i<n; ++i)
//                 recvline[i] = toupper(recvline[i]);
//             write(connfd, recvline, n);
//         }
//         printf("\nclose connection.\n");
//         fflush(NULL);
//         close(connfd);
//     }
//     return 0;
// }