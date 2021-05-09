#ifndef LIST_H
#define LIST_H


#include "portmacro.h"

//链表节点数据结构
struct xLIST_ITEM
{
	TickType_t xItemValue;         /* 辅助值，用于帮助节点做顺序排列 */
	struct xLIST_ITEM* pxNext;     /* 指向链表下一个节点 */
	struct xLIST_ITEM* pxPrevious; /* 指向链表前一个节点 */
	void* pvOwner;                 /* 指向拥有该节点的内核对象，通常是 TCB */
	void* pvContainer;             /* 指向该节点所在的链表 */
};

typedef struct xLIST_ITEM ListItem_t; /* 节点数据类型重定义 */

/*void指针是一种特别的指针, 这个类型不能判断出指向对象的长度, 
任何指针都可以赋值给void指针 ①
void *vp 
type *p
vp=p;  获得变量的地址 不需要转换 (貌似加一个（void*）的情况也有
但是如果赋值给其他类型的指针就需要转换 ②
type *ppp = (type*)vp
同时void指针不能复引用
*vp//错误
vp只知道地址，不知道变量占多少内存无法正确引用
如果想要进行运算，还要进行类型转换
(type*)vp++;
//vp==vp+sizeof(type)

void* 的意思是这是一个指针，你可以在函数中将这个指针转换为任何一个数据结构的指针。
可以说void*功能是相当强大的。
例如，在你设计模块间接口时，暂时无法判定传参类型。
就可以通过void*传入，在函数中将指针转化你需要的类型就可以了。这一点十分关键，包括应用在任务的创建中的传参
*/

//迷你节点数据结构
struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;         /* 辅助值，用于帮助节点做顺序排列 */
	struct xLIST_ITEM* pxNext;     /* 指向链表下一个节点 */
	struct xLIST_ITEM* pxPrevious; /* 指向链表前一个节点 */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t; /* 节点数据类型重定义 */

//链表根数据结构
struct xLIST
{
	UBaseType_t uxNumberOfItems;  /* 链表节点计数器 */
	ListItem_t* pxIndex;          /* 链表节点索引指针,这个pxIndex应该可以指向任和一个节点的地址*/
	MiniListItem_t xListEnd;      /* 用一个迷你节点作为链表最后一个节点 */
};
typedef struct xLIST List_t;


//节点带参宏小函数
/* 初始化节点的拥有者 The owner of a list item is the object (usually a TCB) that contains the list item.*/ 
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner )  ( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )
	
/* 获取节点拥有者 */
#define listGET_LIST_ITEM_OWNER( pxListItem ) ( ( pxListItem )->pvOwner )

/* 初始化节点排序辅助值 */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )  ( ( pxListItem )->xItemValue = ( xValue ) )

/* 获取节点排序辅助值 */
#define listGET_LIST_ITEM_VALUE( pxListItem ) ( ( pxListItem )->xItemValue )

/* 获取链表根节点的节点计数器的值 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList ) ( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/* 获取链表的入口节点 */
#define listGET_HEAD_ENTRY( pxList )  ( ( ( pxList )->xListEnd ).pxNext )

/* 获取节点的下一个节点 */
#define listGET_NEXT( pxListItem ) ( ( pxListItem )->pxNext )

/* 获取链表的最后一个节点 */
#define listGET_END_MARKER( pxList ) ( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )
	
/* 判断链表是否为空 */
#define listLIST_IS_EMPTY( pxList )  ( ( ( pxList )->uxNumberOfItems == ( UBaseType_t ) 0 ) ? pdTRUE : pdFALSE )

/* 获取链表的节点数 */
#define listCURRENT_LIST_LENGTH( pxList )( ( pxList )->uxNumberOfItems )

/* 获取链表第一个节点的 OWNER，即 TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )                                           \
    {                                                                                          \
        List_t * const pxConstList = ( pxList );                                               \
        /* Increment the index to the next item and return the item, ensuring */               \
        /* we don't return the marker used at the end of the list.指向第一个节点（入口节点）  */                         \
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
