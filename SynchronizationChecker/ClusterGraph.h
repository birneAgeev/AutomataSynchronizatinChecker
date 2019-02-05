#pragma once
#include <vector>
#include "ClusterStructure.h"
#include "MultiListGraph.h"

class ClusterGraph {
	struct ClusterGraphEdge {
		int toCluster;
		int fromVertex, toVertex;

		ClusterGraphEdge();
		ClusterGraphEdge(int toCluster, int fromVertex, int toVertex);
	};

	MultiListGraph<ClusterGraphEdge> adjacencyList;
//	std::vector<std::vector<ClusterGraphEdge>> adjacencyList;
	ClusterStructure clusterStructure;
	std::vector<AutomataStatesPair> stablePairs;
	std::vector<int> levels;

	void dfs(int v, std::vector<int>& coloring, std::vector<bool>& used, int d);

public:
	ClusterGraph(const ClusterStructure& clusterStructure, const std::vector<AutomataStatesPair>& stablePairs);

	bool IsColoringExists();
};
