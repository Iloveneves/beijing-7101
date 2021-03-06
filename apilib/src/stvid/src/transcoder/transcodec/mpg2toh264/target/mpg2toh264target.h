/*!
 ************************************************************************
 * \file mpg2toh264target.h
 *
 * \brief MPEG2 to H264 target decoder data structures and functions prototypes
 *
 * \author
 * Olivier Deygas \n
 * CMG/STB \n
 * Copyright (C) 2004 STMicroelectronics
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion */

#ifndef __MPG2TOH264TARGET_H
#define __MPG2TOH264TARGET_H

/* Includes ----------------------------------------------------------------- */
#include "mpg2toh264.h"

#include "stfdma.h"
#include "inject.h"

#ifdef ST_OSLINUX
#include "compat.h"
#else

#ifndef STTBX_REPORT
    #define STTBX_REPORT
#endif
#ifndef STTBX_PRINT    
    #define STTBX_PRINT
#endif    
#include "sttbx.h"
#include "stsys.h"
#endif

#include "vid_ctcm.h"

/* Constants ------------------------------------------------------- */


/* Exported types ----------------------------------------------------------- */
/*! target decoder top level data structure */
typedef struct mpg2toh264target_PrivateData_s
{
  U32 Dummy; /* TO REMOVE */
} mpg2toh264target_PrivateData_t;


/* Exported Variables ------------------------------------------------------- */

/* Exported Macros ---------------------------------------------------------- */


/* Exported Functions ------------------------------------------------------- */

#endif /* #ifdef __MPG2TOH264TARGET_H */

