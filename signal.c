#include "unp.h"

Sigfunc* my_signal(int signo, Sigfunc* func)
{
    struct sigaction act, oldact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if(signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if(sigaction(signo, &act, &oldact) < 0)
        return (SIG_ERR);
    return oldact.sa_handler;
}

Sigfunc* Signal(int signo, Sigfunc* func)
{
    Sigfunc* oldfunc;
    if((oldfunc = my_signal(signo, func)) == SIG_ERR)
        err_sys("Signal error");
    return oldfunc;
}