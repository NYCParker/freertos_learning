#ifndef LIST_H
#define LIST_H


#include "portmacro.h"

//����ڵ����ݽṹ
struct xLIST_ITEM
{
	TickType_t xItemValue;         /* ����ֵ�����ڰ����ڵ���˳������ */
	struct xLIST_ITEM* pxNext;     /* ָ��������һ���ڵ� */
	struct xLIST_ITEM* pxPrevious; /* ָ������ǰһ���ڵ� */
	void* pvOwner;                 /* ָ��ӵ�иýڵ���ں˶���ͨ���� TCB */
	void* pvContainer;             /* ָ��ýڵ����ڵ����� */
};

typedef struct xLIST_ITEM ListItem_t; /* �ڵ����������ض��� */

/*voidָ����һ���ر��ָ��, ������Ͳ����жϳ�ָ�����ĳ���, 
�κ�ָ�붼���Ը�ֵ��voidָ�� ��
void *vp 
type *p
vp=p;  ��ñ����ĵ�ַ ����Ҫת�� (ò�Ƽ�һ����void*�������Ҳ��
���������ֵ���������͵�ָ�����Ҫת�� ��
type *ppp = (type*)vp
ͬʱvoidָ�벻�ܸ�����
*vp//����
vpֻ֪����ַ����֪������ռ�����ڴ��޷���ȷ����
�����Ҫ�������㣬��Ҫ��������ת��
(type*)vp++;
//vp==vp+sizeof(type)

void* ����˼������һ��ָ�룬������ں����н����ָ��ת��Ϊ�κ�һ�����ݽṹ��ָ�롣
����˵void*�������൱ǿ��ġ�
���磬�������ģ���ӿ�ʱ����ʱ�޷��ж��������͡�
�Ϳ���ͨ��void*���룬�ں����н�ָ��ת������Ҫ�����;Ϳ����ˡ���һ��ʮ�ֹؼ�������Ӧ��������Ĵ����еĴ���
*/

//����ڵ����ݽṹ
struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;         /* ����ֵ�����ڰ����ڵ���˳������ */
	struct xLIST_ITEM* pxNext;     /* ָ��������һ���ڵ� */
	struct xLIST_ITEM* pxPrevious; /* ָ������ǰһ���ڵ� */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t; /* �ڵ����������ض��� */

//��������ݽṹ
struct xLIST
{
	UBaseType_t uxNumberOfItems;  /* ����ڵ������ */
	ListItem_t* pxIndex;          /* ����ڵ�����ָ��,���pxIndexӦ�ÿ���ָ���κ�һ���ڵ�ĵ�ַ*/
	MiniListItem_t xListEnd;      /* ��һ������ڵ���Ϊ�������һ���ڵ� */
};
typedef struct xLIST List_t;


//�ڵ���κ�С����
/* ��ʼ���ڵ��ӵ���� The owner of a list item is the object (usually a TCB) that contains the list item.*/ 
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner )  ( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )
	
/* ��ȡ�ڵ�ӵ���� */
#define listGET_LIST_ITEM_OWNER( pxListItem ) ( ( pxListItem )->pvOwner )

/* ��ʼ���ڵ�������ֵ */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )  ( ( pxListItem )->xItemValue = ( xValue ) )

/* ��ȡ�ڵ�������ֵ */
#define listGET_LIST_ITEM_VALUE( pxListItem ) ( ( pxListItem )->xItemValue )

/* ��ȡ������ڵ�Ľڵ��������ֵ */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList ) ( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/* ��ȡ�������ڽڵ� */
#define listGET_HEAD_ENTRY( pxList )  ( ( ( pxList )->xListEnd ).pxNext )

/* ��ȡ�ڵ����һ���ڵ� */
#define listGET_NEXT( pxListItem ) ( ( pxListItem )->pxNext )

/* ��ȡ��������һ���ڵ� */
#define listGET_END_MARKER( pxList ) ( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )
	
/* �ж������Ƿ�Ϊ�� */
#define listLIST_IS_EMPTY( pxList )  ( ( ( pxList )->uxNumberOfItems == ( UBaseType_t ) 0 ) ? pdTRUE : pdFALSE )

/* ��ȡ����Ľڵ��� */
#define listCURRENT_LIST_LENGTH( pxList )( ( pxList )->uxNumberOfItems )

/* ��ȡ�����һ���ڵ�� OWNER���� TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )                                           \
    {                                                                                          \
        List_t * const pxConstList = ( pxList );                                               \
        /* Increment the index to the next item and return the item, ensuring */               \
        /* we don't return the marker used at the end of the list.ָ���һ���ڵ㣨��ڽڵ㣩  */                         \
        ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                           \
        if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) ) \
        {                                                                                      \
            ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                       \
        }                                                                                      \
        ( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;                                         \
    }
	
#define listGET_OWNER_OF_HEAD_ENTRY( pxList )            ( ( &( ( pxList )->xListEnd ) )->pxNext->pvOwner )
	
void vListInitialiseItem(ListItem_t* const pxItem);
void vListInitialise(List_t * const pxList);
void vListInsertEnd(List_t * const pxList, ListItem_t* const pxNewListItem);
void vListInsert(List_t * const pxList, ListItem_t* const pxNewListItem);
UBaseType_t uxLixtRemove(ListItem_t* const pxItemToRemove);
	
#endif /* ifndef LIST_H */
