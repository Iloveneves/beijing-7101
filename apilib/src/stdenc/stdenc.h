/***********************************************************************************
File Name   : stdenc.h

Description : DENC module header file

COPYRIGHT (C) STMicroelectronics 2003

Date               Modification                                             Name
----               ------------                                             ----
06.06.00           Creation.                                                JG
16.10.00           Modification for multi-init ...,                         JG
 *                 Hal for STV0119 and STI55xx,70xx
06 Feb 2001        Reorganize HAL for providing mutual exclusion            HSdLM
 *                 on registers access
21 Feb 2001        Add square pixel, 'PAL N Argentina' and non-interlaced   HSdLM
 *                 features.
22 Jun 2001        Add SECAM/Square pixel capability, new parameters to     HSdLM
 *                 set 444 input, luma trap filter and chroma delay.
 *                 Rename some exported symbols to respect coding rule
 *                 about prefixes.
30 Aou 2001        Remove dependency upon STI2C if not needed, to support   HSdLM
 *                 ST40GX1. Add DeviceType GX1.
03 Dec 2001        Fix DDTS GNBvd10146 for wrapper around STAPI.            HSdLM
16 Apr 2002        Add support for STi7020                                  HSdLM
27 Jun 2003        Add support for new cell version 12 (auxilary encoder)   HSdLM
27 Feb 2004        Add support for STi5100                                  MH
05 Jul 2004        Add 7710/mb391 support                                   TA
************************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __STDENC_H
#define __STDENC_H

/* Includes --------------------------------------------------------------- */
#include "stddefs.h"
/* use I2C to access external DENC on board mb295 (STi7015/20)
 * STDENC_I2C is already defined from makefile when building STDENC libray
 * but not defined when including this file in application build */
#if defined (mb295) && !defined (STDENC_I2C)
#define STDENC_I2C
#endif

#ifdef STDENC_I2C
#include "sti2c.h"
#endif



/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/* Exported Constants ----------------------------------------------------- */

#define STDENC_DRIVER_ID      17
#define STDENC_DRIVER_BASE    (STDENC_DRIVER_ID << 16)

enum
{
    STDENC_ERROR_I2C = STDENC_DRIVER_BASE,
    STDENC_ERROR_INVALID_ENCODING_MODE
};

typedef enum STDENC_EMIAccessWidth_e
{
    STDENC_EMI_ACCESS_WIDTH_8_BITS,
    STDENC_EMI_ACCESS_WIDTH_32_BITS
} STDENC_EMIAccessWidth_t;


typedef enum STDENC_DeviceType_e
{
    STDENC_DEVICE_TYPE_DENC,
    STDENC_DEVICE_TYPE_7015,
    STDENC_DEVICE_TYPE_7020,
    STDENC_DEVICE_TYPE_GX1,
    STDENC_DEVICE_TYPE_4629,
    STDENC_DEVICE_TYPE_V13
} STDENC_DeviceType_t;

typedef enum STDENC_AccessType_e
{
    STDENC_ACCESS_TYPE_EMI,
    STDENC_ACCESS_TYPE_I2C
} STDENC_AccessType_t;

typedef enum STDENC_Mode_e
{
    STDENC_MODE_NONE,
    STDENC_MODE_NTSCM,
    STDENC_MODE_NTSCM_J,
    STDENC_MODE_NTSCM_443,
    STDENC_MODE_PALBDGHI,
    STDENC_MODE_PALM,
    STDENC_MODE_PALN,
    STDENC_MODE_PALN_C,
    STDENC_MODE_SECAM,
    STDENC_MODE_SECAM_AUX
} STDENC_Mode_t;

typedef enum STDENC_Option_e
{
    STDENC_OPTION_BLACK_LEVEL_PEDESTAL,
    STDENC_OPTION_BLACK_LEVEL_PEDESTAL_AUX,
    STDENC_OPTION_CHROMA_DELAY,
    STDENC_OPTION_CHROMA_DELAY_AUX,
    STDENC_OPTION_LUMA_TRAP_FILTER,
    STDENC_OPTION_LUMA_TRAP_FILTER_AUX,
    STDENC_OPTION_FIELD_RATE_60HZ,
    STDENC_OPTION_NON_INTERLACED,
    STDENC_OPTION_SQUARE_PIXEL,
    STDENC_OPTION_YCBCR444_INPUT
} STDENC_Option_t;

/* Exported Types --------------------------------------------------------- */

typedef U32 STDENC_Handle_t;

typedef struct STDENC_EMIAccess_s
{
    void *                  BaseAddress_p;
    void *                  DeviceBaseAddress_p;
    STDENC_EMIAccessWidth_t Width;
} STDENC_EMIAccess_t;

#ifdef STDENC_I2C
typedef struct STDENC_I2CAccess_s
{
    ST_DeviceName_t         DeviceName;
    STI2C_OpenParams_t      OpenParams;
} STDENC_I2CAccess_t;
#endif /* #ifdef STDENC_I2C */

typedef struct STDENC_InitParams_s
{
    STDENC_DeviceType_t     DeviceType;
    U32                     MaxOpen;
    STDENC_AccessType_t     AccessType;
    union
    {
        STDENC_EMIAccess_t  EMI;
#ifdef STDENC_I2C
        STDENC_I2CAccess_t  I2C;
#endif /* #ifdef STDENC_I2C */
    } STDENC_Access;
} STDENC_InitParams_t;

typedef struct STDENC_OpenParams_s
{
    U8 NoUsed;
} STDENC_OpenParams_t;

typedef struct STDENC_TermParams_s
{
    BOOL                    ForceTerminate;
} STDENC_TermParams_t;

typedef struct STDENC_Capability_s
{
    U16  Modes;
    U16  Interlaced;
    U16  SquarePixel;
    U16  FieldRate60Hz;
    S8   MinChromaDelay;
    S8   MaxChromaDelay;
    U8   StepChromaDelay;
    BOOL BlackLevelPedestalAux;
    BOOL ChromaDelayAux;
    BOOL LumaTrapFilterAux;
    BOOL YCbCr444Input;
    U8   CellId;
    STDENC_DeviceType_t DeviceType;
} STDENC_Capability_t;

typedef struct STDENC_NTSCOption_s
{
    BOOL              FieldRate60Hz;
    BOOL              Interlaced;
    BOOL              SquarePixel;
} STDENC_NTSCOption_t;

typedef struct STDENC_PALOption_s
{
    BOOL              Interlaced;
    BOOL              SquarePixel;
} STDENC_PALOption_t;

typedef struct STDENC_ModeOption_s
{
    STDENC_Option_t      Option;
    union
    {
        BOOL              BlackLevelPedestal;
        BOOL              BlackLevelPedestalAux; /* only for macrocell version >= 12 */
        S8                ChromaDelay;   /* in period of pixel clock, so twice pixel delay */
        S8                ChromaDelayAux;
        BOOL              LumaTrapFilter;
        BOOL              LumaTrapFilterAux;
        BOOL              FieldRate60Hz;
        BOOL              Interlaced;
        BOOL              SquarePixel;
        BOOL              YCbCr444Input;
    } Value;
} STDENC_ModeOption_t;

typedef struct STDENC_EncodingMode_s
{
    STDENC_Mode_t   Mode;
    union
    {
        STDENC_NTSCOption_t  Ntsc;
        STDENC_PALOption_t   Pal;
    } Option;
} STDENC_EncodingMode_t;

/* Exported Variables ----------------------------------------------------- */

/* Exported Macros -------------------------------------------------------- */

/* Exported Functions ----------------------------------------------------- */

ST_Revision_t  STDENC_GetRevision (
                    void
                    );
ST_ErrorCode_t STDENC_GetCapability (
                    const ST_DeviceName_t          DeviceName,
                    STDENC_Capability_t *          const Capability_p
                    );

ST_ErrorCode_t STDENC_Init (
                    const ST_DeviceName_t          DeviceName,
                    const STDENC_InitParams_t *    const InitParams_p
                    );
ST_ErrorCode_t STDENC_Open (
                    const ST_DeviceName_t          DeviceName,
                    const STDENC_OpenParams_t *    const OpenParams_p,
                    STDENC_Handle_t *              const Handle_p
                    );
ST_ErrorCode_t STDENC_Close (
                    const STDENC_Handle_t          Handle
                    );
ST_ErrorCode_t STDENC_Term (
                    const ST_DeviceName_t          DeviceName,
                    const STDENC_TermParams_t *    const TermParams_p
                    );

ST_ErrorCode_t STDENC_SetEncodingMode (
                    const STDENC_Handle_t          Handle,
                    const STDENC_EncodingMode_t *  const EncodingMode_p
                    );
ST_ErrorCode_t STDENC_GetEncodingMode (
                    const STDENC_Handle_t          Handle,
                    STDENC_EncodingMode_t *        const EncodingMode_p
                    );
ST_ErrorCode_t STDENC_SetModeOption (
                    const STDENC_Handle_t          Handle,
                    const STDENC_ModeOption_t *    const ModeOption_p
                    );
ST_ErrorCode_t STDENC_GetModeOption (
                    const STDENC_Handle_t          Handle,
                    STDENC_ModeOption_t *          const ModeOption_p
                    );

ST_ErrorCode_t STDENC_SetColorBarPattern(
                    const STDENC_Handle_t          Handle,
                    const BOOL                     ColorBarPattern
                    );

/* Library Restricted exported functions for DENC Registers Accesses
 * These functions must only be used by others stapi libraries
 */
ST_ErrorCode_t STDENC_CheckHandle (     STDENC_Handle_t Handle );
void           STDENC_RegAccessLock (   STDENC_Handle_t Handle );
void           STDENC_RegAccessUnlock ( STDENC_Handle_t Handle );
ST_ErrorCode_t STDENC_ReadReg8 (        STDENC_Handle_t Handle, const U32 Addr, U8 * const Value_p                );
ST_ErrorCode_t STDENC_WriteReg8 (       STDENC_Handle_t Handle, const U32 Addr, const U8 Value                    );
ST_ErrorCode_t STDENC_AndReg8 (         STDENC_Handle_t Handle, const U32 Addr, const U8 AndMask                  );
ST_ErrorCode_t STDENC_OrReg8 (          STDENC_Handle_t Handle, const U32 Addr, const U8 OrMask                   );
ST_ErrorCode_t STDENC_MaskReg8 (        STDENC_Handle_t Handle, const U32 Addr, const U8 AndMask, const U8 OrMask );

/* For backward compatibility */
#define stdencra_CheckHandle(Handle)                          STDENC_CheckHandle(    (Handle))
#define stdencra_Lock(       Handle)                          STDENC_RegAccessLock(  (Handle))
#define stdencra_Unlock(     Handle)                          STDENC_RegAccessUnlock((Handle))
#define stdencra_ReadReg8(   Handle, Addr, Value_p)           STDENC_ReadReg8(       (Handle), (Addr), (Value_p)          )
#define stdencra_WriteReg8(  Handle, Addr, Value)             STDENC_WriteReg8(      (Handle), (Addr), (Value)            )
#define stdencra_AndReg8(    Handle, Addr, AndMask)           STDENC_AndReg8(        (Handle), (Addr), (AndMask)          )
#define stdencra_OrReg8(     Handle, Addr, OrMask)            STDENC_OrReg8(         (Handle), (Addr), (OrMask)           )
#define stdencra_MaskReg8(   Handle, Addr, AndMask, OrMask )  STDENC_MaskReg8(       (Handle), (Addr), (AndMask), (OrMask))

/* C++ support */
#ifdef __cplusplus
}
#endif

#endif /* #ifndef __STDENC_H */

/* End of stdenc.h */

