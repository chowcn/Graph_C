/*
 * Document    : graph.c
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 * Cautions    :
 * -----------------------------------------------------------------------
   Modify History
  -----------------------------------------------------------------------
   Date		| Auther	 | Description
  -----------------------------------------------------------------------
  2017-5-29  Josh.Chow    
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


/* Begin: Edge list function */
UINT edge_list_init(OUT EDGE_S *pHead)
{
	if (NULL == pHead)
	{
		return ERROR_PARAMETER;
	}

	pHead->prev = pHead;
	pHead->next = pHead;
	pHead->uiEdgeWeight = NONE_USE_VALUE;
	pHead->uiNextIndex  = NONE_USE_VALUE;

	return ERROR_SUCCESS;
}


STATIC VOID edge_list_insert(INOUT EDGE_S *former, INOUT EDGE_S *latter, INOUT EDGE_S *new)
{
	former->next = new;
	latter->prev = new;
	new->prev = former;
	new->next = latter;
}


UINT edge_list_add(INOUT EDGE_S *pHead, INOUT EDGE_S *new)
{
	if (NULL == pHead || NULL == new)
	{
		return ERROR_PARAMETER;
	}
	
	edge_list_insert(pHead, pHead->next, new);
	
	return ERROR_SUCCESS;
}


UINT edge_list_add_tail(INOUT EDGE_S *pHead, INOUT EDGE_S *new)
{
	if (NULL == pHead || NULL == new)
	{
		return ERROR_PARAMETER;
	}

	edge_list_insert(pHead->prev, pHead, new);

	return ERROR_SUCCESS;
}


STATIC VOID edge_list_node_sperate(INOUT EDGE_S *former, INOUT EDGE_S *latter, INOUT EDGE_S *split)
{
	former->next = latter;
	latter->prev = former;
	split->prev = split;
	split->next = split;
}


UINT edge_list_split(INOUT EDGE_S *pSplit)
{
	if (NULL == pSplit)
	{
		return ERROR_PARAMETER;
	}

	edge_list_node_sperate(pSplit->prev, pSplit->next, pSplit);
}

UINT edge_list_isempty(IN EDGE_S *pHead)
{
	if (NULL == pHead)
	{
		return BOOL_TRUE;
	}

	return (pHead->next == pHead)?BOOL_TRUE:BOOL_FALSE;
}

EDGE_S *edge_list_node_new(IN UINT uiEdgeWeight, UINT uiNextIndex)
{
	EDGE_S * new = NULL;

	new = (EDGE_S *)malloc(sizeof(EDGE_S));
	if (NULL == new)
	{
		return NULL;
	}
	new->uiEdgeWeight = uiEdgeWeight;
	new->uiNextIndex  = uiNextIndex;

	return new;
}


UINT edge_list_destory(OUT EDGE_S *pHead)
{
	EDGE_S *former = NULL;
	EDGE_S *latter = NULL;

	if (NULL == pHead)
	{
		return ERROR_PARAMETER;
	}

	former = pHead->next;
	while (former != pHead)
	{
		latter = former->next;
		free(former);
		former = latter;
	}

	pHead->prev = pHead;
	pHead->next = pHead;
}


VOID edge_node_print(IN EDGE_S *pEdge)
{
	if (NULL == pEdge)
	{
		printf("NULL pEdge\r\n");
		return ;
	}

	printf("EdgeWeight %u NextIndex %u", 
			pEdge->uiEdgeWeight, pEdge->uiNextIndex);

	return ;
}
/* End : Edge list function */


/*Begin : Vertex fuction */
UINT vertex_init(OUT VERTEX_S *pVertex, IN UINT uiCurrIndex)
{
	UINT uiRet = ERROR_FAILED;

	if (NULL == pVertex)
	{
		return ERROR_PARAMETER;
	}

	pVertex->uiThisIndex = uiCurrIndex;
	pVertex->uiIdegree = 0;
	pVertex->uiEdegree = 0;
	pVertex->enAttr = ATTR_NONE;
	pVertex->enColor = COLOR_NONE;
	pVertex->pParent = NULL;
	pVertex->uiDistance = 0;
	pVertex->uiFindTime = 0;
	pVertex->uiFinishTime = 0;
	uiRet = edge_list_init( &(pVertex->stEdgeHead) );
	if (ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	return ERROR_SUCCESS;
}


UINT vertex_insert_edge(OUT VERTEX_S *pVertex, IN UINT uiNextIndex)
{
	EDGE_S *pHead = NULL;
	EDGE_S *new = NULL;
	UINT uiRet = ERROR_FAILED;

	if (NULL == pVertex)
	{
		return ERROR_PARAMETER;
	}

	new = edge_list_node_new(0, uiNextIndex);
	if (NULL == new)
	{
		return ERROR_RESOURCE;
	}
	
	pHead = &(pVertex->stEdgeHead);
	uiRet = edge_list_add(pHead, new);
	if (ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	return ERROR_SUCCESS;
}

UINT vertex_destory(OUT VERTEX_S *pVertex)
{
	UINT uiRet = ERROR_FAILED;
	EDGE_S *pHead = NULL;

	if (NULL == pVertex)
	{
		return ERROR_PARAMETER;
	}

	pHead = &(pVertex->stEdgeHead);
	uiRet = edge_list_destory(pHead);
	if(ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	return ERROR_SUCCESS;
}


VOID vertex_node_print(IN VERTEX_S *pVertex)
{
	if (NULL == pVertex)
	{
		printf("NULL pVertex\r\n");
		return ;
	}

	printf("ThisIndex %u Color %u Idegree %u Edegree %u Attr %u", 
			pVertex->uiThisIndex, pVertex->enColor, pVertex->uiIdegree, pVertex->uiEdegree, pVertex->enAttr);
	printf(" FindTime %u, FinishTime %u", pVertex->uiFindTime, pVertex->uiFinishTime);
	
	return ;
}
/*End   : Vertex fuction */


/*Begin : Graph function */
UINT graph_init(OUT GRAPH_S * pGraph, IN UINT uiTableSize)
{
	VERTEX_S *pVertexTable = NULL;
	VERTEX_S *pVertexIter = NULL;
	UINT uiRet = ERROR_FAILED;
	UINT uiCurrIndex = 0;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	pVertexTable = (VERTEX_S *)malloc(sizeof(VERTEX_S) * uiTableSize);
	if (NULL == pVertexTable)
	{
		return ERROR_RESOURCE;
	}
	
	pGraph->pVertexTable = pVertexTable;
	pGraph->uiTableSize = uiTableSize;
	
	for_each_vertex(pVertexIter, pGraph)
	{
		uiRet = vertex_init(pVertexIter, uiCurrIndex);
		if (ERROR_SUCCESS != uiRet)
		{
			free(pVertexTable);
			pGraph->uiTableSize = 0;
			return uiRet;
		}
		uiCurrIndex++;
	}

	return ERROR_SUCCESS;
}

UINT graph_destory(OUT GRAPH_S *pGraph)
{
	VERTEX_S *vertexIter = NULL;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	for_each_vertex(vertexIter, pGraph)
	{
		vertex_destory(vertexIter);
	}

	free(pGraph->pVertexTable);
	pGraph->pVertexTable = NULL;
	pGraph->uiTableSize = 0;
}


UINT graph_add_edge(OUT GRAPH_S *pGraph, UINT uiFromIndex, UINT uiToIndex)
{
	UINT uiRet = ERROR_FAILED;
	UINT uiTableSize = 0;

	VERTEX_S *pVertexTable = NULL;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}
	
	uiTableSize = pGraph->uiTableSize;
	if (uiFromIndex >= uiTableSize || uiToIndex >= uiTableSize)
	{
		return ERROR_PARAMETER;
	}

	pVertexTable = pGraph->pVertexTable;
	uiRet = vertex_insert_edge(pVertexTable + uiFromIndex, uiToIndex);
	if (ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	return ERROR_SUCCESS;
}


UINT graph_degree(IN GRAPH_S *pGraph)
{
	VERTEX_S *pVertexTable = NULL;
	VERTEX_S *pVertexIter = NULL;
	EDGE_S   *pEdgeIter = NULL;
	UINT uiCurrIndex = 0;
	UINT uiNextIndex = 0;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	pVertexTable = pGraph->pVertexTable;
	for_each_vertex(pVertexIter, pGraph)
	{
		for_each_edge(pEdgeIter, pVertexIter)
		{
			uiNextIndex = pEdgeIter->uiNextIndex;
			pVertexTable[uiCurrIndex].uiEdegree++;
			uiNextIndex = pEdgeIter->uiNextIndex;
			pVertexTable[uiNextIndex].uiIdegree++;
		}
		uiCurrIndex++;
	}

	return ERROR_SUCCESS;
}


UINT graph_view(IN GRAPH_S *pGraph)
{
	VERTEX_S *pVertexIter = NULL;
	EDGE_S   *pEdgeIter   = NULL;
	UINT uiCurrIndex = 0;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	for_each_vertex(pVertexIter, pGraph)
	{
		vertex_node_print(pVertexIter);
		printf("\r\nEdges :");
		for_each_edge(pEdgeIter, pVertexIter)
		{
			printf("\r\n\t");
			edge_node_print(pEdgeIter);
		}
		printf("\r\n");
		uiCurrIndex++;
	}
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_BSF()
 * Parameters   : 
 					GRAPH_S *pGraph						##  ���ڵ�
 					UINT uiFromIndex					##  ��ʼ��������
 * Return       :	
 					ERROR_PARAMETER					## �������
 					ERROR_SUCCESS 					## ���ɹ�
 * Description  : 	
 					��������㷨������ʼ����Ϊ���ģ�����Χ��ɢ��
 					�������㷨������ͼ������ͨ��graph_path_print()��ȡ��
 					uiFromIndexΪ��㵽���������ĳ��ͨ·��
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28
 */
UINT graph_BSF(IN GRAPH_S *pGraph, IN UINT uiFromIndex)
{
	VERTEX_S *pSourVertex = NULL;
	VERTEX_S *pVertexIter = NULL;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	if(uiFromIndex >= pGraph->uiTableSize)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: ��ʼ��ÿ������Ϊû�б������㷨�����Ķ��� */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
		pVertexIter->uiDistance = INF;
		pVertexIter->pParent = NULL;
	}

	/*Step 2 --Begin: ��ȱ��� */
	/*  ��ʼ���� */
	pSourVertex = &(pGraph->pVertexTable[uiFromIndex]);
	pSourVertex->enColor = GRAY;
	pSourVertex->uiDistance = 0;
	pSourVertex->pParent = NULL;
	
	LIST_NODE_S Queue;
	LIST_NODE_S *pDeNode = NULL;
	VERTEX_S *pCurrVertex = NULL;		/*  ָ��ǰ�����ʶ��� */
	VERTEX_S *pNextVertex = NULL;		/*  ָ����һ�������ʶ��� */
	EDGE_S *pEdgeIter = NULL;			/*  ָ��ÿһ���� */

	/*  ��ʼ������ */
	queue_init(&Queue);

	queue_enqueue(&Queue, pSourVertex, VERTEX_TYPE);

	/*  ������в�Ϊ�գ�ͼ����û�б���ȫ������ */
	while( !queue_is_empty(&Queue) )
	{
		queue_dequeue(&Queue, &pDeNode);
		pCurrVertex = pDeNode->pNodeElem;

		/*  ��ǰ�ڵ��ÿһ��NextIndex */
		for_each_edge(pEdgeIter, pCurrVertex)
		{
			pNextVertex = &(pGraph->pVertexTable[pEdgeIter->uiNextIndex]);

			/*  ���NextIndex��Ӧ�Ľڵ�û�б����ʹ� */
			if (WHITE == pNextVertex->enColor)
			{
				pNextVertex->enColor = GRAY;
				pNextVertex->uiDistance = pCurrVertex->uiDistance + 1;	/*  ����FromIndex�ľ��� */
				pNextVertex->pParent = pCurrVertex;						/*  graph_path_print()ʹ�� */
				queue_enqueue(&Queue, pNextVertex, VERTEX_TYPE);
			}
		}
		pCurrVertex->enColor = BLACK;
		free(pDeNode);	/* Caution : �������ͷŶ�����Դ��whileѭ�����������оͲ���ռ����Դ�� */
	}
	/*Step 2 --End: ��ȱ��� */

	return ERROR_SUCCESS;
}


STATIC VOID graph_path_print(IN GRAPH_S *pGraph, IN VERTEX_S *pSourVertex, IN VERTEX_S *pDestVertex)
{
	if (pSourVertex == pDestVertex)
	{	
		vertex_node_print(pDestVertex);
		printf("\r\n");
	}
	else if (NULL == pDestVertex->pParent)
	{
		printf("No path from %u to %u\r\n", pSourVertex->uiThisIndex, pDestVertex->uiThisIndex);
	}
	else
	{
		graph_path_print(pGraph, pSourVertex, pDestVertex->pParent);		
		vertex_node_print(pDestVertex);
		printf("\r\n");
	}
}


UINT graph_path(IN GRAPH_S *pGraph, IN UINT uiFromIndex, IN UINT uiToIndex)
{
	VERTEX_S *pVertexTable = NULL;

	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	if (uiFromIndex >= pGraph->uiTableSize || uiToIndex >= pGraph->uiTableSize)
	{
		return ERROR_PARAMETER;
	}
	pVertexTable = pGraph->pVertexTable;

	graph_path_print(pGraph, pVertexTable + uiFromIndex, pVertexTable + uiToIndex);

	return ERROR_SUCCESS;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_DFS_visit()
 * Parameters   : 
 					GRAPH_S *pGraph						##  ���ڵ�
 					VERTEX_S *pVertexInGraph			##  ͼ�ж���
 * Return       :	
 					ERROR_PARAMETER					## �������
 					ERROR_SUCCESS 					## ���ɹ�
 					ERROR_DFSCYCLE					## ���������·
 * Description  : 	
 					��������㷨
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28  Josh.Chow     �޸ľ�̬�ֲ�����graph_DFS_visit()����ֵ���ܹ�
                             ���ؾ���DFS�������г���ԭ��
	2017-5-29  Josh.Chow
                             �޸ľ�̬�ֲ�����graph_DFS_visit(),�����DFS��
                             ������������߲�return���������ʣ�����߲�Ӱ��
                             ��������㷨(��·��Ӱ����������㷨)��
 */
UINT g_uiDfsTime = 0;	/*  Record DFS time */

STATIC UINT graph_DFS_visit(IN GRAPH_S *pGraph, IN VERTEX_S *pVertexInGraph)
{
	UINT uiRet = ERROR_SUCCESS;
	
	if (NULL == pGraph || NULL == pVertexInGraph)
	{
		return ERROR_FAILED;
	}

	g_uiDfsTime = g_uiDfsTime + 1;
	pVertexInGraph->uiFindTime = g_uiDfsTime;
	pVertexInGraph->enColor = GRAY;
	
	/*Step 1 -- Begin: �Ե�ǰ����Ϊ��������ȱ���ͼ�ж��� */
	EDGE_S *pEdgeIter = NULL;
	VERTEX_S *pNextVertex = NULL;

	/*  ÿһ��NextIndex */
	for_each_edge(pEdgeIter, pVertexInGraph)
	{
		pNextVertex = &(pGraph->pVertexTable[pEdgeIter->uiNextIndex]);

		/*  �����NextIndex��Ӧ�Ľڵ�û�б����ʹ� */
		if (WHITE == pNextVertex->enColor)
		{
			pNextVertex->pParent = pVertexInGraph;
			uiRet = graph_DFS_visit(pGraph, pNextVertex);
			if (ERROR_SUCCESS != uiRet && ERROR_DFSCYCLE != uiRet)
			{
				break;
			}
		}
		else if (GRAY == pNextVertex->enColor)
		{
			/*  ������ʵ���ɫ���㣬��ʾ�л���� */
			uiRet = ERROR_DFSCYCLE;
		}
		else
		{
			/*  ������ʵ�BLACK ���㣬��������BLACK���� */
		}
	}
	/*Step 1 -- End: �Ե�ǰ����Ϊ��������ȱ���ͼ�ж��� */
	
	g_uiDfsTime = g_uiDfsTime + 1;
	pVertexInGraph->uiFinishTime = g_uiDfsTime;
	pVertexInGraph->enColor = BLACK;

	return uiRet;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_DFS()
 * Parameters   : 
 					GRAPH_S *pGraph						##  ���ڵ�
 * Return       :	
 					ERROR_PARAMETER					## �������
 					ERROR_SUCCESS 					## ���ɹ�
 * Description  : 	
 					��������㷨
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
  	2017-5-29  Josh.Chow     �޸ĺ���graph_DFS()��Բ�ͬ��graph_DFS_visit
                             ����ֵ����������ֻ���س�����߳ɹ���
 */
UINT graph_DFS(IN GRAPH_S *pGraph)
{
	VERTEX_S *pVertexIter = NULL;
	UINT uiRet = ERROR_SUCCESS;
	
	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: ��ʼ��ÿ������Ϊû�б������㷨�����Ķ��� */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
		pVertexIter->uiDistance = INF;
		pVertexIter->pParent = NULL;
	}
	
	g_uiDfsTime = 0;
	
	/*Step 2 --Begin: ��ȱ��� */
	for_each_vertex(pVertexIter, pGraph)
	{
		if (WHITE == pVertexIter->enColor)
		{
			uiRet = graph_DFS_visit(pGraph, pVertexIter);
			if (ERROR_SUCCESS != uiRet && ERROR_DFSCYCLE != uiRet)
			{
				uiRet = ERROR_FAILED;
				break;
			}
			else
			{
				uiRet = ERROR_SUCCESS;
			}
			
		}
	}
	/*Step 2 --End: ��ȱ��� */

	return uiRet;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_topological_sort()
 * Parameters   : 
 					GRAPH_S *pGraph						##  ���ڵ�
 					LIST_NODE_S *pTopologyHead        ##  ��������
 * Return       :	
 					ERROR_PARAMETER					## �������
 					ERROR_SUCCESS 					## ���ɹ�
 					����ֵ
 * Description  : 	
 					�������򣬶������޻�ͼ��������
 					1)��ͼ������������㷨��
 					2)����ÿ���ڵ��FinishTime�����ɴ�С����
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28   Josh.Chow   �޸�vertex_tree_insert()����,���ӱȽ϶��㺯����
                            ��ص���
 */
STATIC UINT graph_compare_finish_time(VOID *pA, VOID *pB)
{
	VERTEX_S *pFormer = pA;
	VERTEX_S *pLatter = pB;
	
	return (pFormer->uiFinishTime > pLatter->uiFinishTime);
}

UINT graph_topological_sort(IN GRAPH_S *pGraph, OUT LIST_NODE_S *pTopologyHead)
{
	UINT uiRet = ERROR_FAILED;
	VERTEX_S *pVertexIter = NULL;
	VERTEX_TREE_NODE_S *pTreeRoot = NULL;
	
	if (NULL == pGraph || NULL == pTopologyHead)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: ��������㷨�����ÿ���ڵ��FinishTime */
	uiRet = graph_DFS(pGraph);
	if (ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	/*Step 2: ���а���FinishTime�ɴ�С���� */
	tree_init(&pTreeRoot);
	for_each_vertex(pVertexIter, pGraph)
	{
		tree_insert(&pTreeRoot, pVertexIter, VERTEX_TYPE, graph_compare_finish_time);
	}

	/*Step 3: ��תΪ�������������ź����� */
	tree_to_list(&pTreeRoot, pTopologyHead);
	
	/*Step 4: �����Դ */
	tree_destory(&pTreeRoot);
	
	return ERROR_SUCCESS;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_greedy_vertex()
 * Parameters   : 
 					GRAPH_S *pGraph						##  ���ڵ�
 					LIST_NODE_S *pstGreedyList		##  ��ȡ���������ͷ
 * Return       :	
 					ERROR_PARAMETER					## �������
 					ERROR_SUCCESS 					## ���ɹ�
 * Description  : 	
 					��ȡ���ٵ�TOP���Ե�ͼ�����ܷ���BOTTOM���Զ��������
 					�㷨˼·:
 						1)��ȡTOP���ԵĶ���ĳ���Edegree��
 						2)�����ݸ�Edegree��TOP���������ɴ�С����
 						3)����̰���㷨��ʾ���������Ķ���Ϊ���Ž⣬���ͨ
 							����TOP�����ܷ��ʵ��²�BOTTOM���㣬���TOP����
 							���뵽��ȡ����������С�
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28
 */
STATIC UINT graph_compare_edegree(VOID *pA, VOID *pB)
{
	VERTEX_S *pFormer = pA;
	VERTEX_S *pLatter = pB;

	return (pFormer->uiEdegree > pLatter->uiEdegree);
}

UINT graph_greedy_vertex(IN GRAPH_S *pGraph, OUT LIST_NODE_S *pstGreedyList)
{
	UINT uiRet = ERROR_SUCCESS;
	VERTEX_S *pVertexIter = NULL;
	VERTEX_S *pNextIter = NULL;
	VERTEX_S *pVertexTable = NULL;
	VERTEX_TREE_NODE_S *pTreeRoot = NULL;
	LIST_NODE_S stVertexList;
	LIST_NODE_S *pNodeIter = NULL;
	EDGE_S *pEdgeIter = NULL;
	BOOL_T uiGreedyFlag = BOOL_FALSE;
	
	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: ����ÿ���ڵ�Ϊδ���ʹ� */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
	}

	/*Step 2: ��ȡÿ���ڵ�ĳ��ȡ���� */
	uiRet = graph_degree(pGraph);
	if (ERROR_SUCCESS != uiRet)
	{
		return ERROR_FAILED;
	}

	/*Step 3: ���ݳ��Ƚ��ڵ���뵽���У����Ӵ��ڸ��ڵ�����Һ���*/
	tree_init(&pTreeRoot);
	for_each_vertex(pVertexIter, pGraph)
	{
		tree_insert(&pTreeRoot, pVertexIter, VERTEX_TYPE, graph_compare_edegree);
	}

	/*Step 4: ������������ķ����������еĶ�����뵽�����У���ʱstVertexList�����Ѿ����ݰ������������ */
	list_head_init(&stVertexList);
	tree_to_list(&pTreeRoot, &stVertexList);

	/*Step 5: ����˳�����stVertexList�еĶ��� */
	pVertexTable = pGraph->pVertexTable;
	for_each_node(pNodeIter, &stVertexList)
	{
		pVertexIter = pNodeIter->pNodeElem;

		/* �����ǰ����Ϊ TOP ���� */
		if (TOP == pVertexIter->enAttr)
		{
			pVertexIter->enColor = BLACK;

			/* ������ǰ�����е����б� */
			for_each_edge(pEdgeIter, pVertexIter)
			{
				pNextIter = pVertexTable + pEdgeIter->uiNextIndex;

				/* ���ͨ���ߣ����ʵ���û�б����ʹ��� BOTTOM �ڵ�*/
				if (BOTTOM == pNextIter->enAttr && WHITE == pNextIter->enColor)
				{
					pNextIter->enColor = BLACK;
					uiGreedyFlag = BOOL_TRUE;		/* ��ǵ�ǰ����Ϊ���Ž� */			
				}
			}

			/* �����ǰ����Ϊ���Ž� */
			if (BOOL_TRUE == uiGreedyFlag)
			{
				uiGreedyFlag = BOOL_FALSE;
				list_node_add(pstGreedyList, pVertexIter, VERTEX_TYPE);
			}
		}
	}

	/*Step 6: �ͷ���Դ */
	tree_destory(&pTreeRoot);
	list_destory(&stVertexList);

	return ERROR_SUCCESS;
}


BOOL_T graph_is_with_cycle(IN GRAPH_S *pGraph)
{
	if (NULL == pGraph)
	{
		return BOOL_FALSE;
	}

	if (ERROR_DFSCYCLE == graph_DFS(pGraph))
	{
		return BOOL_TRUE;
	}

	return BOOL_FALSE;
}
/*End   : Graph function */


#ifdef __cplusplus
}
#endif
