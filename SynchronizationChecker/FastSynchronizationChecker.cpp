#include "FastSynchronizationChecker.h"
#include "Utils.h"
#include "CondensationBuilder.h"
#include "SlowSynchronizationChecker.h"
#include "StablePairsSets.h"
#include <set>
#include "ClusterStructure.h"
#include "ClusterGraph.h"

namespace {
	bool CheckTheorem2Case1(const Graph& graph, const ClusterStructure& clusterStructureA, const ClusterStructure& clusterStructureB, const std::vector<int>& letters) {
		for (int i = 0; i < clusterStructureA.GetClusterCount(); ++i) {
			auto cluster = clusterStructureA.GetClusterInfos()[i];

			if (cluster.cycleLength > 2) {
				int tbCount = 0;
				for (size_t j = 0; j < cluster.cycleStates.size(); ++j) {
					int p = cluster.cycleStates[j];

					if (!clusterStructureB.IsBigCluster(clusterStructureB.GetVertexInfos()[p].clusterIndex))
						++tbCount;
				}

				if (tbCount > int(0.5 * cluster.cycleLength + 0.5))
					return false;
			}
			if (cluster.cycleLength == 2) {
				int dummyLetter = letters[0];
				auto statesPair = AutomataStatesPair(cluster.cycleStates[0], cluster.cycleStates[1], dummyLetter);

				if ((!clusterStructureA.IsVertexInBigCluster(statesPair.GetP()) &&
						!clusterStructureA.IsVertexInBigCluster(statesPair.GetQ())) ||
					(!clusterStructureB.IsVertexInBigCluster(statesPair.GetP()) &&
						!clusterStructureB.IsVertexInBigCluster(statesPair.GetQ())))
					continue;

				std::set<int> states;
				auto pBqB = statesPair.GetNext(graph, letters[1]);
				states.insert(pBqB.GetP());
				states.insert(pBqB.GetQ());
				auto pB2qB2 = pBqB.GetNext(graph, letters[1]);
				states.insert(pB2qB2.GetP());
				states.insert(pB2qB2.GetQ());

				if (states.size() == 2)
					return false;

				if (states.size() == 3) {
					if (!clusterStructureA.IsVertexInBigCluster(pBqB.GetP()) ||
						!clusterStructureA.IsVertexInBigCluster(pBqB.GetQ()))
						return false;
				}

				if (states.size() == 4) {
					if ((!clusterStructureA.IsVertexInBigCluster(pBqB.GetP()) ||
							!clusterStructureA.IsVertexInBigCluster(pBqB.GetQ())) &&
						(!clusterStructureA.IsVertexInBigCluster(pB2qB2.GetP()) ||
							!clusterStructureA.IsVertexInBigCluster(pB2qB2.GetQ())))
						return false;
				}
			}
		}

		return true;
	}

	std::vector<int> MoveSetByLetter(const Graph& graph, const std::vector<int>& states, int letter) {
		std::vector<int> ans(states.size());
		for (size_t i = 0; i < states.size(); ++i) {
			ans[i] = graph[states[i]][letter];
		}

		return ans;
	}

	bool CheckIfSetIsInSmallClusters(const std::vector<int>& states, const ClusterStructure& clusterStructure) {
		for (int a : states) {
			if (clusterStructure.IsBigCluster(clusterStructure.GetVertexInfos()[a].clusterIndex))
				return false;
		}

		return true;
	}

	std::vector<bool> BuildIndexSet(const ClusterStructure& clusterStructure, ClusterStructure::ClusterInfo cluster, int d) {
		std::vector<bool> Ip(d);
		for (int i = 0; i < d; ++i) {
			bool indexIsGood = true;
			for (size_t k = 0; k < cluster.cycleLength / d; ++k) {
				int state = cluster.cycleStates[(i + k * d) % cluster.cycleLength];

				if (clusterStructure.IsBigCluster(clusterStructure.GetVertexInfos()[state].clusterIndex)) {
					indexIsGood = false;
					break;
				}
			}

			if (indexIsGood)
				Ip[i] = true;
		}

		return Ip;
	}

	bool CheckTheorem2Case2(const Graph& graph, const ClusterStructure& clusterStructureA, const ClusterStructure& clusterStructureB, const std::vector<int>& letters) {
		int letterB = letters[1];

		std::vector<bool> isCycleInTb(clusterStructureA.GetClusterCount());
		std::vector<bool> isImageOfCycleInTa(clusterStructureA.GetClusterCount());
		for (int clusterIndex = 0; clusterIndex < clusterStructureA.GetClusterCount(); ++clusterIndex) {
			auto cluster = clusterStructureA.GetClusterInfos()[clusterIndex];
			isCycleInTb[clusterIndex] = CheckIfSetIsInSmallClusters(cluster.cycleStates, clusterStructureB);

			isImageOfCycleInTa[clusterIndex] = CheckIfSetIsInSmallClusters(MoveSetByLetter(graph, cluster.cycleStates, letterB), clusterStructureA);
		}

		for (int clusterIndex1 = 0; clusterIndex1 < clusterStructureA.GetClusterCount(); ++clusterIndex1) {
			for (int clusterIndex2 = clusterIndex1 + 1; clusterIndex2 < clusterStructureA.GetClusterCount(); ++clusterIndex2) {
				auto clusterP = clusterStructureA.GetClusterInfos()[clusterIndex1];
				auto clusterQ = clusterStructureA.GetClusterInfos()[clusterIndex2];

				if (clusterStructureA.IsBigCluster(clusterIndex1) && clusterStructureA.IsBigCluster(clusterIndex2))
					continue;

				if (clusterP.cycleLength > clusterQ.cycleLength)
					std::swap(clusterP, clusterQ);

				if (clusterP.cycleLength == 1) {
					if ((isCycleInTb[clusterIndex1] || isCycleInTb[clusterIndex2]) && (isImageOfCycleInTa[clusterIndex1] || isImageOfCycleInTa[clusterIndex2]))
						return false;
				}
				else {
					int d = Utils::GreatestCommonDivisor(int(clusterP.cycleLength), int(clusterQ.cycleLength));

					std::vector<bool> Ip = BuildIndexSet(clusterStructureB, clusterP, d);
					std::vector<bool> Iq = BuildIndexSet(clusterStructureB, clusterQ, d);

					for (int x = 0; x < d; ++x) {
						std::vector<bool> Iqx(d);
						for (int i = 0; i < d; ++i) {
							Iqx[(i + x) % d] = Iq[i];
						}

						bool unionIsEntireZd = true;
						for (int i = 0; i < d; ++i) {
							unionIsEntireZd &= Ip[i] || Iqx[i];
						}

						if (unionIsEntireZd)
							return false;
					}
				}
			}
		}

		return true;
	}
}

FastSynchrinizationTestResult FastSynchronizationChecker::IsSynchronizableFast(const Graph& graph, const std::vector<int>& letters) const{
	if (letters.size() != 2)
		throw std::runtime_error("Expected two-letter automata.");

	auto n = graph.size();
	auto condensation = CondensationBuilder::GetInstance().BuildCondensation(graph, letters);

	if (!condensation.IsSingleMin())
		return FastSynchrinizationTestResult::NonSynchronizable;

	std::vector<ClusterStructure> clusterStructures;
	ClusterStructure clusterStructureWithSingleHighestTree;
	bool appropriateClusterStructureFound = false;
	int clusterCountThreshold = int(5.0 * log(n));
	for (size_t i = 0; i < letters.size(); ++i) {
		int letter = letters[i];
		auto clusterStructure = ClusterStructure(graph, letter);
		clusterStructures.push_back(clusterStructure);

		if (clusterStructure.GetClusterCount() > clusterCountThreshold)
			return FastSynchrinizationTestResult::DontKnow;

		if (clusterStructure.IsSingleHighestTree() &&
			clusterStructure.DoesHighestOneCrownIntersectsWithCondensationMinComponent(condensation)) {

			clusterStructureWithSingleHighestTree = clusterStructure;
			appropriateClusterStructureFound = true;
		}
	}

	if (!appropriateClusterStructureFound)
		return FastSynchrinizationTestResult::DontKnow;

	auto stablePair = clusterStructureWithSingleHighestTree.GetStablePair();
	auto stablePairsSets = StablePairsSets(graph, stablePair, letters);

	for (size_t i = 0; i < letters.size(); ++i) {
		int letter = letters[i];
		auto clusterStructure = clusterStructures[i];
		auto stablePairs = stablePairsSets.GetStablePairs(letter);
		auto clusterGraph = ClusterGraph(clusterStructure, stablePairs);

		if (clusterGraph.IsColoringExists())
			return FastSynchrinizationTestResult::DontKnow;
	}

	if (!CheckTheorem2Case1(graph, clusterStructures[0], clusterStructures[1], letters) || !CheckTheorem2Case2(graph, clusterStructures[0], clusterStructures[1], letters))
		return FastSynchrinizationTestResult::DontKnow;

	return FastSynchrinizationTestResult::Synchronizable;
}

FastSynchronizationChecker::FastSynchronizationChecker() {}

FastSynchronizationChecker& FastSynchronizationChecker::GetInstance() {
	static FastSynchronizationChecker instance;
	return instance;
}

bool FastSynchronizationChecker::IsSynchronizableFast(const Graph& graph, int sigma) const {
	for (int i = 0; i + 1 < sigma; i += 2) {
		std::vector<int> letters{i, i + 1};

		auto twoLettersResult = FastSynchronizationChecker::IsSynchronizableFast(graph, letters);

		if (twoLettersResult == FastSynchrinizationTestResult::Synchronizable)
			return true;
	}
	return SlowSynchronizationChecker::GetInstance().IsSynchronizableSlow(graph, sigma);
}
