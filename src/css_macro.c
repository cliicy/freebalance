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

#include "ccs_base.h"

//----------------------------------------------------------------------------
// PROTOTYPES/VARIABLES                                                     --
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTIONS                                                                --
//----------------------------------------------------------------------------
const char*
RC_getName( RC_t rc ) 
{
    return rc == RC_OK ? "OK" : "Error" ;
}

//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

