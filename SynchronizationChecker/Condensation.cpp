﻿#include "Condensation.h"

Condensation::Condensation(const std::vector<int>& condensation, const std::vector<int>& initialVerticiesColoring): condensation(condensation), initialVerticiesColoring(initialVerticiesColoring) {
	minComponentIndex = -1;
}

bool Condensation::IsSingleMin() const {
	int minCount = 0;
	for (size_t i = 0; i < condensation.size(); ++i) {
		if (condensation[i] == 0)
			++minCount;
	}
	return minCount <= 1;
}

int Condensation::GetMinComponentIndex() {
	if (minComponentIndex != -1)
		return minComponentIndex;

	for (size_t i = 0; i < condensation.size(); ++i) {
		if (condensation[i] == 0)
			minComponentIndex = i;
	}

	return minComponentIndex;
}
