/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_info.h
  * ����: 	����info��صĽӿ�
  * ����:	������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#ifndef __EIS_API_INFO__
#define __EIS_API_INFO__
#define UI_ADDRESS 0xA0B00000
#define UI_SIZE  0x400000

/*
����˵����
	��ȡϵͳ�������Ϣ�����ַ����ķ�ʽ���أ�һ����Ҫ��ҳ��������ʾ�õġ�Name
	����Ϊһö��ֵ�����±�����ֵ���ܻ����ӣ�����ʱ����¿��ļ�ʱ����ͷ�ļ�Ϊ׼��
����˵����
	���������
		name��ϣ����ȡ����Ϣ���ͣ����ϱ��еĶ��壬ʹ��ʱ��ͷ�ļ�Ϊ׼��
		buf�����������ȡ��Ϣ��buffer �ĵ�ַ
		len�����������ȡ��Ϣ��buffer �ĳ���
	�����������
	�� �أ�
		> 0:��ȡ��Ϣ��ʵ�ʳ���;
		IPANEL_ERR:û�л�ȡ��ָ����Ϣ
  */
extern INT32_T ipanel_porting_system_get_info ( IPANEL_STB_INFO_e name, CHAR_T *buf, INT32_T len);

/*
����˵����
	�����iPanel Middleware ���ڲ�����(�ֿ⣬UI ֮��)�����洢�� Flash �ڴ���,
	�����ʵ�ָú������ڲ������� iPanel �ڷ���iPanel Middleware ���ļ���ͬʱ��
	��, �� STB �����̶�����Ž� Flash �ڴ��У�Ȼ��ͨ����ȷʵ�ָú���������
	iPanel Middleware ������ݵĴ洢��Ϣ��
 ����˵��:
	���������
		address������iPanel Middleware �������ݵĴ洢��ʼ��ַ��
		size������iPanel Middleware �������ݵĳ��ȡ�
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_get_outside_dat_info(CHAR_T **address, INT32_T *size, IPANEL_RESOURCE_TYPE_e type );

#endif

/*--eof------------------------------------------------------------------------------------------------*/

