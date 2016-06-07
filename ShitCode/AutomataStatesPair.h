#pragma once
#include "TypesDeclarations.h"

class AutomataStatesPair {
public:
	int GetP() const;

	int GetQ() const;

	int GetDependLetter() const;

	AutomataStatesPair();

	AutomataStatesPair(int p, int q, int dependLetter);

	bool operator <(const AutomataStatesPair& other) const;

	bool operator ==(const AutomataStatesPair& other) const;

	AutomataStatesPair GetNext(const Graph& graph, int letter) const;

private:
	int p, q;
	int dependLetter;
};
