#pragma once
#include <vector>
#include "TypesDeclarations.h"
#include "Condensation.h"

class CondensationBuilder {
	CondensationBuilder();

	CondensationBuilder(const CondensationBuilder&);
	CondensationBuilder& operator =(CondensationBuilder&);

public:
	static CondensationBuilder& GetInstance();

	Condensation BuildCondensation(const Graph& graph, const std::vector<int>& letters) const;
};
