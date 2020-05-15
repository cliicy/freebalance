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

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "ccs_base.h"
#include "ini.h"
#include "sfxini.h"
#include "cssshm.h"

#define SFXINI_VERSION  1

//----------------------------------------------------------------------------
// PROTOTYPES/VARIABLES                                                     --
//----------------------------------------------------------------------------
static sfxini_t sfxini = {
    .system_version = SFXINI_VERSION,
    .fsb_green_zone = SFX_TOOLS__GREEN_ZONE,
} ;

static int sfxini_handle(void* opq, const char* section,
                                    const char* name, const char* val) ;
//----------------------------------------------------------------------------
// FUNCTIONS                                                                --
//----------------------------------------------------------------------------

sfxini_t*
sfxini__read(void)
{
    ini_parse(SFXINI_FILENAME, sfxini_handle, &sfxini) ;
    return &sfxini ;
}

RC_t
sfxini__write(void)
{
    FILE* file ;
    RC_t  rc = RC_ERROR ;

    do {
        if ((file = fopen(SFXINI_FILENAME, "w")) == NULL) {
            break ;
        }
        fprintf(file, "; Scaleflux application configuration\n") ;
        fprintf(file, "\n") ;
        fprintf(file, "[system]\n") ;
        fprintf(file, "version      = %d\n", sfxini.system_version) ;
        fprintf(file, "init-type    = %d\n", sfxini.system_init_type) ;
        fprintf(file, "[sfx-tools]\n");
        if (sfxini.fsb_modified) {
            fprintf(file, "fsb-green-zone = %d\n", sfxini.fsb_green_zone) ;
        }

        fclose(file) ;
        chmod(SFXINI_FILENAME, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) ;
        rc = RC_OK ;
    } while (0) ;

    return rc ;
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
//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

