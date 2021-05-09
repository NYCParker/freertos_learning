#include "list.h"

//const 通常用在函数形参中，如果形参是一个指针，为了防止在函数内部修改指针指向的数据，就可以用 const 来限制。
/*
int const nValue; // nValue是const
char const * pContent; // *pContent是const, pContent可变
char* const pContent; // pContent是const, *pContent可变
char const* const pContent; // pContent 和 *pContent都是const
*/
//节点初始化
void vListInitialiseItem(ListItem_t* const pxItem)
{
	pxItem->pvContainer = NULL;  //初始化该节点所在的链表为空，表示节点还没有插入任何链表
}

//根初始化
void vListInitialise(List_t * const pxList)
{
	pxList->pxIndex = (ListItem_t* )&(pxList->xListEnd);  /* 将链表索引指针指向最后一个节点（第0个节点）, 索引是指向ListItem_t类型的指针，这里有一个指针（地址）的强制转换*/
	                                                      //这里是地址的强制转换，比如一个int在0x20，它是指针p的值 
	                                                      //如果这时候另一个char在0x21，它是指针ppp的值，那么如果想让ppp指针指向0x20
	                                                      //则 ppp = （char*）p ，或者 ppp = （char*）&0x20
	                                                                                                                                 
	pxList->xListEnd.xItemValue = portMAX_DELAY;          /* 将链表最后一个节点的辅助排序的值设置为最大，确保该节点就是链表的最后节点 */
	pxList->xListEnd.pxNext = (ListItem_t* )&(pxList->xListEnd); 
	pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );  /* 将最后一个节点的 pxNext 和 pxPrevious 指针均指向节点自身，表示链表为空 */
	
	pxList->uxNumberOfItems = (UBaseType_t)0U;
}

//链表尾部插入节点
void vListInsertEnd(List_t * const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t* const pxIndex = pxList->pxIndex;     //引用下来当前列表的Index进行操作，避免修改原来链表的index位置（可能）  //END索引
	
	pxNewListItem->pxNext = pxIndex;
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;
	pxIndex->pxPrevious->pxNext = pxNewListItem;
	pxIndex->pxPrevious = pxNewListItem;             //完成了插入，pxList的pxIndex依然指向xListEnd
	
	pxNewListItem->pvContainer = (void*)pxList;      /* 记住该节点所在的链表 */
	(pxList->uxNumberOfItems)++;                     /* 链表节点计数器++ */  
}

//节点按照升序排列
void vListInsert(List_t * const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t *pxIterator;
	
	const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;    /* 获取节点的排序辅助值 */
	if(xValueOfInsertion == portMAX_DELAY)
	{
		pxIterator = pxList->xListEnd.pxPrevious;      //如果节点值最大和END同级，先把pxIterator指向End的前一个节点
	}
	else
	{
		for ( pxIterator = ( ListItem_t * ) &( pxList->xListEnd );pxIterator->pxNext->xItemValue <= xValueOfInsertion;pxIterator = pxIterator->pxNext )
		{
			//首先把pxIterator指向End（第0个），第二句判断也就是从第一个开始(入口节点）比较xItemValue的值，最终找到一个比不过的next，pxIterator就停在这个next的previous处
			//pxIterator就是节点应该插入的位置（插在pxIterator之后）
		}
	}
	pxNewListItem->pxNext = pxIterator->pxNext; 
	pxNewListItem->pxNext->pxPrevious = pxNewListItem; 
	pxNewListItem->pxPrevious = pxIterator; 
	pxIterator->pxNext = pxNewListItem; 
	
	pxNewListItem->pvContainer = (void*)pxList;
	( pxList->uxNumberOfItems )++;
}

//将节点从链表删除
UBaseType_t uxLixtRemove(ListItem_t* const pxItemToRemove)
{
	List_t* const pxList = (List_t*)pxItemToRemove->pvContainer;   /* 获取节点所在的链表 */
	
	pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
	pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
	
	if ( pxList->pxIndex == pxItemToRemove )
	{
		pxList->pxIndex = pxItemToRemove->pxPrevious;    //如果索引指针恰好在要删除的节点上，则指向原节点的前一个
	}
	
	pxItemToRemove->pvContainer = NULL;      /* 初始化该节点所在的链表为空，表示节点还没有插入任何链表 */
	
	( pxList->uxNumberOfItems )--;
	return pxList->uxNumberOfItems;          /* 返回链表中剩余节点的个数 */
}
