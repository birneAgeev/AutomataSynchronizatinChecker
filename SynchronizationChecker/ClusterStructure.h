#pragma once
#include "AutomataStatesPair.h"
#include "Condensation.h"
#include "MultiListGraph.h"
#include "AdjacencyListIterator.h"

class ClusterStructure {
public:
	struct VertexClusterInfo;
	struct ClusterInfo;

private:
	std::vector<VertexClusterInfo> vertexInfos;
	std::vector<ClusterInfo> clusterInfos;
	int clusterCount;
	std::vector<int> singleLetterGraph;
	MultiListGraph<int> invertedSingleLetterGraph;

	int highestTreeRootCyclePredecessor;
	int highestTreeRoot;
	int highestTreeHeight;
	int secondHighestTreeHeight;

	int dependLetter;

	int TreeDfs(int v, std::vector<bool>& used, int root, int clusterIndex, size_t& clusterSize, int height = 0);

public:
	struct VertexClusterInfo {
		int clusterIndex;
		int treeIndex;
		int height;

		VertexClusterInfo();

		VertexClusterInfo(int clusterIndex, int treeIndex, int height);
	};

	struct ClusterInfo {
		size_t clusterSize;
		size_t cycleLength;
		std::vector<int> cycleStates;

		ClusterInfo();

		ClusterInfo(int clusterSize, int cycleLength);
	};

	ClusterStructure();

	ClusterStructure(const Graph& graph, int letter);

	const std::vector<VertexClusterInfo>& GetVertexInfos() const;

	const std::vector<ClusterInfo>& GetClusterInfos() const;

	const std::vector<int>& GetSingleLetterGraph() const;

	int GetClusterCount() const;

	bool IsSingleHighestTree() const;

	bool DoesHighestOneCrownIntersectsWithCondensationMinComponent(Condensation& condensation) const;

	AutomataStatesPair GetStablePair() const;

	bool IsVertexInBigCluster(int v) const;

	bool IsBigCluster(int index) const;
};
