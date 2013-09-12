/*****************************************************************************
 *
 *  Module      : stlayer_ioctl
 *  Date        : 13-11-2005
 *  Author      : AYARITAR
 *  Description :
 *  Copyright   : STMicroelectronics (c)2005
 *****************************************************************************/


#ifndef STLAYER_IOCTL_H
#define STLAYER_IOCTL_H

#include <linux/ioctl.h>   /* Defines macros for ioctl numbers */


#include "stlayer.h"
#include "layer_rev.h"
#include "stos.h"

/* Magic number used to differenciate STLAYER IOCTL with other drivers' */
#define STLAYER_MAGIC_NUMBER      41

/*** IOCtl defines ***/

#define STLAYER_IOCTL_TYPE   0XFF     /* Unique module id - See 'ioctl-number.txt' */

/* Use STLAYER_MAGIC_NUMBER as magic number */
#define STLAYER_IOC_INIT                    _IO(STLAYER_MAGIC_NUMBER, 0)
#define STLAYER_IOC_TERM                    _IO(STLAYER_MAGIC_NUMBER, 1)
#define STLAYER_IOC_OPEN                    _IO(STLAYER_MAGIC_NUMBER, 2)
#define STLAYER_IOC_CLOSE                   _IO(STLAYER_MAGIC_NUMBER, 3)
#define STLAYER_IOC_GETCAPABILITY           _IO(STLAYER_MAGIC_NUMBER, 4)
#define STLAYER_IOC_GETINITALLOCPARAMS      _IO(STLAYER_MAGIC_NUMBER, 5)
#define STLAYER_IOC_GETLAYERPARAMS          _IO(STLAYER_MAGIC_NUMBER, 6)
#define STLAYER_IOC_SETLAYERPARAMS          _IO(STLAYER_MAGIC_NUMBER, 7)
#define STLAYER_IOC_OPENVIEWPORT            _IO(STLAYER_MAGIC_NUMBER, 8)
#define STLAYER_IOC_CLOSEVIEWPORT           _IO(STLAYER_MAGIC_NUMBER, 9)
#define STLAYER_IOC_ENABLEVIEWPORT          _IO(STLAYER_MAGIC_NUMBER, 10)
#define STLAYER_IOC_DISABLEVIEWPORT         _IO(STLAYER_MAGIC_NUMBER, 11)
#define STLAYER_IOC_ADJUSTVIEWPORTPARAMS    _IO(STLAYER_MAGIC_NUMBER, 12)
#define STLAYER_IOC_SETVIEWPORTPARAMS       _IO(STLAYER_MAGIC_NUMBER, 13)
#define STLAYER_IOC_GETVIEWPORTPARAMS       _IO(STLAYER_MAGIC_NUMBER, 14)
#define STLAYER_IOC_SETVIEWPORTSOURCE       _IO(STLAYER_MAGIC_NUMBER, 15)
#define STLAYER_IOC_GETVIEWPORTSOURCE       _IO(STLAYER_MAGIC_NUMBER, 16)
#define STLAYER_IOC_SETVIEWPORTIORECTANGLE  _IO(STLAYER_MAGIC_NUMBER, 17)
#define STLAYER_IOC_ADJUSTIORECTANGLE       _IO(STLAYER_MAGIC_NUMBER, 18)
#define STLAYER_IOC_GETVIEWPORTIORECTANGLE  _IO(STLAYER_MAGIC_NUMBER, 19)
#define STLAYER_IOC_GETVIEWPORTPOSITION     _IO(STLAYER_MAGIC_NUMBER, 20)
#define STLAYER_IOC_SETVIEWPORTPOSITION     _IO(STLAYER_MAGIC_NUMBER, 21)
#define STLAYER_IOC_GETVIEWPORTPSI          _IO(STLAYER_MAGIC_NUMBER, 22)
#define STLAYER_IOC_SETVIEWPORTPSI          _IO(STLAYER_MAGIC_NUMBER, 23)
#define STLAYER_IOC_GETVIEWPORTSPECIALMODE  _IO(STLAYER_MAGIC_NUMBER, 24)
#define STLAYER_IOC_SETVIEWPORTSPECIALMODE  _IO(STLAYER_MAGIC_NUMBER, 25)
#define STLAYER_IOC_DISABLECOLORKEY         _IO(STLAYER_MAGIC_NUMBER, 26)
#define STLAYER_IOC_ENABLECOLORKEY          _IO(STLAYER_MAGIC_NUMBER, 27)
#define STLAYER_IOC_SETVIEWPORTCOLORKEY     _IO(STLAYER_MAGIC_NUMBER, 28)
#define STLAYER_IOC_GETVIEWPORTCOLORKEY     _IO(STLAYER_MAGIC_NUMBER, 29)
#define STLAYER_IOC_DISABLEBORDERALPHA      _IO(STLAYER_MAGIC_NUMBER, 30)
#define STLAYER_IOC_ENABLEBORDERALPHA       _IO(STLAYER_MAGIC_NUMBER, 31)
#define STLAYER_IOC_GETVIEWPORTALPHA        _IO(STLAYER_MAGIC_NUMBER, 32)
#define STLAYER_IOC_SETVIEWPORTALPHA        _IO(STLAYER_MAGIC_NUMBER, 33)
#define STLAYER_IOC_SETVIEWPORTGAIN         _IO(STLAYER_MAGIC_NUMBER, 34)
#define STLAYER_IOC_GETVIEWPORTGAIN         _IO(STLAYER_MAGIC_NUMBER, 35)
#define STLAYER_IOC_SETVIEWPORTRECORDABLE   _IO(STLAYER_MAGIC_NUMBER, 36)
#define STLAYER_IOC_GETVIEWPORTRECORDABLE   _IO(STLAYER_MAGIC_NUMBER, 37)
#define STLAYER_IOC_GETBITMAPALLOCPARAMS    _IO(STLAYER_MAGIC_NUMBER, 38)
#define STLAYER_IOC_GETBITMAPHEADERSIZE     _IO(STLAYER_MAGIC_NUMBER, 39)
#define STLAYER_IOC_GETPALETTEALLOCPARAMS   _IO(STLAYER_MAGIC_NUMBER, 40)
#define STLAYER_IOC_GETVTGNAME              _IO(STLAYER_MAGIC_NUMBER, 41)
#define STLAYER_IOC_GETVTGPARAMS            _IO(STLAYER_MAGIC_NUMBER, 42)
#define STLAYER_IOC_UPDATEFROMMIXER         _IO(STLAYER_MAGIC_NUMBER, 43)
#define STLAYER_IOC_DISABLEVIEWPORTFILTER   _IO(STLAYER_MAGIC_NUMBER, 44)
#define STLAYER_IOC_ENABLEVIEWPORTFILTER    _IO(STLAYER_MAGIC_NUMBER, 45)
#define STLAYER_IOC_ATTACHALPHAVIEWPORT     _IO(STLAYER_MAGIC_NUMBER, 46)
#define STLAYER_IOC_ALLOCDATA               _IO(STLAYER_MAGIC_NUMBER, 47)
#define STLAYER_IOC_FREEDATA                _IO(STLAYER_MAGIC_NUMBER, 48)
#define STLAYER_IOC_ALLOCDATASECURE         _IO(STLAYER_MAGIC_NUMBER, 49)
#define STLAYER_IOC_SETFLICKERFILTERMODE    _IO(STLAYER_MAGIC_NUMBER, 50)
#define STLAYER_IOC_GETFLICKERFILTERMODE    _IO(STLAYER_MAGIC_NUMBER, 51)
#define STLAYER_IOC_DISABLEVIEWPORTCOLORFILL    _IO(STLAYER_MAGIC_NUMBER, 52)
#define STLAYER_IOC_ENABLEVIEWPORTCOLORFILL    _IO(STLAYER_MAGIC_NUMBER, 53)
#define STLAYER_IOC_SETVIEWPORTCOLORFILL    _IO(STLAYER_MAGIC_NUMBER, 54)
#define STLAYER_IOC_GETVIEWPORTCOLORFILL    _IO(STLAYER_MAGIC_NUMBER, 55)
#define STLAYER_IOC_INFORMPICTURETOBEDECODED    _IO(STLAYER_MAGIC_NUMBER, 56)
#define STLAYER_IOC_COMMITVIEWPORTPARAMS    _IO(STLAYER_MAGIC_NUMBER, 57)
#ifdef STVID_DEBUG_GET_DISPLAY_PARAMS
#define STLAYER_IOC_GETVIDEODISPLAYPARAMS    _IO(STLAYER_MAGIC_NUMBER, 58)
#endif

/* Exported Types --------------------------------------------------------- */

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    ST_DeviceName_t             DeviceName;
    STLAYER_InitParams_t        InitParams;
} STLAYER_Ioctl_Init_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    ST_DeviceName_t             DeviceName;
    STLAYER_TermParams_t        TermParams;
} STLAYER_Ioctl_Term_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    ST_DeviceName_t             DeviceName;
    STLAYER_OpenParams_t        OpenParams;
    STLAYER_Handle_t            Handle;
} STLAYER_Ioctl_Open_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            Handle;
} STLAYER_Ioctl_Close_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    ST_DeviceName_t             DeviceName;
    STLAYER_Capability_t        Capability;
} STLAYER_Ioctl_GetCapability_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Layer_t             LayerType;
    U32                         ViewPortsNumber;
    STLAYER_AllocParams_t       Params;
} STLAYER_Ioctl_GetInitAllocParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t  ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            Handle;
    STLAYER_LayerParams_t       LayerParams;
} STLAYER_Ioctl_GetLayerParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t  ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            Handle;
    STLAYER_LayerParams_t       LayerParams;
} STLAYER_Ioctl_SetLayerParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            LayerHandle;
    STLAYER_ViewPortParams_t    Params;
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_OpenViewPort_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_CloseViewPort_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_EnableViewPort_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_DisableViewPort_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            LayerHandle;
    STLAYER_ViewPortParams_t    Params;

} STLAYER_Ioctl_AdjustViewPortParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_ViewPortParams_t    Params;

} STLAYER_Ioctl_SetViewPortParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_ViewPortParams_t    Params;

} STLAYER_Ioctl_GetViewPortParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_ViewPortSource_t    VPSource;

} STLAYER_Ioctl_SetViewPortSource_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_FlickerFilterMode_t    FFMode;

} STLAYER_Ioctl_SetFlickerFilterMode_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t       VPHandle;
    STLAYER_FlickerFilterMode_t    FFMode;

} STLAYER_Ioctl_GetFlickerFilterMode_t;



typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_ViewPortSource_t    VPSource;

} STLAYER_Ioctl_GetViewPortSource_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_Rectangle_t         InputRectangle;
    STGXOBJ_Rectangle_t         OutputRectangle;

} STLAYER_Ioctl_SetViewPortIORectangle_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_Rectangle_t         InputRectangle;
    STGXOBJ_Rectangle_t         OutputRectangle;

} STLAYER_Ioctl_AdjustIORectangle_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_Rectangle_t         InputRectangle;
    STGXOBJ_Rectangle_t         OutputRectangle;

} STLAYER_Ioctl_GetViewPortIORectangle_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t VPHandle;
    S32                      XPosition;
    S32                      YPosition;

} STLAYER_Ioctl_SetViewPortPosition_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t VPHandle;
    S32                      XPosition;
    S32                      YPosition;

} STLAYER_Ioctl_GetViewPortPosition_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t  VPHandle;
    STLAYER_PSI_t             VPPSI;

} STLAYER_Ioctl_SetViewPortPSI_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t  VPHandle;
    STLAYER_PSI_t             VPPSI;

} STLAYER_Ioctl_GetViewPortPSI_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_OutputMode_t        OuputMode;
    STLAYER_OutputWindowSpecialModeParams_t Params;

} STLAYER_Ioctl_SetViewPortSpecialMode_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_OutputMode_t        OuputMode;
    STLAYER_OutputWindowSpecialModeParams_t Params;

} STLAYER_Ioctl_GetViewPortSpecialMode_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_DisableColorKey_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_EnableColorKey_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_ColorKey_t          ColorKey;


} STLAYER_Ioctl_SetViewPortColorKey_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_ColorKey_t          ColorKey;

} STLAYER_Ioctl_GetViewPortColorKey_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_DisableViewportColorFill_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_EnableViewportColorFill_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_ColorARGB_t          ColorFill;


} STLAYER_Ioctl_SetViewportColorFill_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_ColorARGB_t          ColorFill;

} STLAYER_Ioctl_GetViewportColorFill_t;



typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_DisableBorderAlpha_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_EnableBorderAlpha_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_GlobalAlpha_t       Alpha;

} STLAYER_Ioctl_SetViewPortAlpha_t ;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_GlobalAlpha_t       Alpha;

} STLAYER_Ioctl_GetViewPortAlpha_t;



typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_GainParams_t        Params;

} STLAYER_Ioctl_SetViewPortGain_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_GainParams_t        Params;

} STLAYER_Ioctl_GetViewPortGain_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t   VPHandle;
    BOOL                       Recordable;

} STLAYER_Ioctl_SetViewPortRecordable_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t   VPHandle;
    BOOL                       Recordable;

} STLAYER_Ioctl_GetViewPortRecordable_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    STGXOBJ_Bitmap_t             Bitmap;
    STGXOBJ_BitmapAllocParams_t  Params1;
    STGXOBJ_BitmapAllocParams_t Params2;

} STLAYER_Ioctl_GetBitmapAllocParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    STGXOBJ_Bitmap_t             Bitmap;
    U32                          HeaderSize;

} STLAYER_Ioctl_GetBitmapHeaderSize_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t               ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    STGXOBJ_Palette_t            Palette;
    STGXOBJ_PaletteAllocParams_t Params;

} STLAYER_Ioctl_GetPaletteAllocParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t               ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    ST_DeviceName_t              VTGName;

} STLAYER_Ioctl_GetVTGName_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t               ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    STLAYER_VTGParams_t          VTGParams;

} STLAYER_Ioctl_GetVTGParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t               ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t             LayerHandle;
    STLAYER_OutputParams_t       OutputParams;

} STLAYER_Ioctl_UpdateFromMixer_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_DisableViewPortFilter_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t             ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t   VPHandle;
    STLAYER_Handle_t           FilterHandle;

} STLAYER_Ioctl_EnableViewPortFilter_t;


typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t             ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t   VPHandle;
    STLAYER_Handle_t           MaskedLayer;

} STLAYER_Ioctl_AttachAlphaViewPort_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t          ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t            LayerHandle;
    STLAYER_AllocDataParams_t   AllocDataParams;
    void                        *Address_p;
} STLAYER_Ioctl_AllocData_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t          ErrorCode;

    /* Parameters to the function */
    STLAYER_Handle_t          LayerHandle;
    void                     *Address_p;
} STLAYER_Ioctl_FreeData_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STGXOBJ_PictureInfos_t      PictureInfos;

} STLAYER_Ioctl_InformPictureToBeDecoded_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;

} STLAYER_Ioctl_CommitViewPortParams_t;

typedef struct
{
    /* Error code retrieved by STAPI function */
    ST_ErrorCode_t              ErrorCode;

    /* Parameters to the function */
    STLAYER_ViewPortHandle_t    VPHandle;
    STLAYER_VideoDisplayParams_t        Params;

} STLAYER_Ioctl_GetVideoDisplayParams_t;

#endif
