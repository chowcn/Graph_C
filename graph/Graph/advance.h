/*
 * Document    : Advance.h
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 					��H�ļ������ֽṹ�嶨��:
 					1)ͨ�õ�VERTEX_NODE_S�����ǽڵ�������������С���ջ
 					�Ļ�����ͳ����ʽ�ṹ��
 					2)��VERTEX_TREE_NODE_S�ڵ�
 * Cautions    :
 				#-	���нṹ�嶼ֻ������VERTEX_S *pVertex�������ݽṹ��ʵ
 					���ǻ���:
 						����pVertex����ͼ�Ķ��㣬����Ҫ�ڱ�ʵ�����ͷŸ���Դ
 * -----------------------------------------------------------------------
   Modify History
  -----------------------------------------------------------------------
   Date		| Auther	 | Description
  -----------------------------------------------------------------------
  2017-5-29  Josh.Chow    1���޸�����˶Խڵ㱣���������͵�֧��NODE_ELEM_TYPE 
  						  enElemType���û�����ѡ�񱣴治ͬ��������ݣ�Ŀǰ��
  						  �ֶν����ڴ�ӡ���� �������ã�list_view��view������
  						  ��̬ʶ��Ҫ��ӡ�����͡�
  						  2��Cautions ��������䶯����鿴 ��#-����ʼ�Ĳ���
  						  ��ʾ��ɾ���Ĳ��֡�
  -----------------------------------------------------------------------  				
 */
#ifndef __ADVANCE_H__
#define __ADVANCE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum  tagNodeElemType
{
	VERTEX_TYPE,	/*  �������� */
	EDGE_TYPE,		/*  ������ */
	NONE_TYPE		/*  ������ */
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

	NODE_ELEM_TYPE enElemType;	/*  �洢�û�������������� */
	VOID *pNodeElem;
} LIST_NODE_S;


typedef struct tagTreeNode
{
	struct tagTreeNode *parent;
	struct tagTreeNode *lchild;
	struct tagTreeNode *rchild;
	NODE_ELEM_TYPE enElemType;	/*  �洢�û�������������� */
	VOID *pNodeElem;
} VERTEX_TREE_NODE_S;


typedef UINT (*COMPARE_PF)(VOID *pFormer, VOID *pLatter);


/* ������ʽ���ݽṹ�е����нڵ� */
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
