/// @par Copyright:
/// Copyright (c) by ScaleFlux, Inc.
///
/// ALL RIGHTS RESERVED. These coded instructions and program statements are
/// copyrighted works and confidential proprietary information of ScaleFlux, Inc.
/// They may not be modified, copied, reproduced, distributed, or disclosed to
/// third parties in any manner, medium, or form, in whole or in part.
///
/// @par Description:
/// The Description of this file.

#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/select.h>
#include <signal.h>
#include "ccs_base.h"

#define VSYSTEM_SHELL                   "sh"        /**< sh */
#define VSYSTEM_ARG_MAX                 4           /**< 4 arguments */
#define VSYSTEM_CMD_LEN                 4096         /**< 4096 char */
//----------------------------------------------------------------------------
// PROTOTYPES/VARIABLES                                                     --
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTIONS                                                                --
//----------------------------------------------------------------------------
int
vsystem(const char *format, ...)
{
    pid_t    child ; 
    int      ret = 0 ; 
    sigset_t omask ; 
    struct sigaction sa, intr, quit ; 

    sa.sa_handler = SIG_IGN ; 
    sa.sa_flags   = 0 ; 
    sigemptyset(&sa.sa_mask) ; 

    sigaction(SIGINT,  &sa, &intr) ;
    sigaction(SIGQUIT, &sa, &quit) ; 
    sigaddset(&sa.sa_mask, SIGCHLD) ; 
    sigprocmask(SIG_BLOCK, &sa.sa_mask, &omask) ; 

    if ((child = vfork()) == 0) {
        const char *argv[VSYSTEM_ARG_MAX] ; 
        char command[VSYSTEM_CMD_LEN] ; 
        va_list args ; 

        va_start(args, format) ; 
        vsnprintf(command, sizeof(command), format, args) ; 
        va_end(args) ; 

        argv[0] = VSYSTEM_SHELL ; 
        argv[1] = "-c" ; 
        argv[2] = command ; 
        argv[3] = NULL ; 
        sigaction(SIGINT, &intr, (struct sigaction*)NULL) ; 
        sigaction(SIGQUIT, &intr, (struct sigaction*)NULL) ; 
        if (execvp(VSYSTEM_SHELL, (char **)argv) == -1)
            _exit(127) ; 
        _exit(0) ; 
    } else if (child < 0) {
        ret = -1 ; 
    } else {
        waitpid(child, &ret, 0) ; 
    } 

    sigaction(SIGINT, &intr, (struct sigaction*)NULL) ; 
    sigaction(SIGQUIT, &quit, (struct sigaction*)NULL) ; 
    sigprocmask(SIG_SETMASK, &omask, (sigset_t*)NULL) ; 
    return(ret) ; 
}
//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

