#pragma once
#include <vector>

template<typename T>
class AdjacencyListIterator;

template <typename TData>
class MultiListGraph
{
public:
	friend class AdjacencyListIterator<TData>;
	std::vector<TData> multiList;
	std::vector<int> previous;
	std::vector<int> last;
	int listPtr;

public:
	MultiListGraph(int n, int m)
	{
		multiList.resize(m);
		previous.assign(m, -1);
		last.assign(n, -1);
		listPtr = 0;
	}

	void AddEdge(int v, TData edgeInfo)
	{
		multiList[listPtr] = edgeInfo;
		previous[listPtr] = last[v];
		last[v] = listPtr++;
	}

	AdjacencyListIterator<TData> GetVertexAdjacencyListBegin(int v) const
	{
		return AdjacencyListIterator<TData>(this, last[v]);
	}

	AdjacencyListIterator<TData> GetVertexAdjacencyListEnd() const
	{
		return AdjacencyListIterator<TData>(this, -1);
	}

	int GetVerticesCount()
	{
		return last.size();
	}
};
