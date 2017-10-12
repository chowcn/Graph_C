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
#ifndef __GRAPH_H__
#define __GRAPH_H__


#ifdef __cplusplus
extern "C"{
#endif


#define INF ~(0)


/*
 * 顶点位置属性
 */
typedef enum tagVertexAttr
{
	TOP,
	BOTTOM,
	ATTR_NONE
} VERTEX_ATTR_E;


/*
 *  颜色属性，为图的BSF等遍历算法用
 */
typedef enum color
{
	WHITE,
	BLACK,
	GRAY,
	COLOR_NONE
} COLOR_E;


/*
 * 链接两个节点的边
 */
typedef struct edge
{
	struct edge *prev;
	struct edge *next;	
	UINT  uiEdgeWeight;		/* 边的权重 */ 
	UINT  uiNextIndex;		/* 下个节点的索引 */
} EDGE_S;


typedef struct vertex
{
	UINT     uiThisIndex;	/* 当前顶点的索引值 */
	COLOR_E  enColor;		/* 用来确认当前顶点是否被完全访问到了 */
	UINT     uiIdegree;		/* 入度 */
	UINT     uiEdegree;		/* 初度 */
	UINT     uiFindTime;    /* 顶点查找到时间，拓扑排序*/
	UINT     uiFinishTime;	/* 顶点访问完时间 ， 为拓扑排序用 */
	UINT     uiDistance;    /* 广度优先算法表示距离，未使用 */
	VERTEX_ATTR_E  enAttr;  /* ---------------------- */   
	struct vertex *pParent;	/* 父节点，广度优先算法 */	
	EDGE_S   stEdgeHead;	/* 链表，保存当前节点能访问的下一个节点的索引 */
} VERTEX_S;


typedef struct graph
{
	UINT       uiTableSize;		/* 当前图的顶点表大小 */
	VERTEX_S  *pVertexTable;	/* 顶点表 */
} GRAPH_S;


/* 图中的每一个顶点 */
#define for_each_vertex(vertexIter, graph)	\
	for (vertexIter=(graph)->pVertexTable; vertexIter<(graph)->pVertexTable + (graph)->uiTableSize; vertexIter++)

/* 顶点中的每条边 */
#define for_each_edge(edgeIter, vertex)		\
	for (edgeIter=(vertex)->stEdgeHead.next; edgeIter!=&((vertex)->stEdgeHead); edgeIter=edgeIter->next)

	
EXTERN UINT edge_list_init(OUT EDGE_S *pHead);
EXTERN UINT edge_list_add(INOUT EDGE_S *pHead, INOUT EDGE_S *new);
EXTERN UINT edge_list_add_tail(INOUT EDGE_S *pHead, INOUT EDGE_S *new);
EXTERN VOID edge_node_print(IN EDGE_S *pEdge);
EXTERN UINT edge_list_split(INOUT EDGE_S *pSplit);
EXTERN UINT edge_list_isempty(IN EDGE_S *pHead);
EXTERN UINT edge_list_destory(OUT EDGE_S *pHead);

EXTERN UINT vertex_init(OUT VERTEX_S *pVertex, IN UINT uiThisIndex);
EXTERN UINT vertex_insert_edge(OUT VERTEX_S *pVertex, IN UINT uiNextIndex);
EXTERN VOID vertex_node_print(IN VERTEX_S *pVertex);
EXTERN UINT vertex_destory(OUT VERTEX_S *pVertex);

EXTERN UINT graph_init(OUT GRAPH_S * pGraph, IN UINT uiTableSize);
EXTERN UINT graph_destory(OUT GRAPH_S *pGraph);
EXTERN UINT graph_add_edge(OUT GRAPH_S *pGraph, IN UINT uiFromIndex, IN UINT uiToIndex);
EXTERN UINT graph_view(IN GRAPH_S *pGraph);
EXTERN UINT graph_degree(IN GRAPH_S *pGraph);
EXTERN UINT graph_BSF(IN GRAPH_S *pGraph, IN UINT uiFromIndex);
EXTERN UINT graph_path(IN GRAPH_S *pGraph, IN UINT uiFromIndex, IN UINT uiToIndex);


#ifdef __cplusplus
}
#endif


#endif	/* __GRAPH_H__ */
