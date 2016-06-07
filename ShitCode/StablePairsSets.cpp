#include "StablePairsSets.h"
#include <set>

namespace {
	template <typename T>
	std::vector<T> SetToVector(const std::set<T>& a) {
		std::vector<T> ans;
		for (const auto& entry : a)
			ans.push_back(entry);

		return ans;
	}

	std::vector<AutomataStatesPair> BuildALittleBitStablePairs(const Graph& graph, const AutomataStatesPair& stablePair, const std::vector<int>& letters) {
		int k = 6; // [1/2e] + 1, where 0 < e < 0.125
		std::set<AutomataStatesPair> ans;
		auto cur = stablePair;

		int letter = -1;
		for (int i = 0; i < letters.size(); ++i) {
			int c = letters[i];
			if (c != stablePair.GetDependLetter()) {
				letter = c;
				break;
			}
		}

		for (int i = 0; i < k; ++i) {
			cur = cur.GetNext(graph, letter);

			if (cur.GetP() == cur.GetQ())
				return SetToVector(ans);

			ans.insert(cur);
		}

		return SetToVector(ans);
	}

	std::vector<AutomataStatesPair> BuildStablePairs(const Graph& graph, const std::vector<AutomataStatesPair>& stablePairs, int dependLetter) {
		auto n = graph.size();
		int stablePairsCount = int(pow(double(n), 0.4));

		for (const auto& pair : stablePairs) {
			std::set<AutomataStatesPair> ans;
			auto cur = pair.GetNext(graph, dependLetter);

			while (cur.GetP() != cur.GetQ() && ans.size() < stablePairsCount && ans.count(cur) == 0) {
				ans.insert(cur);
				cur = cur.GetNext(graph, dependLetter);
			}

			auto result = SetToVector(ans);
			if (ans.size() >= stablePairsCount)
				return result;
		}

		return std::vector<AutomataStatesPair>();
	}
}

StablePairsSets::StablePairsSets(const Graph& graph, const AutomataStatesPair& pivotStablePair, const std::vector<int>& letters) {
	pivotLetter = pivotStablePair.GetDependLetter();
	n = graph.size();
	auto aLittleBitStablePairs = BuildALittleBitStablePairs(graph, pivotStablePair, letters);
	stablePairsForSecondaryLetters = BuildStablePairs(graph, aLittleBitStablePairs, pivotStablePair.GetDependLetter());

	if (aLittleBitStablePairs.size() == 0)
		return;

	auto stablePair = aLittleBitStablePairs.front();
	aLittleBitStablePairs = BuildALittleBitStablePairs(graph, stablePair, letters);
	stablePairsForPivotLetter = BuildStablePairs(graph, aLittleBitStablePairs, stablePair.GetDependLetter());
}

std::vector<AutomataStatesPair> StablePairsSets::GetStablePairs(int letter) const {
	return letter == pivotLetter ? stablePairsForPivotLetter : stablePairsForSecondaryLetters;
}
