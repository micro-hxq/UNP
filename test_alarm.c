#include "apue.h"
#include <pwd.h>

static void my_handler(int signo)
{
    struct passwd *ptr;
    if((ptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error");
    alarm(1);
}

static void test()
{
    struct passwd *ptr;
    alarm(1);

    while(1)
    {
        if((ptr = getpwnam("wt")) == NULL)
            err_sys("getpwnam(wt) error");
        if(strcmp(ptr->pw_name, "wt") != 0)
            printf("corrputed, pw_name = %s\n", ptr->pw_name);
    }
}

int main(int argc, char const *argv[])
{
    test();
    
    return 0;
}