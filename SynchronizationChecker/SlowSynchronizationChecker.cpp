#include "SlowSynchronizationChecker.h"

namespace {
	Graph BuildInvertedSquaredAutomaton(const Graph& graph, int sigma) {
		auto n = graph.size();
		Graph invertedSquaredAutomaton;
		invertedSquaredAutomaton.assign(n * n, std::vector<int>(sigma));

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				auto v = i * n + j;

				for (auto c = 0; c < sigma; ++c) {
					auto toI = graph[i][c];
					auto toJ = graph[j][c];

					auto to = toI * n + toJ;
					invertedSquaredAutomaton[to].push_back(int(v));
				}
			}
		}

		return invertedSquaredAutomaton;
	}
}

SlowSynchronizationChecker::SlowSynchronizationChecker() {}

SlowSynchronizationChecker& SlowSynchronizationChecker::GetInstance() {
	static SlowSynchronizationChecker instance;
	return instance;
}

bool SlowSynchronizationChecker::IsSynchronizableSlow(const Graph& graph, int sigma) const {
	auto n = graph.size();
	auto invertedSquaredAutomaton = BuildInvertedSquaredAutomaton(graph, sigma);

	std::vector<bool> used(n * n);
	std::vector<int> q(n * n);
	for (size_t i = 0; i < n; ++i) {
		q[i] = i * n + i;
		used[i * n + i] = true;
	}
	size_t l = 0, r = n;

	while (l < q.size()) {
		int v = q[l++];

		for (size_t i = 0; i < invertedSquaredAutomaton[v].size(); ++i) {
			int to = invertedSquaredAutomaton[v][i];

			if (used[to])
				continue;

			q[r++] = to;
			used[to] = true;
		}
	}

	for (size_t i = 0; i < used.size(); ++i) {
		if (!used[i])
			return false;
	}

	return true;
}
