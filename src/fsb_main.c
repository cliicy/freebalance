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
#include "fsb_base.h"
#include "sfxini.h"
#include "ini.h"
//----------------------------------------------------------------------------
// PROTOTYPES/VARIABLES                                                     --
//----------------------------------------------------------------------------
static RC_t set_opt(int argc, char *argv[]) ;
static RC_t init(void) ;
extern int sfxtool_cmdhdlr__install_systemd();
extern int sfxtool_cmdhdlr__install_sysv();

//default space-balancer is 85
static int   space_pct = 85 ;

#define SFX_TOOLS               "sfx-tools"
#define SFX_TOOLS_SYS_VINIT_SRC "/opt/sfx/scripts/sfx-tools.SysVInit"
#define SYS_VINIT_DIR           "/etc/init.d"
#define SFX_TOOLS_SYS_VINIT_DST SYS_VINIT_DIR "/sfx-tools"

#define SFX_TOOLS_SYSTEMCTL_SRC "/opt/sfx/scripts/sfx-tools.systemctl"
#define SYSTEMCTL_DIR           "/etc/systemd/system"
//#define SYSTEMCTL_DIR           "/home/tcn/etc/systemd/system"  //for user: tcn to debug 
#define SFX_TOOLS_SYSTEMCTL_DST SYSTEMCTL_DIR "/sfx-tools.service"

#define SFXINI_VERSION  1

static sfxini_t sfxini = {
    .system_version = SFXINI_VERSION,
    .fsb_green_zone = SFX_TOOLS__GREEN_ZONE,
} ;

static int sfxini_handle(void* opq, const char* section,
                                    const char* name, const char* val) ;

//----------------------------------------------------------------------------
// FUNCTIONS                                                                --
//----------------------------------------------------------------------------
int
main(int argc, char *argv[])
{
    if (set_opt(argc, argv)                       != RC_OK)  return 0 ;
}

static RC_t
set_opt(int argc, char *argv[])
{
    int flag ;
    RC_t rc = RC_OK ;
    char smg[256] ;
    char arg_v[256] ;
    int pct = 0 ;
    int ret = 0 ;
    sfxini_t *sfxini ;
    while ((flag = getopt(argc, argv, "c:dhis")) != EOF) {
        switch(flag) {
        case 'i': 
            strcpy(smg, "will install sfx-fsb as system style service\n")   ;
            fprintf(stdout, smg) ;
            ret = sfxtool_cmdhdlr__install_systemd() ;
            if ( ret == -1) {
                ret = sfxtool_cmdhdlr__install_sysv();
            }
            if ( ret == -1) {
                fprintf(stdout, "fail to install sfx-fsb") ;
            }
            break ;
        case 's': 
            sfxini = sfxini__read() ;
            sprintf(smg, "the current [space-balancer] value is: %d\n",sfxini->fsb_green_zone)   ; 
            fprintf(stdout, smg) ;
            break ;
        case 'c': 
            space_pct = atoi(optarg);
            sprintf(smg, "the projected-percentage will be set: %s\n",optarg)   ; 
            fprintf(stdout, smg) ;
            break ;
        case 'h': rc = RC_ERROR                  ;  break ;
        }
    }
    if ( rc != RC_OK ) {
        fprintf(stdout,
            "%s <option>+\n"
            "  options:\n"
            "  -i systemd                                       systemd style to start a process during bootup time\n"
            "  -s                                               show the [space-balancer] value:\n"
            "  -c space-balancer projected-percentage  xxx      <integer 0 -- 95>         Percentage\n"
            "  -h                This message\n", argv[0]) ;
    }
   
    return rc ;
}

static RC_t
init(void)
{
    return RC_OK ;
}

static int
sfxini_handle(void* opq, const char* section, const char *name, const char *val)
{
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    if (MATCH("system", "version")) {
        sfxini.system_version = atoi(val) ;
    }
    else if (MATCH("system", "init-type")) {
        sfxini.system_init_type = atoi(val) ;
    }
    else if (MATCH("sfx-tools", "fsb-green-zone")) {
        sfxini.fsb_green_zone = atoi(val) ;
        sfxini.fsb_modified   = 1 ;
    }
}

int
cli_system(const char *fmt, ...)
{
    va_list args ;
    int     ret ;
    char    cmd[256] ;
    va_start (args, fmt) ;
    vsnprintf(cmd, sizeof(cmd), fmt, args) ;
    fprintf(stdout, cmd) ;
    ret = vsystem(cmd) ;
    va_end (args) ;
    return ret ;
}

int
sfxtool_cmdhdlr__install_systemd()
{
    char smg[256] ;
    sfxini_t *sfxini ;
    int bret = 0;
    do {
        if ( access(SYSTEMCTL_DIR, F_OK) != 0 ) {
            sprintf(smg,"Failed to find %s dir for systemd\n", SYSTEMCTL_DIR) ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        if ( access(SYSTEMCTL_DIR, W_OK) != 0 ) {
             sprintf(smg,"No access privilege to install sfx-tools into %s\n",
                          SYSTEMCTL_DIR) ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        if ( access(SFX_TOOLS_SYSTEMCTL_DST, F_OK) == 0 ) {
            sprintf(smg,"sfx-tools for systemd is installed already.\n") ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;  //marked it for debug
        }
        if ( access(SFX_TOOLS_SYSTEMCTL_SRC, F_OK) != 0 ) {
            sprintf(smg,"Failed to find %s, please re-run sfx-app-install.sh\n",
                          SFX_TOOLS_SYSTEMCTL_SRC) ;
            fprintf(stdout, smg) ;
            bret = -1;
			break ;
        }
        sfxini = sfxini__read() ;
        if ( sfxini->system_init_type == SYSINIT_TYPE__SYSV ) {
            sprintf(smg,"sfx-tools for SysV-init has installed. uninstall it first\n") ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        bret = cli_system("/bin/cp %s %s", SFX_TOOLS_SYSTEMCTL_SRC, SFX_TOOLS_SYSTEMCTL_DST) ;
        cli_system("/bin/systemctl enable %s", SFX_TOOLS) ;
        cli_system("/bin/systemctl start %s", SFX_TOOLS) ;
        sfxini->system_init_type = SYSINIT_TYPE__SYSTEMD ;
        if ( sfxini__write() != RC_OK ) {
            sprintf(smg,"Failed to update %s\n", SFXINI_FILENAME) ;
            fprintf(stdout, smg) ;
            bret = -1;
        }
    } while(0) ;
    return bret;
}

int
sfxtool_cmdhdlr__install_sysv()
{
    char smg[256] ;
    sfxini_t *sfxini ;
    int bret = 0;
    do {
        if ( access(SYS_VINIT_DIR, F_OK) != 0 ) {
            sprintf(smg,"Failed to find %s dir for systemd", SYSTEMCTL_DIR) ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        if ( access(SFX_TOOLS_SYS_VINIT_DST, F_OK) == 0 ) {
            sprintf(smg,"sfx-tools for SysV-init is installed already.\n") ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        if ( access(SYS_VINIT_DIR, W_OK) != 0 ) {
            sprintf(smg,"No access privilege to install sfx-tools into %s\n",
                          SYS_VINIT_DIR) ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        if ( access(SFX_TOOLS_SYS_VINIT_SRC, F_OK) != 0 ) {
            sprintf(smg,"Failed to find %s, please re-run sfx-app-install.sh\n",
                          SFX_TOOLS_SYS_VINIT_SRC) ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        sfxini = sfxini__read() ;
        if ( sfxini->system_init_type == SYSINIT_TYPE__SYSTEMD ) {
            sprintf(smg,"sfx-tools for systemd has installed. uninstall it first\n") ;
            fprintf(stdout, smg) ;
            bret = -1;
            break ;
        }
        bret = cli_system("/bin/cp %s %s", SFX_TOOLS_SYS_VINIT_SRC, SFX_TOOLS_SYS_VINIT_DST) ;
        cli_system("/bin/chmod +x %s", SFX_TOOLS_SYS_VINIT_DST) ;
        cli_system("chkconfig --add %s", SFX_TOOLS) ;
        cli_system("/sbin/service %s start", SFX_TOOLS) ;
    } while (0) ;
    return bret;
}

//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

