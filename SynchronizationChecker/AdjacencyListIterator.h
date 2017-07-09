#pragma once
#include <iterator>
#include "MultiListGraph.h"

class AdjacencyListIterator : public std::iterator<std::input_iterator_tag, int>
{
	friend class MultiListGraph;
	AdjacencyListIterator(MultiListGraph* graph, int listPtr);
public:
	AdjacencyListIterator(const AdjacencyListIterator& iterator);

	AdjacencyListIterator& operator++();

	reference operator*() const;

	bool operator ==(const AdjacencyListIterator& other) const;

	bool operator !=(const AdjacencyListIterator& other) const;

private:
	MultiListGraph* graph;
	int listPtr;
};
