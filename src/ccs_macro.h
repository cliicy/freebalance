/// @par Description:
/// The Description of this file.
/// @par Copyright:
/// /// Copyright (c) by ScaleFlux, Inc.
///
/// ALL RIGHTS RESERVED. These coded instructions and program statements are
/// copyrighted works and confidential proprietary information of ScaleFlux, Inc.
/// They may not be modified, copied, reproduced, distributed, or disclosed to
/// third parties in any manner, medium, or form, in whole or in part.
///

#ifndef __CCS_MACRO_H__
#define __CCS_MACRO_H__

//----------------------------------------------------------------------------
// MACROS/STRUCTURES                                                        --
//----------------------------------------------------------------------------

#ifndef NMEL
/// get the element count at compilation time
#define NMEL( _ary_ ) (sizeof(_ary_) / sizeof( (_ary_)[0] ))
#endif

/// For a struct type, get the offset of a specified filed
#define FLD_OFFSET(_struct_t_, _fld_)   ( (uint64_t)&((_struct_t_ *)0)->_fld_ )
/// For a struct type, get the sizeof of a specified field
#define FLD_SIZE(  _struct_t_, _fld_)   ( sizeof(((_struct_t_ *)0)->_fld_) )
/// For a struct type, get the number-of-element count of a specified field
#define FLD_NMEL(  _struct_t_, _fld_)   ( NMEL(((_struct_t_ *)0)->_fld_) )

/// set/unset the _flags_ to the _fValue_
#define CCS_FLAG__SET(  _fValue_, _flags_ )  (_fValue_ |=  (_flags_))
#define CCS_FLAG__UNSET(_fValue_, _flags_ )  (_fValue_ &= ~(_flags_))
/// check the _fValue_ has all/any _flags_
#define CCS_FLAG__HASALL(  _fValue_, _flags_ )  ( ((_fValue_) & (_flags_)) == (_flags_) )
#define CCS_FLAG__HASANY(  _fValue_, _flags_ )  ( ((_fValue_) & (_flags_)) )

/// Define ASCII symbol
#define CCS_SYMBOL_COMMAS        ,
#define CCS_SYMBOL_SEMICOLON     ;
#define CCS_SYMBOL_COLLON        :
#define CCS_SYMBOL_NONE


/// Macro defined enum type
#define CCS_ENUM_DESC__2ENUM( _enum_, _desc_, ... )   _enum_ 
#define CCS_ENUM_DESC__2NAME( _enum_, _desc_, ... ) #_enum_
#define CCS_ENUM_DESC__2DESC( _enum_, _desc_, ... )   _desc_

/// this macro creates _enum_ ## _IDX and _enum_ enum lists
#define CSS_NAME_ENUM_TO_BIT_ENUM( _enum_, _name_, ...) \
    _enum_ ## _IDX, \
    _enum_ = (1 << _enum_ ## _IDX), \
    _enum_ ## _IDX_DUP = _enum_ ## _IDX \
    /* End of GT_NAME_ENUM_TO_BIT_ENUM */

#define CCS_ENUM_DESC__2STR_FUNC( _prefix_, _func_name_, _struct_t_ ) \
    const char* _func_name_( _struct_t_ enumValue ) { \
        static char _func_name_ ## EnumDescs[ _prefix_ ## _CNT + 1][30] = { \
            _prefix_ ## _ALL( CCS_ENUM_DESC__2DESC, CCS_SYMBOL_COMMAS ) \
            "Out_of_Boundary", \
        } ; \
        return _func_name_ ## EnumDescs[ \
            enumValue < 0 || enumValue >= _prefix_ ## _CNT ? _prefix_ ## _CNT \
                                                           : enumValue ] ; \
} /* End of CCS_ENUM_DESC__2STR_FUNC */

/// safe min/max macros - only evaluate arguments once each
#ifndef min
# define min(a, b) ({ typeof(a) __a = (a); \
                      typeof(b) __b = (b); \
                      __a > __b ? __b : __a; \
                   })
#endif
#ifndef max
# define max(a, b) ({ typeof(a) __a = (a); \
                      typeof(b) __b = (b); \
                      __a > __b ? __a : __b; \
                   })
#endif
/// safe maximum of 3 values - only evaluate arguments once each
#define max3(a, b, c) ({ typeof(max((a), (b))) __t; \
                         __t = max((a), (b)); \
                         max(__t, (c)); \
                      })

/// safe minimum of 3 values - only evaluate arguments once each
#define min3(a, b, c) ({ typeof(min((a), (b))) __t; \
                         __t = min((a), (b)); \
                         min(__t, (c)); \
                      })

/** allocate me a new `p' */
#define new_ptr(p)	   	    ((p) = (typeof(p))malloc(sizeof *(p)))
/** like new_ptr() but clears it */
#define new_0(p)		    ((p) = (typeof(p))calloc(1, sizeof *(p)))
/** allocate me a table of `p's */
#define new_array(p, n)		((p) = (typeof(p))malloc((sizeof *(p)) * (n)))
#define new_array0(p, n)	((p) = (typeof(p))calloc((sizeof *(p)), (n)))
/** allocate me a new `p' of a given size */
#define new_ptrsz(p, len)	((p) = (typeof(p))malloc(len))
#define new_ptrsz0(p, len)	((p) = (typeof(p))calloc(len, 1))

#define alloca_ptr(p)		((p) = (typeof(p))alloca(sizeof *(p)))
/// consider using gcc's varsized arrays
#define alloca_array(p, n)	((p) = (typeof(p))alloca((sizeof *(p)) * (n)))
#define alloca_ptrsz(p,len)	((p) = (typeof(p))alloca(len))
#define astrsave(n, s)		((n) = alloca(strlen(s) + 1), strcpy((n), (s)))

#define zero(p)			memset((p), '\0', sizeof *(p))

/// Useful numbers
#define         _1K                     1024
#define         _2K                     (2*_1K)
#define         _4K                     (4*_1K)
#define         _8K                     (8*_1K)
#define         _16K                    (16*_1K)
#define         _32K                    (32*_1K)
#define         _64K                    (64*_1K)
#define         _128K                   (128*_1K)
#define         _256K                   (256*_1K)
#define         _512K                   (512*_1K)
#define         _1M                     (1024*_1K)
#define         _2M                     (2*_1M)
#define         _4M                     (4*_1M)
#define         _8M                     (8*_1M)
#define         _16M                    (16*_1M)
#define         _32M                    (32*_1M)
#define         _64M                    (64*_1M)
#define         _128M                   (128*_1M)
#define         _256M                   (256*_1M)
#define         _512M                   (512*_1M)
#define         _1G                     (1024*_1M)
#define         _2G                     (2*_1G)
#define         _1T                     (1024LL*_1G)
#define         _1P                     (1024LL*_1T)

#if !defined(true) && !defined(TRUE)
typedef unsigned char bool_t;

#define false    0
#define true     1

#endif

typedef enum _RC_t {
#define RC__ALL(__, ... ) \
  __( RC_OK,    "Ok"    ) __VA_ARGS__ \
  __( RC_ERROR, "Error" ) __VA_ARGS__ \
  // End of RC__ALL

    RC__ALL( CCS_ENUM_DESC__2ENUM, CCS_SYMBOL_COMMAS )
    RC__CNT,

} RC_t ;

//----------------------------------------------------------------------------
// EXPORTED VARIABLES/FUNCTIONS                                             --
//----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

extern const char* RC_getName( RC_t rc ) ; ///< return rc description

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

