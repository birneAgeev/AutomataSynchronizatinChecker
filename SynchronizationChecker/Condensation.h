#pragma once
#include <vector>

class Condensation {
public:
	std::vector<int> condensation;
	std::vector<int> initialVerticiesColoring;
private:
	int minComponentIndex;

public:
	Condensation(const std::vector<int>& condensation, const std::vector<int>& initialVerticiesColoring);

	bool IsSingleMin() const;

	int GetMinComponentIndex();
};
