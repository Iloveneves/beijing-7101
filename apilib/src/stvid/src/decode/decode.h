/*******************************************************************************

File name   : decode.h

Description : Video decode header file

COPYRIGHT (C) STMicroelectronics 2004

Date               Modification                                     Name
----               ------------                                     ----
21 Mar 2000        Created                                           HG
16 Mar 2001        new VIDDEC_ConfigureEvent() function              GG
 6 Aug 2001        Add StartUpdateDisplay() function                 HG
*******************************************************************************/

/* Define to prevent recursive inclusion */

#ifndef __VIDEO_DECODE_H
#define __VIDEO_DECODE_H


/* Includes ----------------------------------------------------------------- */

#include "stddefs.h"

#include "stvid.h"

#include "buffers.h"

#include "multidec.h"

#include "producer.h"

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/* Exported Constants ------------------------------------------------------- */

#define VIDDEC_MODULE_BASE   0x500

/* Bit rate evaluation : nb pictures used */
#define VIDDEC_NB_PICTURES_USED_FOR_BITRATE        30

/* Events which are specific to decode and trickmod modules */
#ifdef ST_smoothbackward
enum
{
    /* Keep facultative events as last events ! */
    VIDDEC_START_CODE_FOUND_EVT,
    VIDDEC_START_CODE_SLICE_FOUND_EVT,
    VIDDEC_DECODER_IDLE_EVT,
    VIDDEC_READY_TO_START_BACKWARD_MODE_EVT
};
#endif

#define VIDDEC_READY_TO_DECODE_NEW_PICTURE_EVT          VIDPROD_READY_TO_DECODE_NEW_PICTURE_EVT
#define VIDDEC_NEW_PICTURE_SKIPPED_NOT_REQUESTED_EVT    VIDPROD_NEW_PICTURE_SKIPPED_NOT_REQUESTED_EVT
#define VIDDEC_STOPPED_EVT                              VIDPROD_STOPPED_EVT
#define VIDDEC_DECODE_ONCE_READY_EVT                    VIDPROD_DECODE_ONCE_READY_EVT
#define VIDDEC_RESTARTED_EVT                            VIDPROD_RESTARTED_EVT
#define VIDDEC_DECODE_LAUNCHED_EVT                      VIDPROD_DECODE_LAUNCHED_EVT
#define VIDDEC_SKIP_LAUNCHED_EVT                        VIDPROD_SKIP_LAUNCHED_EVT
#define VIDDEC_SELF_INSTALLED_INTERRUPT_EVT             VIDPROD_SELF_INSTALLED_INTERRUPT_EVT
#define VIDDEC_NEW_PICTURE_PARSED_EVT                   VIDPROD_NEW_PICTURE_PARSED_EVT

#define VIDDEC_DISPLAYED_NONE               VIDPROD_DISPLAYED_NONE
#define VIDDEC_DISPLAYED_UNKNOWN            VIDPROD_DISPLAYED_UNKNOWN
#define VIDDEC_DISPLAYED_PRIMARY_ONLY       VIDPROD_DISPLAYED_PRIMARY_ONLY
#define VIDDEC_DISPLAYED_SECONDARY_ONLY     VIDPROD_DISPLAYED_SECONDARY_ONLY
#define VIDDEC_DISPLAYED_BOTH               VIDPROD_DISPLAYED_BOTH
#define VIDDEC_DisplayedReconstruction_t VIDPROD_DisplayedReconstruction_t

#define VIDDEC_SKIP_NEXT_B_PICTURE      VIDPROD_SKIP_NEXT_B_PICTURE
#define VIDDEC_SKIP_UNTIL_NEXT_I        VIDPROD_SKIP_UNTIL_NEXT_I
#define VIDDEC_SKIP_UNTIL_NEXT_SEQUENCE VIDPROD_SKIP_UNTIL_NEXT_SEQUENCE
#define VIDDEC_SkipMode_t VIDPROD_SkipMode_t

typedef enum
{
    VIDDEC_DECODE_ACTION_NORMAL,        /* Decode picture then pass it to display */
    VIDDEC_DECODE_ACTION_NO_DISPLAY,    /* Decode picture but don't pass it to display */
    VIDDEC_DECODE_ACTION_SKIP           /* Don't decode picture, skip it before decode */
} VIDDEC_DecodeAction_t;

typedef enum
{
    VIDDEC_DECODE_ERROR_TYPE_NONE,          /* Default. */
    VIDDEC_DECODE_ERROR_TYPE_SYNTAX,        /* Generated by the hw decoder when the data of the picture are not consistant. */
    VIDDEC_DECODE_ERROR_TYPE_OVERFLOW,      /* Generated by the hw decoder when the number of slices is more than expected. */
    VIDDEC_DECODE_ERROR_TYPE_UNDERFLOW,     /* Generated by the hw decoder when the number of slices is less than expected. */
    VIDDEC_DECODE_ERROR_TYPE_MISALIGNMENT,  /* Generated by the sw controller when the amout of data parsed are not         */
                                            /* consistant with the amount of data decoded.                                  */
    VIDDEC_DECODE_ERROR_TYPE_TIMEOUT        /* Generated by the sw controller when the max time given to the hw decoder to  */
                                            /* decode a picture is over.                                                    */
} VIDDEC_DecodeErrorType_t;

#ifdef ST_smoothbackward
typedef enum
{
    VIDDEC_START_CODE_SEARCH_MODE_NORMAL,
    VIDDEC_START_CODE_SEARCH_MODE_FROM_ADDRESS
} VIDDEC_StartCodeSearchMode_t;
#endif

#ifdef ST_smoothbackward
typedef enum
{
    VIDDEC_MANUAL_CONTROL_MODE,
    VIDDEC_AUTOMATIC_CONTROL_MODE
} VIDDEC_ControlMode_t;
#endif

/* Exported Constants ------------------------------------------------------- */

extern const VIDPROD_FunctionsTable_t   VIDDEC_Functions;

/* Exported Types ----------------------------------------------------------- */

#define VIDDEC_Handle_t VIDPROD_Handle_t

typedef struct {
    BOOL    HasProfileErrors; /* TRUE if the display dimensions are not in the MPEG2 profile, FALSE otherwise */
    BOOL    HasCriticalSequenceSyntaxErrors; /* TRUE if criticl error in sequence */
    BOOL    HasRecoveredSequenceSyntaxErrors; /* TRUE if a bad aspect ratio is detected but it is recovered. */
    BOOL    FirstPictureOfSequence; /* TRUE if  */
    BOOL    HasPictureSyntaxErrors; /* TRUE if waiting for data when underflow, FALSE if quitting when underflow */
    U32     NbOfPictureSCPerFirstSliceSC; /* Counter of Picture Start Code Per First Slice Start Code */
    BOOL    PreviousPictureHasAMissingField; /* TRUE if we don't get the 2nd field, FALSE otherwise. */
} VIDDEC_PictureSyntaxError_t; /* Errors in the MPEG1 and MPEG2 flow. */

typedef struct {
    U32     NbOfSyntaxError;                    /* Counter on Syntax error generated by the hw decode.          */
    U32     NbOfCriticalError;                  /* Counter on overflow (hw) / underflow (hw) / timeout (sw).    */
    U32     NbOfMisalignmentError;              /* Counter on misalignment detected by the sw.                  */
    VIDDEC_DecodeErrorType_t DecodeErrorType;   /* Gives the new coming decode error type.                      */
} VIDDEC_PictureDecodeError_t; /* Errors during the decode of a picture. */

typedef struct {
    VIDDEC_PictureSyntaxError_t    PictureSyntaxError;
    VIDDEC_PictureDecodeError_t    PictureDecodeError;
    BOOL                           HasStartCodeParsingError; /* TRUE if the MPEG flow is not correct, FALSE otherwise */
    BOOL                           HasIllegalStartCodeError; /* TRUE if we found a start code in the invalid range, FALSE otherwise. */
} VIDDEC_SyntaxError_t; /* Errors in the MPEG1 and MPEG2 flow. */

#ifdef ST_smoothbackward
/* structure with all necessary infos to decode one picture. */
typedef struct
{
   StreamInfoForDecode_t   StreamInfoForDecode; /* Stream information for decode */
   STVID_PictureInfos_t    PictureInfos;
   PictureStreamInfo_t     AdditionalStreamInfo;
   U16                     pTemporalReference;
   U32                     NbFieldsToDisplay;
   VIDCOM_PictureID_t      ExtendedPresentationOrderPictureID;
} VIDDEC_PictureInfoFromDecode_t;

typedef struct
{
   VIDDEC_PictureInfoFromDecode_t *  PictureInfoFromDecode_p;
   VIDDEC_SyntaxError_t *            SyntaxError_p;
   U8                                StartCodeId;
   void *                            StartCodeAddress_p;
   BOOL                              RelevantData; /* TRUE if PictureInfoFromDecode_p and SyntaxError_p are relevant. */
} VIDDEC_StartCodeFoundInfo_t;

/* Structure with infos about the circular bitbuffer */
typedef struct
{
    void* Address;
    U32   Size;
} VIDDEC_CircularBitbufferParams_t;

#endif

typedef struct
{
    void *                          DataInputAddress_p;              /* Bit buffer address */
    VIDBUFF_PictureBuffer_t *       OutputPicture_p;                 /* Picture to decode */
    VIDBUFF_PictureBuffer_t *       OutputDecimatedPicture_p;        /* Picture to decode */
    StreamInfoForDecode_t *         StreamInfoForDecode_p;           /* Stream information for decode */
    STVID_PictureInfos_t *          PictureInfos_p;
    PictureStreamInfo_t *           PictureStreamInfo_p;
#ifdef ST_smoothbackward
    U16                             pTemporalReference;
#endif /* ST_smoothbackward */
} VIDDEC_DecodePictureParams_t;

#define VIDDEC_InitParams_t VIDPROD_InitParams_t

#define VIDDEC_NewPictureDecodeOrSkip_t VIDPROD_NewPictureDecodeOrSkip_t

#define VIDDEC_StartParams_t VIDPROD_StartParams_t

#define VIDDEC_DecoderOperatingMode_t VIDPROD_DecoderOperatingMode_t

#define VIDDEC_Infos_t VIDPROD_Infos_t

#define VIDDEC_UpdateDisplayQueue_t VIDPROD_UpdateDisplayQueue_t

/* Exported Variables ------------------------------------------------------- */

/* Exported Macros ---------------------------------------------------------- */

/* Exported Functions ------------------------------------------------------- */

/* If oldmpeg2codec is used, VIDDEC functions specific for trickmodes are called directly, thus they must be defined */
/* Functions which are only called by trickmod */
ST_ErrorCode_t VIDDEC_SetBitBufferUnderflowMargin(const VIDDEC_Handle_t DecodeHandle, const U32 Level);
ST_ErrorCode_t VIDDEC_SetBitBufferUnderflowRequiredDuration(const VIDDEC_Handle_t DecodeHandle, const U32 Duration);
#ifdef ST_smoothbackward
ST_ErrorCode_t VIDDEC_BackwardModeRequested(const VIDDEC_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_AskToSearchNextStartCode(const VIDPROD_Handle_t DecodeHandle, const VIDDEC_StartCodeSearchMode_t SearchMode, const BOOL FirstSliceDetect, void * const SearchAddress_p);
ST_ErrorCode_t VIDDEC_DecodePicture(const VIDPROD_Handle_t DecodeHandle, VIDDEC_DecodePictureParams_t * const DecodePictureParams_p);
ST_ErrorCode_t VIDDEC_DiscardStartCode(const VIDPROD_Handle_t DecodeHandle, const BOOL DiscardStartCode);
ST_ErrorCode_t VIDDEC_GetCDFifoAlignmentInBytes(const VIDPROD_Handle_t DecodeHandle, U32 * const CDFifoAlignment_p);
ST_ErrorCode_t VIDDEC_GetLinearBitBufferTransferedDataSize(const VIDPROD_Handle_t DecodeHandle, U32 * const TransferedDataSize_p);
ST_ErrorCode_t VIDDEC_GetCircularBitBufferParams(const VIDDEC_Handle_t DecodeHandle, VIDDEC_CircularBitbufferParams_t * const CircularBitbufferParams_p);
ST_ErrorCode_t VIDDEC_GetSCDAlignmentInBytes(const VIDPROD_Handle_t DecodeHandle, U32 * const SCDAlignment_p);
ST_ErrorCode_t VIDDEC_GetControlMode(const VIDPROD_Handle_t DecodeHandle, VIDDEC_ControlMode_t * const ControlMode_p);
BOOL VIDDEC_IsThereARunningDataInjection(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_LaunchDecodeWhenPossible(const VIDPROD_Handle_t DecodeHandle, const VIDDEC_DecodePictureParams_t * const DecodeParams_p);
ST_ErrorCode_t VIDDEC_ResetTemporalRefCounters(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_SearchNextStartCode(const VIDPROD_Handle_t DecodeHandle, const VIDDEC_StartCodeSearchMode_t SearchMode, const BOOL FirstSliceDetect, void * const SearchAddress_p);
ST_ErrorCode_t VIDDEC_SetControlMode(const VIDPROD_Handle_t DecodeHandle, const VIDDEC_ControlMode_t ControlMode);
void VIDDEC_SetDecodeBitBufferInLinearMode(const VIDPROD_Handle_t DecodeHandle, void * const LinearBitBufferStartAddress_p, const U32 LinearBitBufferSize);
ST_ErrorCode_t VIDDEC_WriteStartCode(const VIDDEC_Handle_t DecodeHandle, const U32 SCVal, void * const SCAdd_p);
#endif
ST_ErrorCode_t VIDDEC_AbortBitRateEvaluating(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_DisableBitRateEvaluating(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_EnableBitRateEvaluating(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_GetSmoothBackwardCapability(const VIDPROD_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_NewReferencePicture(const VIDPROD_Handle_t DecodeHandle, VIDBUFF_PictureBuffer_t * const Picture_p);
ST_ErrorCode_t VIDDEC_ReleaseReferencePictures(const VIDDEC_Handle_t DecodeHandle);
ST_ErrorCode_t VIDDEC_NotifyDecodeEvent(const VIDDEC_Handle_t DecodeHandle, STEVT_EventID_t EventID, const void * EventData);
ST_ErrorCode_t VIDDEC_SetNextDecodeAction(const VIDPROD_Handle_t DecodeHandle, const VIDDEC_DecodeAction_t NextDecodeAction);
ST_ErrorCode_t VIDDEC_Stop(const VIDDEC_Handle_t DecodeHandle, const STVID_Stop_t StopMode,const BOOL StopAfterBuffersTimeOut);
ST_ErrorCode_t VIDDEC_GetBitRateValue(const VIDDEC_Handle_t DecodeHandle, U32 * const BitRateValue_p);
/* C++ support */
#ifdef __cplusplus
}
#endif

#endif /* #ifndef __VIDEO_DECODE_H */

/* End of decode.h */