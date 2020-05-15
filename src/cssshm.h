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

#ifndef __CSSSHM_H__
#define __CSSSHM_H__

//----------------------------------------------------------------------------
// MACROS/STRUCTURES                                                        --
//----------------------------------------------------------------------------
#define CSSSHM__DEV_NAME   "/sfx_cssshm"
#define CSSDEV__DEV_MAX     (24)   ///< max dev count
// #define CSSZLIB_WORKER_MAX (256)
#define CSSZLIB_WORKER_MAX (1024)
#define CSSDEV__SESS_MAX    (192)   ///< max sess count
                            
#define CSSSHM__SIGNATURE (0x7a7373632d786673)
#define CSSSHM__SESS_SIGNATURE CSSSHM__SIGNATURE

#define CSSSHM__VERSION   (1) ;

/// @todo Need the local resource manager to monitor the worker life cycle.
///       For now, we use the sequence number to avoid worker count over flow.
#define CSSSHM__GET_WORKER_IDX( _val_ ) ( _val_ & (CSSZLIB_WORKER_MAX - 1) )
#define CSSSHM__ENUM_DESC__2ENUM(_enum_, ...) CSSZLIB_COUNTER_TYPE__ ## _enum_
typedef enum _csszlib_counter_type_t {
#define CSSZLIB_COUNTER_TYPE__ALL(__, ...) \
   /* _enum_,            _name_,             _unit_, _DIV_ */ \
  __( DEFLATE_CNT,       "deflate-cmd"         ,AMT, NONE          )__VA_ARGS__\
  __( DEFLATE_IN,        "deflate-in"          ,BYTE,DEFLATE_CNT   )__VA_ARGS__\
  __( DEFLATE_OUT,       "deflate-out"         ,BYTE,DEFLATE_CNT   )__VA_ARGS__\
  \
  __( SESS_OPEN_CNT,     " sess-open-ok"       ,AMT, NONE          )__VA_ARGS__\
  __( SESS_OPEN_ERR_CNT, " sess-open-err"      ,AMT, NONE          )__VA_ARGS__\
  __( SESS_OPEN_RETRY,   " sess-open-retry"    ,TIME,SESS_OPEN_CNT )__VA_ARGS__\
  __( SESS_OPEN_TIME,    " sess-open-time"     ,TIME,SESS_OPEN_CNT )__VA_ARGS__\
  \
  __( HYBRID_CMD_CNT,    " hybrid-cmd"         ,AMT, NONE          )__VA_ARGS__\
  __( HYBRID_CMD_TIME,   " hybrid-cmd-time"    ,TIME,HYBRID_CMD_CNT)__VA_ARGS__\
  __( HYBRID_CMD_SZ,     " hybrid-cmd-size"    ,BYTE,HYBRID_CMD_CNT)__VA_ARGS__\
  __( CPU_FRAME_TIME,    "  cpu-frame-time"    ,TIME,HYBRID_CMD_CNT)__VA_ARGS__\
  __( DEV_CMD_PUSH_ERR,  "  dev-cmd-push-err"  ,AMT, NONE          )__VA_ARGS__\
  __( DEV_CMD_PUSH_RETRY,"  dev-cmd-push-retry",AMT, HYBRID_CMD_CNT)__VA_ARGS__\
  __( DEV_CMD_PUSH_TIME, "  dev-cmd-push-time" ,TIME,HYBRID_CMD_CNT)__VA_ARGS__\
  __( DEV_RSP_PULL_ERR,  "  dev-rsp-pull-err"  ,AMT, NONE          )__VA_ARGS__\
  __( DEV_RSP_PULL_RETRY,"  dev-rsp-pull-retry",AMT, HYBRID_CMD_CNT)__VA_ARGS__\
  __( DEV_RSP_PULL_TIME, "  dev-rsp-pull-time" ,TIME,HYBRID_CMD_CNT)__VA_ARGS__\
  \
  __( SESS_CLOSE_CNT,    " sess-close-ok"      ,AMT, NONE          )__VA_ARGS__\
  __( SESS_CLOSE_ERR_CNT," sess-close-err"     ,AMT, NONE          )__VA_ARGS__\
  __( SESS_CLOSE_TIME,   " sess-close-time"    ,TIME,SESS_CLOSE_CNT)__VA_ARGS__\
  \
  __( CPU_CMD_CNT,       " cpu-cmd"            ,AMT, NONE          )__VA_ARGS__\
  __( CPU_CMD_TIME,      " cpu-cmd-time"       ,TIME,CPU_CMD_CNT   )__VA_ARGS__\
  __( CPU_CMD_SZ,        " cpu-cmd-size"       ,BYTE,CPU_CMD_CNT   )__VA_ARGS__\
  \
  __( INFLATE_CNT,       "inflate-cmd"         ,AMT, NONE          )__VA_ARGS__\
  __( INFLATE_IN,        "inflate-in"          ,BYTE,INFLATE_CNT   )__VA_ARGS__\
  __( INFLATE_OUT,       "inflate-out"         ,BYTE,INFLATE_CNT   )__VA_ARGS__\
  __( INFLATE_TIME,      "inflate-time"        ,TIME,INFLATE_CNT   )__VA_ARGS__\
/* End of CSSZLIB_COUNTER_TYPE__ALL */

    CSSZLIB_COUNTER_TYPE__ALL( CSSSHM__ENUM_DESC__2ENUM, CCS_SYMBOL_COMMAS )
    CSSZLIB_COUNTER_TYPE__CNT,

    CSSZLIB_COUNTER_TYPE__NONE,

    CSSZLIB_COUNTER_TYPE__AMT,
    CSSZLIB_COUNTER_TYPE__TIME,
    CSSZLIB_COUNTER_TYPE__BYTE,
    CSSZLIB_COUNTER_TYPE__LINE,

    /// @note reserve extra counters for extension.
    CSSZLIB_COUNTER_TYPE__CNT_RESERVED = 28,

    CSSZLIB_COUNTER_TYPE__WATCH = CSSSHM__ENUM_DESC__2ENUM( DEFLATE_CNT ),
} csszlib_counter_type_t ;

typedef struct _cssshm_worker_t {

#define CSSSHM_COUNTER( _worker_, _enum_ ) \
        (_worker_)->counters[ CSSSHM__ENUM_DESC__2ENUM(_enum_) ]
/// csszlib internal macro
#define CSSSHM_T_COUNTER( _enum_ ) CSSSHM_COUNTER( worker_cb->worker, _enum_ )

    /// @note reserve 32 byte for control block
    union { uint8_t _cb[32] ; struct {
        uint64_t signature ;
        uint64_t ts_beg ;
        pid_t    pid ;
    } ; } ;
    uint64_t counters[ CSSZLIB_COUNTER_TYPE__CNT_RESERVED ] ;
} cssshm_worker_t ;

typedef enum _csszlib_engine_type_t {
    CSSZLIB_ENGINE_TYPE__HYBRID = 0x00,
    CSSZLIB_ENGINE_TYPE__CPU    = 0x01,
} csszlib_engine_type_t ;

typedef enum _cssshm_sess_flags_t {
#define CSSSHM_SESS_FLAG__ALL(__, ...) \
   /* _enum_,                     _name_ */ \
  __( CSSSHM_SESS_FLAG__RUNNING,  "running" ) __VA_ARGS__ \
  __( CSSSHM_SESS_FLAG__WATCH,    "watch"   ) __VA_ARGS__ \
  __( CSSSHM_SESS_FLAG__DROP,     "drop"    ) __VA_ARGS__ \
/* End of CSSSHM_SESS_FLAG__ALL */

    CSSSHM_SESS_FLAG__ALL( CSS_NAME_ENUM_TO_BIT_ENUM, CCS_SYMBOL_COMMAS )
    CSSSHM_SESS_FLAG__NONE = 0,
} cssshm_sess_flags_t ;

typedef struct _cssshm_sess_t {
    union { uint8_t _cb[32] ; struct {
        uint64_t signature ;
        uint64_t ina_deflate_cnt ;  ///< last inactive deflate cnt
        pid_t    pid ;
        uint16_t worker_idx ;
        uint16_t dev_id ;
        uint16_t sess_id ;
        uint8_t  flags ;
    } ; } ;
} cssshm_sess_t ;

typedef struct _cssshm_dev_t {
    cssshm_sess_t   sesses[ CSSDEV__SESS_MAX ] ;
} cssshm_dev_t;

typedef struct _sfx_tools_cb_t {
#define SFX_TOOLS__GREEN_ZONE 85
    union { uint8_t _cb[8] ; struct {
        uint8_t fsb_modified ;
        uint8_t fsb_green_zone ;
    } ; } ;
} sfx_tools_cb_t ;

typedef struct _cssshm_t {
    /// @note reserve 32 byte for control block
    union { uint8_t _cb[32] ; struct {
        uint64_t       signature ;
        sfx_tools_cb_t sfx_tools_cb ;
        uint32_t       worker_cnt ;
        uint8_t        version ;
        int8_t         engine_type ;
    } ; } ;
    /// Reserve 4k - 32 payload for extensions
    union { uint8_t _payload[4096 - 32] ; struct {
        int8_t   dummy ;
    } ; } ;
    /*
    cssshm_worker_t workers[ CSSZLIB_WORKER_MAX ] ;
    cssshm_dev_t devs[ CSSDEV__DEV_MAX ] ;
    int8_t devmap[ CSSDEV__DEV_MAX ];
    */
} cssshm_t ;

//----------------------------------------------------------------------------
// EXPORTED VARIABLES/FUNCTIONS                                             --
//----------------------------------------------------------------------------
/// @param create - true: create file is file is not exist
extern cssshm_t* cssshm_init(bool_t create) ;
#endif

//----------------------------------------------------------------------------
// EOF                                                                      --
//----------------------------------------------------------------------------

