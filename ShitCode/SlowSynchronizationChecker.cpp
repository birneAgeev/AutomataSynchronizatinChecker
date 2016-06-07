#include "SlowSynchronizationChecker.h"

namespace {
	Graph BuildInvertedSquaredAutomaton(const Graph& graph, int sigma) {
		auto n = graph.size();
		Graph invertedSquaredAutomaton;
		invertedSquaredAutomaton.resize(n * n);

		for (auto i = 0; i < n; ++i) {
			for (auto j = 0; j < n; ++j) {
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
	std::vector<bool> used;
	auto n = graph.size();
	auto invertedSquaredAutomaton = BuildInvertedSquaredAutomaton(graph, sigma);

	used.assign(n * n, false);
	std::vector<int> q;
	for (int i = 0; i < n; ++i) {
		q.push_back(int(i * n + i));
		used[i * n + i] = true;
	}
	int l = 0;

	while (l < q.size()) {
		int v = q[l++];

		for (int i = 0; i < invertedSquaredAutomaton[v].size(); ++i) {
			int to = invertedSquaredAutomaton[v][i];

			if (used[to])
				continue;

			q.push_back(to);
			used[to] = true;
		}
	}

	for (int i = 0; i < used.size(); ++i) {
		if (!used[i])
			return false;
	}

	return true;
}
