#pragma once
#include <iterator>
#include "MultiListGraph.h"

template <typename TData>
class AdjacencyListIterator : public std::iterator<std::input_iterator_tag, TData>
{
	friend class MultiListGraph<TData>;

	AdjacencyListIterator(const MultiListGraph<TData>* graph, int listPtr) : graph(graph), listPtr(listPtr)
	{
	}

public:
	AdjacencyListIterator(const AdjacencyListIterator<TData>& iterator) : graph(iterator.graph), listPtr(iterator.listPtr)
	{
	}

	AdjacencyListIterator<TData>& operator++()
	{
		listPtr = graph->previous[listPtr];
		return *this;
	}

	TData operator*() const
	{
		return graph->multiList[listPtr];
	}

	bool operator ==(const AdjacencyListIterator<TData>& other) const
	{
		return graph == other.graph && listPtr == other.listPtr;
	}

	bool operator !=(const AdjacencyListIterator<TData>& other) const
	{
		return !(*this == other);
	}

private:
	const MultiListGraph<TData>* graph;
	int listPtr;
};
