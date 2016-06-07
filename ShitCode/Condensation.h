#pragma once
#include <vector>
#include "TypesDeclarations.h"

class Condensation {
public:
	Graph condensation;
	std::vector<int> initialVerticiesColoring;
private:
	int minComponentIndex;

public:
	Condensation(const Graph& condensation, const std::vector<int>& initialVerticiesColoring);

	bool IsSingleMin() const;

	int GetMinComponentIndex();
};
