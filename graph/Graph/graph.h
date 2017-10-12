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
 * ����λ������
 */
typedef enum tagVertexAttr
{
	TOP,
	BOTTOM,
	ATTR_NONE
} VERTEX_ATTR_E;


/*
 *  ��ɫ���ԣ�Ϊͼ��BSF�ȱ����㷨��
 */
typedef enum color
{
	WHITE,
	BLACK,
	GRAY,
	COLOR_NONE
} COLOR_E;


/*
 * ���������ڵ�ı�
 */
typedef struct edge
{
	struct edge *prev;
	struct edge *next;	
	UINT  uiEdgeWeight;		/* �ߵ�Ȩ�� */ 
	UINT  uiNextIndex;		/* �¸��ڵ������ */
} EDGE_S;


typedef struct vertex
{
	UINT     uiThisIndex;	/* ��ǰ���������ֵ */
	COLOR_E  enColor;		/* ����ȷ�ϵ�ǰ�����Ƿ���ȫ���ʵ��� */
	UINT     uiIdegree;		/* ��� */
	UINT     uiEdegree;		/* ���� */
	UINT     uiFindTime;    /* ������ҵ�ʱ�䣬��������*/
	UINT     uiFinishTime;	/* ���������ʱ�� �� Ϊ���������� */
	UINT     uiDistance;    /* ��������㷨��ʾ���룬δʹ�� */
	VERTEX_ATTR_E  enAttr;  /* ---------------------- */   
	struct vertex *pParent;	/* ���ڵ㣬��������㷨 */	
	EDGE_S   stEdgeHead;	/* �������浱ǰ�ڵ��ܷ��ʵ���һ���ڵ������ */
} VERTEX_S;


typedef struct graph
{
	UINT       uiTableSize;		/* ��ǰͼ�Ķ�����С */
	VERTEX_S  *pVertexTable;	/* ����� */
} GRAPH_S;


/* ͼ�е�ÿһ������ */
#define for_each_vertex(vertexIter, graph)	\
	for (vertexIter=(graph)->pVertexTable; vertexIter<(graph)->pVertexTable + (graph)->uiTableSize; vertexIter++)

/* �����е�ÿ���� */
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
