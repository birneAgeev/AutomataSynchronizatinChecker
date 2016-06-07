#include "CondensationBuilder.h"

CondensationBuilder::CondensationBuilder() {}
namespace{
	Graph BuildInvertedGraph(const Graph& graph, const std::vector<int>& letters) {
		Graph invertedGraph;

		invertedGraph.resize(graph.size());
		for (size_t i = 0; i < graph.size(); ++i) {
			for (size_t j = 0; j < letters.size(); ++j) {
				int letter = letters[j];
				invertedGraph[graph[i][letter]].push_back(i);
			}
		}

		return invertedGraph;
	}

	void OrderDfs(const Graph& graph, const std::vector<int>& letters, int v, std::vector<bool>& used, std::vector<int>& order) {
		used[v] = true;

		for (size_t i = 0; i < letters.size(); ++i) {
			int letter = letters[i];
			int to = graph[v][letter];

			if (!used[to])
				OrderDfs(graph, letters, to, used, order);
		}

		order.push_back(v);
	}

	void ClusterDfs(const Graph& graph, int v, std::vector<bool>& used, std::vector<int>& cluster, int clusterNumber) {
		used[v] = true;
		cluster[v] = clusterNumber;

		for (size_t i = 0; i < graph[v].size(); ++i) {
			int to = graph[v][i];

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
	Graph condensation;
	std::vector<int> order;
	std::vector<bool> used;
	std::vector<int> cluster;

	auto n = graph.size();

	order.resize(0);

	used.assign(n, false);
	for (size_t i = 0; i < n; ++i) {
		if (!used[i])
			OrderDfs(graph, letters, i, used, order);
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

			condensation[cluster[v]].push_back(cluster[to]);
		}
	}

	return Condensation(condensation, cluster);
}
