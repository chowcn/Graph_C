/*
 * Document    : Advance.h
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 					本H文件由两种结构体定义:
 					1)通用的VERTEX_NODE_S，它是节点操作，链表、队列、堆栈
 					的基础，统称链式结构。
 					2)树VERTEX_TREE_NODE_S节点
 * Cautions    :
 				#-	所有结构体都只保存了VERTEX_S *pVertex，本数据结构的实
 					现是基于:
 						所有pVertex都是图的顶点，不需要在本实现中释放该资源
 * -----------------------------------------------------------------------
   Modify History
  -----------------------------------------------------------------------
   Date		| Auther	 | Description
  -----------------------------------------------------------------------
  2017-5-29  Josh.Chow    1、修改添加了对节点保存数据类型的支持NODE_ELEM_TYPE 
  						  enElemType，用户可以选择保存不同种类的数据，目前本
  						  字段仅仅在打印上有 稍许作用，list_view等view函数，
  						  动态识别要打印的类型。
  						  2、Cautions 中有稍许变动，请查看 ‘#-’开始的部分
  						  表示被删除的部分。
  -----------------------------------------------------------------------  				
 */
#ifndef __ADVANCE_H__
#define __ADVANCE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum  tagNodeElemType
{
	VERTEX_TYPE,	/*  顶点类型 */
	EDGE_TYPE,		/*  边类型 */
	NONE_TYPE		/*  无类型 */
}NODE_ELEM_TYPE;

typedef union tagListNodeElem
{
	VERTEX_S *pVertex;
	EDGE_S   *pEdge;
}LIST_NODE_ELEM;


typedef struct tagListNode
{
	struct tagListNode *prev;
	struct tagListNode *next;

	NODE_ELEM_TYPE enElemType;	/*  存储用户保存的数据类型 */
	VOID *pNodeElem;
} LIST_NODE_S;


typedef struct tagTreeNode
{
	struct tagTreeNode *parent;
	struct tagTreeNode *lchild;
	struct tagTreeNode *rchild;
	NODE_ELEM_TYPE enElemType;	/*  存储用户保存的数据类型 */
	VOID *pNodeElem;
} VERTEX_TREE_NODE_S;


typedef UINT (*COMPARE_PF)(VOID *pFormer, VOID *pLatter);


/* 遍历链式数据结构中的所有节点 */
#define for_each_node(pNodeIter, pListHead)		\
	for(pNodeIter=(pListHead)->next; pNodeIter != (pListHead); pNodeIter = pNodeIter->next)


EXTERN UINT list_head_init(LIST_NODE_S *pList);
EXTERN UINT list_destory(LIST_NODE_S *pListHead);
EXTERN UINT list_node_add(LIST_NODE_S *pListHead, VOID *pNodeElem, NODE_ELEM_TYPE enElemType);
EXTERN BOOL_T list_is_empty(LIST_NODE_S *pNode);
EXTERN UINT list_view(LIST_NODE_S *pListHead);


EXTERN UINT queue_init(LIST_NODE_S *pQueue);
EXTERN UINT queue_enqueue(LIST_NODE_S *pQueue, VOID *pNodeElem, NODE_ELEM_TYPE enElemType);
EXTERN UINT queue_dequeue(LIST_NODE_S *pQueue, LIST_NODE_S **ppDeNode);
EXTERN BOOL_T queue_is_empty(LIST_NODE_S *pNode);
EXTERN UINT queue_destory(LIST_NODE_S *pQueue);
EXTERN UINT queue_view(LIST_NODE_S *pQueue);


EXTERN UINT stack_init(LIST_NODE_S *pStack);
EXTERN UINT stack_push(LIST_NODE_S *pStack, VOID *pNodeElem, NODE_ELEM_TYPE enElemType);
EXTERN UINT stack_pop(LIST_NODE_S *pStack, LIST_NODE_S **ppPopNode);
EXTERN UINT stack_destory(LIST_NODE_S *pStack);
EXTERN UINT stack_view(LIST_NODE_S *pStack);


EXTERN UINT tree_init(VERTEX_TREE_NODE_S **ppRoot);
EXTERN UINT tree_insert(VERTEX_TREE_NODE_S **ppRoot, VOID *pNodeElem, NODE_ELEM_TYPE enElemType, COMPARE_PF pfCompare);
EXTERN VOID tree_prev_view(VERTEX_TREE_NODE_S *pNode);
EXTERN VOID tree_destory(VERTEX_TREE_NODE_S **ppNode);
EXTERN UINT tree_to_queue(VERTEX_TREE_NODE_S **ppNode, LIST_NODE_S *pQueue);


#ifdef __cplusplus
}
#endif

#endif /*  __ADVANCE_H__ */
