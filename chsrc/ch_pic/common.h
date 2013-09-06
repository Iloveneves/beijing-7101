/*kernel ͨ�ú������塢�궨�塢��������*/

#ifndef  _COMMON_H_
#define  _COMMON_H_
/*��������Լ��: �� �������繫˾��C���Դ��뿪�������淶 V2.0��
 ע�Ͳ�Ҫ��'//' ,��Ϊ��Щ����������ʶ��
***********************************************************************/


#include "os_plat.h"

/*typedef S32 CH_BOOL;

#define CH_TRUE 1
#define CH_FALSE 0*/


/***********************************************************************
** MACROS:      CH_BIT
**              CH_BITMASK
** DESCRIPTION:
** Bit masking macros.  XXX n must be <= 31 to be portable
***********************************************************************/
#define CH_BIT(n)       ((U32)1 << (n))
#define CH_BITMASK(n)   (CH_BIT(n) - 1)

#define CH_BEGIN_MACRO  do {
#define CH_END_MACRO    } while (0)

#define CH_ARRAY_LENGTH(array_)   (sizeof(array_)/sizeof(array_[0]))

/* ���ݽṹ��Ա�ĵ�ַ���ؽṹ�ĵ�ַ */
#define STRUCT_ENTRY(pMemberPtr,STRUCT_TYPE,MemberVar)\
 	((STRUCT_TYPE *)((unsigned long)(pMemberPtr)-(unsigned long)(&((STRUCT_TYPE *)0)->MemberVar)))

/*ȡ�����Сֵ*/
#define	CH_MIN(x,y)	((x)<(y)?(x):(y))
#define	CH_MAX(x,y)	((x)>(y)?(x):(y))
#define CH_ABS(x) (((x)<0)?(-(x)):(x))
#define CH_SQU(x)	((x) * (x))
#define CH_ZERO(x)	memset(&(x), 0, sizeof(x))
/*�ж�ֱ��(a1,a2) �Ƿ���ֱ��(b1,b2)�����غ�*/
#define CH_MIDLINE(a1,a2,b1, b2) ((((a1) >= (b1)) && ((a1) <= (b2))) || (((a2) >= (b1)) && ((a2) <= (b2)))		\
						|| (((b1) >= (a1)) && ((b1) <= (a2))) || (((b2) >= (a1)) && ((b2) <= (a2))))

#define z_INVALIDED	((int*)0xff)		/*��Ч��ֵ*/
#define z_FALSE	((int*)0)		/*��*/
#define z_TRUE	((int*)1)		/*��*/
#define z_UserCancle    (-5)	/*�û�ȡ������������ */
#if 0
/* �޷���64λ���� */
typedef struct  
{
#ifdef IS_LITTLE_ENDIAN
	U32 ul_Low32;
	U32 ul_High32;
#else
	U32 ul_High32;
	U32 ul_Low32;
#endif
}CH_UNSIGNED_64,*PCH_UNSIGNED_64;

/* ����64λ���� */
typedef CH_UNSIGNED_64 CH_SIGNED_64,*PCH_SIGNED_64;

/*64λ������ز���*/

/*�Ƿ�����*/
#define CH64_IS_ZERO(a)        (((a).ul_High32 == 0) && ((a).ul_Low32 == 0))

/*�Ƿ����*/
#define CH64_EQ(a, b)        (((a).ul_High32 == (b).ul_High32) && ((a).ul_Low32 == (b).ul_Low32))

/*����*/
#define CH64_NE(a, b)        (((a).ul_High32 != (b).ul_High32) || ((a).ul_Low32 != (b).ul_Low32))

/* >= 0 */
#define CH64_GE_ZERO(a)        (((a).ul_High32 >> 31) == 0)

/*���űȽ�a,b��ֵop �ǲ�����: ������: > , >= , < , <= ,==,!=*/
#define CH64_CMP(a, op, b)    (((a).ul_High32 == (b).ul_High32) ? ((a).ul_Low32 op (b).ul_Low32) : \
                 ((S32)(a).ul_High32 op (S32)(b).ul_High32))

/*�޷��űȽ�a,b��ֵop �ǲ�����: ������: > , >= , < , <= ,==,!=*/
#define CH64_UCMP(a, op, b)    (((a).ul_High32 == (b).ul_High32) ? ((a).ul_Low32 op (b).ul_Low32) : \
                 ((a).ul_High32 op (b).ul_High32))

/*a,b�����㣬�����r ��*/
#define CH64_AND(r, a, b)        ((r).ul_Low32 = (a).ul_Low32 & (b).ul_Low32, \
                 (r).ul_High32 = (a).ul_High32 & (b).ul_High32)

/*a,b�����㣬�����r ��*/
#define CH64_OR(r, a, b)        ((r).ul_Low32 = (a).ul_Low32 | (b).ul_Low32, \
                 (r).ul_High32 = (a).ul_High32 | (b).ul_High32)

/*a,b������㣬�����r ��*/
#define CH64_XOR(r, a, b)        ((r).ul_Low32 = (a).ul_Low32 ^ (b).ul_Low32, \
                 (r).ul_High32 = (a).ul_High32 ^ (b).ul_High32)

/*r,a�����㣬�����r ��*/
#define CH64_OR2(r, a)        ((r).ul_Low32 = (r).ul_Low32 | (a).ul_Low32, \
                 (r).ul_High32 = (r).ul_High32 | (a).ul_High32)

/*a ������, �����r ��*/
#define CH64_NOT(r, a)        ((r).ul_Low32 = ~(a).ul_Low32, \
                 (r).ul_High32 = ~(a).ul_High32)

/*a��,�����r ��*/
#define CH64_NEG(r, a)        ((r).ul_Low32 = -(S32)(a).ul_Low32, \
                 (r).ul_High32 = -(S32)(a).ul_High32 - ((r).ul_Low32 != 0))
                 
/*a,b�ӷ�����, ����� r ��*/
#define CH64_ADD(r, a, b) \
{ \
	CH_UNSIGNED_64 _a, _b; \
	_a = a;\
	_b = b; \
	(r).ul_Low32 = _a.ul_Low32 + _b.ul_Low32; \
	(r).ul_High32 = _a.ul_High32 + _b.ul_High32 + ((r).ul_Low32 < _b.ul_Low32); \
}

/* a ��b ������� r ��*/
#define CH64_SUB(r, a, b) \
{ \
	CH_UNSIGNED_64 _a, _b; \
	_a.ul_High32 = (a).ul_High32;\
	_a.ul_Low32 = (a).ul_Low32;\
	_b.ul_High32 = (b).ul_High32; \
	_b.ul_Low32 = (b).ul_Low32; \
	(r).ul_Low32 = _a.ul_Low32 - _b.ul_Low32; \
	(r).ul_High32 = _a.ul_High32 - _b.ul_High32 - (_a.ul_Low32 < _b.ul_Low32); \
}

/* a ��b ������� r ��*/
#define CH64_MUL(r, a, b) \
{ \
	CH_UNSIGNED_64 _a, _b; \
	_a.ul_High32 = (a).ul_High32;\
	_a.ul_Low32 = (a).ul_Low32;\
	_b.ul_High32 = (b).ul_High32; \
	_b.ul_Low32 = (b).ul_Low32; \
	CH64_MUL32(r, _a.ul_Low32, _b.ul_Low32); \
	(r).ul_High32 += _a.ul_High32 * _b.ul_Low32 + _a.ul_Low32 * _b.ul_High32; \
}

/*ȡ32λ�ĵ�16λ*/
#define CH_LO16(a)        ((a) & CH_BITMASK(16))
/*ȡ32λ�ĸ�16λ*/
#define CH_HI16(a)        ((a) >> 16)

/*a,b ����32 λ�������,�����64λ����r ��*/
#define CH64_MUL32(r, a, b) \
{ \
	U32 _a1, _a0, _b1, _b0, _y0, _y1, _y2, _y3; \
	_a1 = CH_HI16(a), _a0 = CH_LO16(a); \
	_b1 = CH_HI16(b), _b0 = CH_LO16(b); \
	_y0 = _a0 * _b0; \
	_y1 = _a0 * _b1; \
	_y2 = _a1 * _b0; \
	_y3 = _a1 * _b1; \
	_y1 += CH_HI16(_y0);                         /* can't carry */ \
	_y1 += _y2;                                /* might carry */ \
	if (_y1 < _y2)    \
		_y3 += (U32)(CH_BIT(16));  /* propagate */ \
	(r).ul_Low32 = (CH_LO16(_y1) << 16) + CH_LO16(_y0); \
	(r).ul_High32 = _y3 + CH_HI16(_y1); \
}

/*  a ���� b , qp ����, rp ������,
  *  a,b �����޷�������*/
void CH_udivmod(PCH_UNSIGNED_64 rpu64_qp,
				     PCH_UNSIGNED_64 rpu64_rp,
				     PCH_UNSIGNED_64 rpu64_a,
				     PCH_UNSIGNED_64 rpu64_b);

/*  a ���� b , qp ����, rp ������  
  *  a,b �����޷�������*/
#define CH64_UDIVMOD(qp, rp, a, b)    CH_udivmod(((qp)?&(qp),0),((rp)?&(rp),0),&a,&b)

/*  a ���� b , r ���� ��: r=a / b
  *  a,b ���Ƿ��Ż��޷�������*/
#define CH64_DIV(r, a, b) \
{ \
	CH_UNSIGNED_64 _a, _b; \
	U32 _negative;\
	_negative = ((S32)(a).ul_High32) < 0; \
	if (_negative)\
	{ \
		CH64_NEG(_a, a); \
	}\
	else\
	{ \
		_a.ul_High32 = (a).ul_High32;\
		_a.ul_Low32 = (a).ul_Low32;\
	} \
	if (((S32)(b).ul_High32) < 0) \
	{ \
		_negative ^= 1; \
		CH64_NEG(_b, b); \
	}\
	else\
	{ \
		_b.ul_High32 = (b).ul_High32; \
		_b.ul_Low32 = (b).ul_Low32; \
	} \
	CH64_UDIVMOD(r,0, _a, _b); \
	if (_negative) \
		CH64_NEG(r, r); \
}

/*  a ���� b , r ������(ģ) ��: r=a%b
  *  a,b ���Ƿ��Ż��޷�������*/
#define CH64_MOD(r, a, b) \
{ \
	CH_UNSIGNED_64 _a, _b; \
	U32 _negative;\
	_negative = ((S32)(a).ul_High32) < 0; \
	if (_negative)\
	{ \
		CH64_NEG(_a, a); \
	}\
	else\
	{ \
		_a.ul_High32 = (a).ul_High32;\
		_a.ul_Low32 = (a).ul_Low32;\
	} \
	if (((S32)(b).ul_High32) < 0)\
	{ \
		CH64_NEG(_b, b); \
	}\
	else\
	{ \
		_b.ul_High32 = (b).ul_High32; \
		_b.ul_Low32 = (b).ul_Low32; \
	} \
	CH64_UDIVMOD(0,r, _a, _b); \
	if (_negative) \
		CH64_NEG(r, r); \
}

/*a ����b[0...64]λ,��: r = a << b	
  * a,r ����64λ����*/
#define CH64_SHL(r, a, b)\
{ \
	if (b)\
	{ \
		CH_UNSIGNED_64 _a; \
		_a.ul_High32 = (a).ul_High32;\
		_a.ul_Low32 = (a).ul_Low32;\
		if ((b) < 32)\
		{ \
			(r).ul_Low32 = _a.ul_Low32 << ((b) & 31); \
			(r).ul_High32 = (_a.ul_High32 << ((b) & 31)) | (_a.ul_Low32 >> (32 - (b))); \
		}\
		else\
		{ \
			(r).ul_Low32 = 0; \
			(r).ul_High32 = _a.ul_Low32 << ((b) & 31); \
		} \
	} \
	else\
	{ \
		(r).ul_High32 = (a).ul_High32;\
		(r).ul_Low32 = (a).ul_Low32;\
	} \
}

/*a ����b[0...64]λ,��: r = a << b	
  * a ����32 λ����,r ��64 λ����*/
#define CH64_ISHL(r, a, b) \
{ \
	if (b) \
	{ \
		CH_UNSIGNED_64 _a; \
		_a.ul_Low32 = (a); \
		_a.ul_High32 = 0; \
		if ((b) < 32) \
		{ \
			(r).ul_Low32 = (a) << ((b) & 31); \
			(r).ul_High32 = ((a) >> (32 - (b))); \
		}\
		else\
		{ \
			(r).ul_Low32 = 0; \
			(r).ul_High32 = (a) << ((b) & 31); \
		} \
	}\
	else\
	{ \
		(r).ul_Low32 = (a); \
		(r).ul_High32 = 0; \
	} \
}

/*a ����b[0...64]λ,��: r = a >> b	
  * a ����64 λ����,r ��64 λ����*/
#define CH64_SHR(r, a, b)\
{ \
	if (b)\
	{ \
		CH_UNSIGNED_64 _a; \
		_a.ul_High32 = (a).ul_High32;\
		_a.ul_Low32 = (a).ul_Low32;\
		if ((b) < 32) \
		{ \
			(r).ul_Low32 = (_a.ul_High32 << (32 - (b))) | (_a.ul_Low32 >> ((b) & 31)); \
			(r).ul_High32 = ((S32)_a.ul_High32) >> ((b) & 31); \
		}\
		else\
		{ \
			(r).ul_Low32 = ((S32)_a.ul_High32) >> ((b) & 31); \
			(r).ul_High32 = ((S32)_a.ul_High32) >> 31; \
		} \
	}\
	else\
	{ \
		(r).ul_High32 = (a).ul_High32;\
		(r).ul_Low32 = (a).ul_Low32;\
	} \
}

/*a ����b[0...64]λ,��: r = a >> b	
  * a ����32 λ����,r ��64 λ����*/
#define CH64_USHR(r, a, b) \
{ \
	if (b) \
	{ \
		CH_UNSIGNED_64 _a; \
		_a.ul_High32 = (a).ul_High32;\
		_a.ul_Low32 = (a).ul_Low32;\
		if ((b) < 32) \
		{ \
			(r).ul_Low32 = (_a.ul_High32 << (32 - (b))) | (_a.ul_Low32 >> ((b) & 31)); \
			(r).ul_High32 = _a.ul_High32 >> ((b) & 31); \
		}\
		else\
		{ \
			(r).ul_Low32 = _a.ul_High32 >> ((b) & 31); \
			(r).ul_High32 = 0; \
		} \
	}\
	else\
	{ \
		(r).ul_High32 = (a).ul_High32;\
		(r).ul_Low32 = (a).ul_Low32;\
	} \
}

/*64 λת����32 λ*/
#define CH64_L2I(i, l)        ((i) = (l).ul_Low32)
#define CH64_L2UI(ui, l)        ((ui) = (l).ul_Low32)

/*����32 λת����64 λ*/
#define CH64_I2L(l, i)        {S32 _i = ((S32)(i)) >> 31; (l).ul_Low32 = (i); (l).ul_High32 = _i; }

/*�޷���32 λת����64 λ*/
#define CH64_UI2L(l, ui)      ((l).ul_Low32 = (ui), (l).ul_High32 = 0)


#endif

typedef struct _time_
{
	U16 ui_Year;
	U8  uc_Month;
	U8  uc_Date;
	U8  uc_Hour;
	U8  uc_Minute;
	U8  uc_Second;
	U16 ui_MiSecond;
}CH_TIME,*PCH_TIME;

/* ����ṹ */
typedef struct _ListStr_
{
    struct _ListStr_ *p_Next;
    struct _ListStr_ *p_Prev;
}CH_LISTPTR,*PCH_LISTPTR;

/* ����Ԫ�� "_e" �������У�������"_l"֮ǰ.*/
#define CH_LSIT_INSERT_BEFORE(_e,_l)	 \
{		 \
	(_e)->p_Next = (_l);	 \
	(_e)->p_Prev = (_l)->p_Prev; \
	(_l)->p_Prev->p_Next = (_e); \
	(_l)->p_Prev = (_e);	 \
}

/* ����Ԫ�� "_e" �������У�������"_l"֮��.*/
#define CH_LIST_INSERT_AFTER(_e,_l)	 \
{		 \
	(_e)->p_Next = (_l)->p_Next; \
	(_e)->p_Prev = (_l);	 \
	(_l)->p_Next->p_Prev = (_e); \
	(_l)->p_Next = (_e);	 \
}

/** Remove the element "_e" from it's circular list.*/
#define CH_LIST_REMOVE_LINK(_e)	       \
{		       \
	(_e)->p_Prev->p_Next = (_e)->p_Next; \
	(_e)->p_Next->p_Prev = (_e)->p_Prev; \
}

/* Remove the element "_e" from it's circular list. Also initializes the
 * linkage. */
#define CH_LIST_REMOVE_AND_INIT_LINK(_e)    \
{		       \
	(_e)->p_Prev->p_Next = (_e)->p_Next; \
	(_e)->p_Next->p_Prev = (_e)->p_Prev; \
	(_e)->p_Next = (_e);	       \
	(_e)->p_Prev = (_e);	       \
}

/* Return non-zero if the given circular list "_l" is empty, zero if the
 * circular list is not empty */
#define CH_LIST_CLIST_IS_EMPTY(_l) ((CH_BOOL)((_l)->p_Next == (_l)))

/** Initialize a circular list */
#define CH_LIST_INIT_CLIST(_l)  \
{	   \
	(_l)->p_Next = (_l); \
	(_l)->p_Prev = (_l); \
}


/* ��̬ͨ������ */
/* ��һ��λ���� 0 �����һ��λ���� iMaxElementNumber-1 */
typedef struct 
{
	S32 iCurrentIndex;    /* ��ǰԪ�ص���������ʱδ�� */

	S32 iElementCount;/* ������Ԫ�ظ��� */
	S32 iMaxElementNumber;/* ���������Ԫ�ظ��� */
	void** ppData; /* ����ָ������ */
}CH_GENERAL_ARRAY,*PCH_GENERAL_ARRAY;

/* �Ƚ�����Ԫ�ش�С�ĺ������Ͷ���
 * ������
 *		pElement1 ��Ҫ�Ƚϵĵ�1��Ԫ�� 
 *		pElement2 ��Ҫ�Ƚϵĵ�2��Ԫ�� 
 *		pOtherArg ���Ƚ���Ҫ�������������
 * ����ֵ:
 * 	< 0     :     ��ʾ pElement1 С��  pElement2
 *	0       :     ��ʾ pElement1 ����  pElement2
 *	> 0     :     ��ʾ pElement1 ����  pElement2
 */
typedef S32 (* ARRAY_COMPARATOR_FUNC) (void* pElement1,void* pElement2,void* pOtherArg);

S32 CH_StrArrayComp(void* p1,void *p2,void*pOtherArg);


/* ����һ���������Ϊ iMaxElementNum ������,
 * ������������һ�� CH_GENERAL_ARRAY �ṹ�ռ�,
 * Ȼ����� CH_InitArray ��ʼ���ṹ��Ա*/
PCH_GENERAL_ARRAY CH_CreateArray(S32 iMaxElementNum);

/* ��ʼ���ṹ��Ա������ iMaxElementNum ��Ԫ�ؿռ�
 * �� iMaxElementNum <= 0,�� ��iMaxElementNum = 32��
 * ע�⣺
 *      ����ʱ rp_Array �����Ѿ������ڴ棬�������������CH_GENERAL_ARRAY �ṹ�ڴ�
 *      ����ֻ�Ƿ���������Ԫ�ص��ڴ� */
CH_BOOL CH_InitArray(PCH_GENERAL_ARRAY rp_Array,S32 iMaxElementNum);

/* ȡ����ĵ� iIndex ��Ԫ��,(��һ��λ���� 0 �����һ��λ��
 * �� iMaxElementNumber-1)Ԫ����Ȼ�������С�
 * ����ֵ��һ��voidָ�룬���Ը�����Ҫת�����������͵�ָ���ֱ��
 * ת����32λ����ʹ�� */
void* CH_GetArrayElement(PCH_GENERAL_ARRAY pArray,S32 iIndex);

/* ��������ĵ� iIndex ��Ԫ��,(��һ��λ���� 0 �����һ��λ��
 * �� iMaxElementNumber-1)��
 * �ɹ�����TRUEM��ʧ�ܷ���FALSE */
CH_BOOL CH_SetArrayElement(PCH_GENERAL_ARRAY pArray,S32 iIndex,void* pElement);

/* ��������Ԫ�ظ��� */
S32 CH_GetArrayCount(PCH_GENERAL_ARRAY pArray);

/* �����������Ԫ�ظ��� */
S32 CH_MaxArrayCount(PCH_GENERAL_ARRAY pArray);

/* �������в����������Ԫ��(pElement)��ƥ���Ԫ�ص�λ��(��һ��λ����0,���һ��λ���� iMaxElementNumber-1)
 * ����:
 *		pArray:Ҫ�������
 *		pElement��Ҫƥ�������Ԫ��
 *		fn_ComFunc:�Ƚ�����Ԫ�ش�С�ĺ���
 * ���أ�
 *		-1: û���ҵ�ƥ�������Ԫ��
 *	   >=0: ƥ��Ԫ���������е�λ��*/
S32 CH_FindElementInArray(PCH_GENERAL_ARRAY pArray,void* pElement,ARRAY_COMPARATOR_FUNC fn_ComFunc);

/* ����Ԫ�� pElement �������е�λ��(��һ��λ���� 0 �����һ��λ��
 * �� iMaxElementNumber-1),���������з��� -1 */
S32 CH_IndexOfArray(PCH_GENERAL_ARRAY pArray,void* pElement);

/* ��iIndexλ�ò���һ��Ԫ��,���iIndex ��ֵ> pArray->iElementCount ���� <��
 * ��������ĩβ׷��.
 * ����ʵ�ʲ����λ�ã����� -1 ��ʾ����*/
S32 CH_InsertArrayElement(PCH_GENERAL_ARRAY pArray,void* pElement,S32 iIndex);

/* ׷��һ��Ԫ�ص�����ĩβ */
CH_BOOL CH_AppendArrayElement(PCH_GENERAL_ARRAY pArray,void* pElement);

/* ��pSourArray�е�Ԫ��׷�ӵ�pDestArray����ĩβ */
CH_BOOL CH_AppendArray(PCH_GENERAL_ARRAY pDestArray,PCH_GENERAL_ARRAY pSourArray);

/* ��������ɾ��Ԫ��:pElement(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�������ȥ��) */
CH_BOOL CH_RemoveArrayElement(PCH_GENERAL_ARRAY pArray,void* pElement);

/* ɾ�������е� iIndex ��Ԫ��(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�������ȥ��) 
 * iIndex < 0 �� >Ԫ�ظ���,��ʾ��ɾ��β��Ԫ��
 * ����ֵ���ɹ������ش�������ɾ����Ԫ�أ�ʧ�ܣ����� NULL */
void* CH_RemoveArrayElementIndex(PCH_GENERAL_ARRAY pArray,S32 iIndex);

/* �ø�����Ԫ���滻ԭ���� iIndex λ�õ�Ԫ�� 
 *  iIndex < 0 �� >Ԫ�ظ���,��ʾֱ��������β��׷��Ԫ��
 * ����ֵ���ɹ������ش������б��滻��Ԫ��,�����ֱ��׷�ӵ�Ԫ��
 * ����׷�ӵ�Ԫ�أ���:rp_Element��ʧ�ܣ����� NULL */
void* CH_ReplaceArrayElementIndex(PCH_GENERAL_ARRAY rp_Array,void* rp_Element,S32 rl_Index);

/* �ڴ��ͷź��� */
typedef void (* ARRAY_DELE_FUNC) (void* pElement, void *pOtherArg);

/* ������������е�Ԫ��,�����ͷ��� CH_InitArray �����Ԫ�ؿռ�,�����ʼ��Ϊ�����顣
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�������ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��*/
void CH_ResetArray(PCH_GENERAL_ARRAY pArray,CH_BOOL bIsFreeElement,
				   ARRAY_DELE_FUNC fnFreeFunc,void *pOtherArg);

/* ������������е�Ԫ��,�ͷ��� CH_InitArray �����Ԫ�ؿռ�,�����ʼ��Ϊ�����顣
 * �´�ʹ��ʱӦ�����µ��� CH_InitArray �����ʹ��
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�������ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��*/
void CH_ClearArray(PCH_GENERAL_ARRAY pArray,CH_BOOL bIsFreeElement,
				   ARRAY_DELE_FUNC fnFreeFunc,void *pOtherArg);

/* ���ڴ���ɾ������,�����ȵ��� CH_ClearArray �ͷ�CH_InitArray �����Ԫ�ؿռ䣬
 * Ȼ����� CH_FreeMem �ͷ����鱾������ݽṹ(CH_GENERAL_ARRAY)�ռ�,�ͷź�,
 * pArray ָ������Ч�� */
void CH_DestroyArray(PCH_GENERAL_ARRAY pArray);

/* ��������������Ԫ�ص������� */
CH_BOOL CH_ResetArrayMaxCount(PCH_GENERAL_ARRAY pArray,S32 iNewMaxElementNumber);

/* �������е�Ԫ������,����ȽϺ�����fnArrayCompFunc ,
 * fnArrayCompFunc ���أ�
 	< 0     :     ��ʾ pElement1 С��  pElement2(pElement1��pElement2��ǰ��)
	0       :     ��ʾ pElement1 ����  pElement2
	> 0     :     ��ʾ pElement1 ����  pElement2(pElement1��pElement2�ĺ���)
 * ��� fnArrayCompFunc == NULL ��ô��pElement1 �� pElement2 ת����S32 ����ֱ�ӱȽ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnArrayCompFunc��pOtherArg����,һ���������ΪNULL��*/
void CH_SortArray(PCH_GENERAL_ARRAY pArray,ARRAY_COMPARATOR_FUNC fnArrayCompFunc,void *pOtherArg);

/* �������һ��Ԫ��,����ȽϺ�����fnArrayCompFunc������ͬCH_SortArray��
 * ���������δ����ģ���ú���ִ�к󣬲���λ����û������� 
 * ���øú���ʱ��Ҫ��֤��������ͬһ���ȽϺ��������
 * ����ʵ�ʲ����λ��,���� -1 ��ʾʧ��. */
S32 CH_SortInsertElementToArray(PCH_GENERAL_ARRAY pArray,void* pElement,
								ARRAY_COMPARATOR_FUNC fnArrayCompFunc,void *pOtherArg);

/* ö�ٲ�������
  * ����:  CH_TRUE: ���Լ���ö����һ��Ԫ��
  *                CH_FALSE:ֹͣö����һ��Ԫ�� */
  #define CH_PRE_CALLBACK
typedef CH_BOOL (*CH_PRE_CALLBACK ARRAY_ENUM_FUNC) (void* pElement, void *pOtherArg);
/* ö���������±�Ϊ iStartIndex �� iEndIndex(����iStartIndex �� iEndIndex) ������Ԫ��,
 * �� iStartIndex < iEndIndex,��iStartIndex ˳������
 * �� iStartIndex > iEndIndex,��iStartIndex ˳�����
 * Ԫ�ؽ��еĲ����� fnEnumFunc ���塡 
 *      ����:pOtherArg��ֱ�Ӵ�������fnEnumFunc��pOtherArg����,һ���������ΪNULL��
 *  ����ʵ��ö�ٵ�Ԫ�ظ���,ʧ�ܷ���-1*/
S32 CH_EnumerateArrayElement(PCH_GENERAL_ARRAY pArray,S32 iStartIndex,S32 iEndIndex,
								 ARRAY_ENUM_FUNC fnEnumFunc,void *pOtherArg);


/* ���� */
/* ����ͷλ���Ƕ�ӦԪ������ 0 ������β��ӦԪ������λ���� iMaxElementNumber-1 
typedef struct 
{
	S32 iElementCount;  ������Ԫ�ظ��� 
	S32 iMaxElementNumber; ���������Ԫ�ظ��� 
	S32 iCurrentIndex;�����е�ǰԪ�ص����� 
	void** ppData;  ����Ԫ��ָ������ 
}CH_DEQUEUE,*PCH_DEQUEUE; */

#if 0
#define CH_DEQUEUE CH_GENERAL_ARRAY
#define PCH_DEQUEUE PCH_GENERAL_ARRAY
#else
typedef CH_GENERAL_ARRAY CH_DEQUEUE,*PCH_DEQUEUE;
#endif


/* ����һ���������Ϊ iMaxElementNum �Ķ���,
 * ������������һ�� CH_DEQUEUE �ṹ�ռ�,
 * Ȼ����� CH_InitDequeue ��ʼ���ṹ��Ա
 * PCH_DEQUEUE CH_CreateDequeue(S32 iMaxElementNum); */
#define CH_CreateDequeue CH_CreateArray

/* ��ʼ���ṹ��Ա������ iMaxElementNum ��Ԫ�ؿռ�
 * �� iMaxElementNum <= 0,�� ��iMaxElementNum = 32��
 * ע�⣺
 *      ����ʱ pDequeue �����Ѿ������ڴ棬�������������CH_DEQUEUE �ṹ�ڴ�
 *      ����ֻ�Ƿ���������Ԫ�ص��ڴ�,����ԭ�ͣ�
 * CH_BOOL CH_InitDequeue(PCH_DEQUEUE pDequeue,S32 iMaxElementNum); */
#define CH_InitDequeue CH_InitArray

/* �ڴ��ͷź��� 
 * typedef void (* DEQUEUE_DELE_FUNC) (void* pElement, void *pOtherArg); */
#define DEQUEUE_DELE_FUNC ARRAY_DELE_FUNC
/* ������������е�Ԫ��,�ͷ��� CH_InitDequeue �����Ԫ�ؿռ�,���г�ʼ��Ϊ�ն���,
 * �´�ʹ��ʱӦ�����µ��� CH_InitDequeue �����ʹ��
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�ǴӶ�����ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��
   void CH_ClearDequeue(PCH_DEQUEUE pDequeue,CH_BOOL bIsFreeElement,
				   DEQUEUE_DELE_FUNC fnFreeFunc,void *pOtherArg); */
#define CH_ClearDequeue CH_ClearArray

/* ������������е�Ԫ��,�����ͷ��� CH_InitDequeue �����Ԫ�ؿռ�,���г�ʼ��Ϊ�ն���,
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�ǴӶ�����ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��
   void CH_ResetDequeue(PCH_DEQUEUE pDequeue,CH_BOOL bIsFreeElement,
				   DEQUEUE_DELE_FUNC fnFreeFunc,void *pOtherArg); */
#define CH_ResetDequeue CH_ResetArray

/* ���ڴ���ɾ������
   void CH_DestroyDequeue(PCH_DEQUEUE pDequeue); */
#define CH_DestroyDequeue CH_DestroyArray

/* ����ջԪ�ظ��� */
#define  CH_GetDequeueCount CH_GetArrayCount

/* ö�ٲ������� */
#define DEQUEUE_ENUM_FUNC ARRAY_ENUM_FUNC
/* ö�ٶ���������Ϊ iStartIndex �� iEndIndex(����iStartIndex �� iEndIndex) �Ķ���Ԫ��,
 * Ԫ�ؽ��еĲ����� fnEnumFunc ���塡 
 *      ����:pOtherArg��ֱ�Ӵ�������fnEnumFunc��pOtherArg����,һ���������ΪNULL��
   CH_BOOL CH_EnumerateDequeueElement(PCH_DEQUEUE pDequeue,S32 iStartIndex,S32 iEndIndex,
								 DEQUEUE_ENUM_FUNC fnEnumFunc,void *pOtherArg); */
#define CH_EnumerateDequeueElement CH_EnumerateArrayElement

/* �ڶ�����ɾ��Ԫ��:pElement(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�ǴӶ�����ȥ��) 
   CH_BOOL CH_RemoveDequeueElement(PCH_DEQUEUE pDequeue,void* pElement); */
#define CH_RemoveDequeueElement CH_RemoveArrayElement

/* ɾ�������е� iIndex ��Ԫ��(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�ǴӶ�����ȥ��) 
 * iIndex < 0 �� >Ԫ�ظ���,��ʾ��ɾ��β��Ԫ��
 * ����ֵ���ɹ�,���شӶ�����ɾ����Ԫ�أ�ʧ��,���� NULL 
   void* CH_RemoveDequeueElementIndex(PCH_DEQUEUE pDequeue,S32 iIndex); */
#define CH_RemoveDequeueElementIndex CH_RemoveArrayElementIndex

/* ȡ���еĵ� iIndex ��Ԫ��,(��һ��λ���� 0 �����һ��λ��
 * �� iMaxElementNumber-1)Ԫ����Ȼ�ڶ����С�
 * ����ֵ��һ��voidָ�룬���Ը�����Ҫת�����������͵�ָ���ֱ��
 * ת����32λ����ʹ�� 
   void* CH_GetDequeueElement(PCH_DEQUEUE pDequeue,S32 iIndex); */
#define  CH_GetDequeueElement CH_GetArrayElement 

/* �ڶ��е�iIndexλ�ò���һ��Ԫ��,���iIndex ��ֵ> pArray->iElementCount ���� <��
 * ���ڶ���ĩβ׷��.
 * ����ʵ�ʲ����λ�ã����� -1 ��ʾ���� */
S32 CH_InsertDequeueElement(PCH_DEQUEUE rp_Dequeue,void* rp_Element,S32 rl_Index);

/* ׷��һ��Ԫ�ص�����ĩβ */
CH_BOOL CH_AppendDequeueElement(PCH_DEQUEUE pDequeue,void* pElement); 

/* ��pSourDequeue�е�Ԫ��׷�ӵ�pDestDequeueĩβ */
CH_BOOL CH_AppendDequeue(PCH_DEQUEUE pDestDequeue,PCH_DEQUEUE pSourDequeue); 

/* �������ö�����Ԫ�ص������� 
	CH_BOOL CH_ResetDequeueMaxCount(PCH_DEQUEUE pDequeue,S32 iNewMaxElementNumber); */
#define CH_ResetDequeueMaxCount CH_ResetArrayMaxCount



/* ջ(Statck)���� */
/* ջ��λ���Ƕ�ӦԪ������ 0 ��ջ����ӦԪ������λ���� iMaxElementNumber-1 
typedef struct 
{
	S32 iElementCount;  ������Ԫ�ظ��� 
	S32 iMaxElementNumber;���������Ԫ�ظ��� 
	S32 iCurrentIndex; �����е�ǰԪ�ص����� 
	void** ppData;  ����Ԫ��ָ������ 
}CH_STACK,*PCH_STACK; */

#if 0
#define CH_STACK CH_GENERAL_ARRAY
#define PCH_STACK PCH_GENERAL_ARRAY
#else
typedef CH_GENERAL_ARRAY CH_STACK,*PCH_STACK;
#endif

/* ����һ���������Ϊ iMaxElementNum ��ջ,
 * ������������һ�� CH_STACK �ṹ�ռ�,
 * Ȼ����� CH_InitStack ��ʼ���ṹ��Ա
 * PCH_STACK CH_CreateStack(S32 iMaxElementNum); */
#define CH_CreateStack CH_CreateArray

/* ��ʼ���ṹ��Ա������ iMaxElementNum ��Ԫ�ؿռ�
 * �� iMaxElementNum <= 0,�� ��iMaxElementNum = 32��
 * ע�⣺
 *      ����ʱ pStack �����Ѿ������ڴ棬������������� CH_STACK �ṹ�ڴ�
 *      ����ֻ�Ƿ���������Ԫ�ص��ڴ�,����ԭ�ͣ�
 * CH_BOOL CH_InitStack(PCH_STACK pStack,S32 iMaxElementNum); */
#define CH_InitStack CH_InitArray

/* �ڴ��ͷź��� 
 * typedef void (*STACK_DELE_FUNC) (void* pElement,void *pOtherArg); */
#define STACK_DELE_FUNC ARRAY_DELE_FUNC
/* ���ջ�����е�Ԫ��,�ͷ��� CH_InitStack �����Ԫ�ؿռ�,ջ��ʼ��Ϊ��ջ,
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�ջ��ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��
   void CH_ClearStack(PCH_STACK pStack,CH_BOOL bIsFreeElement,
				   STACK_DELE_FUNC fnFreeFunc,void *pOtherArg); */
#define CH_ClearStack CH_ClearArray

/* ���ջ�����е�Ԫ��,���ǲ��ͷ��� CH_InitStack �����Ԫ�ؿռ�,ջ��ʼ��Ϊ��ջ,
 * �´�ʹ��ʱӦ�����µ��� CH_InitStack �����ʹ��
 * ����bIsFreeElement==CH_FALSE ʱ,Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�ջ��ȥ��
 * ����bIsFreeElement==CH_TRUE ʱ,�� fnFreeFunc ��Ԫ�ش��ڴ������,��
 * fnFreeFunc Ϊ NULL ����ֱ���� CH_FreeMem() ��Ԫ���ͷ� 
 *      ����:pOtherArg��ֱ�Ӵ�������fnFreeFunc��pOtherArg����,һ���������ΪNULL��
   void CH_ResetStack(PCH_STACK pStack,CH_BOOL bIsFreeElement,
				   STACK_DELE_FUNC fnFreeFunc,void *pOtherArg); */
#define CH_ResetStack CH_ResetArray

/* ���ڴ���ɾ��ջ 
  void CH_DestroyStack(PCH_STACK pStack); */
#define CH_DestroyStack CH_DestroyArray

/* ����ջԪ�ظ��� */
#define  CH_GetStackCount CH_GetArrayCount

/* ��Ԫ��ѹ��ջ,���ƶ�ջָ�� */
CH_BOOL CH_PushStack(PCH_STACK pStack,void* pStackElement); 

/* ȡջ��Ԫ�أ�����ջָ������һλ */
void* CH_PopStack(PCH_STACK pStack);

/* �������ö�����Ԫ�ص������� 
 CH_BOOL CH_ResetStackMaxCount(PCH_STACK pStack,S32 iNewMaxCount); */
#define CH_ResetStackMaxCount CH_ResetArrayMaxCount

/* ��ջ��ָ��λ����ȡһ��Ԫ��,ջ��λ���� pStack->iElementCount-1,ջ��λ���� 0
 * ����ֵ��һ��voidָ�룬���Ը�����Ҫת�����������͵�ָ���ֱ��
 * ת����32λ����ʹ�� 
   void* CH_GetStackElement(PCH_STACK pStack,S32 iIndex); */
#define  CH_GetStackElement CH_GetArrayElement 

/* ��ջ��ָ��λ�ò���һ��Ԫ��,ջ��λ���� pStack->iElementCount-1,ջ��λ���� 0,
 * ���iIndex ��ֵ> pArray->iElementCount ���� <������� */
CH_BOOL CH_InsertStackElement(PCH_STACK pStack,void* pStackElement,S32 iIndex);

/* ��pSourStack�е�Ԫ��׷�ӵ�pDestStackĩβ */
CH_BOOL CH_AppendStack(PCH_STACK pDestStack,PCH_STACK pSourStack);

/* ��ջ��ɾ��Ԫ��:pElement(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�ջ��ȥ��) 
   CH_BOOL CH_RemoveStackElement(PCH_STACK pStack,void* pStackElement)*/
#define  CH_RemoveStackElement CH_RemoveArrayElement

/* ɾ��ջ�е� iIndex ��Ԫ��(Ԫ�ز�û�д��ڴ����ͷţ�ֻ�Ǵ�ջ��ȥ��) 
 * iIndex < 0 �� >Ԫ�ظ���,��ʾջ����Ԫ��
 * ����ֵ���ɹ������ش�ջ��ɾ����Ԫ�أ�ʧ�ܣ����� NULL 
   void* CH_RemoveStackElementIndex(PCH_STACK pStack,S32 iIndex); */
#define CH_RemoveStackElementIndex CH_RemoveArrayElementIndex

/* ȡջ��Ԫ�أ����ǲ����ƶ�ջָ��,Ԫ������ջ�� */
#define CH_GetStackTopElement(pStack) CH_GetStackElement(pStack,pStack->iElementCount-1)

/* ȡջ��Ԫ�أ����ǲ����ƶ�ջָ��,Ԫ������ջ�� */
#define CH_GetStackBottomElement(pStack) CH_GetStackElement(pStack,0)








/*�ַ�������*/

/*���ַ���pSrc �п���iLen ���ַ��� pDest��
  *����:iLen :�������ֽ���,��iLen <=0 ���pSrc ȫ��������
  * ����:�ɹ�- pDest
                  ʧ��- NULL*/
S8 *CH_StrCopyLen(S8 *rp_Dest,S8 *rp_Src,S32 rl_Len);

/* ���ַ���pSrc ������ pDest��
  * ����:�ɹ�- pDest
                  ʧ��- NULL
  * ����ԭ��:
		S8 *CH_StrCopy(S8 *pDest,S8 *pSrc);*/
#define CH_StrCopy(pDest,pSrc) CH_StrCopyLen(pDest,pSrc,-1)

/* �����ַ���pSrc��pDest�����pDest��NULL,ֱ�ӷ����ڴ�
 * ���pDest��ΪNULL���ͷ��ڴ棬�ٷ����ڴ� */
#define CH_ACopyStr(pDest,pSrc) CH_ACopyMem(pDest,pSrc,CH_StrLen(pSrc)+1)


/*�����ַ�c���ַ���pInStr�еĸ���,iLen��ʾ���ַ����в��ҵĳ��ȣ�iLen==-1��ʾȫ������*/
S32 CH_CountChar(S8 *pInStr,S32 iLen,S8 c);

/*��pInStr�й����ַ�c*/
void CH_SkipChar(S8 *pInStr,S8 c);

/*�����ַ����ַ�����*/
S8 *CH_InsertCharToStr(S8 *pInStr,S8 c,S32 iIndex);

/* ���ַ����е�iOldChar �滻Ϊ iNewChar,�滻����Ϊ iLen*/
CH_BOOL CH_ReplaceCharacter(S8* pInStr,S32 iLen,S8 iOldChar,S8 iNewChar);

/*���ַ�chת���ɴ�д*/
S8 CH_ASCCharToUpper(S8 ch);

/*���ַ�chת����Сд*/
S8 CH_ASCCharToLower(S8 ch);

/*���ַ���szStrת���ɴ�д*/
S8* CH_ASCStrToUpper(S8* szStr);

/*���ַ���szStrת����Сд*/
S8* CH_ASCStrToLower(S8* szStr);

/*�ж��Ƿ�ʱASC�ַ��Ŀո�*/
CH_BOOL CH_IsAsciiSpace(S8 iChar);

/*�ж��Ƿ�ʱASC�ַ�����ĸ*/
/* CH_BOOL CH_IsAsciiAlpha(S8 iChar); */
#define CH_IsAsciiAlpha(iChar) ((CH_BOOL)(((iChar) >= 'a' && (iChar) <= 'z') ||((iChar) >= 'A' && (iChar) <= 'Z')))

/*�ж��Ƿ�ʱASC�ַ�������*/
/* CH_BOOL CH_IsAsciiDigit(S8 iChar); */
#define  CH_IsAsciiDigit(iChar) ((CH_BOOL)(((iChar) >= '0') && ((iChar) <= '9')))

/* ��szStrǰ��(���)�� cSet �ַ�ȥ��,�����ַ���ǰ�Ƶ�
  * ͷ��ַ��ʼ�ĵط�*/
S8* CH_LeftTrim(S8* szStr,S8 cSet);

/* ��szStr�д� iStartPos ��ʼ�� iLen ���ַ�ȥ����
 * iLen == -1����ʾ�� iStartPos ��ʼ�������ַ���*/
S8* CH_StrCut(S8* szStr,S32 iStartPos,S32 iLen);

/*��szStr����(�ұ�)�� cSet �ַ�ȥ��*/
S8* CH_RightTrim(S8* szStr,S8 cSet);

/* ��szStrǰ��(���)�ͺ���(�ұ�)�� cSet �ַ�ȥ��,
  * �����ַ���ǰ�Ƶ�ͷ��ַ��ʼ�ĵط�*/
S8* CH_AllTrim(S8* szStr,S8 cSet);

/* ���ַ�iChar׷�ӵ�szDest �ĺ���,����szDest,������Ҫ��֤szDest���ڴ�ռ��㹻 */
S8* CH_AppendChar(S8 *szDest,S8 iChar);

/* ��szSrc׷�ӵ�szDest �ĺ���,����szDest,ʹ��ʱӦ�ñ�֤szDest���㹻�Ŀռ�
 * ���������ַ��� */
S8* CH_AppendStr(S8 *szDest,S8* szSrc);

/* ���ַ�iChar׷�ӵ�szDest �ĺ���,����pDest,���pDest�ռ䲻��,�����Զ����·���pDest���ڴ� 
 * ������iDestBuffLen,��ʾpDest��󻺳�����С*/
S8* CH_AutoAppendChar(S8 *pDest,S32 iDestBuffLen,S8 iChar);

/* ��szSrc׷�ӵ�pDest �ĺ���,����pDest,���pDest�ռ䲻��,�����Զ����·���pDest���ڴ� 
 * ������iDestBuffLen,��ʾpDest��󻺳�����С*/
S8* CH_AutoAppendStr(S8 *pDest,S32 iDestBuffLen,S8* szSrc);

/* ��szStrǰ��(���)�� szSet �ַ���ȥ��,�����ַ���ǰ�Ƶ�
  * ͷ��ַ��ʼ�ĵط�*/
S8* CH_LeftStrTrim(S8* szStr,S8 *szSet);

/*��szStr����(�ұ�)�� szSet �ַ���ȥ��*/
S8* CH_RightStrTrim(S8* szStr,S8 *szSet);

/* ��szStrǰ��(���)�ͺ���(�ұ�)�� szSet �ַ���ȥ��,
  * �����ַ���ǰ�Ƶ�ͷ��ַ��ʼ�ĵط�*/
S8* CH_AllStrTrim(S8* szStr,S8 *szSet);

/*�ж��ַ����Ƿ�Ϊ��*/
#define CH_StrIsEmpty(pStr)  ((pStr) == NULL || CH_StrLen(pStr) == 0)

/*Compare characters in two buffers.
	Return Value    Relationship of First count Bytes of szSrc and szDest 
	< 0             szSrc less than szDest 
	0               szSrc identical to szDest 
	> 0             szSrc greater than szDest 
*/
S32 CH_StrCompare(S8* szSrc,S8 *szDest);

/* �Ƚ������ַ���ǰ rl_Len ���ַ�,
 * ����ͬ�� */
S32 CH_StrnCompare(S8* szSrc,S8 *szDest,S32 rl_Len);

/* ��Сд�������ַ����Ƚ� */
S32 CH_NotCaseSensitiveStrCompare(S8* szSrc,S8 *szDest);

/* �Ƚ������ַ���ǰ rl_Len ���ַ�,
 * ����ͬ�� */
S32 CH_NotCaseSensitiveStrnCompare(S8* szSrc,S8 *szDest,S32 rl_Len);

/*����c���ַ����е�λ��,��һλ����0,û�з���-1*/
S32 CH_FindCharPos(S8* pInDestBuff,S8 c);

/*���ַ����в����ַ��������ص�һ�����ַ��ĵ�ַ,û��NULL*/
S8* CH_FindChar(S8* pInDestBuff,S8 c);

/*���ұ߿�ʼ�����ַ� c ���ַ����е�λ��,��һλ����0,û�з���-1*/
S32 CH_RFindCharPos(S8* pInDestBuff,S8 c);

/*���ұ߿�ʼ���ַ����в����ַ��������ص�һ�����ַ��ĵ�ַ,û��NULL*/
S8* CH_RFindChar(S8* pInDestBuff,S8 c);

/**********************************************************
 * ���ַ��� string �в����ַ��� strCharSet 
 * ������
 * strCharSet:�����ַ���            bIsCaseSensitive:�Ƿ��Сд����
 * iFindLen:�� string �������ҵĳ���. ��iFindLen = -1,��ȫ������
 * �ɹ������� string ��,���ַ��� strCharSet��ͷ���ַ���ָ��
 * ʧ�ܻ�û���ҵ�����NULL
 ************************************************************/
S8 *CH_FindString(S8 *sString,S32 iFindLen,S8 *strCharSet,CH_BOOL bIsCaseSensitive);

/**********************************************************
 * ���ַ��� string �в����ַ��� strCharSet 
 * ������
 * strCharSet:�����ַ���            bIsCaseSensitive:�Ƿ��Сд����
 * iFindLen:�� string �������ҵĳ���. ��iFindLen = -1,��ȫ������
 * �ɹ��������ַ��� strCharSet ��λ��,��һλ����0
 * ʧ�ܻ�û���ҵ����� -1��
 ************************************************************/
S32 CH_GetSubStrPos(S8 *sString,S32 iFindLen,S8 *strCharSet,CH_BOOL bIsCaseSensitive);

/**********************************************************
 * ���ַ��� string �л�ȡ��iStartPos��ʼ������ΪiLen���ַ���
 * �ɹ�����szSubStrָ��,
 * ʧ�ܷ��� NULL��
 ************************************************************/
S8* CH_GetSubString(S8 *sString,S32 iStartPos,S32 iLen,S8 *szSubStr,S32 iMaxSubLen);

/*ת��һ���ַ���������,iRadix = 10 ��ʮ����ת��
 * iRadix = 16 ʱ16 ����ת��,0 �Զ����,���������*/
#define Radix_AUTODETECT 0
#define Radix_10                  10
#define Radix_16                  16
CH_BOOL  CH_StrToInteger(S32 *piOutInterger,S8 *szInString,S8 iRadix);

/* ��������:����ʵ�� FindCharInSet����*/
S8 GetFindInSetFilter(S8* rp_Set);

/* �������ܣ����ַ����в����ַ����е��ַ��������ص�һ���ҵ���λ��
 * ����:   
 *      rp_Data:������ַ�������'\0'Ϊ������־
 *      rl_DataLen:���ַ����в��ҵĳ��ȣ������� -1 �����������
 * ���� rp_Set:Ҫ���ҵ��ַ���,�Կ��ַ�����
 * ���أ�
 *      1.���ڡ�rp_Data�����ҵ��ַ���(rp_Set)���κ�һ���ַ�ʱ�����ҽ�����
 *      �����ַ��� rp_Data ��λ��(ע�⣺��һ��λ���� 0)
 *      2.����ڡ�rp_Data���в����ҵ��ַ���(rp_Set)���κ�һ���ַ�ʱ������-1
 *  */
S32 CH_FindCharInSet(S8 *rp_Data,U32 rl_DataLen,S8* rp_Set);


/*���ܣ��ֽ��ַ���Ϊһ���Ǵ���rsz_SourceΪҪ�ֽ���ַ�����rsz_DelimΪ�ָ����ַ�����
  
  ˵���������� rsz_Source �в��Ұ����� rsz_Delim �е��ַ�����NULL('\0')���滻��
		������ȥ��ͷ���ķֽ��,Ȼ����ҵ�һ���ֽ������NULL('\0')���滻��ֱ���ұ������ַ�����
		rsz_OutNewStr �ں����ɹ�����ʱָ����һ����Ǵ���
  ����ֵ: ����ȥ��ͷ���ֽ�����ַ�����ַ����û�б�Ǵ�ʱ�򷵻ؿ��ַ�NULL��*/
S8* CH_Strtok(S8* rsz_Source,S8* rsz_Delim,S8** rsz_OutNewStr);




#define CH_AUTOSTRING_BLOCKLEN 128
/* 
   ����һ�����ȿ��Զ�̬�仯���ַ����ṹ
               �������ֽṹ���Ա������Ƶ������ϵͳ�ڴ�����
   �ͷź������Ӷ������ڴ���Ƭ��
  */
typedef struct 
{
	S8 *p_String;	/* ʵ�ʵĴ����ַ�����ָ��*/
	U32 ul_Size;	/* p_String �ܴ������󳤶ȣ��ַ���p_String
				     ��ʵ�ʳ��� ���ɺ���CH_StrLen()�ó���
				     ��Ӧ��С��u_Size*/
	U32 ul_BlockLen;/* ÿ�����ӵ��ڴ泤�� */			    
}CH_AutoString_t,*PCH_AutoString_t;


/* ����һ���鳤��Ϊrul_BlockLen ��CH_AutoString_t �ַ���
  * ��rul_BlockLenС��CH_AUTOSTRING_BLOCKLEN, ��ul_BlockLen ȱʡΪ
  * CH_AUTOSTRING_BLOCKLEN 
  * ����Ϊp_String ���ݳ�Ա����ul_BlockLen �ֽڵĿռ�(����CH_InitAutoStringʵ��)*/
PCH_AutoString_t CH_CreateAutoString(U32 rul_BlockLen);

/* ��ʼ��rp_AutoString �����ݳ�Ա.
  * ���ú���ʱ,rp_AutoString �������Ѿ�������ڴ棬����������
  * Ϊrp_AutoString �����ڴ棬����Ϊp_String ���ݳ�Ա����ul_BlockLen 
  * �ֽڵĿռ�
  * ��rul_BlockLenС��CH_AUTOSTRING_BLOCKLEN, ��ul_BlockLen ȱʡΪ
  * CH_AUTOSTRING_BLOCKLEN 
  */
CH_BOOL CH_InitAutoString(PCH_AutoString_t rp_AutoString,U32 rul_BlockLen);

/*�����ַ����ַ�����*/
PCH_AutoString_t CH_InsertCharToAutoStr(PCH_AutoString_t rp_Dest,S8 rc_Char,S32 rl_Index);

/*����ͨ�ַ�rc_Char ׷�ӵ� rp_Dest��,�����Զ��������峤��*/
PCH_AutoString_t CH_AppendCharToAutostr(PCH_AutoString_t rp_Dest,S8  rc_Char);

/* ���ַ���rp_Src ׷��rl_CopyLen ���ַ��� rp_Dest��,�����Զ�����rp_Dest �Ļ��峤��
  * ��rl_CopyLen < 0 ����rl_CopyLen > CH_StrLen(rp_Src) ʱ,����rp_Src �е������ַ���*/
PCH_AutoString_t CH_AppendSubstrToAutostr(PCH_AutoString_t rp_Dest,S8 * rp_Src,S32 rl_CopyLen);

/* ����ͨ�ַ���rp_Src ׷�ӵ� rp_Dest��,�����Զ��������峤��
  * ����ԭ��:
 		PCH_AutoString_t CH_AppendStrToAutostr(PCH_AutoString_t rp_Dest,S8 * rp_Src); */
#define CH_AppendStrToAutostr(rp_Dest,rp_Src) CH_AppendSubstrToAutostr((rp_Dest),(rp_Src),-1)

/*�ж��ַ����Ƿ�Ϊ��*/
#define CH_AutostrIsEmpty(rp_Str)  ((rp_Str) == NULL || CH_StrIsEmpty((rp_Str)->p_String))

#define CH_ResetAutostr(rp_AutoString)\
{\
	if (rp_AutoString)\
	{\
		(*((rp_AutoString)->p_String)) = 0;\
	}\
}

#define CH_AutostrLen(rp_AutoString) CH_StrLen((rp_AutoString)->p_String)

/* ���rp_AutoString �����ݳ�Ա���ͷ�rp_AutoString->p_String���ڴ�ռ�,
  * ���Ǻ��������ͷ�rp_AutoString �ṹ��
  * ����ԭ��:
  * void CH_ClearAutoString(PCH_AutoString_t rp_AutoString)
  */
#define CH_ClearAutostr(rp_AutoString)\
{\
	if(rp_AutoString)\
	{\
		if((rp_AutoString)->p_String)\
			CH_FreeMem((rp_AutoString)->p_String);\
		(rp_AutoString)->p_String = NULL;\
		(rp_AutoString)->ul_Size = 0;\
	}\
}

/* ���rp_AutoString �����ݳ�Ա���ͷ�rp_AutoString->p_String���ڴ�ռ�,
  * �ͷ�rp_AutoString �ṹ�Ĵ洢�ռ�.��rp_AutoString ΪNULL.
  * ����ԭ��:
  * void CH_DeleteAutostr(PCH_AutoString_t rp_AutoString)
  */
#define CH_DeleteAutostr(rp_AutoString)\
{\
	if(rp_AutoString)\
	{\
		CH_ClearAutostr(rp_AutoString);\
		CH_FreeMem(rp_AutoString);\
		rp_AutoString = NULL;\
	}\
}









/*����ͨ�ú���*/

/* ��pSrc����iSrcLen�ֽڵ�pDest�����pDest��NULL,ֱ�ӷ����ڴ�
 * ���pDest��ΪNULL���ͷ��ڴ� */
#define CH_ACopyMem(pDest,pSrc,iSrcLen)\
{\
	if(pDest)	CH_FreeMem(pDest);\
	(pDest) = CH_AllocMem(iSrcLen);\
	if(pDest){\
		CH_MemCopy((pDest),(pSrc),(iSrcLen));\
	};\
}


/*���� size���ֽڵ��ڴ�,����ʼ��Ϊ iValue*/
void *CH_AllocMemAndSetValue(S32 size,S8 iValue);

/*���� size���ֽڵ��ڴ�,����ʼ��Ϊ 0*/
#define CH_AllocMemAndZero(size) CH_AllocMemAndSetValue((size),0)

/*ΪTypeName���ͱ��������ڴ沢��ʼ��Ϊ 0 ,���ط����ָ��*/
#define CH_AllocTypeAndZero(TypeName) (TypeName*)CH_AllocMemAndSetValue(sizeof(TypeName),0)

/* �ͷ�ָ�� pStr ���ڴ�ռ䣬����pStr��=��NULL 
 * ע�⣺ʹ�øú�����ʱ�����뱣֤ pStr �Ƕ�̬������ڴ档
 * ����ԭ�ͣ�
 *			void CH_DeletePointer(void * pStr); */
#define CH_DeletePointer(pStr)\
{\
	if(pStr)\
	{\
		CH_FreeMem(pStr);\
		pStr = NULL;\
	}\
}

CH_BOOL CH_U32ToTime(U32 ul_time,PCH_TIME rp_Time);
CH_BOOL CH_U64ToTime(PU64 pu64_time,PCH_TIME rp_Time);
CH_BOOL CH_FormatTime(PCH_TIME rp_Time,S8* rpc_buf,S32 l_buflen,S8* rpc_Foramt);


#endif
