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
#ifndef __SFXINI_H__
#define __SFXINI_H__

#define SFXINI_FILENAME "/opt/sfx/etc/sfx.ini"
//----------------------------------------------------------------------------
// MACROS/STRUCTURES                                                        --
//----------------------------------------------------------------------------
typedef enum _sysinit_type_t {
    SYSINIT_TYPE__NONE,
    SYSINIT_TYPE__SYSTEMD,
    SYSINIT_TYPE__SYSV,
} sysinit_type ;

typedef struct _sfxini_t {
    int  system_version ;
    int  system_init_type ;
    int  fsb_modified ;
    int  fsb_green_zone ;
} sfxini_t ;

//----------------------------------------------------------------------------
// EXPORTED VARIABLES/FUNCTIONS                                             --
//----------------------------------------------------------------------------
extern sfxini_t* sfxini__read(void) ;
extern RC_t      sfxini__write(void) ;
#endif

//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

