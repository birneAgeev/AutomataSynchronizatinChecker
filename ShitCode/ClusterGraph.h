#pragma once
#include <vector>
#include "ClusterStructure.h"

class ClusterGraph {
	struct ClusterGraphEdge {
		int toCluster;
		int fromVertex, toVertex;

		ClusterGraphEdge(int toCluster, int fromVertex, int toVertex);
	};

	std::vector<std::vector<ClusterGraphEdge>> adjacencyList;
	ClusterStructure clusterStructure;
	std::vector<AutomataStatesPair> stablePairs;

	void dfs(int v, std::vector<int>& coloring, std::vector<bool>& used, int d);

public:
	ClusterGraph(const ClusterStructure& clusterStructure, const std::vector<AutomataStatesPair>& stablePairs);

	bool IsColoringExists();
};
