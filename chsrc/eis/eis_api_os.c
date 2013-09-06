/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_os.c
  * ����: 	ʵ��OS��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"

#include "eis_include\ipanel_os.h"
#include "eis_api_os.h"
#include "stlite.h"
#include "..\main\initterm.h"
#include "..\dbase\vdbase.h"
#ifdef __EIS_API_DEBUG_ENABLE__
#define __EIS_API_OS_DEBUG__
#endif
extern ST_Partition_t   *gp_appl2_partition;
 extern BOX_INFO_STRUCT	*pstBoxInfo;    /* Box information from e2prom */



ST_Partition_t           *gp_Eis_partition_VID_LMI = NULL;


/*������:     void CH_Eispartition_init_VIDLMI(void)
  *������Ա:
  *����ʱ��:
  *��������:��ʼ��Ӧ����64 EIS �ڴ�
  *�����㷨:
  *���õ�ȫ�ֱ����ͽṹ:
  *���õ���Ҫ����:��
  *����ֵ˵��:��
  *������:��                                           */
void CH_Eispartition_init_VIDLMI(void)
{
    U8 *p_Mem;
      p_Mem = ( U8 *)(osplus_cached_address((U8*)(0xB4000000),true));
      gp_Eis_partition_VID_LMI = partition_create_heap((U8*)p_Mem,0x03800000);	
}


static eis_task_t geis_task[EIS_MAX_TASK_NUM];	/* ����task�ṹ */
static semaphore_t *gpsema_eis_time	= NULL;	/* ���ڱ�֤ipanel_porting_time_ms������*/

/*
����˵����
	�������ָ����С���ڴ�顣�ڴ���亯���� iPanel MiddleWare�����ɴε��ø�
	������iPanel MiddleWare Ҫ��ÿ�ε��øú���ϵͳ������ڴ�ռ����������������
	�ڴ档
����˵����
	���������
		memsize��������ڴ��С�����ֽ�Ϊ��λ������0��Ч��С�ڵ���0��Ч��
	�����������
	�� �أ�
		!=IPANEL_NULL: �ɹ���������Ч�ڴ�����ʼ��ַ��
		==IPANEL_NULL��ʧ�ܣ����ص��ǿ�ָ�롣
*/
VOID *ipanel_porting_malloc(INT32_T memsize)
{
	void *p_return = NULL;
      
	p_return = memory_allocate (gp_appl2_partition  , memsize );

	if ( NULL == p_return )
	{

	#ifdef __EIS_API_OS_DEBUG__
		eis_report ( "\nEIS no mem<%d>", memsize );
	#endif

             	p_return = memory_allocate ( gp_Eis_partition_VID_LMI, memsize );

		memset(p_return,0,memsize);
		   
		return p_return;

	}
	
	memset(p_return,0,memsize);

	return p_return;
}

/*
����˵����
	�ͷ�ptr ָ�����ڴ�ռ䣬���Ҹ��ڴ�ռ��������ipanel_porting_malloc ����ġ�
����˵����
	���������
		ptr ��ָ��Ҫ�ͷŵ��ڴ��ָ�롣
	�����������
*/
VOID ipanel_porting_free(VOID *ptr)
{
	if ( NULL == ptr )
	{
		return;
	}

       if((U32)ptr < 0x94000000)
	{
	memory_deallocate ( gp_appl2_partition, ptr );
	}
	else
	{
	     memory_deallocate ( gp_Eis_partition_VID_LMI, ptr );
	}


	ptr = NULL;

	return;
}

#define TICKS_PER_SEC				( ST_GetClocksPerSecond() )
#define TICKS_MS_PER_OVERFLOW		( (0xffffffff  / TICKS_PER_SEC)*1000 )
#define TICKS_PER_MS				( ST_GetClocksPerSecond()  / 1000 )

typedef struct  tagEIS_TIME{
		unsigned long last_ticks;
		unsigned long overflow_times;
}eis_time_t;

static eis_time_t eis_time;
static unsigned long last_ticks;

/*
����˵����
	�������ʱ���ĳ�ʼ��
ע��:
	������ÿ���������ʼʱ����
  */
void eis_timeinit(void)
{
	eis_time.last_ticks 		= 0;
	eis_time.overflow_times 	= 0;
	gpsema_eis_time = semaphore_create_fifo(1);
}

UINT32_T get_time_ms(void)
{
	unsigned long cur_ticks;

	cur_ticks = time_now();

	return cur_ticks / TICKS_PER_MS;	

}
/*
����˵����
	iPanel Middleware ���ô˺�����ȡ��ǰʱ���STB ����ʱ��֮�侭���ĺ�������
	ipanel���ڻ�û�д�����ʱ���������⣬�������������������ᵼ�����벻����
	�����������������iPanel Middleware����������ʱ�����ֵ��49�졣����˺�����
	��õ�ʱ����������Ԫ�ؼ���õ���ͬ��Ҫע��������⡣���⣬ʵ���߱��뱣֤ͨ��
	ipanel_porting_time_ms()��õ�ʱ��Ҫ�������ԣ���ȷ�ԣ���ʵ�ʵ���ֲ�����У���
	�����ⶼ���������������ʵ���ϡ�
ע�⣺�ú�����ʹ��Ƶ�ʺܸߣ���ȷ����ִ��Ч�ʾ����ܸ�Ч������ֵ׼ȷ��ʵ��
	�ߵ�ʱ�Ӿ��������С��1 ���롣
����˵����
  */
UINT32_T ipanel_porting_time_ms(VOID)
{
	unsigned long cur_ticks;
#if 1
	if(NULL != gpsema_eis_time)	
		semaphore_wait(gpsema_eis_time);
	
	cur_ticks 	 = time_now ();
	if((eis_time.last_ticks >cur_ticks)&&(eis_time.last_ticks -cur_ticks>500)&&(eis_time.last_ticks>0xffff0000))
	{
		eis_time.overflow_times+=eis_time.last_ticks/TICKS_PER_MS;
		eis_report("\n time overflow!!");
	}
	eis_time.last_ticks =cur_ticks;
	cur_ticks = cur_ticks / TICKS_PER_MS+eis_time.overflow_times;
	
	if(NULL != gpsema_eis_time)	
		semaphore_signal ( gpsema_eis_time );
	return (cur_ticks);

#else

	cur_ticks = time_now();

	return cur_ticks / TICKS_PER_MS;	
#endif

}

/*
����˵����
	���������м���Ƿ�Ҫ����
����˵����
	��������� ��
	�����������
	�� �أ� ��
  */
void ipanel_porting_check_reboot ( void )
{
	int i;

	if ( 0 == geis_reboot_timeout )
	{
		return;
	}
	else
	{
		i = time_after ( time_now (), geis_reboot_timeout );

		if ( 1 == i )
		{
			reset_cpu ();	/* �������� */
		}
	}

	return;
}

/*
����˵����
	����ϵͳ�ڶ������Ӻ�����������
����˵����
	���������
		s�������������������s ��λΪ�롣s��0����ʾ��������ϵͳ��
		�����������
	�� �أ�
		IPANEL_OK: �ɹ���
		IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_system_reboot(INT32_T s)
{
	if ( 0 == s )
	{
		reset_cpu ();	/* �������� */
	}
	else
	{
		geis_reboot_timeout = time_plus ( time_now (), ST_GetClocksPerSecondLow() );
	}
}

/*
����˵����
	����ϵͳ�������ģʽ������ָ��ʱ�������Ϊ��λ���ָ���������ģʽ��ipanel
	Middleware ��ϵͳ�Ƿ�ʹ�������û��Ҫ�󣬿ͻ������Լ���Ʒ�����������δ�
	��
����˵����
	���������
		s������s ���ָ�����ģʽ��IPANEL_WAIT_FOREVER(-1)Ϊ���ô���
	�����������
	�� �أ�
		IPANEL_OK : �ɹ���
		IPANEL_ERR : ʧ�ܡ�
*/
INT32_T ipanel_porting_system_standby(INT32_T s)
{
	eis_report("\n ipanel_porting_system_standby s=%d",s);
	switch(s)
	{
		case -1:
			pstBoxInfo->abiBoxPoweredState=1;
			//CH_NVMUpdate(idNvmBoxInfoId); 
			eis_report("\n ����pstBoxInfo->abiBoxPoweredState=%d",pstBoxInfo->abiBoxPoweredState);
			return IPANEL_OK;
		case -3:
			pstBoxInfo->abiBoxPoweredState=0;
			//CH_NVMUpdate(idNvmBoxInfoId); 
			eis_report("\n ����pstBoxInfo->abiBoxPoweredState=%d",pstBoxInfo->abiBoxPoweredState);
	return IPANEL_OK;
}
}

/*
����˵��:
	eis���̴���
ע��:
	��ϵͳ��ʼ����ʱ�����
����˵��:
	��
  */
void ipanel_porting_task_init ( void )
{
	int i;

	for ( i = 0; i < EIS_MAX_TASK_NUM; i ++ )
	{
		geis_task[i].mu32_TaskID 		= 0;
		geis_task[i].mu8_TaskPriority	= 0;
		geis_task[i].mp_TaskStack		= NULL;
	}
}

/*
����˵����
	���ڴ���һ���̡߳�
����˵����
	���������
		name��һ��������ֽڳ��ַ�����ϵͳ���߳�����Ӧ��Ψһ��
		func���߳����庯����ڵ�ַ������ԭ�Ͷ������£�
		typedef VOID (*IPANEL_TASK_PROC)(VOID *param);
		param���߳����庯���Ĳ����б�ָ��(����ΪIPANEL_NULL)��
		priority�����ȼ���(ipanel ���ȼ���0 ��31��0 ���,31 ���)��
		stack_size��ջ��С�����ֽ�Ϊ��λ
	����������ޣ�
	�� �أ�
		!= IPANEL_NULL���ɹ��������߳�ʵ�������
		����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_task_create(CONST CHAR_T *name, IPANEL_TASK_PROC func, VOID *param, INT32_T priority, UINT32_T stack_size)
{
	int 		prio;
	UINT32_T p_Task;
	int		i, i_Error;
	static tdesc_t				g_UsifTaskDesc;


	/* Ѱ�ҿ���δ�õ�task */
	for ( i = 0; i < EIS_MAX_TASK_NUM; i ++ )
	{
		if ( 0 == geis_task[i].mu32_TaskID && 0 == geis_task[i].mu8_TaskPriority )
		{
			break;
		}
	}

	if ( i >= EIS_MAX_TASK_NUM )
	{
		eis_report ( "\n++>eis �޿��ý���!" );
		return IPANEL_NULL;
	}

	prio = priority / 2 +3;
	if(prio>15)
		prio=12;

	if(memcmp(name,"SCKT",4)==0)
	{
		prio = 12;
	}
	eis_report ( "\n++>eis ipanel_porting_task_create taskname=%s, priority=%d,actpri=%d" ,name,priority,prio);
#if 0
	geis_task[i].mp_TaskStack	= memory_allocate ( SystemPartition, stack_size );

	if ( NULL == geis_task[i].mp_TaskStack )
	{
		eis_report ( "\n++>eis ���̿ռ����ʧ��!" );
		return IPANEL_NULL;
	}
#endif	
	geis_task[i].mu8_TaskPriority = prio;

	p_Task = (UINT32_T)Task_Create ( (void(*)(void*))*func, param,stack_size+8*1024, prio, (const char *)name, 0 );

	geis_task[i].mu32_TaskID 		= p_Task;

	eis_report ( "\n++>eis task id=0x%x", p_Task );
	
	return p_Task;
}

/*
����˵����
	��������һ���̡߳�
����˵����
	���������
		handle���߳̾��(��0 �Ҵ��ڣ���Ч)��
	����������ޣ�
	�� �أ�
		IPANEL_OK���ɹ���
		IPANEL_ERR��ʧ��
*/
INT32_T ipanel_porting_task_destroy(UINT32_T handle)
#if 0
{
	int rt;
	task_t *hwtsk = NULL;

	if(handle)
	{
		hwtsk = (task_t*)handle;

		if(hwtsk->__id->task_state == 0)
		{
			eis_report ("HW LOCK(%s %s\n",__FILE__,__LINE__);  
			task_lock();
			hwtsk->__id->task_state = 1;
			hwtsk->__id->task_tdesc = hwtsk->__desc;
			task_unlock();
			eis_report ("HW UNLOCK(%s %s\n",__FILE__,__LINE__);
		}
		rt = task_suspend(hwtsk->__id);
		eis_report ("task_suspend %s\n", rt ? (char*)GetErrorText(rt) : "ok");/**/
		rt = task_kill(hwtsk->__id, 0, 0/*1*/);
		eis_report ("HW kILL (%s %s\n",__FILE__,__LINE__);
		/*�ȴ��������*/
		rt = task_wait(&hwtsk->__id, 1, TIMEOUT_INFINITY);
		eis_report ("HW task wait = %d\n", rt);
		/*����ɾ����ջ��*/
		rt = task_delete(hwtsk->__id);
		eis_report ("HW task delete = %d\n", rt );

		if(rt == 0)
		{
			CH_FreeMem(hwtsk->__desc);
			CH_FreeMem(hwtsk->__stack);
			CH_FreeMem(hwtsk->__id);
			CH_FreeMem(hwtsk);
		}
		else
			eis_report ("\n\n ### @@@ task_delete FAILED ### @@@ \n");
		//check_memStatus("vod destroy!!");   
		eis_report ("%s: delete task 0x%x return %d-- %s\n",rt ? "ERROR" : "INFO", handle, rt,  (char*)GetErrorText(rt));
	}
}
#else
{
	int i, error;
	task_t *task_list_p[2];

	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy ���Ϊ��" );
		return IPANEL_ERR;
	}

	for ( i = 0; i < EIS_MAX_TASK_NUM; i ++ )
	{
		if ( handle == geis_task[i].mu32_TaskID )
			break;
	}

	if ( i >= EIS_MAX_TASK_NUM )
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy û�ҵ���Ӧ�ľ��" );
		return IPANEL_ERR;
	}

	task_list_p[0]=(task_t*)handle;    
	task_list_p[1]=NULL;

	error = task_wait(task_list_p, 1, TIMEOUT_INFINITY);

	if ( 0 != error )
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy task_wait ����" );
		//return IPANEL_ERR;
	}
	else
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy task_wait ��ȷ" );
	}
	
	error = task_delete((task_t*)handle);
	handle = NULL;

	if(0!=error)
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy task_delete ����" );

		//return IPANEL_ERR;
	}
	else
	{
		eis_report ( "\n++>eis ipanel_porting_task_destroy task_delete ��ȷ" );		
	}

	/* �ͷſռ� */
	if ( NULL != geis_task[i].mp_TaskStack )
	{
		memory_deallocate ( SystemPartition, geis_task[i].mp_TaskStack );
		geis_task[i].mp_TaskStack = NULL;
	}

	geis_task[i].mu32_TaskID 		= 0;
	geis_task[i].mu8_TaskPriority	= 0;
	geis_task[i].mt_TaskDesc		= 0;

	return IPANEL_OK;
}
#endif

/*
����˵����
	����ǰ�̹߳���ָ��ʱ�䣬ͬʱ�ó�CPU �������߳�ʹ�á�
����˵����
	���������
		ms�������̵߳�ʱ�䳤�ȣ���λΪ���롣
	����������ޣ�
	�� �أ���
*/
VOID ipanel_porting_task_sleep(INT32_T ms)
{
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n <<<<[ipanel_porting_task_sleep] ms= %d\n",ms);
#endif

	task_delay ( ((unsigned int)(ST_GetClocksPerSecond()) * ms) / 1000 );
}

/*
����˵����
	����һ���ź�����iPanel MiddleWare ֻʹ�û����ź�������ʹ�ü����ź�����
����˵����
	���������
		name��һ��������ַ����ַ�����ϵͳ���ź���������Ӧ��Ψһ��
		initial_count������ʼ������(ֻ��0 ��1 ��Ч)
		wait_mode����������������ź�����Чʱ���ȴ����ź������̻߳������
		�Ĵ���������ѡ�
		�C IPANEL_TASK_WAIT_FIFO �C �������ȳ��ķ�ʽ�ڵȴ��߳��зַ�
		��Ϣ
		�C IPANEL_TASK_WAIT_PRIO �C ������������ȼ����̡߳�
	�����������
	�� �أ�
		����IPANEL_NULL�� �ɹ����ź��������
		����IPANEL_NULL ��ʧ�ܡ�
*/
UINT32_T ipanel_porting_sem_create(const CHAR_T*name, INT32_T initial_count, UINT32_T wait_mode)
{
	UINT32_T sema;
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n time=%d <<<<[ipanel_porting_sem_create] ms= %s",get_time_ms(),name);
#endif

	/* initial_countֻ��Ϊ0��1 */
	initial_count = ( initial_count > 1 || initial_count < 0 ) ? 1 : initial_count;

	sema = (UINT32_T)semaphore_create_fifo_timeout( (U32)initial_count );
	eis_report ( "  handel = %x\n",sema);

	return sema;
}

/*
����˵����
	����һ���ź�����
����˵����
	���������
		handle���ź����������ipanel_porting_sem_create ��á�
	�����������
	�� �أ�
		IPANEL_OK���ɹ���
		IPANEL_ERR��ʧ�ܡ�
*/
INT32_T ipanel_porting_sem_destroy(UINT32_T handle)
{
	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_sem_destroy ���Ϊ��" );
		return IPANEL_ERR;
	}
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n <<<<[ipanel_porting_sem_destroy] ms= %x\n",handle);
#endif

	semaphore_delete ( (semaphore_t *)handle );
	handle = 0;

	return IPANEL_OK;
}

/*
����˵����
	�ź����ȴ���
����˵����
	���������
		handle���ź����������ipanel_porting_sem_create ��á�
		wait_time���ȴ�ʱ�䣬��λΪ���롣ΪIPANEL_NO_WAIT(0)ʱ��ʾ���ȴ���
		�����أ�ΪIPANEL_WAIT_FOREVER(-1)��ʾ���õȴ���
		�����������
	�� �أ�
		IPANEL_OK����ʾ�ȴ����źţ�
		IPANEL_ERR����ʾû�еȵ��ź�,���쳣��ֹ��
*/
INT32_T ipanel_porting_sem_wait(UINT32_T handle,INT32_T wait_time)
#if 1
{
	int ret;
	clock_t  time;
	int val;

#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "<<<<[ipanel_porting_sem_wait] sem_handle = %x,%d\n",handle,wait_time);
#endif

	if ( NULL == handle )
	{
#ifdef __EIS_API_OS_DEBUG__
		eis_report ( "<<<<[ipanel_porting_sem_wait] failed  this sem didn't exist\n");
#endif
		return IPANEL_ERR;
	}		

	if(wait_time == 0)
	{
    		val = semaphore_wait_timeout((semaphore_t*)handle,TIMEOUT_IMMEDIATE);
		
	}
	else if(wait_time == -1)
	{
		
		val =  semaphore_wait_timeout((semaphore_t*)handle,TIMEOUT_INFINITY);	

	}
	else if(wait_time >0)
	{
		
		time = time_plus(time_now(), ST_GetClocksPerSecondLow()/1000*wait_time);
		val =  semaphore_wait_timeout((semaphore_t*)handle,&time);	
	}
	else
	{
#ifdef __EIS_API_OS_DEBUG__
		eis_report ( "<<<<[ipanel_porting_sem_wait] failed  wait_time ERR\n");
#endif
		return IPANEL_ERR;

	}
	
	if(val == 0)
	{
#ifdef __EIS_API_OS_DEBUG__
		eis_report ( "<<<<[ipanel_porting_sem_wait] sem_handle = %x SUCESSFUL \n",handle);
#endif
		ret = IPANEL_OK;
	}
	else
	{
#ifdef __EIS_API_OS_DEBUG__
		eis_report ( "<<<<[ipanel_porting_sem_wait] sem_handle = %x FAILED! \n",handle);
#endif
		ret = IPANEL_ERR;
	}
	
        return ret;
}
#else
{
	clock_t 	time_out;
	int		error;

	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_sem_wait ���Ϊ��" );
		return IPANEL_ERR;
	}

	eis_report ( "\n++>eis signal wait=<%s, 0x%x, %d>",  task_name((task_t*)task_id()), handle, wait_time );

	if ( 0 == wait_time )
	{
		error = semaphore_wait_timeout ( (semaphore_t *)handle, TIMEOUT_IMMEDIATE );
		if ( 0 == error )
		{
			eis_report ( "\n++>eis wait end1" );
			return IPANEL_OK;
		}
		else
		{
			eis_report ( "\n++>eis wait end2" );
			return IPANEL_ERR;
		}
		return IPANEL_OK;
	}
	else if ( -1 == wait_time )
	{
		semaphore_wait_timeout ( (semaphore_t *)handle, TIMEOUT_INFINITY );
		return IPANEL_OK;
	}
	else
	{
		time_out = time_plus ( time_now(), ST_GetClocksPerSecondLow() * wait_time / 1000 );
		error = semaphore_wait_timeout ( (semaphore_t *)handle, &time_out );
		if ( 0 == error )
		{
			eis_report ( "\n++>eis wait end3" );
			return IPANEL_OK;
		}
		else
		{
			eis_report ( "\n++>eis wait end4" );
			return IPANEL_ERR;
		}
	}

	return IPANEL_ERR;
}
#endif

/*
����˵����
	�ͷ��ź�����
����˵����
	���������
		handle���ź����������ipanel_porting_sem_create ��á�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_sem_release(UINT32_T handle)
{
	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_sem_release ���Ϊ��" );
		return IPANEL_ERR;
	}

#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n++>eis signal release=<%s, 0x%x>", task_name((task_t*)task_id()), handle );
#endif
	semaphore_signal ( (semaphore_t *)handle );
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n++>eis release end" );
#endif
	return IPANEL_OK;
}

/*
����˵����
	����һ����Ϣ���С�ÿ����Ϣ�ṹ����ΪIPANEL_QUEUE_MESSAGE������Ϊ16 ��
	�ڡ�
����˵����
	���������
		name��һ��������ַ����ַ�����ϵͳ�ж��е�����Ӧ��Ψһ��
		len��������ָ������������ܱ������Ϣ����������������Ϣ�ﵽ���ֵʱ��
		����������������Ϸ���Ϣ�Ĳ�����ʧ��
		wait_mode�������������������������Ϣʱ���ȴ��ڴ˶����ϵ��̻߳��
		����Ĵ���������ѡ�
		�C IPANEL_TASK_WAIT_FIFO �������ȳ��ķ�ʽ�ڵȴ��߳��зַ�
		��Ϣ
		�C IPANEL_TASK_WAIT_PRIO ������������ȼ����̡߳�
	�����������
		�� �أ�
		����IPANEL_NULL: �ɹ�����Ϣ���о��;
		����IPANEL_NULL: ʧ�ܡ�
*/
UINT32_T ipanel_porting_queue_create(const CHAR_T *name, UINT32_T len, UINT32_T wait_mode)
{
	UINT32_T queue;

	eis_report ( "\n++>eis ipanel_porting_queue_create" );

	queue = (UINT32_T)message_create_queue_timeout ( sizeof ( IPANEL_QUEUE_MESSAGE ), 50 );
	if ( 0 == queue )
   	{
		eis_report ( "\n++>eis ipanel_porting_queue_create fail<%s>", (char*)name );
		return IPANEL_NULL;
   	}
   	else
   	{
		symbol_register ( name, ( void * ) queue );
   	}

	return queue;
}

/*
����˵����
	����һ����Ϣ���С�
����˵����
	���������
		handle����Ϣ���о������ipanel_porting_queue_create ��á�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_queue_destroy ( UINT32_T handle )
{
	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_queue_destroy ��Ч���" );
		return IPANEL_ERR;
	}

	message_delete_queue ( (message_queue_t*)handle );
	handle = 0;

	return IPANEL_OK;
}

/*
����˵����
	������Ϣ��ע�⣺�������غ�msg ָ��Ľṹ�е����ݾ���Ч�ˣ�����ʵ����Ҫ
	ע���Լ����������Ϣ��
����˵����
	���������
		handle����Ϣ���о������ipanel_porting_queue_create ��á�
		msg��ָ��һ��IPANEL_QUEUE_MESSAGE �ṹ��һ��ָ�룬�ṹ�а���
		Ҫ�ŵ���Ϣ�����е���Ϣ
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_queue_send(UINT32_T handle,IPANEL_QUEUE_MESSAGE *msg)
{
	IPANEL_QUEUE_MESSAGE *pMsg;
	clock_t	time_out;
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n time=%d <<<<[ipanel_porting_queue_send] handle= %x",get_time_ms(),handle);
#endif

	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_queue_send ��Ч���" );
		return IPANEL_ERR;
	}

	time_out = time_plus ( time_now(), ST_GetClocksPerSecondLow() );

	pMsg = (IPANEL_QUEUE_MESSAGE *)message_claim_timeout ( (message_queue_t*)handle, &time_out );

	if ( NULL == pMsg )
	{
		eis_report ( "\n++>eis ipanel_porting_queue_send ��Ϣ����ʧ��" );
		return IPANEL_ERR;
	}

	pMsg->q1stWordOfMsg = msg->q1stWordOfMsg;
	pMsg->q2ndWordOfMsg = msg->q2ndWordOfMsg;
	pMsg->q3rdWordOfMsg = msg->q3rdWordOfMsg;
	pMsg->q4thWordOfMsg = msg->q4thWordOfMsg;

	message_send ( (message_queue_t*)handle, pMsg );

	return IPANEL_OK;
}

/*
����˵����
	��ָ�����н�����Ϣ����Ϣ������Ҫ֧��ָ��ʱ��ĳ�ʱ�ȴ�ģʽ��
����˵����
	���������
		handle����Ϣ���о������ipanel_porting_queue_create ��á�
		msg��ָ��һ��IPANEL_QUEUE_MESSAGE �ṹ��һ��ָ�룬����Ӷ���
		���յ�����Ϣ������Ϊ�ա�
		wait_time���ȴ�ʱ�䣬��λΪ���롣ΪIPANEL_NO_WAIT(0)ʱ��ʾ�������أ�
		ΪIPANEL_WAIT_FOREVER(-1)��ʾ���õȴ���
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_queue_recv(UINT32_T handle,IPANEL_QUEUE_MESSAGE *msg, INT32_T wait_time)
{
	clock_t time_out;
	IPANEL_QUEUE_MESSAGE *p_msg = NULL;
#ifdef __EIS_API_OS_DEBUG__
	eis_report ( "\n time=%d <<<<[ipanel_porting_queue_send] handle= %x",get_time_ms(),handle);
#endif

	if ( 0 == handle )
	{
		eis_report ( "\n++>eis ipanel_porting_queue_send ��Ч���" );
		return IPANEL_ERR;
	}

	if ( 0 == wait_time )
	{
		p_msg = (IPANEL_QUEUE_MESSAGE *)message_receive_timeout ( (message_queue_t*)handle, TIMEOUT_IMMEDIATE );

		if ( NULL == p_msg )
		{
			return IPANEL_ERR;
		}
	}
	else if ( -1 == wait_time )
	{
		p_msg = (IPANEL_QUEUE_MESSAGE *)message_receive_timeout ( (message_queue_t*)handle, TIMEOUT_INFINITY );

		if ( NULL == p_msg )
		{
			return IPANEL_ERR;
		}
	}
	else
	{
		time_out = time_plus ( time_now(), ST_GetClocksPerSecondLow() / 1000 * wait_time );
		p_msg = (IPANEL_QUEUE_MESSAGE *)message_receive_timeout ( (message_queue_t*)handle, &time_out );

		if ( NULL == p_msg )
		{
			return IPANEL_ERR;
		}
	}

	eis_report ( "\ncome here!" );
	msg->q1stWordOfMsg 	= p_msg->q1stWordOfMsg;
	msg->q2ndWordOfMsg = p_msg->q2ndWordOfMsg;
	msg->q3rdWordOfMsg 	= p_msg->q3rdWordOfMsg;
	msg->q4thWordOfMsg 	= p_msg->q4thWordOfMsg;
	message_release((message_queue_t*)handle, p_msg);

	return IPANEL_OK;
}

/*--eof-----------------------------------------------------------------------------------------------------*/

