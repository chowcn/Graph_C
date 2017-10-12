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
 					GRAPH_S *pGraph						##  树节点
 					UINT uiFromIndex					##  起始顶点索引
 * Return       :	
 					ERROR_PARAMETER					## 错误入参
 					ERROR_SUCCESS 					## 求解成功
 * Description  : 	
 					广度优先算法，以起始顶点为中心，向周围扩散。
 					经过该算法整理后的图，可以通过graph_path_print()求取以
 					uiFromIndex为起点到其它顶点的某条通路。
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

	/*Step 1: 初始化每个顶点为没有被优先算法操作的顶点 */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
		pVertexIter->uiDistance = INF;
		pVertexIter->pParent = NULL;
	}

	/*Step 2 --Begin: 广度遍历 */
	/*  起始定点 */
	pSourVertex = &(pGraph->pVertexTable[uiFromIndex]);
	pSourVertex->enColor = GRAY;
	pSourVertex->uiDistance = 0;
	pSourVertex->pParent = NULL;
	
	LIST_NODE_S Queue;
	LIST_NODE_S *pDeNode = NULL;
	VERTEX_S *pCurrVertex = NULL;		/*  指向当前被访问顶点 */
	VERTEX_S *pNextVertex = NULL;		/*  指向下一个被访问顶点 */
	EDGE_S *pEdgeIter = NULL;			/*  指向每一条边 */

	/*  初始化队列 */
	queue_init(&Queue);

	queue_enqueue(&Queue, pSourVertex, VERTEX_TYPE);

	/*  如果队列不为空，图顶点没有被完全访问完 */
	while( !queue_is_empty(&Queue) )
	{
		queue_dequeue(&Queue, &pDeNode);
		pCurrVertex = pDeNode->pNodeElem;

		/*  当前节点的每一个NextIndex */
		for_each_edge(pEdgeIter, pCurrVertex)
		{
			pNextVertex = &(pGraph->pVertexTable[pEdgeIter->uiNextIndex]);

			/*  如果NextIndex对应的节点没有被访问过 */
			if (WHITE == pNextVertex->enColor)
			{
				pNextVertex->enColor = GRAY;
				pNextVertex->uiDistance = pCurrVertex->uiDistance + 1;	/*  距离FromIndex的距离 */
				pNextVertex->pParent = pCurrVertex;						/*  graph_path_print()使用 */
				queue_enqueue(&Queue, pNextVertex, VERTEX_TYPE);
			}
		}
		pCurrVertex->enColor = BLACK;
		free(pDeNode);	/* Caution : 本处有释放队列资源，while循环结束，队列就不会占用资源了 */
	}
	/*Step 2 --End: 广度遍历 */

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
 					GRAPH_S *pGraph						##  树节点
 					VERTEX_S *pVertexInGraph			##  图中顶点
 * Return       :	
 					ERROR_PARAMETER					## 错误入参
 					ERROR_SUCCESS 					## 求解成功
 					ERROR_DFSCYCLE					## 求解遇到环路
 * Description  : 	
 					深度优先算法
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28  Josh.Chow     修改静态局部函数graph_DFS_visit()返回值，能够
                             返回具体DFS求解过程中出错原因。
	2017-5-29  Josh.Chow
                             修改静态局部函数graph_DFS_visit(),在求解DFS过
                             程中遇到回向边不return，继续访问，回向边不影响
                             深度优先算法(环路不影响深度优先算法)。
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
	
	/*Step 1 -- Begin: 以当前顶点为起点继续深度遍历图中顶点 */
	EDGE_S *pEdgeIter = NULL;
	VERTEX_S *pNextVertex = NULL;

	/*  每一个NextIndex */
	for_each_edge(pEdgeIter, pVertexInGraph)
	{
		pNextVertex = &(pGraph->pVertexTable[pEdgeIter->uiNextIndex]);

		/*  如果有NextIndex对应的节点没有被访问过 */
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
			/*  如果访问到灰色顶点，表示有回向边 */
			uiRet = ERROR_DFSCYCLE;
		}
		else
		{
			/*  如果访问到BLACK 顶点，则跳过该BLACK顶点 */
		}
	}
	/*Step 1 -- End: 以当前顶点为起点继续深度遍历图中顶点 */
	
	g_uiDfsTime = g_uiDfsTime + 1;
	pVertexInGraph->uiFinishTime = g_uiDfsTime;
	pVertexInGraph->enColor = BLACK;

	return uiRet;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_DFS()
 * Parameters   : 
 					GRAPH_S *pGraph						##  树节点
 * Return       :	
 					ERROR_PARAMETER					## 错误入参
 					ERROR_SUCCESS 					## 求解成功
 * Description  : 	
 					深度优先算法
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
  	2017-5-29  Josh.Chow     修改函数graph_DFS()针对不同的graph_DFS_visit
                             返回值进行修正，只返回出错或者成功。
 */
UINT graph_DFS(IN GRAPH_S *pGraph)
{
	VERTEX_S *pVertexIter = NULL;
	UINT uiRet = ERROR_SUCCESS;
	
	if (NULL == pGraph)
	{
		return ERROR_PARAMETER;
	}

	/*Step 1: 初始化每个顶点为没有被优先算法操作的顶点 */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
		pVertexIter->uiDistance = INF;
		pVertexIter->pParent = NULL;
	}
	
	g_uiDfsTime = 0;
	
	/*Step 2 --Begin: 广度遍历 */
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
	/*Step 2 --End: 广度遍历 */

	return uiRet;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_topological_sort()
 * Parameters   : 
 					GRAPH_S *pGraph						##  树节点
 					LIST_NODE_S *pTopologyHead        ##  拓扑排序
 * Return       :	
 					ERROR_PARAMETER					## 错误入参
 					ERROR_SUCCESS 					## 求解成功
 					其他值
 * Description  : 	
 					拓扑排序，对有向无环图进行排序。
 					1)对图进行深度优先算法。
 					2)按照每个节点的FinishTime进行由大到小排序。
 * Date         :	2017-5-28
 * -----------------------------------------------------------------------
    Modify History
   -----------------------------------------------------------------------
    Date      | Auther     | Description
   -----------------------------------------------------------------------
    2017-5-28   Josh.Chow   修改vertex_tree_insert()调用,增加比较顶点函数供
                            其回调。
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

	/*Step 1: 深度优先算法，完成每个节点的FinishTime */
	uiRet = graph_DFS(pGraph);
	if (ERROR_SUCCESS != uiRet)
	{
		return uiRet;
	}

	/*Step 2: 树中按照FinishTime由大到小排序 */
	tree_init(&pTreeRoot);
	for_each_vertex(pVertexIter, pGraph)
	{
		tree_insert(&pTreeRoot, pVertexIter, VERTEX_TYPE, graph_compare_finish_time);
	}

	/*Step 3: 树转为链表，既是拓扑排好序了 */
	tree_to_list(&pTreeRoot, pTopologyHead);
	
	/*Step 4: 清空资源 */
	tree_destory(&pTreeRoot);
	
	return ERROR_SUCCESS;
}


/* -----------------------------------------------------------------------
 * Function     : 	graph_greedy_vertex()
 * Parameters   : 
 					GRAPH_S *pGraph						##  树节点
 					LIST_NODE_S *pstGreedyList		##  求取结果的链表头
 * Return       :	
 					ERROR_PARAMETER					## 错误入参
 					ERROR_SUCCESS 					## 求解成功
 * Description  : 	
 					求取最少的TOP属性的图顶点能访问BOTTOM属性顶点的链表。
 					算法思路:
 						1)求取TOP属性的顶点的出度Edegree。
 						2)并根据该Edegree对TOP顶点排序，由大到小排序。
 						3)根据贪心算法表示，出度最大的顶点为最优解，如果通
 							过该TOP顶点能访问到下层BOTTOM定点，则该TOP定点
 							插入到求取结果的链表中。
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

	/*Step 1: 设置每个节点为未访问过 */
	for_each_vertex(pVertexIter, pGraph)
	{
		pVertexIter->enColor = WHITE;
	}

	/*Step 2: 求取每个节点的出度、入度 */
	uiRet = graph_degree(pGraph);
	if (ERROR_SUCCESS != uiRet)
	{
		return ERROR_FAILED;
	}

	/*Step 3: 根据出度将节点插入到树中，左孩子大于父节点大于右孩子*/
	tree_init(&pTreeRoot);
	for_each_vertex(pVertexIter, pGraph)
	{
		tree_insert(&pTreeRoot, pVertexIter, VERTEX_TYPE, graph_compare_edegree);
	}

	/*Step 4: 按照中序遍历的方法，把树中的顶点插入到链表中，此时stVertexList链表已经根据按出度排序好了 */
	list_head_init(&stVertexList);
	tree_to_list(&pTreeRoot, &stVertexList);

	/*Step 5: 按照顺序访问stVertexList中的顶点 */
	pVertexTable = pGraph->pVertexTable;
	for_each_node(pNodeIter, &stVertexList)
	{
		pVertexIter = pNodeIter->pNodeElem;

		/* 如果当前顶点为 TOP 定点 */
		if (TOP == pVertexIter->enAttr)
		{
			pVertexIter->enColor = BLACK;

			/* 遍历当前顶点中的所有边 */
			for_each_edge(pEdgeIter, pVertexIter)
			{
				pNextIter = pVertexTable + pEdgeIter->uiNextIndex;

				/* 如果通过边，访问到了没有被访问过的 BOTTOM 节点*/
				if (BOTTOM == pNextIter->enAttr && WHITE == pNextIter->enColor)
				{
					pNextIter->enColor = BLACK;
					uiGreedyFlag = BOOL_TRUE;		/* 标记当前定点为最优解 */			
				}
			}

			/* 如果当前顶点为最优解 */
			if (BOOL_TRUE == uiGreedyFlag)
			{
				uiGreedyFlag = BOOL_FALSE;
				list_node_add(pstGreedyList, pVertexIter, VERTEX_TYPE);
			}
		}
	}

	/*Step 6: 释放资源 */
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
