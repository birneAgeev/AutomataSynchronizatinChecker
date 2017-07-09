#pragma once
#include <vector>
#include "AdjacencyListIterator.h"

class MultiListGraph
{
	friend class AdjacencyListIterator;
	std::vector<int> multiList;
	std::vector<int> previous;
	std::vector<int> last;
	int listPtr;

public:
	MultiListGraph(int n, int m);

	void AddEdge(int v, int to);

	AdjacencyListIterator GetVertexAdjacencyListBegin(int v) const;

	AdjacencyListIterator GetVertexAdjacencyListEnd() const;
};

