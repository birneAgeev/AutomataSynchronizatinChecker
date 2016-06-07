#pragma once
#include "TypesDeclarations.h"

class FastSynchronizationChecker {
	FastSynchronizationChecker();

	FastSynchronizationChecker(const FastSynchronizationChecker&);
	FastSynchronizationChecker& operator =(FastSynchronizationChecker&);

public:
	static FastSynchronizationChecker& GetInstance();

	bool IsSynchronizableFast(const Graph& graph, int sigma) const;
};
