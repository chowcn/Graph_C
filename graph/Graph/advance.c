/*
 * Document    : Advance.c
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 					本C文件由多部分组成，节点操作、链表、队列、堆栈、树。
 					根据各部分的struct，分为:
 					1)其中节点操作是以双向链表节点为模版来操作的，封装出多
 					个通用的函数，基本以"vertex_node_"开头的函数都是此封装。
 					2)链表、队列、堆栈，此三种封装函数都是以节点操作的封装
 					为基础。
 					3)树为单独一种封装
 * Cautions    :
 					请查看对应H文件中的Cautions。
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date		| Auther	 | Description
   -----------------------------------------------------------------------
   2017-5-29  Josh.Chow    1、修改添加了对节点保存数据类型的支持NODE_ELEM_TYPE 
						   enElemType，用户可以选择保存不同种类的数据，目前本
						   字段仅仅在打印上有 稍许作用，list_view等view函数，
						   动态识别要打印的类型。
						   2、(仅仅有关插入操作的函数变动较大，其他函数因与用户
						   数据无关，所以基本无变化)
   -----------------------------------------------------------------------  				
 */
 #ifdef __cplusplus
 extern "C"{
 #endif


#include "common.h"
#include "graph.h"
#include "advance.h"
#include <stdlib.h>
#include <stdio.h>


/* Begin: LIST_NODE_S common operations */
STATIC VOID node_init(INOUT LIST_NODE_S *pNode)
{
	pNode->prev = pNode;
	pNode->next = pNode;
	pNode->pNodeElem = NULL;	/* LIST_NODE_S 头中不保存数据 */
}

STATIC LIST_NODE_S * node_new(IN VOID *pNodeElem, IN NODE_ELEM_TYPE enType)
{
	LIST_NODE_S *new = NULL;
	
	new = (LIST_NODE_S *)malloc(sizeof(LIST_NODE_S));
	if(NULL == new)
	{
		return NULL;
	}
	new->pNodeElem = pNodeElem;
	new->enElemType = enType;

	return new;
}


/*
 * Function     : 	node_insert()
 * Parameters   : 
 					LIST_NODE_S *former	## 	要插入的位置的前一个节点
 					LIST_NODE_S *latter	##	后一个节点
 					LIST_NODE_S *new		##  新节点
 * Return       :	
 					无返回值
 * Description  : 	
 					局部函数，仅仅本C文件使用，简化节点插入的代码。局部函
 					数不对入参进行检测，由其调用者保证。
 						这是编程规范之一，保证效率。
 					在former和latter之间插入new。
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28
 */
STATIC VOID node_insert(INOUT LIST_NODE_S *former, INOUT LIST_NODE_S *latter, INOUT LIST_NODE_S *new)
{
	former->next = new;
	latter->prev = new;
	new->prev = former;
	new->next = latter;
}


STATIC VOID node_separate(INOUT LIST_NODE_S *former, INOUT LIST_NODE_S *latter, INOUT LIST_NODE_S *split)
{
	former->next = latter;
	latter->prev = former;
	split->next = split;
	split->prev = split;
}


STATIC UINT node_split(INOUT LIST_NODE_S *split)
{
	if (NULL == split)
	{
		return ERROR_PARAMETER;
	}

	node_separate(split->prev, split->next, split);

	return ERROR_SUCCESS;
}


STATIC UINT node_is_empty(IN LIST_NODE_S *pNode)
{
	return (pNode->next == pNode) ? BOOL_TRUE:BOOL_FALSE;
}


/* End: LIST_NODE_S common operations */


/* Begin: List common operations */
UINT list_head_init(INOUT LIST_NODE_S *pList)
{
	if (NULL == pList)
	{
		return ERROR_PARAMETER;
	}

	node_init(pList);

	return ERROR_SUCCESS;
}

UINT list_node_add(INOUT LIST_NODE_S *pListHead, IN VOID *pNodeElem, IN NODE_ELEM_TYPE enElemType)
{
	LIST_NODE_S *pNewNode = NULL;
	
	if (NULL == pListHead || NULL == pNodeElem)
	{
		return ERROR_PARAMETER;
	}
	
	pNewNode = node_new(pNodeElem, enElemType);
	if(NULL == pNewNode)
	{
		return ERROR_RESOURCE;
	}
	node_insert(pListHead, pListHead->next, pNewNode);

	return ERROR_SUCCESS;
}


/*
 * Function     : 	list_node_add_tail()
 * Parameters   : 
 					LIST_NODE_S *pListHead		##  链表头
 					VOID *pVertex				##  
 * Return       :	
 					ERROR_PARAMETER					##  错误入参
 					ERROR_SUCCESS 					##  插入成功
 					ERROR_RESOURCE					##  内存申请失败
 * Description  : 	
 					根据顶点指针申请一个新的链表节点，并把该节点插入到链表
 					尾部tail。
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28
 */
UINT list_node_add_tail(INOUT LIST_NODE_S *pListHead, IN VOID *pNodeElem, IN NODE_ELEM_TYPE enElemType)
{
	LIST_NODE_S *pNewNode = NULL;
	
	if (NULL == pListHead || NULL == pNodeElem)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: 申请新链表节点 */
	pNewNode = node_new(pNodeElem, enElemType);
	if(NULL == pNewNode)
	{
		return ERROR_RESOURCE;
	}

	/*Step 2: 新节点插入到链表尾部 */
	node_insert(pListHead->prev, pListHead, pNewNode);
	
	return ERROR_SUCCESS;
}

BOOL_T list_is_empty(IN LIST_NODE_S *pListHead)
{
	if (NULL == pListHead)
	{
		return BOOL_TRUE;
	}

	return node_is_empty(pListHead);
}


STATIC UINT list_node_print(IN LIST_NODE_S *pListNode)
{
	if (VERTEX_TYPE == pListNode->enElemType)
	{
		vertex_node_print(pListNode->pNodeElem);
		printf("\r\n");
	}
	else if (EDGE_TYPE == pListNode->enElemType)
	{
		edge_node_print(pListNode->pNodeElem);
		printf("\r\n");
	}
	else
	{
		/*  None support print type */
		return ERROR_FAILED;
	}

	return ERROR_SUCCESS;
}


UINT list_view(LIST_NODE_S *pListHead)
{
	LIST_NODE_S *pNodeIter = NULL;
	VERTEX_S *pCurrVertex = NULL;

	if (NULL == pListHead)
	{
		return ERROR_PARAMETER;
	}

	for_each_node(pNodeIter, pListHead)
	{
		list_node_print(pNodeIter);
	}
	
	return ERROR_SUCCESS;
}


UINT list_destory(LIST_NODE_S *pListHead)
{
	LIST_NODE_S *former = NULL;
	LIST_NODE_S *latter = NULL;
	
	if (NULL == pListHead)
	{
		return ERROR_PARAMETER;
	}
	
	former = pListHead->next;
	while(former != pListHead)
	{
		latter = former->next;
		former->next = NULL;
		former->prev = NULL;
		free(former);
		former = latter;
	}
	pListHead->prev = pListHead;
	pListHead->next = pListHead;

	return ERROR_SUCCESS;
}

/* End: List  common operations */

/* Begin: Queue common operations */
UINT queue_init(LIST_NODE_S *pQueue)
{
	if (NULL == pQueue)
	{
		return ERROR_PARAMETER;
	}

	node_init(pQueue);

	return ERROR_SUCCESS;
}


UINT queue_enqueue(LIST_NODE_S *pQueue, VOID *pNodeElem, NODE_ELEM_TYPE enElemType)
{
	LIST_NODE_S *new = NULL;
	UINT uiRet = ERROR_SUCCESS;

	if (NULL == pQueue || NULL == pNodeElem)
	{
		return ERROR_PARAMETER;
	}

	new = node_new(pNodeElem, enElemType);
	if(NULL == new)
	{
		return ERROR_RESOURCE;
	}
	node_insert(pQueue, pQueue->next, new);

	return ERROR_SUCCESS;
}

BOOL_T queue_is_empty(LIST_NODE_S *pQueue)
{
	if (NULL == pQueue)
	{
		return BOOL_TRUE;
	}

	return node_is_empty(pQueue);
}

UINT queue_dequeue(LIST_NODE_S *pQueue, LIST_NODE_S **ppDeNode)
{
	UINT uiRet = ERROR_FAILED;
	
	if (NULL == pQueue || NULL == ppDeNode)
	{
		return ERROR_PARAMETER;
	}

	if (BOOL_TRUE == queue_is_empty(pQueue))
	{
		*ppDeNode == NULL;
		uiRet = ERROR_FAILED;
	}
	else
	{
		*ppDeNode = pQueue->prev;
		node_split(*ppDeNode);
		uiRet = ERROR_SUCCESS;
	}

	return uiRet;	
}

UINT queue_view(LIST_NODE_S *pQueue)
{
	if (NULL == pQueue)
	{
		return ERROR_PARAMETER;
	}
	
	return  list_view(pQueue);
}

UINT queue_destory(LIST_NODE_S *pQueue)
{
	if (NULL == pQueue)
	{
		return ERROR_PARAMETER;
	}
	
	return list_destory(pQueue);
}
/* End: Queue common operations */


/* Begin: Stack common operations */
UINT stack_init(LIST_NODE_S *pStack)
{
	if (NULL == pStack)
	{
		return ERROR_PARAMETER;
	}

	node_init(pStack);

	return ERROR_SUCCESS;
}


UINT stack_push(LIST_NODE_S *pStack, VOID *pNodeElem, NODE_ELEM_TYPE enElemType)
{
	LIST_NODE_S *new = NULL;

	if (NULL == pStack || NULL == pNodeElem)
	{
		return ERROR_PARAMETER;
	}

	new = node_new(pNodeElem, enElemType);
	if (NULL == new)
	{
		return ERROR_RESOURCE;
	}

	node_insert(pStack, pStack->next, new);

	return ERROR_SUCCESS;
}

UINT stack_pop(LIST_NODE_S *pStack, LIST_NODE_S **ppPopNode)
{
	if (NULL == pStack || NULL == ppPopNode)
	{
		return ERROR_RESOURCE;
	}
 
	*ppPopNode == NULL;
	if (BOOL_TRUE != node_is_empty(pStack))
	{
		*ppPopNode = pStack->prev;
		node_split(*ppPopNode);
	}

	return ERROR_SUCCESS;
}


UINT stack_view(LIST_NODE_S *pStack)
{
	if (NULL == pStack)
	{
		return ERROR_PARAMETER;
	}

	list_view(pStack);
}


UINT stack_destory(LIST_NODE_S *pStack)
{
	if (NULL == pStack)
	{
		return ERROR_PARAMETER;
	}

	return list_destory(pStack);
}
/* End: Stack common operations */



/*Begin: vertex tree common operations  */
STATIC VERTEX_TREE_NODE_S *tree_node_new(VOID *pNodeElem, NODE_ELEM_TYPE enElemType)
{
	VERTEX_TREE_NODE_S *new = NULL;

	new = (VERTEX_TREE_NODE_S *)malloc(sizeof(VERTEX_TREE_NODE_S));
	if(NULL == new)
	{
		return NULL;
	}

	new->parent = NULL;
	new->lchild = NULL;
	new->rchild = NULL;
	new->pNodeElem = pNodeElem;
	new->enElemType = enElemType;
	
	return new;
}


UINT tree_init(VERTEX_TREE_NODE_S **ppRoot)
{
	if (NULL == ppRoot)
	{
		return ERROR_PARAMETER;
	}

	*ppRoot == NULL;

	return ERROR_SUCCESS;
}


/*
 * Function     : 	tree_insert_node()
 * Parameters   : 
 					VERTEX_TREE_NODE_S **ppRoot		##  根节点
 					VERTEX_TREE_NODE_S *pNew		##  新节点
 					COMPARE_PF pfCompare			##  函数指针，用来比较
 														两个顶点中数据的
 * Return       :	
 					ERROR_SUCCESS 					## 插入成功
 					其他值
 * Description  : 
 					树节点插入，左孩子 > 父节点 > 右节点
 					通过调用函数指针来比较。
 						如果函数指针返回值 != 0则表示第一个参数要大于第二个
 						参数，则插入到左孩子的位置。否则插入到右孩子的位置。
 					
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28   Josh.Chow    增加回调函数指针COMPARE_PF pfCompare，使得该
                             树能够动态适应用户根据不同的顶点内容来做成不
                             同的树。该pfCompare函数由用户完成、并在插入节
                             点的地方给其赋值。
 */
STATIC UINT tree_insert_node(VERTEX_TREE_NODE_S **ppRoot, VERTEX_TREE_NODE_S *pNew, COMPARE_PF pfCompare)
{
	VERTEX_TREE_NODE_S *pCurrent = NULL;
	VERTEX_TREE_NODE_S *pParent = NULL;

	/*Step 1: 查找插入节点的父节点 */
	pCurrent = *ppRoot;
	while (pCurrent != NULL)
	{
		pParent = pCurrent;
		if (pfCompare(pNew->pNodeElem, pCurrent->pNodeElem))
		{
			pCurrent = pCurrent->lchild;
		}
		else
		{
			pCurrent = pCurrent->rchild;
		}
	}

	/*Step 2: 插入节点 */
	if (NULL == pParent)
	{
		*ppRoot = pNew;
	}
	else if (pfCompare(pNew->pNodeElem, pParent->pNodeElem))
	{
		pParent->lchild = pNew;
	}
	else
	{
		pParent->rchild = pNew;
	}

	return ERROR_SUCCESS;
}

UINT tree_insert(VERTEX_TREE_NODE_S **ppRoot, VOID *pNodeElem, NODE_ELEM_TYPE enElemType, COMPARE_PF pfCompare)
{
	VERTEX_TREE_NODE_S *pNewTreeNode = NULL;
	UINT uiRet = ERROR_SUCCESS;

	if (NULL == ppRoot || NULL == pNodeElem || NULL == pfCompare)
	{
		return ERROR_PARAMETER;
	}

	pNewTreeNode = tree_node_new(pNodeElem, enElemType);
	if (NULL == pNewTreeNode)
	{
		return ERROR_RESOURCE;
	}

	uiRet = tree_insert_node(ppRoot, pNewTreeNode, pfCompare);
	if (ERROR_SUCCESS != uiRet)
	{
		free(pNewTreeNode);
	}

	return uiRet;
}


STATIC VOID tree_node_print(VERTEX_TREE_NODE_S *pNode)
{
	if (VERTEX_TYPE == pNode->enElemType)
	{
		vertex_node_print(pNode->pNodeElem);
		printf("\r\n");
	}
	else if(EDGE_TYPE == pNode->enElemType)
	{
		edge_node_print(pNode->pNodeElem);
		printf("\r\n");
	}
	else
	{
		/*  None support print type */
	}
}


VOID tree_prev_view(VERTEX_TREE_NODE_S *pNode)
{
}


VOID tree_post_view(VERTEX_TREE_NODE_S *pNode)
{
}


VOID tree_mid_view(VERTEX_TREE_NODE_S *pNode)
{
	if (NULL == pNode)
	{
		return ;
	}
	
	tree_mid_view(pNode->lchild);
	tree_node_print(pNode);
	tree_mid_view(pNode->rchild);
}


VOID tree_destory(VERTEX_TREE_NODE_S **ppNode)
{
	VERTEX_TREE_NODE_S *pCurrent = NULL;

	if (NULL == ppNode)
	{
		return ;
	}

	pCurrent = *ppNode;
	if(NULL == pCurrent)
	{
		return ;
	}

	tree_destory(&(pCurrent->lchild));
	tree_destory(&(pCurrent->rchild));
	free(pCurrent);

	*ppNode = NULL;
}


UINT tree_to_queue(VERTEX_TREE_NODE_S **ppNode, LIST_NODE_S *pQueue)
{
	VERTEX_TREE_NODE_S *pCurrent = NULL;
	UINT uiRet = ERROR_SUCCESS;

	if (NULL == ppNode || NULL == pQueue)
	{
		return ERROR_PARAMETER;
	}

	pCurrent = *ppNode;
	if (NULL == pCurrent)
	{
		return ERROR_SUCCESS;
	}

	uiRet += tree_to_queue(&(pCurrent->lchild), pQueue);
	if (ERROR_SUCCESS == uiRet)
	{
		queue_enqueue(pQueue, pCurrent->pNodeElem, pCurrent->enElemType);
	}
	uiRet += tree_to_queue(&(pCurrent->rchild), pQueue);

	return uiRet;
}


/*
 * Function     : 	tree_to_list()
 * Parameters   : 
 					VERTEX_TREE_NODE_S **ppNode		##  树节点
 					LIST_NODE_S *pListHead		##  链表头
 * Return       :	
 					ERROR_SUCCESS 					## 插入成功
 					其他值
 * Description  : 	
 					本树程序是按照出度: 左孩子 > 父节点 > 右孩子来组织的，
 					按照中序遍历，第一个被插入的节点是最大的，最后一个插
 					入的节点时最小的。把当前树节点插入到链表中时，要保证
 					链表是从大到小排序，需要插入节点到链表的tail。
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28
 */
UINT tree_to_list(VERTEX_TREE_NODE_S **ppNode, LIST_NODE_S *pListHead)
{
	VERTEX_TREE_NODE_S *pCurrent = NULL;
	UINT uiRet = ERROR_SUCCESS;

	if (NULL == ppNode || NULL == pListHead)
	{
		return ERROR_PARAMETER;
	}

	pCurrent = *ppNode;
	if (NULL == pCurrent)
	{
		return ERROR_SUCCESS;
	}

	uiRet += tree_to_list(&(pCurrent->lchild), pListHead);
	if (ERROR_SUCCESS == uiRet)
	{
		list_node_add_tail(pListHead, pCurrent->pNodeElem, pCurrent->enElemType);
	}
	uiRet += tree_to_list(&(pCurrent->rchild), pListHead);

	return uiRet;
}
/*End: vertex tree common operations  */


#ifdef __cplusplus
}
#endif
