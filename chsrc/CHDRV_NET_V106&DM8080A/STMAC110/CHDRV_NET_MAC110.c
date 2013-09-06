/*--------------------------------------------------------------------
DSP:���ļ�����smscЭ��ջ��ֲ��7101����(�Դ����������)ƽ̨�ϵĵײ�����
�շ����Ƶ�ʵ��
AUTHOR: WUFEI
TIME:2008-12-19
--------------------------------------------------------------------*/

#include "appltype.h"
#include "CHDRV_NET.h"
#include "CHDRV_NET_MAC110.h"
#include "initterm.h"


/*static*/ CHDRV_TCPIP_Handle_t*  gpch_Ethernet_Hndl_p = NULL;/*��̫���������ָ��*/
static u8_t CHDRV_MAC_ADDR[6];/*ȫ��buff����ָ��, MAC��ַ��Ϣ*/
ethernet_info_t  gst_Info;/*�õ���̫����������Ϣ*/
extern interrupt_name_t OS21_INTERRUPT_ETH_MAC;
extern interrupt_name_t OS21_INTERRUPT_MDINT     __attribute__((weak));
extern interrupt_name_t OS21_INTERRUPT_IRL_ENC_8 __attribute__((weak));
/*20071221 add*/
#define   MAX_REDA_TASK               14

#define tlm_to_virtual(add) add

static U32 CONFIG_BASE_ADDRESS_MAPPED;
static U32 ILC_BASE_ADDRESS_MAPPED;


#define CH_ASYNC_TYPE 1
//ethernet_async_t   g_async_read[MAX_REDA_TASK];
//ethernet_async_t   g_async_write[MAX_REDA_TASK];
CHDRV_NET_SEMAPHORE  gp_write_seamaphore;
CHDRV_NET_SEMAPHORE  gp_read_seamaphore[MAX_REDA_TASK];
CHDRV_NET_SEMAPHORE  gp_io_semaphore = NULL;
#define STMAC110_INT     (&OS21_INTERRUPT_IRL_ENC_8)
#define STMAC110_ALT_INT (&OS21_INTERRUPT_MDINT)
#define STMAC110_BASE_ADDRESS   0xB8110000                   /*��̫������оƬ�Ĵ�����ַ*/
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


U32 SYS_MapRegisters(U32 PhysicalAddress,U32 Size,char *Name)
{
 return((U32)STOS_MapRegisters((void *)PhysicalAddress,Size,Name));
}
/*
 * Perform any actions neccessary to initialise the hardware ready for the
 * stmac110 to function. The argument passed in will specify what mode to
 * operate the PHY in (MII, RMII using internal clock or RMII using external
 * clock).
 */
static int mb442_stmac110_init_fn (void *arg1, void *arg2)
{
  /* arg1 is the pointer to the stmac110 device instance - we don't use it */
  (void) arg1;

  /* Program system configuration registers for ethernet use, not DVO */
  OSPLUS_SET32(SYSTEM_CONFIG_7, _BIT(16));
  OSPLUS_SET32(SYSTEM_CONFIG_7, _BIT(17));

  /* Switch based on argument to determine MII/RMII mode to use */
  switch ((unsigned int) arg2)
  {
  case MII_MODE:
    /* MII interface active using internal clock */
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(18));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(19));
    break;

  case RMII_INT_MODE:
    /* RMII interface active using internal clock */
    OSPLUS_SET32(SYSTEM_CONFIG_7,   _BIT(18));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(19));
    break;

  case RMII_EXT_MODE:
    /* RMII interface active using external clock */
    OSPLUS_SET32(SYSTEM_CONFIG_7, _BIT(18));
    OSPLUS_SET32(SYSTEM_CONFIG_7, _BIT(19));
    break;

  default:
    CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG, ("ERROR: Unsupported argument (%d)", (unsigned int) arg2));
    return -1;
  }

  /* Set/clear any other system configuration bits based upon STb709 cut */
  if (((OSPLUS_READ32(SYSTEM_CONFIG_BASE) >> 28) & 0x0f) == 0)
  {
    /* We are dealing with a STb7109 cut 1.0 */
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(21));
    OSPLUS_SET32(SYSTEM_CONFIG_7,   _BIT(24));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(25));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(26));
  }
  else
  {
    /* We are dealing with a STb7109 cut 2.0 or later */
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(21));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(24));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(25));
    OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(26));
  }

  /* Enable external interrupts in system configuration 10 register */
  OSPLUS_SET32(SYSTEM_CONFIG_10, 0x0000000f);

  if (STMAC110_INT != NULL)
  {
    /* Old toolset (no ILC EXT INT support) */

    /* Route the interrupts from the ILC to the ST40 INTC */
    OSPLUS_WRITE32(ILC_PRIORITY_REG(64), 0x8004);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(65), 0x8005);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(66), 0x8006);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(67), 0x8007);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(70), 0x8007);

    /* Instruct ILC to trigger an interrupt when high */
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(64), 0x01);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(65), 0x01);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(66), 0x01);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(67), 0x01);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(70), 0x01);
  }

  return 0;
}


/*
 * Perform any actions neccessary to deinitialise the hardware. This function
 * is optional and need not be specified.
 */
static int mb442_stmac110_deinit_fn (void *arg1, void *arg2)
{
  /* We don't use either of the args */
  (void) arg1;
  (void) arg2;

  /* Ensure ethernet doesn't use pads shared with DVO */
  OSPLUS_CLEAR32(SYSTEM_CONFIG_7, 1<<16);

  /* Ensure MII interface pads are not functional */
  OSPLUS_CLEAR32(SYSTEM_CONFIG_7, 1<<17);

  if (STMAC110_INT != NULL)
  {
    /* Old toolset (no ILC EXT INT support) */

    /* Clear the interrupt routing */
    OSPLUS_WRITE32(ILC_PRIORITY_REG(64), 0x0);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(65), 0x0);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(66), 0x0);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(67), 0x0);
    OSPLUS_WRITE32(ILC_PRIORITY_REG(70), 0x0);

    /* Clear the ILC trigger mode */
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(64), 0x0);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(65), 0x0);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(66), 0x0);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(67), 0x0);
    OSPLUS_WRITE32(ILC_TRIGMODE_REG(70), 0x0);

    /* Disable the interrupts */
    OSPLUS_WRITE32(ILC_CLR_ENABLE_REG(64), _BIT(64));
    OSPLUS_WRITE32(ILC_CLR_ENABLE_REG(65), _BIT(65));
    OSPLUS_WRITE32(ILC_CLR_ENABLE_REG(66), _BIT(66));
    OSPLUS_WRITE32(ILC_CLR_ENABLE_REG(67), _BIT(67));
    OSPLUS_WRITE32(ILC_CLR_ENABLE_REG(70), _BIT(70));
  }

  return 0;

  return 0;
}


/*
 * Return the DMA burst length parameter that should be used on the target.
 * This parameter is specific to the STb7109 cut.
 */
static int mb442_stmac110_dma_burst_len_fn (void *arg1, void *arg2)
{
  /* arg1 is the pointer to the stmac110 device instance - we don't use it */
  (void) arg1;

  /* arg2 points to the unsigned int to put the burst length in to */
  OSPLUS_ASSERT(arg2 != NULL);

  /* Read the device ID from the system configuration registers */
  switch ((OSPLUS_READ32(SYSTEM_CONFIG_BASE) >> 28) & 0x0f)
  {
  case 0:  /* cut 1.0 */
    *((unsigned int *) arg2) = 1;
    return 0;

  default: /* cut 2.0 or later */
    *((unsigned int *) arg2) = 32;
    return 0;
  }
}


/*
 * This function needs to be called to enable the PHY interrupts. In most cases
 * this function just enables the external interrupts (the PHY interrupt is
 * usually connected to an external interrupt).
 */
static int mb442_stmac110_phy_intr_enb_fn (void *arg1, void *arg2)
{
  /* We don't use the args */
  (void) arg1;
  (void) arg2;

  if (STMAC110_INT != NULL)
  {
    /* Old toolset (no ILC EXT INT support) */
    /* Enable the MDINT interrupt now that it has been routed */
    OSPLUS_WRITE32(ILC_SET_ENABLE_REG(64), _BIT(64));
    OSPLUS_WRITE32(ILC_SET_ENABLE_REG(65), _BIT(65));
    OSPLUS_WRITE32(ILC_SET_ENABLE_REG(66), _BIT(66));
    OSPLUS_WRITE32(ILC_SET_ENABLE_REG(67), _BIT(67));
    OSPLUS_WRITE32(ILC_SET_ENABLE_REG(70), _BIT(70));
  }

  return 0;
}


/*
 * This function will be called each time the PHY link speed or duplex settings
 * change. It is required on STb7109 platforms so that system configuration 7
 * register is updated with the current link speed. The argument passed in is a
 * standard ethernet link speed and duplex define from osplus/ioctls/ethernet.h
 */
static int mb442_stmac110_phy_link_cb_fn (void *arg1, void *arg2)
{
  /* arg1 is the pointer to the stmac110 device instance - we don't use it */
  (void) arg1;

  /* Only process if the PHY link is up */
  if ((unsigned int) arg2 != 0)
  {
    /* Set the correct link speed in the system configuration 7 register */
    if ((unsigned int) arg2 & OSPLUS_ETH_LINK_10MBPS)
    {
      /* Set 10Mbps link speed */
      OSPLUS_CLEAR32(SYSTEM_CONFIG_7, _BIT(20));
    }
    else if ((unsigned int) arg2 & OSPLUS_ETH_LINK_100MBPS)
    {
      /* Set 100Mbps link speed */
      OSPLUS_SET32(SYSTEM_CONFIG_7, _BIT(20));
    }

    /* Set the correct link duplex in the system configuration 7 register */
    if ((unsigned int) arg2 & OSPLUS_ETH_LINK_HDX)
    {
      /* Set half duplex */
    }
    else if ((unsigned int) arg2 & OSPLUS_ETH_LINK_FDX)
    {
      /* Set full duplex */
    }
  }

  return 0;
}
/* ========================================================================
   Name:        TCPIPi_OSPLUS_STx7109_STxMAC_Init
   Description: Initialize a stmac110 interface
   ======================================================================== */

static U32 TCPIPi_OSPLUS_STx7109_STxMAC_Init(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   return(0xFFFFFFFF);
  }

 /* Ensure ethernet uses pads shared with DVO */
 /* ----------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<16));

 /* Ensure MII interface pads are functional */
 /* ---------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<17));

 /* arg determines if MII or RMII mode activated */
 /* -------------------------------------------- */
 switch((U32)arg)
  {
   /* MII interface active using internal clock */
   case 0: OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<18));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           break;
   /* MII interface active using external clock */
   case 1: OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<18));
           OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           break;
   /* RMII interface active using internal clock */
   case 2: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<18));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           break;
   /* RMII interface active using external clock */
   case 3: OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<18));
           OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           break;
  }

 /* Clear the following bits as recommended by validation */
 /* ----------------------------------------------------- */
 OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<21));
 OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<24));
 OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<25));
 OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<26));

 /* Enable external interrupts (should be enabled by default) */
 /* --------------------------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x128/*SYSTEM_CONFIG10*/),0x0000000F);

 /* Route Irq */
 /* --------- */
 /* Route the MDINT interrupt from the ILC to the ST40 INTC */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x800/*ILC_PRIORITY_REG*/+8*70),0x8007);
 /* Instruct ILC to trigger an interrupt when High */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x804/*ILC_TRIGMODE_REG*/+8*70),0x01);

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TCPIPi_OSPLUS_STx7109_STxMAC_SetDmaBurstLength
   Description: Set dma burst length for a stmac110 interface
   ======================================================================== */

static U32 TCPIPi_OSPLUS_STx7109_STxMAC_SetDmaBurstLength(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if ((stmac110==NULL)||(arg==NULL))
  {
   return(0xFFFFFFFF);
  }

 /* Read the device ID from the system configuration registers */

 	*((U32 *)arg)=32;

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TCPIPi_OSPLUS_STx7109_STxMAC_SetLinkSpeed
   Description: Set link speed of the phy using a stmac110 interface
   ======================================================================== */

static U32 TCPIPi_OSPLUS_STx7109_STxMAC_SetLinkSpeed(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   return(0xFFFFFFFF);
  }

 /* Set the correct link speed in the sys_cfg7 register */
 /* --------------------------------------------------- */
 if ((U32)arg&OSPLUS_ETH_LINK_100MBPS)
  {
   /* Set 100Mbps link speed */
   OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<20));
  }
 else
  {
   /* Set 10Mbps link speed */
   OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<20));
  }

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TCPIPi_OSPLUS_STx7109_STxMAC_PhyEnable
   Description: Enable the phy device interrupt
   ======================================================================== */

static U32 TCPIPi_OSPLUS_STx7109_STxMAC_PhyEnable(void *stmac110,void *arg)
{
 /* Check stmac110 context */
 /* ---------------------- */
 if (stmac110==NULL)
  {
   return(0xFFFFFFFF);
  }

 /* Enable the phy interrupt */
 /* ------------------------ */
 /* Enable the MDINT interrupt now that it has been routed */
 OSPLUS_WRITE32((ILC_BASE_ADDRESS_MAPPED+0x500/*ILC_SET_ENABLE_REG*/+4*(70/32)),(1<<(70%32)));

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TCPIPi_OSPLUS_STx7111_STxMAC_Init
   Description: Initialize a stgmac interface
   ======================================================================== */

static U32 TCPIPi_OSPLUS_STx7111_STxMAC_Init(void *stgmac,void *arg)
{
 /* Check stgmac context */
 /* -------------------- */
 if (stgmac==NULL)
  {
   return(0xFFFFFFFF);
  }

 /* Ensure ethernet uses pads shared with DVO */
 /* ----------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<16));

 /* arg determines if MII or RMII mode activated */
 /* -------------------------------------------- */
 switch((U32)arg)
  {
   /* PHY/MII interface active using internal clock of the backend */
   case 0: OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<24));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<25));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<26));
           break;
   /* PHY/MII interface active using external clock oscillator */
   case 1: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<24));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<25));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<26));
           break;
   /* PHY/RMII interface active using internal clock of the backend */
   case 2: OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<24));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<25));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<26));
           break;
   /* PHY/RMII interface active using external clock oscillator */
   case 3: OSPLUS_SET32  ((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<19));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<24));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<25));
           OSPLUS_CLEAR32((CONFIG_BASE_ADDRESS_MAPPED+0x11C/*SYSTEM_CONFIG7*/),(1<<26));
           break;
  }

 /* Enable external interrupts (should be enabled by default) */
 /* --------------------------------------------------------- */
 OSPLUS_SET32((CONFIG_BASE_ADDRESS_MAPPED+0x128/*SYSTEM_CONFIG10*/),0x0000000F);

 /* Route Irq */
 /* --------- */

 /* Return no errors */
 /* ---------------- */
 return(0);
}
#define ST7109_CFG_BASE_ADDRESS               tlm_to_virtual(0x19001000) /*SysConf registers*/
#define ST7109_ILC_BASE_ADDRESS               tlm_to_virtual(0x18000000)

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


#if 0
	/* Specify a function to initialise the stmac110 hardware */
	registry_key_set(STMAC110_CONFIG_PATH"/0",    STMAC110_CONFIG_INIT_FN,      mb442_stmac110_init_fn);
	/* Argument to stmac110 hardware initialise function to specify MII mode */
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_INIT_ARG,         MII_MODE);

	/* Function to determine correct DMA burst length setting to use */
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_BURST_LEN_FN, mb442_stmac110_dma_burst_len_fn);

	/* Function to be called each time the PHY link settings change */
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_LINK_CB,      mb442_stmac110_phy_link_cb_fn);

	/* Function to enable the PHY interrupt at the hardware level */
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_INTR_ENB_FN,  mb442_stmac110_phy_intr_enb_fn);

	/*Set MAC Packet filter*//*flex 2009-4-16*/
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_MAC_PKT_FILTER, (void *)(OSPLUS_ETH_FILTER_DIRECTED|OSPLUS_ETH_FILTER_BROADCAST));

	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_LINK,  (void *)(OSPLUS_ETH_LINK_100MBPS|OSPLUS_ETH_LINK_FDX));
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_RX_DESC,  (void *)(1024));
	registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_TX_DESC,  (void *)(1024));
	//registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_RX_DESC,  (void *)(1024));
	// registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_TX_DESC,  (void *)(1024));


	/*���õ�ǰϵͳSTMAC100��̫������������*/
	Err =  registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_COUNT            , (void *)1);
	/*�������õ�ǰϵͳSTMAC100��̫���������Ĵ�����ַ*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_BASE             , (void *)STMAC110_BASE_ADDRESS);
	/*�������õ�ǰϵͳSTMAC100��̫���������ж�*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_INTR         , (void *)&OS21_INTERRUPT_ETH_MAC/*1*/);
	/*�������õ�ǰϵͳSTMAC100��̫����������ѡ�ж�*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_ALT_INTR     , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫���������ж�LEVEL*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_INTR_LEVEL   , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫���������ж�TRIGGER*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_DMA_INTR_TRIGGER , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫����������ʼ��ʱ���MAC MODULE*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_MMC_MODULE       , (void *)0);
#if 0/*20070813 change*/
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY MODULE*/    
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_MODULE       , (void *)1);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�豸����*/    
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_TYPE         , (void *)STMAC110_PHY_TYPE_STE101P);
#endif    
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�ڲ���ַ*/    
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_ADDRESS      , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�ж�*/    
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_INTR         , (void *)&OS21_INTERRUPT_IRL_ENC_8/*2*/);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY��ѡ�ж�*/   
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_ALT_INTR     , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�ж�LEVEL*/     
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_INTR_LEVEL   , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�ж�TRIGGER*/     
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_INTR_TRIGGER , (void *)0);
	/*�������õ�ǰϵͳSTMAC100��̫������������PHY�Ƿ��Զ�����auto-negotiation,0Ϊ�Զ�����*/     
	/*Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_ANEG_OFF     , (void *)0);*/
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_LINK,  (void *)(OSPLUS_ETH_LINK_100MBPS|OSPLUS_ETH_LINK_FDX));

	/*�������õ�ǰϵͳSTMAC100��̫������������PHYģʽ��
	0 - The PHY should operate in MII mode.
	1 - The PHY should operate in RMII mode using an internal clock.
	2 - The PHY should operate in RMII mode using an external clock.*/         
	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_INIT_ARG, (void *)0); /* 0:MII */ /* 1: RMII Internal clock */ /* 2: RMII External clock */

//	Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_TASK_PRI         , (void *0);
	//Err |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PLUGINS         , (void *)0);
#else
  S32 ErrorValue;
 CONFIG_BASE_ADDRESS_MAPPED = SYS_MapRegisters(ST7109_CFG_BASE_ADDRESS ,  0x200 , "TCPIP_CFG");
 ILC_BASE_ADDRESS_MAPPED    = SYS_MapRegisters(ST7109_ILC_BASE_ADDRESS , 0x1000 , "TCPIP_ILC");

  /* Configure the OS+ registry */
  /* -------------------------- */
  ErrorValue =registry_key_set(STMAC_CONFIG_PATH,STXMAC_CONFIG_COUNT                , (void *)1);
//  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_MAC_ADDRESS      , (void *)TCPIPi_OSPLUS_Lan[LanIndex].LanMacAddress);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_INIT_FN          , (void *)TCPIPi_OSPLUS_STx7109_STxMAC_Init);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_INIT_ARG         , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_BASE             , (void *)SYS_MapRegisters(STMAC110_BASE_ADDRESS,0x2000,"STxMAC"));
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR         , (void *)&OS21_INTERRUPT_ETH_MAC);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_ALT_INTR     , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR_LEVEL   , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_INTR_TRIGGER , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_BURST_LEN_FN , (void *)TCPIPi_OSPLUS_STx7109_STxMAC_SetDmaBurstLength);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_ADDRESS      , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR         , (void *)&OS21_INTERRUPT_IRL_ENC_8);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_ALT_INTR     , (void *)&OS21_INTERRUPT_MDINT);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_LEVEL   , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_TRIGGER , (void *)0);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LOOPBACK     , (void *)1);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LINK         , (void *)(OSPLUS_ETH_LINK_100MBPS|OSPLUS_ETH_LINK_FDX|OSPLUS_ETH_LINK_ANEG_ON));
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_LINK_CB      , (void *)TCPIPi_OSPLUS_STx7109_STxMAC_SetLinkSpeed);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_PHY_INTR_ENB_FN  , (void *)TCPIPi_OSPLUS_STx7109_STxMAC_PhyEnable);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_RX_DESC      , (void *)1024);
  ErrorValue|=registry_key_set(STMAC_CONFIG_PATH"/0",STXMAC_CONFIG_DMA_TX_DESC      , (void *)1024);
#endif
	ErrCode = stmac110_initialize();
//	ErrCode = ch_stmac110_initialize();
//	OSPLUS_WRITE32 ( STMAC110_BASE_ADDRESS+ (MAC_FLOW_CONTROL),(MAC_FLOW_CONTROL_FCE));

	if(ErrCode == -1)
	{
		CHDRV_NET_ERROR(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Init() -> FAILED(%d)", ErrCode));
		return(ErrCode);
	}
	else
	{
		CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,( "MAC110_Init() -> OK"));
	}

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

	CHDRV_NET_Version_t version;
       int	i_Loop;
#ifdef TESTAPP_LOOPBACK
	st_ErrorCode = registry_key_set (STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_PHY_LOOPBACK,(void *) 1);
#endif

	OSPLUS_UNCACHED_ADDR(st_ErrorCode);
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
	for(i_Loop = 0;i_Loop < MAX_REDA_TASK;i_Loop++)
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
/*������:CHDRV_TCPIP_Handle_t *CHDRV_TCPIP_OpenEthenet ( )    */
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
	int i_Loop = 0;
	int i;
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

#if 0
{
    U32 OSPLUS_Flags=0;

    OSPLUS_Flags |= OSPLUS_ETH_FILTER_DIRECTED;/**/

    /* Set promiscuous mode */
    {
     //   OSPLUS_Flags |= OSPLUS_ETH_FILTER_PROMISCUOUS;
    }

    /* Set receive of all multicasts */

    {
      //  OSPLUS_Flags |= OSPLUS_ETH_FILTER_ALL_MULTICAST;;
    }

    /* Set receive of all broadcasts */
    {
        OSPLUS_Flags |= OSPLUS_ETH_FILTER_BROADCAST;
    }

    /* Program a new multicast table */

     /* Update filter packet mode */
    if(device_ioctl(gpch_Ethernet_Hndl_p, OSPLUS_IOCTL_ETH_SET_PKT_FILTER, (void *)OSPLUS_Flags) !=0 )
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Set Packet Filter() -> FIALED"));
    }

} 
#endif
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
	int i;
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
	CHDRV_NET_TRACE(CHDRV_NET_DRIVER_DEBUG,("STMAC110 driver MAC address:%02x:%02x:%02x:%02x:%02x:%02x\n",	\
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
	st_ErrorCode |= registry_key_set(STMAC110_CONFIG_PATH"/0",STMAC110_CONFIG_MAC_ADDRESS,(void *) rpuc_MacAddr);                                           
	st_ErrorCode |= device_ioctl ((CHDRV_TCPIP_Handle_t*)rh_NetDevice, OSPLUS_IOCTL_ETH_SET_MAC, (void *)rpuc_MacAddr);
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
	
	for(i_Loop = 0;i_Loop < MAX_REDA_TASK;i_Loop++)
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
STPIO_Handle_t g_PHYPIOhandle;

CHDRV_NET_RESULT_e CHDRV_NET_Reset(CHDRV_NET_Handle_t rh_NetDevice)
{
	STPIO_OpenParams_t    ST_OpenParams;
	
	extern ST_DeviceName_t PIO_DeviceName[] ;
#if 1
	ST_OpenParams.ReservedBits      = PIO_BIT_3|PIO_BIT_4|PIO_BIT_6;
	ST_OpenParams.BitConfigure[3]  = STPIO_BIT_BIDIRECTIONAL;     
	ST_OpenParams.BitConfigure[4]  = STPIO_BIT_OUTPUT;     
	ST_OpenParams.BitConfigure[6]  = STPIO_BIT_OUTPUT;          
	ST_OpenParams.IntHandler       = NULL;         
	
	STPIO_Open(PIO_DeviceName[2], &ST_OpenParams, &g_PHYPIOhandle );
	/*PIO2.3 ����Ϊ��*/
	STPIO_Clear(g_PHYPIOhandle,0x08); 
      /*PIO2.4 ����Ϊ��*/
	STPIO_Set(g_PHYPIOhandle,0x10); 

	  
	
       STPIO_Clear(g_PHYPIOhandle,0x40); 
	
	MILLI_DELAY(100);

	STPIO_Set(g_PHYPIOhandle,0x40); 
	MILLI_DELAY(100); 	
     //  STPIO_Close(PHYPIOhandle);
   	dm8606_int();
#else
	ST_OpenParams.ReservedBits      = PIO_BIT_6;
	ST_OpenParams.BitConfigure[6]  = STPIO_BIT_OUTPUT;          
	ST_OpenParams.IntHandler       = NULL;         
	
	STPIO_Open(PIO_DeviceName[2], &ST_OpenParams, &PHYPIOhandle );
	STPIO_Clear(PHYPIOhandle,0x40); 
	MILLI_DELAY(100);

	STPIO_Set(PHYPIOhandle,0x40); 
	MILLI_DELAY(100); 	
#endif
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
	u8_t Status = 0;
	CHDRV_NET_ASSERT(rpuc_LinkStatus!=NULL);
	Status += CH_DMPORT0_LINKUP();
	Status += CH_DMPORT1_LINKUP();
	Status += CH_DMPORT2_LINKUP();
	Status += CH_DMPORT3_LINKUP();
	*rpuc_LinkStatus = Status;
	return CHDRV_NET_OK;
}

/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_DebugInfo(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  �ر�NETģ��                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:��								*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
void CHDRV_NET_DebugInfo(void)
{
}
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
	CHDRV_NET_SemaphoreSignal(gp_print_seamaphore,-1);
	
#endif
}


/*EOF*/
