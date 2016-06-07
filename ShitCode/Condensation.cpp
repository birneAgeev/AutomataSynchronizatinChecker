#include "Condensation.h"

Condensation::Condensation(const Graph& condensation, const std::vector<int>& initialVerticiesColoring): condensation(condensation), initialVerticiesColoring(initialVerticiesColoring) {
	minComponentIndex = -1;
}

bool Condensation::IsSingleMin() const {
	int minCount = 0;
	for (int i = 0; i < condensation.size(); ++i) {
		if (condensation[i].size() == 0)
			++minCount;
	}
	return minCount <= 1;
}

int Condensation::GetMinComponentIndex() {
	if (minComponentIndex != -1)
		return minComponentIndex;

	for (int i = 0; i < condensation.size(); ++i) {
		if (condensation[i].size() == 0)
			minComponentIndex = i;
	}

	return minComponentIndex;
}
