#pragma once
#include <iterator>
#include "MultiListGraph.h"

class AdjacencyListIterator : public std::iterator<std::input_iterator_tag, int>
{
	friend class MultiListGraph;
	AdjacencyListIterator(const MultiListGraph* graph, int listPtr);
public:
	AdjacencyListIterator(const AdjacencyListIterator& iterator);

	AdjacencyListIterator& operator++();

	value_type operator*() const;

	bool operator ==(const AdjacencyListIterator& other) const;

	bool operator !=(const AdjacencyListIterator& other) const;

private:
	const MultiListGraph* graph;
	int listPtr;
};
