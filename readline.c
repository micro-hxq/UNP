#include "unp.h"

static int      read_cnt;
static char*    read_ptr;
static char     read_buf[MAXLINE];

static ssize_t read_aux(int fd, char* vptr)
{
    if(read_cnt <=0)
    {
        while((read_cnt = read(fd, read_buf, sizeof(read_buf))) <0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }
        if(read_cnt == 0)
            return 0;
        read_ptr = read_buf;
    }

    --read_cnt;
    *vptr = *read_ptr++;
    return 1;
}

ssize_t readline(int fd, void* vptr, size_t maxlen)
{
    ssize_t n,rc;
    char*   ptr = vptr;
    char    ch;

    for(n = 1; n < maxlen; ++n)
    {
        if((rc = read_aux(fd,&ch)) ==1)
        {
            *ptr++ = ch;
            if(ch == '\n')
                break;
        }
        else if(rc == 0)
        {
            *ptr = 0;
            return n-1;
        }
        else
        {
            return -1;
        }
    }
    *ptr = 0;
    return n;
}

static int readlinebuf(void** vptr)
{
    if(read_cnt)
        *vptr = read_ptr;
    return read_cnt;
}

/*
int main()
{
    // char *str = "this is a test program. To test whether readline work correctly.";
    char str[20];
    ssize_t n = readline(fileno(stdin), str, sizeof(str));
    printf("realine string: %s\n", str);
    printf("realine %ld bytes", n);

    void *vptr;
    printf("left bytes: %d ", readlinebuf(&vptr));
    char *ptr = vptr;
    printf("left string: %s", ptr);
    return 0;
}
*/