/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_os.h
  * ����: 	����OS��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#ifndef __EIS_API_OS__
#define __EIS_API_OS__

#define EIS_MAX_TASK_NUM	50		/* ��������50������ */
/*
  * struct: eis_task_t
  * desc:  ����eis�Ľ��̽ṹ
  */
typedef struct eis_task_s
{
	U32		mu32_TaskID;			/* ����ָ���ַ */
	U32		mu8_TaskPriority;		/* �������ȼ� */
	U8		*mp_TaskStack;			/* ���̶�ջָ�� */
	tdesc_t	mt_TaskDesc;			/* �������� */
}eis_task_t;

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
extern VOID *ipanel_porting_malloc(INT32_T memsize);

/*
����˵����
	�ͷ�ptr ָ�����ڴ�ռ䣬���Ҹ��ڴ�ռ��������ipanel_porting_malloc ����ġ�
����˵����
	���������
		ptr ��ָ��Ҫ�ͷŵ��ڴ��ָ�롣
	�����������
*/
extern VOID ipanel_porting_free(VOID *ptr);

/*
����˵����
	�������ʱ���ĳ�ʼ��
ע��:
	������ÿ���������ʼʱ����
  */
extern void eis_timeinit(void);

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
extern UINT32_T ipanel_porting_time_ms(VOID);

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
extern INT32_T ipanel_porting_system_reboot(INT32_T s);

/*
����˵����
	���������м���Ƿ�Ҫ����
����˵����
	��������� ��
	�����������
	�� �أ� ��
  */
extern void ipanel_porting_check_reboot ( void );

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
extern INT32_T ipanel_porting_system_standby(INT32_T s);


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
extern INT32_T ipanel_porting_task_destroy(UINT32_T handle);

/*
����˵����
	����ǰ�̹߳���ָ��ʱ�䣬ͬʱ�ó�CPU �������߳�ʹ�á�
����˵����
	���������
		ms�������̵߳�ʱ�䳤�ȣ���λΪ���롣
	����������ޣ�
	�� �أ���
*/
extern VOID ipanel_porting_task_sleep(INT32_T ms);

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
extern UINT32_T ipanel_porting_sem_create(const CHAR_T*name, INT32_T initial_count, UINT32_T wait_mode);

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
extern INT32_T ipanel_porting_sem_destroy(UINT32_T handle);


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
extern INT32_T ipanel_porting_sem_wait(UINT32_T handle,INT32_T wait_time);

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
extern INT32_T ipanel_porting_sem_release(UINT32_T handle);

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
extern UINT32_T ipanel_porting_queue_create(const CHAR_T *name, UINT32_T len, UINT32_T wait_mode);

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
extern INT32_T ipanel_porting_queue_destroy ( UINT32_T handle );

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
extern INT32_T ipanel_porting_queue_send(UINT32_T handle,IPANEL_QUEUE_MESSAGE *msg);

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
extern INT32_T ipanel_porting_queue_recv(UINT32_T handle,IPANEL_QUEUE_MESSAGE *msg, INT32_T wait_time);

#endif

/*--eof-----------------------------------------------------------------------------------------------------*/

