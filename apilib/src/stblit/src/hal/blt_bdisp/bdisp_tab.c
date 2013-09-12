/*******************************************************************************

File name   : bdisp_tab.c

Description : Contains conversion tables

COPYRIGHT (C) STMicroelectronics 2000.

Date               Modification                                     Name
----               ------------                                     ----
01 Jun 2000        Created                                          HE
*******************************************************************************/

/* Private preliminary definitions (internal use only) ---------------------- */


/* Includes ----------------------------------------------------------------- */

#include "stddefs.h"


/* Private Types ------------------------------------------------------------ */


/* Private Constants -------------------------------------------------------- */


/* Private Variables (static)------------------------------------------------ */


/* Global Variables --------------------------------------------------------- */
#if defined(ST_7109) || defined(ST_7200)
const U16 stblit_TableROP[28][28]  =
{
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x202 ,0x21  ,0x8000,0x8000,0x31  ,0x8000,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x0   ,0x4000,0x4000,0x4000,0x4000,0x4000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x0   ,0x4000,0x4000,0x4000,0x4000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x4000,0x0   ,0x4000,0x4000,0x4000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x4000,0x4000,0x0   ,0x4000,0x4000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x4000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }
};
#endif /* defined(ST_7109) || defined(ST_7200) */

const U16 stblit_TableBlend[28][28]  =
{
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x21  ,0x4   ,0x8000,0x31  ,0x84  ,0x8000,0x29  ,0x8000,0x8000,0x39  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x6   ,0x6   ,0x8000,0x36  ,0x86  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }
 };


const U16 stblit_TableSingleSrc2[28][28]  =
{
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 ,0x4  ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x21  ,0x21  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x0   ,0x4000,0x4000,0x4000,0x4000,0x4000,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x0   ,0x4000,0x4000,0x4000,0x4000,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x4000,0x0   ,0x4000,0x4000,0x4000,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x4000,0x4000,0x4000,0x0   ,0x4000,0x4000,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x4000,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x2   ,0x8000,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x6   ,0x6   ,0x8000,0x86  ,0x86  ,0x8000,0x23  ,0x23  ,0x8000,0xC6  ,0xC6  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x0   ,0x0   ,0x8000,0x85  ,0x85  ,0x8000,0x23  ,0x23  ,0x8000,0xC5  ,0xC5  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x203 ,0x0   ,0x0   ,0x8000,0x85  ,0x85  ,0x8000,0x23  ,0x23  ,0x8000,0xC5  ,0xC5  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x15  ,0x15  ,0x8000,0x0   ,0x0   ,0x8000,0x55  ,0x55  ,0x8000,0xD5  ,0xD5  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x213 ,0x15  ,0x15  ,0x8000,0x0   ,0x0   ,0x8000,0x55  ,0x55  ,0x8000,0xD5  ,0xD5  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0xD   ,0xD   ,0x8000,0x8D  ,0x8D  ,0x8000,0x0   ,0x0   ,0x8000,0xCD  ,0xCD  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x9   ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0x20B ,0xD   ,0xD   ,0x8000,0x8D  ,0x8D  ,0x8000,0x0   ,0x0   ,0x8000,0xCD  ,0xCD  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x1D  ,0x1D  ,0x8000,0x9D  ,0x9D  ,0x8000,0x5D  ,0x5D  ,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x21B ,0x1D  ,0x1D  ,0x8000,0x9D  ,0x9D  ,0x8000,0x5D  ,0x5D  ,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }
 };

const U16 stblit_TableSingleSrc1[28][28]  =
{
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x4   ,0x4   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x24  ,0x24  ,0x24  ,0x24  ,0x24  ,0x24  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0xA4  ,0xA4  ,0xA4  ,0xA4  ,0xA4  ,0xA4  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x64  ,0x64  ,0x64  ,0x64  ,0x64  ,0x64  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0xE4  ,0xE4  ,0xE4  ,0xE4  ,0xE4  ,0xE4  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }
 };

#if defined(ST_7109) || defined(ST_7200)
const U16 stblit_TableMB[28][28]  =
{
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x1   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x11  ,0x11  ,0x8000,0x9   ,0x9   ,0x8000,0x19  ,0x19  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x11  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x11  ,0x11  ,0x8000,0x0   ,0x0   ,0x8000,0x19  ,0x19  ,0x8000,0x9   ,0x9   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x9    ,0x9   ,0x9   ,0x9   ,0x9  ,0x9   ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x9   ,0x9   ,0x8000,0x19  ,0x19  ,0x8000,0x0   ,0x0   ,0x8000,0x11  ,0x11  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x19  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x19  ,0x19  ,0x8000,0x9   ,0x9   ,0x8000,0x11  ,0x11  ,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }

};
#else /* 7109 */
const U16 stblit_TableMB[28][28]  =
{
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x24  ,0x24  ,0x24  ,0x24  ,0x24  ,0x24  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x0   ,0x0   ,0x8000,0x84  ,0x84  ,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0xA4  ,0xA4  ,0xA4  ,0xA4  ,0xA4  ,0xA4  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x84  ,0x84  ,0x8000,0x0   ,0x0   ,0x8000,0xC4  ,0xC4  ,0x8000,0x44  ,0x44  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x64   ,0x64  ,0x64  ,0x64  ,0x64 ,0x64  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x44  ,0x44  ,0x8000,0xC4  ,0xC4  ,0x8000,0x0   ,0x0   ,0x8000,0x84  ,0x84  ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0xE4  ,0xE4  ,0xE4  ,0xE4  ,0xE4  ,0xE4  ,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0xC4  ,0xC4  ,0x8000,0x44  ,0x44  ,0x8000,0x84  ,0x84  ,0x8000,0x0   ,0x0   ,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 },
 { 0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x8000 }
 };
#endif /* 7109 */

#if defined (ST_7109) || defined(ST_7200)
const U32 stblit_VersatileMatrix[8][12]  =
{
 {409,298,0,-208,298,-100,0,298,517,-19,-19,-19}, /* YCbCr to RGB 601 Gfx Matrix signed */
 {409,298,0,-208,298,-100,0,298,517,-223,135,-277}, /* YCbCr to RGB 601 Gfx Matrix unsigned */
 {459,298,0,-136,298,-54,0,298,541,-19,19,-19}, /* YCbCr to RGB 709 Gfx Matrix signed */
 {459,298,0,-136,298,-54,0,298,541,-249,76,-289}, /* YCbCr to RGB 709 Gfx Matrix unsigned */
 {112,-94,-18,66,129,25,-38,-75,112,0,16,0}, /* RGB to YCbCr 601 Gfx Matrix signed */
 {112,-94,-18,66,129,25,-38,-75,112,128,16,128}, /* RGB to YCbCr 601 Gfx Matrix unsigned */
 {112,-102,-13,47,157,16,-26,-87,112,0,16,0}, /* RGB to YCbCr 709 Gfx Matrix signed */
 {112,-102,-13,47,157,16,-26,-87,112,128,16,128} /* RGB to YCbCr 709 Gfx Matrix unsigned */
};
#endif /*defined (ST_7109) || defined(ST_7200)*/

/* Private Macros ----------------------------------------------------------- */


/* Private Function prototypes ---------------------------------------------- */


/* Functions ---------------------------------------------------------------- */


/*******************************************************************************
Name        :
Description :
Parameters  :
Assumptions :
Limitations :
Returns     :
*******************************************************************************/


/*
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
*/


/* End of blt_tab.c */