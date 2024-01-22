#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

/* void segv_handler (int signum, siginfo_t *info, void *ucontext) */
void segv_handler (int signum)
{
    switch (signum) {
    case SIGSEGV:
        printf ("Caught SIGSEGV. We just exit for now\n");
        exit (-1);
    default:
        printf ("We did not catch this signal: %d\n", signum);
    }

}

/* show a signal handler that can catch a segmentation fault */
int main (int argc, char *argv[])
{
    /* data structure related to signal handling */
    struct sigaction sa;

    /* initialize the data structure */
    sa.sa_handler = segv_handler;
    sigemptyset (&sa.sa_mask);  /* clean up the data structure of any spurious
                                                     data */
    sa.sa_flags = SA_RESTART; /* Restart functions if interrupted by handler */
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {  /* We specify a handler for
                                                                         SIGSEGV */
        printf ("Cannot initialize a signal handler\n");
        return -1;
    }

    /* The normal code from before */
    
    /* declare a pointer to char */
    char *str;

    /* define some garbage literal string */
    char *literal = "adioofsdfffkfsdfsdlf;lflsdfdsfjfjklfjdfjdsfjdlfjdsl;f"; 

    printf ("string contents of the literal string = %s\n", literal);

    /* now try copying the contents of the literal into the pointer without
       allocating any memory thereby causing segmentation fault. But this will
       get caught by our signal handler
    */
    strcpy (str, literal);

    printf ("string contents pointed to by the pointer = %s\n", str);
}
