/*--------------------------------------------------------------------
DSP:���ļ�����smscЭ��ջ��ֲ��7101����(�Դ����������)ƽ̨�ϵĵײ�����
�շ����Ƶ�ʵ��
AUTHOR: WUFEI
TIME:2008-12-19
--------------------------------------------------------------------*/

#include "CHDRV_NET.h"
#include "CHDRV_NET_STxMAC.h"
#include "osplus\stxmac.h"
#include "stpio.h"
#include "stos.h"

static U32 CONFIG_BASE_ADDRESS_MAPPED;
static U32 ILC_BASE_ADDRESS_MAPPED;

/*static*/ device_handle_t *  gpch_Ethernet_Hndl_p = NULL;/*��̫���������ָ��*/
static u8_t CHDRV_MAC_ADDR[6];/*ȫ��buff����ָ��, MAC��ַ��Ϣ*/
ethernet_info_t  gst_Info;/*�õ���̫����������Ϣ*/
static Mutex_t             TCPIPi_Mutex_API;

extern interrupt_name_t OS21_INTERRUPT_ETH_MD    __attribute__ ((weak));
extern interrupt_name_t OS21_INTERRUPT_ETHERNET  __attribute__ ((weak));

#define CH_ASYNC_TYPE 1
/*20071221 add*/
#define MAX_READ_TASK  10
//ethernet_async_t   g_async_read[MAX_READ_TASK];
//ethernet_async_t   g_async_write[MAX_READ_TASK];
CHDRV_NET_SEMAPHORE  gp_write_seamaphore;
CHDRV_NET_SEMAPHORE  gp_read_seamaphore[MAX_READ_TASK];
CHDRV_NET_SEMAPHORE  gp_io_semaphore = NULL;
#define STMAC110_INT     (&OS21_INTERRUPT_IRL_ENC_8)
#define STMAC110_ALT_INT (&OS21_INTERRUPT_MDINT)
#define STMAC110_BASE_ADDRESS 0xFDE00000                  /*��̫������оƬ�Ĵ�����ַ*/



#define TCPIP_BUF_MAX              1024
#define ms_delay(ms)		CHDRV_NET_ThreadDelay(ms)
/*
 * Various defines and macros used in this file.
 */
#define SYSTEM_CONFIG_BASE 0xb9001000
#define SYSTEM_CONFIG_7    (SYSTEM_CONFIG_BASE + 0x0000011c)
#define SYSTEM_CONFIG_10   (SYSTEM_CONFIG_BASE + 0x00000128)
#define ILC_BASE_ADDR      0xb8000000

#define MII_MODE           0
#define RMII_INT_MODE      1
#define RMII_EXT_MODE      2

#define _BIT(i)               (1 << (i % 32))
#define _REG_OFF(i)           (sizeof(int) * (i / 32))
#define ILC_SET_ENABLE_REG(i) (ILC_BASE_ADDR + 0x500 + _REG_OFF(i))
#define ILC_CLR_ENABLE_REG(i) (ILC_BASE_ADDR + 0x480 + _REG_OFF(i))
#define ILC_EXT_WAKPOL_EN_REG (ILC_BASE_ADDR + 0x680)
#define ILC_PRIORITY_REG(i)   (ILC_BASE_ADDR + 0x800 + (8 * i))
#define ILC_TRIGMODE_REG(i)   (ILC_BASE_ADDR + 0x804 + (8 * i))

#ifdef printf
#undef printf
#endif
#define printf CHDRV_NET_Print
#define STXMAC_DRIVER_DEBUG 	0/*��������ĵ���,������ʽ���ʱӦ�ر�*/
#if STXMAC_DRIVER_DEBUG
static CHDRV_NET_SEMAPHORE  gp_print_seamaphore;
#endif
void ch_tcpip_dbgprintdata(char *name,void *data,int datalen)
{
	
#if 0
	int loop;
	CHDRV_NET_SemaphoreWait(gp_print_seamaphore,-1);

	if(data == NULL || name == NULL)
		return;
	printf("\n#############%s(PRINT START)##########",name);
	printf("\n#############DATA LEN = [%d]##########",datalen);
	for(loop = 0;loop < datalen;loop++)
	{
		if(loop%16 == 0)
			printf("\n");
		printf("%02x ",((unsigned char *)data)[loop]);
	}
	printf("\n#############%s(PRINT END)############\n",name);
	CHDRV_NET_SemaphoreSignal(gp_print_seamaphore);
	
#endif
}


/* ========================================================================
   Name:        TCPIPi_LockAPI
   Description: Lock API for reentrance
   ======================================================================== */

static void TCPIPi_LockAPI(void)
{
 Mutex_Lock(TCPIPi_Mutex_API);
}

/* ========================================================================
   Name:        TCPIPi_UnlockAPI
   Description: Unlock API for reentrance
   ======================================================================== */

static void TCPIPi_UnlockAPI(void)
{
 Mutex_Unlock(TCPIPi_Mutex_API);
}

static U32 TCPIPi_OSPLUS_STx5197_STxMAC_SetDmaBurstLength_fn(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if ((stmac110==NULL)||(arg==NULL))
  {
   CHDRV_NET_NOTICE(CHDRV_NET_DRIVER_DEBUG,("TCPIPi_OSPLUS_STx5197_STxMAC_SetDmaBurstLength():**ERROR** !!! Stmac context is a NULL pointer !!!\n"));
   return(0xFFFFFFFF);
  }

 /* Read the device ID from the system configuration registers */
 /* ---------------------------------------------------------- */
 *((U32 *)arg)=32;

 /* Return no errors */
 /* ---------------- */
 return(0);
}

extern u32_t SYS_MapRegisters(U32 PhysicalAddress,U32 Size,char *Name);

static U32 TCPIPi_OSPLUS_STx5197_STxMAC_SetLinkSpeed_fn(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   CHDRV_NET_NOTICE(CHDRV_NET_DRIVER_DEBUG,("TCPIPi_OSPLUS_STx5197_STxMAC_SetLinkSpeed():**ERROR** !!! stmac context is a NULL pointer !!!\n"));
   return(0xFFFFFFFF);
  }

 /* Set the correct link speed in the config control E register */
 /* ----------------------------------------------------------- */
 if ((U32)arg&OSPLUS_ETH_LINK_100MBPS)
  {
   /* Set 100Mbps link speed */
   OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<1));
  }
 else
  {
   /* Set 10Mbps link speed */
   OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<1));
  }

 /* Return no errors */
 /* ---------------- */
 return(0);
}
static U32 TCPIPi_OSPLUS_STx5197_STxMAC_PhyEnable_fn(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   CHDRV_NET_NOTICE(CHDRV_NET_DRIVER_DEBUG,("TCPIPi_OSPLUS_STx5197_STxMAC_PhyEnable():**ERROR** !!! stmac context is a NULL pointer !!!"));
   return(0xFFFFFFFF);
  }

 /* Enable the phy interrupt */
 /* ------------------------ */
#if defined(mb676)||defined(mb704)||defined(cab5197)
 /* Enable the MDINT interrupt now that it has been routed */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x500/*ILC_SET_ENABLE_REG*/+4*(25/32)),(1<<(25%32)));
#endif

 /* Return no errors */
 /* ---------------- */
 return(0);
}
static U32 TCPIPi_OSPLUS_STx5197_STxMAC_Init_fn(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   CHDRV_NET_NOTICE(CHDRV_NET_DRIVER_DEBUG,("TCPIPi_OSPLUS_STx5197_STxMAC_Init():**ERROR** !!! stmac context is a NULL pointer !!!\n"));
   return(0xFFFFFFFF);
  }

 /* Ensure ethernet uses pads shared with TS */
 /* ---------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<0));

 /* arg determines if MII or RMII mode activated */
 /* -------------------------------------------- */
 switch((U32)arg)
  {
   /* MII interface active using internal clock */
   case 0: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<8));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<7));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<2));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<6));
           break;
   /* MII interface active using external clock */
   case 1: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<8));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<7));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<2));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<6));
           break;
   /* RMII interface active using internal clock */
   case 2: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<8));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<7));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<2));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<6));
           break;
   /* RMII interface active using external clock */
   case 3: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<8));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<7));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<2));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x008/*CFG_CTRL_E*/),(1<<6));
           break;
  }

 /* Route Irq */
 /* --------- */
#if defined(mb676)||defined(mb704)||defined(cab5197)
 /* Route the MDINT interrupt from the ILC to the ST40 INTC */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x800/*ILC_PRIORITY_REG*/+8*25),0x8007);
 /* Instruct ILC to trigger an interrupt when High */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x804/*ILC_TRIGMODE_REG*/+8*25),0x01);
#endif

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/*--------------------------------------------------------------------------*/
/*������:ST_ErrorCode_t CH_MAC110_Init( void )                    */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-08-10                        */
/*������������:7109/7101 ����MAC110 ��̫��������ʼ��              */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵��������, ʧ��,ST_NO_ERROR,�ɹ�                         */   
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
ST_ErrorCode_t CH_MAC110_Init( void )
{
	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	S32 Err=0;
	s32_t ErrorValue;
	 /* Create semaphores */
	 Mutex_Init_Fifo(TCPIPi_Mutex_API);
	 /* Lock API */
	 TCPIPi_LockAPI();
 
	CONFIG_BASE_ADDRESS_MAPPED = SYS_MapRegisters(ST5197_CFG_BASE_ADDRESS ,  0x200 , "TCPIP_CFG");
	ILC_BASE_ADDRESS_MAPPED    = SYS_MapRegisters(ST5197_ILC_BASE_ADDRESS , 0x1000 , "TCPIP_ILC");
	/* Configure the OS+ registry */
	ErrorValue =registry_key_set(STMAC_CONFIG_PATH,STXMAC_CONFIG_COUNT                , (void *)1);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_INIT_FN          , (void *)TCPIPi_OSPLUS_STx5197_STxMAC_Init_fn);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_INIT_ARG         , (void *)MII_MODE);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_BASE             , (void *)SYS_MapRegisters(STMAC110_BASE_ADDRESS,0x2000,"STxMAC"));
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR         , (void *)&OS21_INTERRUPT_ETHERNET);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_ALT_INTR     , (void *)0);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR_LEVEL   , (void *)0);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR_TRIGGER , (void *)0);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_BURST_LEN_FN , (void *)TCPIPi_OSPLUS_STx5197_STxMAC_SetDmaBurstLength_fn);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_ADDRESS      , (void *)0);
#ifdef cab5197
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR         , (void *)0);
#else  
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR         , (void *)&OS21_INTERRUPT_ETH_MD);
#endif
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_ALT_INTR     , (void *)0);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_LEVEL   , (void *)0);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_TRIGGER , (void *)0);
#ifdef TESTAPP_LOOPBACK
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LOOPBACK     , (void *)1);
#endif
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LINK         , (void *)(OSPLUS_ETH_LINK_100MBPS|OSPLUS_ETH_LINK_FDX|OSPLUS_ETH_LINK_ANEG_ON));
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LINK_CB      , (void *)TCPIPi_OSPLUS_STx5197_STxMAC_SetLinkSpeed_fn);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_ENB_FN  , (void *)TCPIPi_OSPLUS_STx5197_STxMAC_PhyEnable_fn);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_RX_DESC      , (void *)TCPIP_BUF_MAX);
	ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_TX_DESC      , (void *)(TCPIP_BUF_MAX/4));

	PIO_ETHER_Reset(TRUE);
	ms_delay(10);
	PIO_ETHER_Reset(FALSE);
	ms_delay(10);
	PIO_ETHER_Reset(TRUE);
	ms_delay(100);

	/* Create the stxmac device */
	if (stmac_initialize()!=0)
	{
		ErrorValue = -1;
	}
  
	if(ErrorValue!=0)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Init() -> FAILED(%d)", ErrCode));
		 TCPIPi_UnlockAPI();
		return(ErrCode);
	}
	else
	{
		CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Init() -> OK"));
	}
	 TCPIPi_UnlockAPI();

	return(ErrCode);
}
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Init								*/
/* �������� : �����豸��ʼ��								*/
/* ��ڲ��� : 											*/
/* rpfun_CallBack,�ص�����ָ�룬��ʱ���ã����Ժ���չʹ��	*/
/* rph_NetDevice,�豸���									*/
/* ruc_MacAddr ,MAC��ַ									*/
/* ���ڲ��� : ��											*/
/* ����ֵ: �ɹ�����CHDRV_NET_OK,����ʧ��					*/
/* ע��:	�κ���һ�㲻�ظ�����							*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Init(void)
{
	ST_ErrorCode_t st_ErrorCode=ST_NO_ERROR;
	CHDRV_NET_RESULT_e CHDRV_NET_ret = CHDRV_NET_OK;
	int i_Loop = 0;
	CHDRV_NET_Version_t version;
	
#if STXMAC_DRIVER_DEBUG
	CHDRV_NET_SemaphoreCreate(1,&gp_print_seamaphore);
#endif
	/*��ʼ����̫��������*/
	st_ErrorCode=CH_MAC110_Init();
	if(st_ErrorCode != ST_NO_ERROR)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("Init ethernet controller fail!"));
		return CHDRV_NET_FAIL;		
	}
	/*TX Semaphore init*/
	CHDRV_NET_ret = CHDRV_NET_SemaphoreCreate(0,&gp_write_seamaphore);
	if(CHDRV_NET_ret != CHDRV_NET_OK || gp_write_seamaphore == NULL)
	{
		st_ErrorCode = 1;
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Init create sem fail errcode(%d)", st_ErrorCode));
	}
	/*RX Semaphore init*/
	for(i_Loop = 0;i_Loop < MAX_READ_TASK;i_Loop++)
	{
		CHDRV_NET_ret = CHDRV_NET_SemaphoreCreate(0,&gp_read_seamaphore[i_Loop]);
		if(CHDRV_NET_ret != CHDRV_NET_OK || gp_read_seamaphore[i_Loop] == NULL)
		{
			st_ErrorCode = 1;
			CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Init create sem fail errcode(%d)", st_ErrorCode));
			break;
		}
	}
	
	CHDRV_NET_ret = CHDRV_NET_SemaphoreCreate(1,&gp_io_semaphore);
	if(CHDRV_NET_ret != CHDRV_NET_OK || gp_io_semaphore == NULL)
	{
		st_ErrorCode = 1;
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Init create sem fail errcode(%d)", st_ErrorCode));
	}
	
	if(st_ErrorCode == 0)
	{ 
		CHDRV_NET_GetVersion(&version);
		CHDRV_NET_Print("*******************************************\r\n");
		CHDRV_NET_Print("            CHDRV_NET INIT  OK!\r\n");
		CHDRV_NET_Print("            (v%d.%d.%d.%d)\r\n",version.i_InterfaceMainVer,version.i_InterfaceSubVer,version.i_ModuleMainVer,version.i_ModuleSubVer);
		CHDRV_NET_Print("            Built time: %s,%s\r\n",__DATE__,__TIME__);
		CHDRV_NET_Print("*******************************************\r\n");
		return CHDRV_NET_OK;
	}
	else
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Init fail errcode(%d)", st_ErrorCode));
		return CHDRV_NET_FAIL;
	}
}
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_RESULT_e CHDRV_TCPIP_OpenEthenet ( )    */
/*�����˺Ϳ���ʱ��:flex 2009-04-14                        */
/*������������:��һ����̫��������                               */
/*���������	*/
/*ri_NetifIndex:������������ */
/*�������:                                                     */
/*rph_NetDevice:������������*/
/*����ֵ˵����CHDRV_NET_OK��ʾ�����ɹ�,������ʾ����ʧ��*/   
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Open(s32_t ri_NetifIndex, CHDRV_NET_Handle_t *rph_NetDevice)
{
	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	device_handle_t* Ethernet_Hndl_p;
	CHDRV_NET_ASSERT(rph_NetDevice != NULL);
	CHDRV_NET_ASSERT(ri_NetifIndex == 0);
	Ethernet_Hndl_p = device_open (ETHER_DEVICE_NAME, OSPLUS_O_RDWR);
	if(Ethernet_Hndl_p == NULL)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Open() -> ERROR"));
		return CHDRV_NET_FAIL;
	}
	else
	{
		CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Open() -> OK"));
	}

	ErrCode = device_start(ETHER_DEVICE_NAME);
	if(ErrCode==ST_NO_ERROR)
	{
		gpch_Ethernet_Hndl_p = Ethernet_Hndl_p;
		(*rph_NetDevice) = (CHDRV_NET_Handle_t)gpch_Ethernet_Hndl_p;
	}
	else
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,( "device_start() -> ERROR"));
		return CHDRV_NET_FAIL;
	}
	/*��Ϊ�첽����ģʽ*/
#if CH_ASYNC_TYPE
	ErrCode |= device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_ASYNC_READ_SUPPORTED, (void *)1);
	ErrCode |= device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_ASYNC_WRITE_SUPPORTED, (void *)1);
#endif
#ifdef TESTAPP_LOOPBACK
	ErrCode |= device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_LOOPBACK_ON, NULL);
#endif	
	if(ErrCode != ST_NO_ERROR)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Init fail errcode(%d)", ErrCode));
		device_close (gpch_Ethernet_Hndl_p);
		return CHDRV_NET_FAIL;
	}
	return CHDRV_NET_OK;
}
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_Close */
/*�����˺Ϳ���ʱ��:flex 2009-04-14                        */
/*������������:�ر�һ����̫��������                               */
/*���������				*/
/*rph_NetDevice:�����豸���		*/
/*�������: ��                                                    */
/*����ֵ˵����CHDRV_NET_OK��ʾ�����ɹ�                   */   
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Close( CHDRV_NET_Handle_t rph_NetDevice)
{
	CHDRV_NET_ASSERT(rph_NetDevice != 0);
	device_close (gpch_Ethernet_Hndl_p);
	gpch_Ethernet_Hndl_p == NULL;
	return CHDRV_NET_OK;
}
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_GetMacAddr( )                     */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-08-10                        */
/*������������:�õ�оƬ�������õ�MAC��ַ*/
/*���������  rh_NetDevice,�豸���*/
/*�������:     rpuc_MacAddr,MAC��ַ*/                                              
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_GetMacAddr(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_MacAddr)
{
	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	CHDRV_NET_RESULT_e ErrRet = CHDRV_NET_OK;

	CHDRV_NET_ASSERT(rh_NetDevice!= 0);
	CHDRV_NET_ASSERT(rpuc_MacAddr!= NULL);

	ErrCode = device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_GET_INFO, &gst_Info);
	if(ErrCode == -1){
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CH_GetMacInfo() -> FAILED(%d)", ErrCode));
		device_close (gpch_Ethernet_Hndl_p);
		ErrRet = CHDRV_NET_FAIL;
		return(ErrRet);
	}else{
		CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,("CH_GetMacInfo() -> OK"));
	}
	
	memset(CHDRV_MAC_ADDR,0,6);
	ErrCode = device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_GET_MAC, CHDRV_MAC_ADDR);
	if(ErrCode == -1){
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,( "MAC110 Read MAC address -> FAILED()"));
		device_close (gpch_Ethernet_Hndl_p);
		ErrRet = CHDRV_NET_FAIL;
		return(ErrRet);
	}
	memcpy(rpuc_MacAddr,CHDRV_MAC_ADDR,6);
	CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,("STMAC110 driver MAC address:%02x:%02x:%02x:%02x:%02x:%02x",	\
	(unsigned char) CHDRV_MAC_ADDR[0],	\
	(unsigned char) CHDRV_MAC_ADDR[1],	\
	(unsigned char) CHDRV_MAC_ADDR[2],	\
	(unsigned char) CHDRV_MAC_ADDR[3],	\
	(unsigned char) CHDRV_MAC_ADDR[4],	\
	(unsigned char) CHDRV_MAC_ADDR[5]));

	ErrRet = CHDRV_NET_OK;
	return(ErrRet);
}
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetMacAddr(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������: ���������豸MAC��ַ                            */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:
/* rh_NetDevice,�豸���*/
/* rpuc_MacAddr,MAC��ַ*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetMacAddr(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_MacAddr)
{
	ST_ErrorCode_t st_ErrorCode=ST_NO_ERROR;
	CHDRV_NET_ASSERT(rpuc_MacAddr!=NULL);
	CHDRV_NET_ASSERT(rh_NetDevice!=0);
	st_ErrorCode|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_MAC_ADDRESS      , (void *)rpuc_MacAddr);
	st_ErrorCode |= device_ioctl ((device_handle_t*)rh_NetDevice, OSPLUS_IOCTL_ETH_SET_MAC, (void *)rpuc_MacAddr);
	if(st_ErrorCode==ST_NO_ERROR){
		return CHDRV_NET_OK;
	}else{
		return CHDRV_NET_FAIL;
	}
}
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetPHYMode(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ������������ģʽ                          */
/*����:		*/
/* rh_NetDevice,�豸���*/
/* renm_PHYMode,����ģʽ*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetPHYMode(
CHDRV_NET_Handle_t rh_NetDevice,
CHDRV_NET_PHY_MODE_e	renm_PHYMode)
{
	return CHDRV_NET_OK;
}
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetMultiCast(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ����Ϊ�ಥģʽ                          */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:
/* rh_NetDevice,�豸���*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetMultiCast(CHDRV_NET_Handle_t	rh_NetDevice)
{
	return CHDRV_NET_OK;
}
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_Term(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  �ر�NETģ��                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:��								*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Term (void)
{
	s32_t i_Loop = 0;
	CHDRV_NET_RESULT_e CHDRV_NET_ret = CHDRV_NET_OK;
	
	for(i_Loop = 0;i_Loop < MAX_READ_TASK;i_Loop++)
	{
		CHDRV_NET_ret = CHDRV_NET_SemaphoreFree(gp_read_seamaphore[i_Loop]);
		if(CHDRV_NET_ret != CHDRV_NET_OK )
		{
			CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Term create sem fail errcode(%d)", CHDRV_NET_ret));
			break;
		}
		else
		{
			gp_read_seamaphore[i_Loop] = 0;
		}
	}
	CHDRV_NET_ret = CHDRV_NET_SemaphoreFree(gp_io_semaphore);
	if(CHDRV_NET_ret != CHDRV_NET_OK )
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("CHDRV_NET_Term create sem fail errcode(%d)", CHDRV_NET_ret));
	}
	else
	{
		gp_io_semaphore = 0;
	}
	return CHDRV_NET_ret;
}
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_Reset*/
/*�����˺Ϳ���ʱ��:zengxianggen     2006-06-01                    */
/*������������:  ��λethenet оƬ                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:
/* rh_NetDevice,�豸���*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Reset(CHDRV_NET_Handle_t rh_NetDevice)
{
	STPIO_OpenParams_t    ST_OpenParams;
	STPIO_Handle_t        PHYPIOhandle;
	extern ST_DeviceName_t PIO_DeviceName[] ;
	PIO_ETHER_Reset(TRUE);
	ms_delay(10);
	PIO_ETHER_Reset(FALSE);
	ms_delay(10);
	PIO_ETHER_Reset(TRUE);
	ms_delay(100);
	return CHDRV_NET_OK;
}
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_GetLinkStatus*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������: �õ�link״̬                         */
/*����:
/* rh_NetDevice,�豸���*/
/* ���:*/
/* rpuc_LinkStatus,link״̬*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_GetLinkStatus(CHDRV_NET_Handle_t	rh_NetDevice,u8_t *rpuc_LinkStatus)
{
	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	unsigned int i_Status = 0;
	CHDRV_NET_ASSERT(rpuc_LinkStatus!=NULL);
	
	ErrCode = device_ioctl (gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_GET_STATUS, &i_Status);
	if(ErrCode != ST_NO_ERROR)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,("GET ethernet status failed!"));
		return CHDRV_NET_FAIL;
	}
	CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,("GET ethernet status = [%x]",i_Status));
	if(i_Status&(OSPLUS_ETH_STATUS_LINK_UP|OSPLUS_ETH_STATUS_HDX|OSPLUS_ETH_STATUS_FDX|OSPLUS_ETH_STATUS_10MBPS|OSPLUS_ETH_STATUS_100MBPS))
	{
		CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,("LINK UP!"));
		*rpuc_LinkStatus = 1;
	}
	else
	{
		CHDRV_NET_NOTICE(CHDRV_NET_DRIVER_DEBUG,("LINK DOWN!"));
		*rpuc_LinkStatus = 0;
	}
	return CHDRV_NET_OK;
	

}

/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_DebugInfo(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ��ʾһЩ������Ϣ                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:��								*/
/*����ֵ˵������*/
/*--------------------------------------------------------------------------*/
void CHDRV_NET_DebugInfo(void)
{
}

/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_GetPacketLength									*/
/* �������� : ��ȡ��ǰ��Ҫ�����յ����ݰ�����*/
/* ��ڲ��� : ��						*/
/* ���ڲ��� : ��																*/
/* ����ֵ:	����0����				*/
/* ע��:			*/
/*--------------------------------------------------------------------------*/
u16_t CHDRV_NET_GetPacketLength (CHDRV_NET_Handle_t rph_NetDevice)
{
	return 0;
}


/*EOF*/
