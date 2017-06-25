#pragma once
#include "TypesDeclarations.h"

enum FastSynchrinizationTestResult {
	DontKnow = 0,
	Synchronizable = 1,
	NonSynchronizable = 2
};

class FastSynchronizationChecker {

	FastSynchronizationChecker();

	FastSynchronizationChecker(const FastSynchronizationChecker&);
	FastSynchronizationChecker& operator =(FastSynchronizationChecker&);

public:
	static FastSynchronizationChecker& GetInstance();

	bool IsSynchronizableFast(const Graph& graph, int sigma) const;

	FastSynchrinizationTestResult IsSynchronizableFast(const Graph& graph, const std::vector<int>& letters) const;
};
