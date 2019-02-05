#include "ClusterStructure.h"
#include <algorithm>

int ClusterStructure::TreeDfs(int v, std::vector<bool>& used, int root, int clusterIndex, size_t& clusterSize, int height) {
	used[v] = true;
	++clusterSize;
	vertexInfos[v] = VertexClusterInfo(clusterIndex, root, height);
	int ans = height;

	for (auto it = invertedSingleLetterGraph.GetVertexAdjacencyListBegin(v); it != invertedSingleLetterGraph.GetVertexAdjacencyListEnd(); ++it) {
		int to = *it;
		if (used[to])
			continue;

		int subtreeHeight = TreeDfs(to, used, root, clusterIndex, clusterSize, height + 1);
		ans = std::max(ans, subtreeHeight);
	}

	return ans;
}

ClusterStructure::VertexClusterInfo::VertexClusterInfo():
	clusterIndex(-1), treeIndex(-1), height(-1) { }

ClusterStructure::VertexClusterInfo::VertexClusterInfo(int clusterIndex, int treeIndex, int height):
	clusterIndex(clusterIndex), treeIndex(treeIndex), height(height) { }

ClusterStructure::ClusterInfo::ClusterInfo():
	clusterSize(0), cycleLength(0), cycleStates(std::vector<int>()) { }

ClusterStructure::ClusterInfo::ClusterInfo(int clusterSize, int cycleLength):
	clusterSize(clusterSize), cycleLength(cycleLength) { }

ClusterStructure::ClusterStructure() : invertedSingleLetterGraph(0, 0){
	vertexInfos = std::vector<VertexClusterInfo>();
	clusterInfos = std::vector<ClusterInfo>();
	singleLetterGraph = std::vector<int>();
	clusterCount = 0;
	highestTreeHeight = -1;
	secondHighestTreeHeight = -1;
	highestTreeRoot = -1;
	highestTreeRootCyclePredecessor = -1;
	dependLetter = -1;
}

ClusterStructure::ClusterStructure(const Graph& graph, int letter) : invertedSingleLetterGraph(graph.size(), graph.size()) {
	auto n = graph.size();
	singleLetterGraph.resize(n);
	invertedSingleLetterGraph = MultiListGraph<int>(n, n);
	vertexInfos.resize(n);

	highestTreeHeight = -1;
	secondHighestTreeHeight = -1;
	highestTreeRoot = -1;
	highestTreeRootCyclePredecessor = -1;

	dependLetter = letter;

	for (size_t i = 0; i < n; ++i) {
		singleLetterGraph[i] = graph[i][letter];
		invertedSingleLetterGraph.AddEdge(graph[i][letter], i);
	}

	std::vector<bool> used(n, false);
	std::vector<bool> isCycleState(n, false);
	clusterCount = 0;
	std::vector<int> stack;
	stack.reserve(n);
	std::vector<int> cycleStates;
	cycleStates.reserve(n);
	for (size_t p = 0; p < n; ++p) {
		if (used[p])
			continue;

		auto currentClusterInfo = ClusterInfo();

		stack.clear();
		int cur = p;
		while (!used[cur]) {
			used[cur] = true;
			stack.push_back(cur);
			cur = singleLetterGraph[cur];
		}

		cycleStates.clear();
		int last = cur;

		while (!stack.empty() && stack.back() != last) {
			cycleStates.push_back(stack.back());
			isCycleState[stack.back()] = true;
			stack.pop_back(); //enumerate cycle states
		}
		stack.pop_back();
		cycleStates.push_back(last);
		isCycleState[last] = true;
		std::reverse(cycleStates.begin(), cycleStates.end());

		currentClusterInfo.cycleLength = cycleStates.size();
		currentClusterInfo.clusterSize = currentClusterInfo.cycleLength;
		currentClusterInfo.cycleStates = cycleStates;

		while (!stack.empty()) {
			used[stack.back()] = false; //unuse non-cycle states
			stack.pop_back();
		}

		//run dfs from all lvl1 states in the inverted graph
		for (size_t i = 0; i < cycleStates.size(); ++i) {
			int root = cycleStates[i];
			vertexInfos[root] = VertexClusterInfo(clusterCount, root, -1);
			int previousInCycle = cycleStates[int(i) - 1 < 0 ? cycleStates.size() - 1 : i - 1];
			for (auto it = invertedSingleLetterGraph.GetVertexAdjacencyListBegin(root); it != invertedSingleLetterGraph.GetVertexAdjacencyListEnd(); ++it) {
				int q = *it;
				if (isCycleState[q])
					continue;

				int height = TreeDfs(q, used, q, clusterCount, currentClusterInfo.clusterSize);

				if (height >= highestTreeHeight) {
					secondHighestTreeHeight = highestTreeHeight;
					highestTreeHeight = height;
					highestTreeRoot = q;
					highestTreeRootCyclePredecessor = previousInCycle;
					continue;
				}
				if (height > secondHighestTreeHeight) {
					secondHighestTreeHeight = height;
				}
			}
		}
		clusterInfos.push_back(currentClusterInfo);

		++clusterCount;
	}
}

const std::vector<ClusterStructure::VertexClusterInfo>& ClusterStructure::GetVertexInfos() const {
	return vertexInfos;
}

const std::vector<ClusterStructure::ClusterInfo>& ClusterStructure::GetClusterInfos() const {
	return clusterInfos;
}

int ClusterStructure::GetClusterCount() const {
	return clusterCount;
}

const std::vector<int>& ClusterStructure::GetSingleLetterGraph() const {
	return singleLetterGraph;
}

bool ClusterStructure::IsSingleHighestTree() const {
	return highestTreeHeight != secondHighestTreeHeight;
}

bool ClusterStructure::DoesHighestOneCrownIntersectsWithCondensationMinComponent(Condensation& condensation) const {
	auto n = singleLetterGraph.size();
	for (size_t i = 0; i < n; ++i) {
		if (vertexInfos[i].height > secondHighestTreeHeight && condensation.initialVerticiesColoring[i] == condensation.GetMinComponentIndex())
			return true;
	}

	return false;
}

AutomataStatesPair ClusterStructure::GetStablePair() const {
	return AutomataStatesPair(highestTreeRootCyclePredecessor, highestTreeRoot, dependLetter);
}

bool ClusterStructure::IsVertexInBigCluster(int v) const {
	return IsBigCluster(vertexInfos[v].clusterIndex);
}

bool ClusterStructure::IsBigCluster(int index) const {
	auto n = singleLetterGraph.size();
	size_t clusterSizeThreshold = size_t(pow(double(n), 0.45));
	return clusterInfos[index].clusterSize > clusterSizeThreshold;
}
