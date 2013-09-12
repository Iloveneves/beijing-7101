/*******************************************************************************

File name   : bdisp_flt.c

Description : Contains filters tables

COPYRIGHT (C) STMicroelectronics 2001.

Date               Modification                                     Name
----               ------------                                     ----
11 Jun 2004        Created                                          HE
*******************************************************************************/


/* Includes ----------------------------------------------------------------- */
#include "bdisp_flt.h"

/* Exported Constants ------------------------------------------------------- */

/* Exported Types ----------------------------------------------------------- */

/* Exported Variables ------------------------------------------------------- */
const U8 HFilterBuffer[STBLIT_HFILTER_COEFFICIENTS_SIZE * STBLIT_DEFAULT_NUMBER_FILTERS]  =
{

/* Filter 921 */
 0x00,  0x00,  0x00,  0x00,  0x40,  0x00,  0x00,  0x00,
 0x00,  0x00,  0xff,  0x07,  0x3d,  0xfc,  0x01,  0x00,
 0x00,  0x01,  0xfd,  0x11,  0x36,  0xf9,  0x02,  0x00,
 0x00,  0x01,  0xfb,  0x1b,  0x2e,  0xf9,  0x02,  0x00,
 0x00,  0x01,  0xf9,  0x26,  0x26,  0xf9,  0x01,  0x00,
 0x00,  0x02,  0xf9,  0x30,  0x19,  0xfb,  0x01,  0x00,
 0x00,  0x02,  0xf9,  0x39,  0x0e,  0xfd,  0x01,  0x00,
 0x00,  0x01,  0xfc,  0x3e,  0x06,  0xff,  0x00,  0x00,

/* Filter 1024 */
 0x00,  0x00,  0x00,  0x00,  0x40,  0x00,  0x00,  0x00,
 0xff,  0x03,  0xfd,  0x08,  0x3e,  0xf9,  0x04,  0xfe,
 0xfd,  0x06,  0xf8,  0x13,  0x3b,  0xf4,  0x07,  0xfc,
 0xfb,  0x08,  0xf5,  0x1f,  0x34,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x2b,  0x2a,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x35,  0x1e,  0xf4,  0x08,  0xfb,
 0xfc,  0x07,  0xf5,  0x3c,  0x12,  0xf7,  0x06,  0xfd,
 0xfe,  0x04,  0xfa,  0x3f,  0x07,  0xfc,  0x03,  0xff,

/* Filter 1126 */
 0x00,  0x00,  0x00,  0x00,  0x40,  0x00,  0x00,  0x00,
 0xff,  0x03,  0xfd,  0x08,  0x3e,  0xf9,  0x04,  0xfe,
 0xfd,  0x06,  0xf8,  0x13,  0x3b,  0xf4,  0x07,  0xfc,
 0xfb,  0x08,  0xf5,  0x1f,  0x34,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x2b,  0x2a,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x35,  0x1e,  0xf4,  0x08,  0xfb,
 0xfc,  0x07,  0xf5,  0x3c,  0x12,  0xf7,  0x06,  0xfd,
 0xfe,  0x04,  0xfa,  0x3f,  0x07,  0xfc,  0x03,  0xff,

/* Filter 1228 */
 0x00,  0x00,  0x00,  0x00,  0x40,  0x00,  0x00,  0x00,
 0xff,  0x03,  0xfd,  0x08,  0x3e,  0xf9,  0x04,  0xfe,
 0xfd,  0x06,  0xf8,  0x13,  0x3b,  0xf4,  0x07,  0xfc,
 0xfb,  0x08,  0xf5,  0x1f,  0x34,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x2b,  0x2a,  0xf1,  0x09,  0xfb,
 0xfb,  0x09,  0xf2,  0x35,  0x1e,  0xf4,  0x08,  0xfb,
 0xfc,  0x07,  0xf5,  0x3c,  0x12,  0xf7,  0x06,  0xfd,
 0xfe,  0x04,  0xfa,  0x3f,  0x07,  0xfc,  0x03,  0xff,

/* Filter 1331 */
 0xfd,  0x04,  0xfc,  0x05,  0x39,  0x05,  0xfc,  0x04,
 0xfc,  0x06,  0xf9,  0x0c,  0x39,  0xfe,  0x00,  0x02,
 0xfb,  0x08,  0xf6,  0x17,  0x35,  0xf9,  0x02,  0x00,
 0xfc,  0x08,  0xf4,  0x20,  0x30,  0xf4,  0x05,  0xff,
 0xfd,  0x07,  0xf4,  0x29,  0x28,  0xf3,  0x07,  0xfd,
 0xff,  0x05,  0xf5,  0x31,  0x1f,  0xf3,  0x08,  0xfc,
 0x00,  0x02,  0xf9,  0x38,  0x14,  0xf6,  0x08,  0xfb,
 0x02,  0x00,  0xff,  0x3a,  0x0b,  0xf8,  0x06,  0xfc,

/* Filter 1433 */
 0xfc,  0x06,  0xf9,  0x09,  0x34,  0x09,  0xf9,  0x06,
 0xfd,  0x07,  0xf7,  0x10,  0x32,  0x02,  0xfc,  0x05,
 0xfe,  0x07,  0xf6,  0x17,  0x2f,  0xfc,  0xff,  0x04,
 0xff,  0x06,  0xf5,  0x20,  0x2a,  0xf9,  0x01,  0x02,
 0x00,  0x04,  0xf6,  0x27,  0x25,  0xf6,  0x04,  0x00,
 0x02,  0x01,  0xf9,  0x2d,  0x1d,  0xf5,  0x06,  0xff,
 0x04,  0xff,  0xfd,  0x31,  0x15,  0xf5,  0x07,  0xfe,
 0x05,  0xfc,  0x02,  0x35,  0x0d,  0xf7,  0x07,  0xfd,

/* Filter 1536 */
 0xfe,  0x06,  0xf8,  0x0b,  0x30,  0x0b,  0xf8,  0x06,
 0xff,  0x06,  0xf7,  0x12,  0x2d,  0x05,  0xfa,  0x06,
 0x00,  0x04,  0xf6,  0x18,  0x2c,  0x00,  0xfc,  0x06,
 0x01,  0x02,  0xf7,  0x1f,  0x27,  0xfd,  0xff,  0x04,
 0x03,  0x00,  0xf9,  0x24,  0x24,  0xf9,  0x00,  0x03,
 0x04,  0xff,  0xfd,  0x29,  0x1d,  0xf7,  0x02,  0x01,
 0x06,  0xfc,  0x00,  0x2d,  0x17,  0xf6,  0x04,  0x00,
 0x06,  0xfa,  0x05,  0x30,  0x0f,  0xf7,  0x06,  0xff,

/* Filter 2048 */
 0x05,  0xfd,  0xfb,  0x13,  0x25,  0x13,  0xfb,  0xfd,
 0x05,  0xfc,  0xfd,  0x17,  0x24,  0x0f,  0xf9,  0xff,
 0x04,  0xfa,  0xff,  0x1b,  0x24,  0x0b,  0xf9,  0x00,
 0x03,  0xf9,  0x01,  0x1f,  0x23,  0x08,  0xf8,  0x01,
 0x02,  0xf9,  0x04,  0x22,  0x20,  0x04,  0xf9,  0x02,
 0x01,  0xf8,  0x08,  0x25,  0x1d,  0x01,  0xf9,  0x03,
 0x00,  0xf9,  0x0c,  0x25,  0x1a,  0xfe,  0xfa,  0x04,
 0xff,  0xf9,  0x10,  0x26,  0x15,  0xfc,  0xfc,  0x05,

/* Filter 3072 */
 0xfc,  0xfd,  0x06,  0x13,  0x18,  0x13,  0x06,  0xfd,
 0xfc,  0xfe,  0x08,  0x15,  0x17,  0x12,  0x04,  0xfc,
 0xfb,  0xfe,  0x0a,  0x16,  0x18,  0x10,  0x03,  0xfc,
 0xfb,  0x00,  0x0b,  0x18,  0x17,  0x0f,  0x01,  0xfb,
 0xfb,  0x00,  0x0d,  0x19,  0x17,  0x0d,  0x00,  0xfb,
 0xfb,  0x01,  0x0f,  0x19,  0x16,  0x0b,  0x00,  0xfb,
 0xfc,  0x03,  0x11,  0x19,  0x15,  0x09,  0xfe,  0xfb,
 0xfc,  0x04,  0x12,  0x1a,  0x12,  0x08,  0xfe,  0xfc,

/* Filter 4096 */
 0xfe,  0x02,  0x09,  0x0f,  0x0e,  0x0f,  0x09,  0x02,
 0xff,  0x02,  0x09,  0x0f,  0x10,  0x0e,  0x08,  0x01,
 0xff,  0x03,  0x0a,  0x10,  0x10,  0x0d,  0x07,  0x00,
 0x00,  0x04,  0x0b,  0x10,  0x0f,  0x0c,  0x06,  0x00,
 0x00,  0x05,  0x0c,  0x10,  0x0e,  0x0c,  0x05,  0x00,
 0x00,  0x06,  0x0c,  0x11,  0x0e,  0x0b,  0x04,  0x00,
 0x00,  0x07,  0x0d,  0x11,  0x0f,  0x0a,  0x03,  0xff,
 0x01,  0x08,  0x0e,  0x11,  0x0e,  0x09,  0x02,  0xff,

/* Filter 5120 */
 0x00,  0x04,  0x09,  0x0c,  0x0e,  0x0c,  0x09,  0x04,
 0x01,  0x05,  0x09,  0x0c,  0x0d,  0x0c,  0x08,  0x04,
 0x01,  0x05,  0x0a,  0x0c,  0x0e,  0x0b,  0x08,  0x03,
 0x02,  0x06,  0x0a,  0x0d,  0x0c,  0x0b,  0x07,  0x03,
 0x02,  0x07,  0x0a,  0x0d,  0x0d,  0x0a,  0x07,  0x02,
 0x03,  0x07,  0x0b,  0x0d,  0x0c,  0x0a,  0x06,  0x02,
 0x03,  0x08,  0x0b,  0x0d,  0x0d,  0x0a,  0x05,  0x01,
 0x04,  0x08,  0x0c,  0x0d,  0x0c,  0x09,  0x05,  0x01,

/* Filter 6144 */
 0x03,  0x06,  0x09,  0x0b,  0x09,  0x0b,  0x09,  0x06,
 0x03,  0x06,  0x09,  0x0b,  0x0c,  0x0a,  0x08,  0x05,
 0x03,  0x06,  0x09,  0x0b,  0x0c,  0x0a,  0x08,  0x05,
 0x04,  0x07,  0x09,  0x0b,  0x0b,  0x0a,  0x08,  0x04,
 0x04,  0x07,  0x0a,  0x0b,  0x0b,  0x0a,  0x07,  0x04,
 0x04,  0x08,  0x0a,  0x0b,  0x0b,  0x09,  0x07,  0x04,
 0x05,  0x08,  0x0a,  0x0b,  0x0c,  0x09,  0x06,  0x03,
 0x05,  0x08,  0x0a,  0x0b,  0x0c,  0x09,  0x06,  0x03,

};

const U8 VFilterBuffer[STBLIT_VFILTER_COEFFICIENTS_SIZE * STBLIT_DEFAULT_NUMBER_FILTERS]  =
{
/* Filter 921 */
 0x00,  0x00,  0x40,  0x00,  0x00,
 0x00,  0x05,  0x3d,  0xfe,  0x00,
 0x00,  0x0d,  0x36,  0xfd,  0x00,
 0xff,  0x17,  0x2d,  0xfd,  0x00,
 0xfe,  0x23,  0x21,  0xfe,  0x00,
 0xfd,  0x2e,  0x16,  0xff,  0x00,
 0xfd,  0x37,  0x0c,  0x00,  0x00,
 0xfe,  0x3e,  0x04,  0x00,  0x00,

/* Filter 1024 */
 0x00,  0x00,  0x40,  0x00,  0x00,
 0xfd,  0x09,  0x3c,  0xfa,  0x04,
 0xf9,  0x13,  0x39,  0xf5,  0x06,
 0xf5,  0x1f,  0x31,  0xf3,  0x08,
 0xf3,  0x2a,  0x28,  0xf3,  0x08,
 0xf3,  0x34,  0x1d,  0xf5,  0x07,
 0xf5,  0x3b,  0x12,  0xf9,  0x05,
 0xfa,  0x3f,  0x07,  0xfd,  0x03,

/* Filter 1126 */
 0x00,  0x00,  0x40,  0x00,  0x00,
 0xfd,  0x09,  0x3c,  0xfa,  0x04,
 0xf9,  0x13,  0x39,  0xf5,  0x06,
 0xf5,  0x1f,  0x31,  0xf3,  0x08,
 0xf3,  0x2a,  0x28,  0xf3,  0x08,
 0xf3,  0x34,  0x1d,  0xf5,  0x07,
 0xf5,  0x3b,  0x12,  0xf9,  0x05,
 0xfa,  0x3f,  0x07,  0xfd,  0x03,

/* Filter 1228 */
 0x00,  0x00,  0x40,  0x00,  0x00,
 0xfd,  0x09,  0x3c,  0xfa,  0x04,
 0xf9,  0x13,  0x39,  0xf5,  0x06,
 0xf5,  0x1f,  0x31,  0xf3,  0x08,
 0xf3,  0x2a,  0x28,  0xf3,  0x08,
 0xf3,  0x34,  0x1d,  0xf5,  0x07,
 0xf5,  0x3b,  0x12,  0xf9,  0x05,
 0xfa,  0x3f,  0x07,  0xfd,  0x03,

/* Filter 1331 */
 0xfc,  0x05,  0x3e,  0x05,  0xfc,
 0xf8,  0x0e,  0x3b,  0xff,  0x00,
 0xf5,  0x18,  0x38,  0xf9,  0x02,
 0xf4,  0x21,  0x31,  0xf5,  0x05,
 0xf4,  0x2a,  0x27,  0xf4,  0x07,
 0xf6,  0x30,  0x1e,  0xf4,  0x08,
 0xf9,  0x35,  0x15,  0xf6,  0x07,
 0xff,  0x37,  0x0b,  0xf9,  0x06,

/* Filter 1433 */
 0xf8,  0x0a,  0x3c,  0x0a,  0xf8,
 0xf6,  0x12,  0x3b,  0x02,  0xfb,
 0xf4,  0x1b,  0x35,  0xfd,  0xff,
 0xf4,  0x23,  0x30,  0xf8,  0x01,
 0xf6,  0x29,  0x27,  0xf6,  0x04,
 0xf9,  0x2e,  0x1e,  0xf5,  0x06,
 0xfd,  0x31,  0x16,  0xf6,  0x06,
 0x02,  0x32,  0x0d,  0xf8,  0x07,

/* Filter 1536 */
 0xf6,  0x0e,  0x38,  0x0e,  0xf6,
 0xf5,  0x15,  0x38,  0x06,  0xf8,
 0xf5,  0x1d,  0x33,  0x00,  0xfb,
 0xf6,  0x23,  0x2d,  0xfc,  0xfe,
 0xf9,  0x28,  0x26,  0xf9,  0x00,
 0xfc,  0x2c,  0x1e,  0xf7,  0x03,
 0x00,  0x2e,  0x18,  0xf6,  0x04,
 0x05,  0x2e,  0x11,  0xf7,  0x05,

/* Filter 2048 */
 0xfb,  0x13,  0x24,  0x13,  0xfb,
 0xfd,  0x17,  0x23,  0x0f,  0xfa,
 0xff,  0x1a,  0x23,  0x0b,  0xf9,
 0x01,  0x1d,  0x22,  0x07,  0xf9,
 0x04,  0x20,  0x1f,  0x04,  0xf9,
 0x07,  0x22,  0x1c,  0x01,  0xfa,
 0x0b,  0x24,  0x17,  0xff,  0xfb,
 0x0f,  0x24,  0x14,  0xfd,  0xfc,

/* Filter 3072 */
 0x05,  0x10,  0x16,  0x10,  0x05,
 0x06,  0x11,  0x16,  0x0f,  0x04,
 0x08,  0x13,  0x15,  0x0e,  0x02,
 0x09,  0x14,  0x16,  0x0c,  0x01,
 0x0b,  0x15,  0x15,  0x0b,  0x00,
 0x0d,  0x16,  0x13,  0x0a,  0x00,
 0x0f,  0x17,  0x13,  0x08,  0xff,
 0x11,  0x18,  0x12,  0x07,  0xfe,

/* Filter 4096 */
 0x09,  0x0f,  0x10,  0x0f,  0x09,
 0x09,  0x0f,  0x12,  0x0e,  0x08,
 0x0a,  0x10,  0x11,  0x0e,  0x07,
 0x0b,  0x11,  0x11,  0x0d,  0x06,
 0x0c,  0x11,  0x12,  0x0c,  0x05,
 0x0d,  0x12,  0x11,  0x0c,  0x04,
 0x0e,  0x12,  0x11,  0x0b,  0x04,
 0x0f,  0x13,  0x11,  0x0a,  0x03,

/* Filter 5120 */
 0x0a,  0x0e,  0x10,  0x0e,  0x0a,
 0x0b,  0x0e,  0x0f,  0x0e,  0x0a,
 0x0b,  0x0f,  0x10,  0x0d,  0x09,
 0x0c,  0x0f,  0x10,  0x0d,  0x08,
 0x0d,  0x0f,  0x0f,  0x0d,  0x08,
 0x0d,  0x10,  0x10,  0x0c,  0x07,
 0x0e,  0x10,  0x0f,  0x0c,  0x07,
 0x0f,  0x10,  0x10,  0x0b,  0x06,

/* Filter 6144 */
 0x0b,  0x0e,  0x0e,  0x0e,  0x0b,
 0x0b,  0x0e,  0x0f,  0x0d,  0x0b,
 0x0c,  0x0e,  0x0f,  0x0d,  0x0a,
 0x0c,  0x0e,  0x0f,  0x0d,  0x0a,
 0x0d,  0x0f,  0x0e,  0x0d,  0x09,
 0x0d,  0x0f,  0x0f,  0x0c,  0x09,
 0x0e,  0x0f,  0x0e,  0x0c,  0x09,
 0x0e,  0x0f,  0x0f,  0x0c,  0x08,

};