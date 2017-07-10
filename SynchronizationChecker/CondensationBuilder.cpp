#include "CondensationBuilder.h"
#include "MultiListGraph.h"
#include "AdjacencyListIterator.h"

CondensationBuilder::CondensationBuilder() {}
namespace{
	MultiListGraph<int> BuildInvertedGraph(const Graph& graph, const std::vector<int>& letters) {
		MultiListGraph<int> invertedGraph(graph.size(), graph.size() * letters.size());

		for (size_t i = 0; i < graph.size(); ++i) {
			for (size_t j = 0; j < letters.size(); ++j) {
				int letter = letters[j];
				invertedGraph.AddEdge(graph[i][letter], i);
			}
		}

		return invertedGraph;
	}

	void OrderDfs(const Graph& graph, const std::vector<int>& letters, int v, std::vector<bool>& used, std::vector<int>& order, size_t& currentOrderSize) {
		used[v] = true;

		for (size_t i = 0; i < letters.size(); ++i) {
			int letter = letters[i];
			int to = graph[v][letter];

			if (!used[to])
				OrderDfs(graph, letters, to, used, order, currentOrderSize);
		}

		order[currentOrderSize++] = v;
	}

	void ClusterDfs(const MultiListGraph<int>& graph, int v, std::vector<bool>& used, std::vector<int>& cluster, int clusterNumber) {
		used[v] = true;
		cluster[v] = clusterNumber;

		for (auto it = graph.GetVertexAdjacencyListBegin(v); it != graph.GetVertexAdjacencyListEnd(); ++it)
		{
			int to = *it;

			if (used[to])
				continue;

			ClusterDfs(graph, to, used, cluster, clusterNumber);
		}
	}
}

CondensationBuilder& CondensationBuilder::GetInstance() {
	static CondensationBuilder instance;
	return instance;
}

Condensation CondensationBuilder::BuildCondensation(const Graph& graph, const std::vector<int>& letters) const {
	std::vector<int> condensation;
	auto n = graph.size();
	std::vector<int> order(n);
	std::vector<bool> used;
	std::vector<int> cluster;

	used.assign(n, false);
	size_t currentOrderSize = 0;
	for (size_t i = 0; i < n; ++i) {
		if (!used[i])
			OrderDfs(graph, letters, i, used, order, currentOrderSize);
	}
	std::reverse(order.begin(), order.end());

	auto invertedGraph = BuildInvertedGraph(graph, letters);
	used.assign(n, false);
	cluster.assign(n, -1);
	int clusterNumber = 0;
	for (size_t i = 0; i < n; ++i) {
		int v = order[i];
		if (!used[v])
			ClusterDfs(invertedGraph, v, used, cluster, clusterNumber++);
	}

	condensation.resize(clusterNumber);
	for (size_t v = 0; v < n; ++v) {
		for (size_t i = 0; i < letters.size(); ++i) {
			int letter = letters[i];
			int to = graph[v][letter];

			if (cluster[v] == cluster[to])
				continue;

			++condensation[cluster[v]];
		}
	}

	return Condensation(condensation, cluster);
}
