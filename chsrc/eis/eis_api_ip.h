/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_ip.h
  * ����: 	����˫����صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#ifndef __EIS_API_IP__
#define __EIS_API_IP__

#define MAX_SOCKETS 16

/*
����˵����
	����һ���׽��֡�
����˵����
	���������
		family��ָ��Ҫ�������׽���Э���塣IPANEL_AF_INET����ʾIPv4 Э�飬
		IPANEL_AV_INET6 ��ʾIPv6 Э�飬����ֵ����
		type �� ָ��Ҫ�������׽��ֵ����͡�IPANEL_SOCK_STREAM �� ��ʾ
		SOCK_STREAM��IPANEL_SOCK_DGRAM����ʾSOCK_DGRAM��
		����ֵ����
		protocol��ָ������Э�顣ͨ������ΪIPANEL_IPPROTO_IP����ʾ��type ƥ
		���Ĭ��Э�飬����ֵ����
	�����������
	�� �أ�
		���׽��ֽ����ɹ����򷵻طǸ������������򷵻�-1��
		> �� 0: �׽��ֽ����ɹ����򷵻طǸ����������������ֵӦ�ò�����
		IPANEL_NFDBITS �Ķ��巶Χ��
		IPANEL_ERR: �����׽���ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_open(INT32_T family,INT32_T type, INT32_T protocol);


/*
����˵����
	��ָ���Ķ˿ں����׽��ְ���һ��
����˵����
	���������
		sockfd���׽���������
		ipaddr����������IP ��ַ��IP �汾�����ṹ���ָ��
		port���˿ں�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_bind(INT32_T sockfd,IPANEL_IPADDR *ipaddr, INT32_T port);


/*
����˵����
	��Ӧ�������󣬽������Ӳ�����һ���µ�socket ������������������
����˵����
	���������
		sockfd�������׽���������
		ipaddr������IP �汾�����Ľṹ��ָ��
	���������
		ipaddr���������ӶԷ�IP ��ַ�Ľṹ��ָ��
		port���������ӶԷ��Ķ˿ں�
		�� �أ�
		>��0:ִ�гɹ��򷵻طǸ���������
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_accept(INT32_T sockfd,IPANEL_IPADDR *ipaddr, INT32_T *port);


/*
����˵����
	���׽�����ָ�����׽�����������
����˵����
	���������
		sockfd���׽���������
		ipaddr������������IP ��ַ��IP �汾�����Ľṹ��ָ��
		port�������ӵĶ˿ں�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_connect(INT32_T sockfd,IPANEL_IPADDR *ipaddr, INT32_T port);


/*
����˵����
	�������ݵ��׽��֡�
����˵����
	���������
		sockfd���׽���������
		buf�����������ݵĻ�����
		len�������������ݵĳ���
		flags������ѡ�һ����Ϊ0������ֵ����
	�����������
	�� �أ�
		>0:ִ�гɹ�����ʵ�ʷ��͵����ݳ��ȡ�
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_send(INT32_T sockfd,CHAR_T *buf, INT32_T lenght, INT32_T flags);


/*
����˵����
	�������ݵ�ָ���׽��֡�
����˵����
	���������
		sockfd���׽���������
		buf�����������ݵĻ�����
		len�������������ݵĳ���
		flags������ѡ�һ����Ϊ0������ֵ����
		ipaddr������Ŀ��IP ��ַ��IP �汾�����Ľṹ��ָ��
		port��Ŀ�Ķ˿ں�
	�����������
	�� �أ�
		>=0:ִ�гɹ�����ʵ�ʷ��͵����ݳ��ȡ�
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_sendto(INT32_T sockfd,CHAR_T *buf, INT32_T len, INT32_T flags, IPANEL_IPADDR*ipaddr, INT32_T port);


/*
����˵����
	��һ���׽��ֽ������ݡ�
����˵����
	���������
		sockfd���׽���������
		buf�����ڽ������ݵĻ�����
		len���������ĳ���
		flags������ѡ�һ����Ϊ0������ֵ����
	�����������
	�� �أ�
		>=0: ִ�гɹ�����ʵ�ʽ��յ����ݳ��ȡ�
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_recv(INT32_T sockfd,CHAR_T *buf, INT32_T len, INT32_T flags);


/*
����˵����
	��һ��ָ�����׽��ֽ������ݡ�
����˵����
	���������
		sockfd���׽���������
		buf�����ڽ������ݵĻ�����
		len���������ĳ���
		flags������ѡ�һ����Ϊ0������ֵ����
		ipaddr������IP �汾�����Ľṹ��ָ��
	���������
		ipaddr�����淢�ͷ�IP ��ַ�Ľṹ��ָ��
		port�����淢�ͷ��Ķ˿ں�
	�� �أ�
		>=0: ִ�гɹ�����ʵ�ʽ��յ����ݳ��ȡ�
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_recvfrom(INT32_T sockfd, CHAR_T *buf, INT32_T len, INT32_T flags, IPANEL_IPADDR *ipaddr, INT32_T *port);


/*
����˵����
	��ȡ���׽����йصı���Э���ַ����������ַ��
����˵����
	���������
		sockfd���׽���������
		ipaddr������IP �汾�����Ľṹ��ָ��
	���������
		ipaddr����������Э��IP ��ַ�Ľṹ��ָ��
		port����������Э��Ķ˿ں�
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_getsockname ( INT32_T sockfd, IPANEL_IPADDR *ipaddr, INT32_T *port );


/*
����˵����
	ȷ��һ�������׽��ֵ�״̬���ж��׽������Ƿ������ݣ������ܷ���һ���׽���
	д�����ݡ�
����˵����
	���������
		nfds�� ��Ҫ�����ļ���������������ֵӦ�ñ�readset��writeset��
		exceptset ����������󣬶�����ʵ�ʵ��ļ�����������
		readset�� �������ɶ��Ե�һ���ļ�������
		writeset����������д�Ե�һ���ļ�������
		exceptset�������������״̬���ļ���������������������״̬
		timeout�� ����0 ��ʾ�ȴ����ٺ��룻ΪIPANEL_NO_WAIT(0)ʱ��ʾ���ȴ���
		�����أ�ΪIPANEL_WAIT_FOREVER(-1)��ʾ���õȴ���
	�����������
	�� �أ�
		��Ӧ�����Ķ�Ӧ�����ļ�����������������readset��writeset��exceptset
		�������ݾ���ǡ��λ�ñ��޸ģ�
		IPANEL_OK:Ҫ��ѯ���ļ��������׼����;
		IPANEL_ERR:ʧ�ܣ��ȴ���ʱ�����
  */
extern INT32_T ipanel_porting_socket_select(INT32_T nfds,IPANEL_FD_SET_S *readset, IPANEL_FD_SET_S *writeset,IPANEL_FD_SET_S *exceptset, INT32_T timeout);


/*
����˵����
	��ȡ�׽��ֵ����ԡ��ɻ�ȡ�׽��ֵ����Լ��±���get ��ΪY �������
����˵����
	���������
		sockfd�� �׽���������
		level�� ѡ���Ĳ��
		optname����Ҫ��ȡ��������
		level 				optname 						get 	set 	��������

							IPANEL_SO_BROADCAST 		Y 	Y 	INT32_T
		IPANEL_SOL_SOCKET	IPANEL_SO_KEEPALIVE 			Y 	Y 	INT32_T
							IPANEL_SO_REUSEADDR 		Y 	Y 	INT32_T

		IPANEL_IPPROTO_IP	IPANEL_IP_ADD_MEMBERSHIP 	N 	Y 	PANEL_IP_MREQ
							IPANEL_IP_DROP_MEMBERSHIP 	N 	Y 	PANEL_IP_MREQ
	���������
		optval��ָ�򱣴�����ֵ�Ļ�����
		optlen��ָ�򱣴�����ֵ�ĳ���
	�� �أ�
		IPANEL_OK:�ɹ�
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_getsockopt(INT32_T sockfd, INT32_T level, INT32_T optname, VOID *optval,INT32_T *optlen);


/*
����˵����
	�����׽��ֵ����ԡ������õ��׽��ֵ����Լ��±���set ��ΪY �������
����˵����
	���������
		sockfd���׽���������
		level�� ѡ���Ĳ��
		optname����Ҫ���õ�������
		optval��ָ�򱣴�����ֵ�Ļ�����
		optlen�� optval �������ĳ���
	�����������
	�� �أ�
		IPANEL_OK:�ɹ���
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_setsockopt(INT32_T sockfd,INT32_T level,INT32_T optname,VOID *optval,INT32_T optlen);


/*
����˵����
	�ı��׽�������Ϊ��������ע���˺���ֻ�ṩ���׽�����������Ϊ�����������ṩ
	���׽�����������Ϊ�����Ĺ��ܡ�
����˵����
	���������
		sockfd���׽���������
		cmd���������ͣ�ΪIPANEL_FIONBIO
		arg�����������1 ��ʾ������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_ioctl ( INT32_T sockfd, INT32_T cmd, INT32_T arg );


/*
����˵����
	��ֹ��һ���׽����Ͻ������ݵĽ����뷢�͡�������Ҫʹ�ô˺�����
����˵����
	���������
		sockfd���׽�����������
		what��ΪIPANEL_DIS_RECEIVE ��ʾ��ֹ���գ�ΪIPANEL_DIS_SEND ��ʾ
		��ֹ���ͣ�ΪIPANEL_DIS_ALL ��ʾͬʱ��ֹ���ͺͽ��ա�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_shutdown(INT32_T sockfd, INT32_T what);


/*
����˵����
	�ر�ָ�����׽���
����˵����
	���������
		sockfd���׽���������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
extern INT32_T ipanel_porting_socket_close(INT32_T sockfd);


/*
����˵����
	��ȡ׼����iPanel MiddleWare ͬʱ����socket ����������ע�⣺����Ŀ����
	��8 �����ϣ�16 �����£���[8~16]��ȱʡΪ8����iPanel MiddleWare ֻ�������ҳ��
	����8 ��socket ���㹻�ˣ���iPanel MiddleWare �������ҳ��ʱ��ͬʱ����Ҫʹ��
	VOIP��VOD �㲥��Ӧ�þ���Ҫ���socket ������ٶȣ�������ܳ���socket ���Ѿ�ʹ
	�ã���Ҫ�ر�һЩsocket ��������ĳ��Ӧ�õ������
����˵����
	�����������
	�����������
�� �أ�
	>0:����iPanel MiddleWare �ɴ��������socket ��Ŀ��
	IPANEL_ERR:ʧ�ܡ�
  */
extern INT32_T ipanel_porting_socket_get_max_num ( VOID);
#endif

/*--eof------------------------------------------------------------------------------------------------------------*/

