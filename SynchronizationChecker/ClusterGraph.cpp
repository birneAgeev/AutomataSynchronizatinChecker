#include "ClusterGraph.h"
#include "Utils.h"

ClusterGraph::ClusterGraphEdge::ClusterGraphEdge(): toCluster(0), fromVertex(0), toVertex(0)
{
}

ClusterGraph::ClusterGraphEdge::ClusterGraphEdge(int toCluster, int fromVertex, int toVertex): toCluster(toCluster), fromVertex(fromVertex), toVertex(toVertex) { }

void ClusterGraph::dfs(int v, std::vector<int>& coloring, std::vector<bool>& used, int d) {
	used[v] = true;

	for (auto it = adjacencyList.GetVertexAdjacencyListBegin(v); it != adjacencyList.GetVertexAdjacencyListEnd(); ++it) {
		int to = (*it).toCluster;

		if (used[to])
			continue;

		int p = (*it).fromVertex;
		int q = (*it).toVertex;

		int pHeght = levels[p];
		int qHeight = levels[q];

		coloring[to] = ((qHeight - pHeght + coloring[v]) % d + d) % d;
		dfs(to, coloring, used, d);
	}
}

int GetLevel(int v, const ClusterStructure& clusterStructure, std::vector<int>& levels) {
	if (levels[v] != -1)
		return levels[v];

	auto vCluster = clusterStructure.GetVertexInfos()[v].clusterIndex;
	auto pivotVertex = clusterStructure.GetClusterInfos()[vCluster].cycleStates.front();
	int level = 0;
	if (v == pivotVertex)
		level = 0;
	else
		level = GetLevel(clusterStructure.GetSingleLetterGraph()[v], clusterStructure, levels) + 1;

	levels[v] = level;
	return level;
}

ClusterGraph::ClusterGraph(const ClusterStructure& clusterStructure, const std::vector<AutomataStatesPair>& stablePairs): adjacencyList(0, 0), clusterStructure(clusterStructure), stablePairs(stablePairs) {
	adjacencyList = MultiListGraph<ClusterGraphEdge>(clusterStructure.GetClusterCount(), 2*stablePairs.size());

	for (size_t i = 0; i < stablePairs.size(); ++i) {
		auto stablePair = stablePairs[i];
		int pCluster = clusterStructure.GetVertexInfos()[stablePair.GetP()].clusterIndex;
		int qCluster = clusterStructure.GetVertexInfos()[stablePair.GetQ()].clusterIndex;

		if (!clusterStructure.IsBigCluster(pCluster) || !clusterStructure.IsBigCluster(qCluster))
			continue;

		adjacencyList.AddEdge(pCluster, ClusterGraphEdge(qCluster, stablePair.GetP(), stablePair.GetQ()));
		adjacencyList.AddEdge(qCluster, ClusterGraphEdge(pCluster, stablePair.GetQ(), stablePair.GetP()));
	}

	size_t n = clusterStructure.GetSingleLetterGraph().size();
	levels = std::vector<int>(n, -1);
	for (size_t i = 0; i < n; ++i) {
		levels[i] = GetLevel(i, clusterStructure, levels);
	}
}

bool ClusterGraph::IsColoringExists() {
	int d = int(clusterStructure.GetClusterInfos().front().cycleLength);
	for (int i = 1; i < clusterStructure.GetClusterCount(); ++i) {
		d = Utils::GreatestCommonDivisor(d, int(clusterStructure.GetClusterInfos()[i].cycleLength));
	}

	auto n = adjacencyList.GetVerticesCount();

	std::vector<int> coloring;
	std::vector<bool> used;
	coloring.assign(n, -1);
	used.assign(n, false);
	coloring[0] = 0;

	dfs(0, coloring, used, d);

	for (size_t i = 0; i < n; ++i) {
		if (!used[i])
			return true;
	}

	if (d == 1)
		return false;

	for (size_t i = 0; i < stablePairs.size(); ++i) {
		int p = stablePairs[i].GetP();
		int q = stablePairs[i].GetQ();

		int pHeght = levels[p];
		int qHeight = levels[q];
		int pCluster = clusterStructure.GetVertexInfos()[p].clusterIndex;
		int qCluster = clusterStructure.GetVertexInfos()[q].clusterIndex;

		d = Utils::GreatestCommonDivisor(d, pHeght - qHeight - coloring[pCluster] + coloring[qCluster]);
	}

	return d != 1;
}
