/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_demux.h
  * ����: 	���嵥��������ȡ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-24
  * ===================================================================================
  */

#ifndef __EIS_API_DEMUX__
#define __EIS_API_DEMUX__

#define EIS_FILTER_SUM 					48 /* filter���������ֵ */
#define EIS_SLOT_SUM 					32	/* slot���������ֵ */
#define ulMaxFilterNumber 				32
#define EIS_DMX_INVALID_CHANNEL_ID 	(0xffffffff) /* Invalid channel ID */

#define EIS_MEM_PIECE				4096	/* һ�����ݵĴ�С */
#define EIS_MEM_NUM				250

typedef struct ipanel_mem_s
{
	//semaphore_t *         p_memsemaphore;
	int				channelindex;
	int                         filterindex;/* �Ƕ�Ӧ�Ǹ�filter������*/
	U8				mem_node[EIS_MEM_PIECE];
	U16				mem_len;
	U8				uOccupy;			/* �Ƿ�ռ�� */
}eis_mem_t;


/* define filter struct */
typedef struct EIS_FILTER_s
{
	boolean 				enable; 			/* �Ƿ��Ѿ��� */
	boolean 				bOccupy; 		/* �Ƿ�ռ�� */
	short 				iActFilter; 		/* ʵ�ʷ����filterֵ*/
	signed char 			slotDemux; 		/* ��Ӧ��slot������ */
	unsigned char 		irFilterLength; 	/* ���������� */
	boolean 				notequal;
	short 				table_id;
	unsigned char 		coef[16]; 		/* ���������� */
	unsigned char 		mask[16]; 		/* ���������� */
	unsigned char 		excl[16]; 		/* ���������� */
} EIS_FILTER_t;

/* ����irdetoʹ�õ�slot �ṹ */
typedef struct EIS_SLOT_s
{
	boolean 						bOccupy; 				/* �Ƿ�ռ�� */
	short 						iActSlot; 				/* ��ʵֵ */
	unsigned int 					PidValue; 				/* slot��Ӧ��pidֵ */
	IPANEL_DMX_NOTIFY_FUNC 		fctcallback; 				/* ���Ӻ��� */
	unsigned char 				ucMaxNoofFilters; 		/* slot��Ӧ��filter�������� */
	unsigned char 				ucNoofFiltersEngaged; 	/* slotʹ�õ�filter�ĸ��� */
	unsigned char 				ucFiltersStartNum; 		/* slotʹ�õ����ڴ򿪵� filter�ĸ��� */
	boolean 						bInUse; 					/* True :in use ,false no use*/
	IPANEL_DMX_CHANNEL_TYPE_e	e_ChannelType;			/* channel������ */
}EIS_SLOT_t;

/*
����˵����
		����demux ���ݵ���ʱ��֪ͨ�ص�������
		����������ʱ��ͨ������ص������������ݷ��͸�iPanel MiddleWare���ײ�Ӧ�ñ�֤
	�ص���������֮ǰ�������е����ݲ����Ķ�������Section(PES packet)Ӧ����һ��������
	buffer �У�����ײ�ʹ��ѭ��buffer �������ݣ�������ѭ��buffer ��β��ʱ������Ҫ��
	�������section(PES packet)����װ������
		����iPanel MiddleWare �Ļص�������ʹ�����ź���������ƣ��������������������
	���б����ã�����ͻ�ʵ��demux ���ݵĽ��չ����̣߳�Ȼ���ٴ��߳��е���iPanel
	MiddleWare �Ļص�������
	�ص������Ķ����ǣ�
		typedef VOID (*IPANEL_DMX_NOTIFY_FUNC)(UINT32_T channel, UINT32_T
		filter, BYTE_T *buf, INT32_T len)��
	��������˵�����£�
		channel �Cͨ�������������
		filter �C ���������������
		buf�C �ײ��ŵ�����Section(PES packet)���ݵ���ʼ��ַ��
		len �C buf ����Ч���ݵĳ��ȡ�
����˵����
	���������
		func�� �ص���������ڵ�ַ��
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_demux_set_notify(IPANEL_DMX_NOTIFY_FUNC func);

/*
����˵����
	����һ��ͨ����������
����˵����
	���������
		poolsize���ײ����õĸ�ͨ���Ļ���صĳߴ磨��0x10000(64kB)�ȣ�����ʵ
		�ʷ��仺���ʱ����Ҫ�������ֵ������������ÿ��ͨ����buffer
		��С��32KB��(64KB?)
		type: ͨ�����ͣ��������ö��������ȡֵ��
		typedef enum
		{
			IPANEL_DMX_DATA_PSI_CHANNEL = 0,
			IPANEL_DMX_DATA_PES_CHANNEL,
			IPANEL_DMX_VIDEO_CHANNEL,
			IPANEL_DMX_AUDIO_CHANNEL,
			IPANEL_DMX_PCR_CHANNEL
		} IPANEL_DMX_CHANNEL_TYPE_e;
	�����������
	�� �أ�
		== IPANEL_NULL: ����ͨ�����ɹ���
		!= IPANEL_NULL: ����һ��ͨ���������ľ����
*/
extern UINT32_T ipanel_porting_demux_create_channel(INT32_T poolsize, IPANEL_DMX_CHANNEL_TYPE_e type);

/*
����˵����
	��pid ���õ�ͨ����������
����˵����
	���������
		channel��ͨ�����������
		pid�� Ҫ���˵�pid
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_demux_set_channel_pid(UINT32_T channel,INT16_T pid);

/*
����˵����
	����һ��������������
����˵����
	���������
		channel��ͨ�������������
	�����������
	�� �أ�
		����IPANEL_NULL:�������ɹ���
		!=IPANEL_NULL:����һ�������������ľ����
*/
extern UINT32_T ipanel_porting_demux_create_filter(UINT32_T channel);

/*
����˵����
	�޸�һ�������������Ĺ��˲�����
����˵����
	���������
		channel�� ͨ�������������
		filter�� �����������ľ����
		wide�����˿�ȣ�
		coef����Ҫ���˵�section ͷ��ƥ�����飬���������wide ����ЧԪ��
		coef[0]~coef[wide��1]����������data&mask == coef ��������
		coef[0]�����������е�section ͷ�ĵ�һ���ֽڣ���table_id��
		coef[1]�����������е�section ͷ�ĵڶ����ֽڣ�
		coef[2]�����������е�section ͷ�ĵ������ֽڣ�
		coef[3]�����������е�section ͷ�ĵ��ĸ��ֽڣ�
		������ƣ�ֱ����wide ���ֽڡ�
		mask����Ҫ���˵�section ͷ���������飬���������wide ����ЧԪ��
		mask[0]~mask[wide��1]��
		mask[0]�����������е�section ͷ�ĵ�һ���ֽڵ�mask��
		mask[1]�����������е�section ͷ�ĵڶ����ֽڵ�mask��
		mask[2]�����������е�section ͷ�ĵ������ֽڵ�mask��
		mask[3]�����������е�section ͷ�ĵ��ĸ��ֽڵ�mask��
		������ƣ�ֱ����wide ���ֽڡ�
		excl �� ��Ҫ���˵�section ͷ���������飬 ���������wide ����ЧԪ��
		excl[0]~excl[wide��1]�����������з���ѡ��
		excl[0]�����������е�section ͷ�ĵ�һ���ֽڵ�excl��
		excl[1]�����������е�section ͷ�ĵڶ����ֽڵ�excl��
		excl[2]�����������е�section ͷ�ĵ������ֽڵ�excl��
		excl[3]�����������е�section ͷ�ĵ��ĸ��ֽڵ�excl��
		������ƣ�ֱ����wide ���ֽڡ�
		�����߼��������湫ʽ��
		��excl==0 ʱ��
		src&mask == coef&mask
		��excl!��0 ʱ��
		(src & (mask & ~excl))==(coef & (mask & ~excl))
		&& (src & (mask & excl))!=(coef & (mask & excl))
		ʹ�÷�����������Թ���section ���ݵĹ�����������wide Ϊ11�������Ҫ��table id
		Ϊ0x4E ��Section ��������һ���ֽڵĵ�4bit ��Ϊ1100B �����ݹ��˳������������Ĺ�
		������Ӧ������Ϊ��
		coef��[0x4E 0x00 0x00 0x0C 0x00 0x00(����5byte ��0)]
		mask��[0xFF 0x00 0x00 0xFF 0x00 0x00(����5byte ��0)]
		excl��[0x00 0x00 0x00 0x0C 0x00 0x00(����5byte ��0)]
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�	
*/
extern INT32_T ipanel_porting_demux_set_filter(UINT32_T channel, UINT32_T filter, UINT32_T wide, BYTE_T coef[], BYTE_T mask[], BYTE_T excl[]);


/*
����˵����
	����һ��ͨ�����������������ݣ�
����˵����
	���������
		channel�� ͨ�������������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ��
*/
extern INT32_T ipanel_porting_demux_start_channel(UINT32_T channel);

/*
����˵����
	ֹͣһ���������ݵ�ͨ�����������������ٹ�������
����˵����
	���������
		channel�� ͨ�����������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ��
*/
extern INT32_T ipanel_porting_demux_stop_channel(UINT32_T channel);

/*
����˵����
	����һ��ͨ����������
����˵����
	���������
		channel��ͨ�����������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_demux_destroy_channel(UINT32_T channel);

/*
����˵����
	ʹָ���Ķ��������������ٽ������ݡ�
����˵����
	���������
		channel��ͨ�������������
		filter���������������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_demux_disable_filter(UINT32_T channel,UINT32_T filter);


/*
����˵����
	ʹָ���Ķ������������Խ������ݡ�
����˵����
	���������
		channel��ͨ�������������
		filter���������������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ��
*/
extern INT32_T ipanel_porting_demux_enable_filter(UINT32_T channel, UINT32_T filter);

/*
����˵����
	����ָ���Ķ�����������
����˵����
	���������
		channel��ͨ�������������
		filter���������������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_demux_destroy_filter(UINT32_T channel, UINT32_T filter);

/*
����˵����
	��Demux ����һ�������������������úͻ�ȡDemux �豸�Ĳ��������ԡ�
����˵����
	���������
		op �� ��������
		typedef enum
		{
			IPANEL_DEMUX_SET_CHANNEL_NOTIFY =1,
			IPANEL_DEMUX_SET_SRC,
			IPANEL_DEMUX_SET_STREAM_TYPE,
			IPANEL_DEMUX_GET_BUFFER,
			IPANEL_DEMUX_PUSH_STREAM,
			IPANEL_DEMUX_STC_FETCH_MODE,
		} IPANEL_DEMUX_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
		op, arg ȡֵ����ֲ�ĵ�����
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_demux_ioctl(IPANEL_DEMUX_IOCTL_e op, VOID *arg);

/*
˵����init EnReach DMX data
���أ�OC_DEMUX_STATUS��
*/
extern BOOL Eis_DMX_Init(void);


/*
˵����init EnReach DMX data
���أ�OC_DEMUX_STATUS��
*/
extern void Eis_DMX_term ( void );

#endif

/*--eof-----------------------------------------------------------------------------------------------------*/

