/// $Id: STm7100_AudioDecoder_Types.h,v 1.3 2003/11/26 17:03:07 lassureg Exp $
/// @file     : ACC_Multicom/ACC_Transformers/Audio_DecoderTypes.h
///
/// @brief    : Generic Audio Decoder types for Multicom
///
/// @par OWNER: Gael Lassure
///
/// @author   : Gael Lassure
///
/// @par SCOPE:
///
/// @date     : 2004-11-1
///
/// &copy; 2003 ST Microelectronics. All Rights Reserved.
///


#ifndef _STM7100_AUDIO_DECODERTYPES_H_
#define _STM7100_AUDIO_DECODERTYPES_H_

#include "acc_mme.h"
#include "acc_mmedefines.h"
#include "pcmprocessing_decodertypes.h"
#include "audio_decodertypes.h"
typedef struct
{
	U32                         StructSize;  //!< Size of this structure

	MME_CMCGlobalParams_t       CMC;         //!< Common DMix Config : could be used by decoder or by generic downmix
	MME_DeEmphGlobalParams_t    DeEmph;      //!< DeEmphasis Filtering
	MME_BassMgtGlobalParams_t   BassMgt;
	MME_DCRemoveGlobalParams_t  DcRemove;
	MME_DMixGlobalParams_t		DMix;	//Down Mix
	MME_PLIIGlobalParams_t		ProLogicII;
	MME_TsxtGlobalParams_t		TruSurXT;
	MME_OmniGlobalParams_t      OmniMain;
	MME_AC3ExGlobalParams_t		Ac3Ex;
	MME_TempoGlobalParams_t		TempoCtrl;
	MME_CSIIGlobalParams_t      	Csii;        //!< SRS CircleSurround II
	MME_NEOGlobalParams_t		DTSNeo; // DTS Neo

} MME_STm7100PcmProcessingGlobalParams_t;


typedef struct
{
	U32                               StructSize;                          //!< Size of this structure
	U32                               DecConfig[ACC_MAX_DEC_CONFIG_SIZE];  //!< Private Config of Audio Decoder
	MME_STm7100PcmProcessingGlobalParams_t PcmParams;                           //!< PcmPostProcessings Params

} MME_STm7100AudioDecoderGlobalParams_t;

typedef struct
{
	U32                               StructSize; //!< Size of this structure

	//! System Init 
	enum eAccProcessApply             CacheFlush; //!< If ACC_DISABLED then the cached data aren't sent back to DDR
	enum eAccBoolean                  BlockWise;  //!< Perform the decoding blockwise instead of framewise 
	enum eFsRange                     SfreqRange;

	U8                                NChans[ACC_MIX_MAIN_AND_AUX];
	U8                                ChanPos[ACC_MIX_MAIN_AND_AUX];

	//! Decoder specific parameters
	MME_STm7100AudioDecoderGlobalParams_t	GlobalParams;

} MME_STm7100AudioDecoderInitParams_t;

#define TARGET_AUDIODECODER_INITPARAMS_STRUCT     MME_STm7100AudioDecoderInitParams_t
#define TARGET_AUDIODECODER_GLOBALPARAMS_STRUCT   MME_STm7100AudioDecoderGlobalParams_t
#define TARGET_AUDIODECODER_PCMPARAMS_STRUCT      MME_STm7100PcmProcessingGlobalParams_t


#endif /* _STM7100_AUDIODECODER_DECODERTYPES_H_ */
