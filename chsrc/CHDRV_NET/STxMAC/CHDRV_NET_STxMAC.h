/*******************ͷ�ļ�˵��ע��**********************************/
/*��˾��Ȩ˵������Ȩ��2007���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����CHDRV_NET_MAC110.h                                              */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2007-08-10                                             */
/*ͷ�ļ����ݸ�Ҫ����������/�����ȣ���                              */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*                                                                 */
/*******************************************************************/
/*****************�������벿��**************************************/
#ifndef __CHDRV_NET_MAC110__
#define __CHDRV_NET_MAC110__
/*******************************************************************/
/**************** #include ����*************************************/
#include "stddef.h"
#include "stddefs.h"

#include "stcommon.h"
#include <osplus.h>
#include <osplus/device.h>
#include <osplus/registry.h>
#include <osplus/stmac110.h>
#include <osplus/dev.h>
#include <osplus/ethernet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(ARCHITECTURE_ST40)
#define ARCHITECTURE_ST40
#endif

#define ETHER_DEVICE_NAME       "eth0"                       /*��̫���豸��������,���뱣��һ��*/

/*******************************************************************/
/***************  �ⲿ������������ *********************************/

 static void CH_ReadPacketCallBack0(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack1(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack2(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack3(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack4(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack5(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack6(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack7(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack8(struct ethernet_async_s * r_syn_info);
 static void CH_ReadPacketCallBack9(struct ethernet_async_s * r_syn_info);

/*******************************************************************/
#endif

