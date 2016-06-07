#include "AutomataStatesPair.h"
#include <algorithm>

int AutomataStatesPair::GetP() const {
	return p;
}

int AutomataStatesPair::GetQ() const {
	return q;
}

int AutomataStatesPair::GetDependLetter() const {
	return dependLetter;
}

AutomataStatesPair::AutomataStatesPair() {
	p = q = dependLetter = -1;
}

AutomataStatesPair::AutomataStatesPair(int p, int q, int dependLetter):
	p(std::min(p, q)), q(std::max(p, q)), dependLetter(dependLetter) { }

bool AutomataStatesPair::operator<(const AutomataStatesPair& other) const {
	return p < other.p ||
		p == other.p && q < other.q;
}

bool AutomataStatesPair::operator==(const AutomataStatesPair& other) const {
	return p == other.p && q == other.q;
}

AutomataStatesPair AutomataStatesPair::GetNext(const Graph& graph, int letter) const {
	return AutomataStatesPair(graph[p][letter], graph[q][letter], letter);
}
