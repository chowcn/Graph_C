/*
 * Document    : main.c
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 					本main.c文件仅作为功能验证使用
 * Cautions    :
 					NONE
   -----------------------------------------------------------------------  				
 */
#include <stdio.h>
#include "common.h"
#include "graph.h"
#include "advance.h"

#if 0
/*
 * 
 */
int main(void)
{
	GRAPH_S G;
	
	VERTEX_TREE_NODE_S *pRoot = NULL;
	VERTEX_S *pVertexIter = NULL;
	LIST_NODE_S Queue;

	graph_init(&G, 4);
	
	printf("A graph :   \r\n");
	printf("  0  ->  2  \r\n");
	printf("  ^      ^  \r\n");
	printf("  |      |  \r\n");
	printf("  1  <-  3  \r\n");
	graph_add_edge(&G, 0, 2);
	graph_add_edge(&G, 1, 0);
	graph_add_edge(&G, 3, 1);
	graph_add_edge(&G, 3, 2);
	
	graph_degree(&G);

	printf("\t------graph_view--------\r\n");
	graph_view(&G);
	
	tree_init(&pRoot);
	for_each_vertex(pVertexIter, &G)
	{
		tree_insert(&pRoot, pVertexIter, VERTEX_TYPE, graph_compare_edegree);
	}

	printf("\t------vertex tree mid view by edegree-----\r\n");
	vertex_tree_mid_view(pRoot);

	queue_init(&Queue);
	
	tree_to_queue(&pRoot, &Queue);
	
	printf("\t-----vertex queue view-------\r\n");
	queue_view(&Queue);
		
	printf("\t-----graph BSF 3 --------------\r\n");
	graph_BSF(&G, 3);
	
	printf("\t-----graph BSF 3, 1 --------------\r\n");
	graph_path(&G, 3, 1);
	
	printf("\t-----release memory----------\r\n");
	tree_destory(&pRoot);
	queue_destory(&Queue);
	graph_destory(&G);

	return 0;
}
#endif

#if 0
/*
 * 贪心算法
 */
int main(void)
{
	GRAPH_S G;
	VERTEX_S *pVertexIter  = NULL;
	LIST_NODE_S stGreedyList;

	/*Step 1: 图初始化*/
	graph_init(&G, 8);

	/*Step 2 -- Begin : 添加图顶点、边信息 */
	for_each_vertex(pVertexIter, &G)
	{
		if (pVertexIter->uiThisIndex < 4)
		{
			pVertexIter->enAttr = TOP;
		}
		else
		{
			pVertexIter->enAttr = BOTTOM;
		}
	}
	
	graph_add_edge(&G, 0, 4);
	graph_add_edge(&G, 0, 5);
	graph_add_edge(&G, 1, 5);
	graph_add_edge(&G, 2, 5);
	graph_add_edge(&G, 2, 6);
	graph_add_edge(&G, 3, 4);
	graph_add_edge(&G, 3, 6);
	graph_add_edge(&G, 3, 7);
	/*Step 2 -- End : 添加图顶点、边信息 */

	/*Step 3: 图遍历 */
	printf("-----------------------------------------------------------------\r\n");
	printf("                           Graph view \r\n");
	printf("-----------------------------------------------------------------\r\n");
	graph_view(&G);


	/*Step 4: 贪心算法实现 */
	list_head_init(&stGreedyList);
	graph_greedy_vertex(&G, &stGreedyList);

	/*Step 5: 贪心算法结果打印 */
	printf("-----------------------------------------------------------------\r\n");
	printf("Greedy algrithm find less TOP vertex can access all BOTTOM vertex\r\n");
	printf("-----------------------------------------------------------------\r\n");
	list_view(&stGreedyList);
	printf("-----------------------------------------------------------------\r\n");
	
	/*Step 6: 资源释放 */
	graph_destory(&G);
	list_destory(&stGreedyList);
	
	return 0;
}
#endif


#if 0
/*
 * 拓扑排序
 */
int main(void)
{
	GRAPH_S G;
	VERTEX_S *pVertexIter  = NULL;
	LIST_NODE_S stWearOrdersHead;
	LIST_NODE_S *pNodeIter = NULL;
	UINT uiThisIndex = 0;
	
	/*  可穿戴的物品 */
	char *pcThings[9] = { "knickers",		/*0 -- 内裤*/
						  "trousers",		/*1 -- 裤子*/
						  "waistband",		/*2 -- 要带*/
						  "shirt",			/*3 -- 衬衣*/
						  "necktie",		/*4 -- 领带*/
						  "jacket",			/*5 -- 夹克*/
						  "socks",			/*6 -- 袜子*/
						  "shoes",			/*7 -- 鞋子*/
						  "watch"			/*8 -- 手表*/
						};
	
	/*Step 1: 图初始化*/
	graph_init(&G, 9);

	/*Step 2 -- Begin : 添加图顶点、边信息，图下标与pcTings[s]下标保持一致，边表示顺序 */
	graph_add_edge(&G, 0, 1);
	graph_add_edge(&G, 0, 7);
	graph_add_edge(&G, 1, 7);
	graph_add_edge(&G, 1, 2);
	graph_add_edge(&G, 2, 5);
	graph_add_edge(&G, 3, 2);
	graph_add_edge(&G, 3, 4);
	graph_add_edge(&G, 4, 5);
	graph_add_edge(&G, 6, 7);
	/*Step 2 -- Begin : 添加图顶点、边信息 */

	/*Step 3: 拓扑排序 */
	list_head_init(&stWearOrdersHead);
	graph_topological_sort(&G, &stWearOrdersHead);
	
	/*  图遍历 */
	printf("-----------------------------------------------------------------\r\n");
	printf("                           Graph view \r\n");
	printf("-----------------------------------------------------------------\r\n");
	graph_view(&G);

	/* 链表遍历 */
	printf("-----------------------------------------------------------------\r\n");
	printf("                           Topolgical sort view \r\n");
	printf("-----------------------------------------------------------------\r\n");
	list_view(&stWearOrdersHead);


	printf("-----------------------------------------------------------------\r\n");
	printf("                           Wearing Order \r\n");
	printf("-----------------------------------------------------------------\r\n");
	for_each_node(pNodeIter, &stWearOrdersHead)
	{
		uiThisIndex = pNodeIter->pVertex->uiThisIndex;
		printf("\tWearing %s\r\n", pcThings[uiThisIndex]);
	}

	/*Step : 资源释放 */
	graph_destory(&G);
	list_destory(&stWearOrdersHead);
	
	return 0;
}
#endif


/*
 * 判断环路
 */
int main(void)
{
	GRAPH_S G;
	UINT uiThisIndex = 0;
	
	/*Step 1: 图初始化*/
	graph_init(&G, 6);

	/*Step 2 -- Begin : 添加图顶点、边信息，图下标与pcTings[s]下标保持一致，边表示顺序 */
	graph_add_edge(&G, 0, 1);
	graph_add_edge(&G, 1, 2);
	graph_add_edge(&G, 2, 3);
	graph_add_edge(&G, 3, 4);
	graph_add_edge(&G, 3, 5);
	graph_add_edge(&G, 5, 4);
	graph_add_edge(&G, 3, 1);
	/*Step 2 -- Begin : 添加图顶点、边信息 */

	/*  图遍历 */
	printf("-----------------------------------------------------------------\r\n");
	printf("                           Graph view \r\n");
	printf("-----------------------------------------------------------------\r\n");
	graph_view(&G);

	if (BOOL_TRUE == graph_is_with_cycle(&G))
	{
		printf("Graph with cycle\r\n");
	}
	else
	{
		printf("Graph is acyclic\r\n");
	}
	
	/*Step : 资源释放 */
	graph_destory(&G);
	
	return 0;
}

