#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>
#include <fstream>
#include <random>
#include <iostream>

namespace Utils {
	namespace {
		const std::mt19937::result_type seed = std::random_device{}();

		std::mt19937 eng{seed};
	}

	inline unsigned int RandomNumber(unsigned int min, unsigned int max) {
		return std::uniform_int_distribution<unsigned int>{min, max}(eng);
	}

	inline int GreatestCommonDivisor(int a, int b) {
		return b ? GreatestCommonDivisor(b, a % b) : a;
	}

	inline bool DoubleEqual(double a, double b) {
		return fabs(a - b) < EPS;
	}

	inline bool DoubleLess(double a, double b) {
		return a < b && !DoubleEqual(a, b);
	}

	inline void PrintInGraphViz(const Graph& graph, const std::string& fileName) {
		std::string dot = "digraph G{\n";
		for (size_t i = 0; i < graph.size(); ++i) {
			for (size_t j = 0; j < graph[i].size(); ++j) {
				std::string edgeLabel = "";
				edgeLabel += j + 'a';

				dot += " " + std::to_string(i + 1) + "->" + std::to_string(graph[i][j] + 1) + "[label=" + edgeLabel + "]" + ";\n";
			}
		}
		dot += "}";

		std::ofstream out(fileName.c_str());
		out << dot;
		out.close();

		ShellExecuteA(nullptr, "open", "C:\\Program Files (x86)\\Graphviz2.38\\bin\\sfdp.exe", (fileName + " + -Tjpg -O").c_str(), nullptr, SW_HIDE);
	}

	inline Graph GenerateRandomAutomaton(int n, int sigma) {
		Graph result;
		result.resize(n);

		for (int i = 0; i < sigma; ++i) {
			for (int v = 0; v < n; ++v) {
				result[v].push_back(RandomNumber(0, n - 1));
			}
		}

		return result;
	}

	inline void ReadAutomaton(Graph& graph, int& sigma, const std::string& fileName) {
		graph.clear();

		FILE* input;
		errno_t errorNumber = fopen_s(&input, fileName.c_str(), "rt");
		if (errorNumber) {
			std::cerr << "Specified or default input file not found";
			exit(EXIT_FAILURE);
		}

		int n, m;
		fscanf_s(input, "%d%d%d", &n, &m, &sigma);
		graph.resize(n);
		for (int i = 0; i < n; ++i) {
			graph[i].resize(sigma);
		}

		for (int i = 0; i < m; ++i) {
			int from;
			int to;
			int letter;
			fscanf_s(input, "%d%d%d", &from, &to, &letter);
			--from;
			--to;

			graph[from][letter] = to;
		}

		if (input)
			fclose(input);
	}
}
