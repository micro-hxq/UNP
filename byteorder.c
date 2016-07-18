#include "unp.h"

int main(int argc, char const *argv[])
{
    union {
        short s_;
        char  c[sizeof(short)];
    } un;

    un.s_ = 0x0102;
    printf("cup vendor : %s\n", CPU_VENDOR_OS);
    if(sizeof(short) == 2)
    {
        if(un.c[0] == 1 && un.c[1] == 2)
            printf("big-endian\n");
        else if(un.c[0] == 2 && un.c[1] == 1)
            printf("little-endian\n");
        else
            printf("unknown\n");
    }
    else
    {
        printf("short owns %ld byte(s).\n", sizeof(short));
    }

    return 0;
}