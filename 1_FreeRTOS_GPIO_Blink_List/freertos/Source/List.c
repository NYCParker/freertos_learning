#include "list.h"

//const ͨ�����ں����β��У�����β���һ��ָ�룬Ϊ�˷�ֹ�ں����ڲ��޸�ָ��ָ������ݣ��Ϳ����� const �����ơ�
/*
int const nValue; // nValue��const
char const * pContent; // *pContent��const, pContent�ɱ�
char* const pContent; // pContent��const, *pContent�ɱ�
char const* const pContent; // pContent �� *pContent����const
*/
//�ڵ��ʼ��
void vListInitialiseItem(ListItem_t* const pxItem)
{
	pxItem->pvContainer = NULL;  //��ʼ���ýڵ����ڵ�����Ϊ�գ���ʾ�ڵ㻹û�в����κ�����
}

//����ʼ��
void vListInitialise(List_t * const pxList)
{
	pxList->pxIndex = (ListItem_t* )&(pxList->xListEnd);  /* ����������ָ��ָ�����һ���ڵ㣨��0���ڵ㣩, ������ָ��ListItem_t���͵�ָ�룬������һ��ָ�루��ַ����ǿ��ת��*/
	                                                      //�����ǵ�ַ��ǿ��ת��������һ��int��0x20������ָ��p��ֵ 
	                                                      //�����ʱ����һ��char��0x21������ָ��ppp��ֵ����ô�������pppָ��ָ��0x20
	                                                      //�� ppp = ��char*��p ������ ppp = ��char*��&0x20
	                                                                                                                                 
	pxList->xListEnd.xItemValue = portMAX_DELAY;          /* ���������һ���ڵ�ĸ��������ֵ����Ϊ���ȷ���ýڵ������������ڵ� */
	pxList->xListEnd.pxNext = (ListItem_t* )&(pxList->xListEnd); 
	pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );  /* �����һ���ڵ�� pxNext �� pxPrevious ָ���ָ��ڵ�������ʾ����Ϊ�� */
	
	pxList->uxNumberOfItems = (UBaseType_t)0U;
}

//����β������ڵ�
void vListInsertEnd(List_t * const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t* const pxIndex = pxList->pxIndex;     //����������ǰ�б��Index���в����������޸�ԭ�������indexλ�ã����ܣ�  //END����
	
	pxNewListItem->pxNext = pxIndex;
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;
	pxIndex->pxPrevious->pxNext = pxNewListItem;
	pxIndex->pxPrevious = pxNewListItem;             //����˲��룬pxList��pxIndex��Ȼָ��xListEnd
	
	pxNewListItem->pvContainer = (void*)pxList;      /* ��ס�ýڵ����ڵ����� */
	(pxList->uxNumberOfItems)++;                     /* ����ڵ������++ */  
}

//�ڵ㰴����������
void vListInsert(List_t * const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t *pxIterator;
	
	const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;    /* ��ȡ�ڵ��������ֵ */
	if(xValueOfInsertion == portMAX_DELAY)
	{
		pxIterator = pxList->xListEnd.pxPrevious;      //����ڵ�ֵ����ENDͬ�����Ȱ�pxIteratorָ��End��ǰһ���ڵ�
	}
	else
	{
		for ( pxIterator = ( ListItem_t * ) &( pxList->xListEnd );pxIterator->pxNext->xItemValue <= xValueOfInsertion;pxIterator = pxIterator->pxNext )
		{
			//���Ȱ�pxIteratorָ��End����0�������ڶ����ж�Ҳ���Ǵӵ�һ����ʼ(��ڽڵ㣩�Ƚ�xItemValue��ֵ�������ҵ�һ���Ȳ�����next��pxIterator��ͣ�����next��previous��
			//pxIterator���ǽڵ�Ӧ�ò����λ�ã�����pxIterator֮��
		}
	}
	pxNewListItem->pxNext = pxIterator->pxNext; 
	pxNewListItem->pxNext->pxPrevious = pxNewListItem; 
	pxNewListItem->pxPrevious = pxIterator; 
	pxIterator->pxNext = pxNewListItem; 
	
	pxNewListItem->pvContainer = (void*)pxList;
	( pxList->uxNumberOfItems )++;
}

//���ڵ������ɾ��
UBaseType_t uxLixtRemove(ListItem_t* const pxItemToRemove)
{
	List_t* const pxList = (List_t*)pxItemToRemove->pvContainer;   /* ��ȡ�ڵ����ڵ����� */
	
	pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
	pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
	
	if ( pxList->pxIndex == pxItemToRemove )
	{
		pxList->pxIndex = pxItemToRemove->pxPrevious;    //�������ָ��ǡ����Ҫɾ���Ľڵ��ϣ���ָ��ԭ�ڵ��ǰһ��
	}
	
	pxItemToRemove->pvContainer = NULL;      /* ��ʼ���ýڵ����ڵ�����Ϊ�գ���ʾ�ڵ㻹û�в����κ����� */
	
	( pxList->uxNumberOfItems )--;
	return pxList->uxNumberOfItems;          /* ����������ʣ��ڵ�ĸ��� */
}
