/*******************ͷ�ļ�˵��ע��**********************************/
/*��˾��Ȩ˵������Ȩ��2007���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����chdrv_tcpip.h                                              */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2007-08-10                                             */
/*ͷ�ļ����ݸ�Ҫ����������/�����ȣ���                              */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*                                                                 */
/*******************************************************************/
/*****************�������벿��**************************************/
#ifndef CHDRV_TCPIP
#define CHDRV_TCPIP
/*******************************************************************/
/**************** #include ����*************************************/
#include "packet_manager.h"
#include "CHDRV_NET.h"
#include "task_manager.h"

/*******************************************************************/
/**************** #typedef �������� ********************************/
#define       CH_BOOL               boolean           
#define		CH_RX_QUEUE

/*******************************************************************/
/***************  �ⲿ������������ *********************************/

#if (SMSC_ERROR_ENABLED)
#define SMSC911X_PRIVATE_DATA_SIGNATURE (0xB43784B8)
#endif

typedef struct NETCARD_PRIVATE_DATA_ {
	DECLARE_SIGNATURE
	mem_ptr_t mIoAddress;		/* Base address */
	int mInterfaceIndex;
	u32_t mIdRev;
	u32_t mGeneration;
#if SMSC_ERROR_ENABLED
	s32_t software_irq_signal; /* only used for testing ISR signaling */
#endif
	TASK mRxTask;
	TASK mTxTask;
	PACKET_QUEUE mTxPacketQueue;
#ifdef CH_RX_QUEUE	
	PACKET_QUEUE mRxPacketQueue;/*20081111 add*/
#endif
#if USE_PHY_WORK_AROUND
#define MIN_PACKET_SIZE (64)
	u8_t mLoopBackTxPacket[MIN_PACKET_SIZE];
	u8_t mLoopBackRxPacket[MIN_PACKET_SIZE];
	u32_t mResetCount;
#endif
} NETCARD_PRIVATE_DATA;

/*******************************************************************/
/***************  �ⲿ������������ *********************************/
void CH_Handle_RxData_Task(void *arg);
 void CH_TCPIP_TxTask(void * arg);
NETCARD_PRIVATE_DATA * EthernetCardInfo_Initialize(struct NETWORK_INTERFACE * networkInterface);
void EthernetOutput(	struct NETWORK_INTERFACE * networkInterface,PPACKET_BUFFER packet);
boolean CH_InitEthernetIO( struct NETWORK_INTERFACE *netif,int interfaceIndex);

/*******************************************************************/
#endif

