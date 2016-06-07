#pragma once
#include "AutomataStatesPair.h"

class StablePairsSets {
	std::vector<AutomataStatesPair> stablePairsForPivotLetter;
	std::vector<AutomataStatesPair> stablePairsForSecondaryLetters;
	int pivotLetter;
	size_t n;

public:
	StablePairsSets(const Graph& graph, const AutomataStatesPair& pivotStablePair, const std::vector<int>& letters);

	std::vector<AutomataStatesPair> GetStablePairs(int letter) const;
};
