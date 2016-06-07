#pragma once
#include "TypesDeclarations.h"

class SlowSynchronizationChecker {
	SlowSynchronizationChecker();

	SlowSynchronizationChecker(const SlowSynchronizationChecker&);
	SlowSynchronizationChecker& operator =(SlowSynchronizationChecker&);

public:
	static SlowSynchronizationChecker& GetInstance();

	bool IsSynchronizableSlow(const Graph& graph, int sigma) const;
};
