#include "MultiListGraph.h"

MultiListGraph::MultiListGraph(int n, int m)
{
	multiList.resize(m);
	previous.assign(m, -1);
	last.assign(n, -1);
	listPtr = 0;
}

void MultiListGraph::AddEdge(int v, int to)
{
	multiList[listPtr] = to;
	previous[listPtr] = last[v];
	last[v] = listPtr++;
}

AdjacencyListIterator MultiListGraph::GetVertexAdjacencyListBegin(int v) const
{
	return AdjacencyListIterator(this, last[v]);
}

AdjacencyListIterator MultiListGraph::GetVertexAdjacencyListEnd() const
{
	return AdjacencyListIterator(this, -1);
}
