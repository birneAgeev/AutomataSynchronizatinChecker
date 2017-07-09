#include "AdjacencyListIterator.h"

AdjacencyListIterator::AdjacencyListIterator(const MultiListGraph* graph, int listPtr): graph(graph), listPtr(listPtr)
{
}

AdjacencyListIterator::AdjacencyListIterator(const AdjacencyListIterator& iterator): graph(iterator.graph), listPtr(iterator.listPtr)
{
}

AdjacencyListIterator& AdjacencyListIterator::operator++()
{
	listPtr = graph->previous[listPtr];
	return *this;
}

AdjacencyListIterator::value_type AdjacencyListIterator::operator*() const
{
	return graph->multiList[listPtr];
}

bool AdjacencyListIterator::operator==(const AdjacencyListIterator& other) const
{
	return graph == other.graph && listPtr == other.listPtr;
}

bool AdjacencyListIterator::operator!=(const AdjacencyListIterator& other) const
{
	return !(*this == other);
}
