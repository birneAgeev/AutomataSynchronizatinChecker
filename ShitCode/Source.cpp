#pragma once

//For large automata (~1e6 states) please increase stack size
#pragma comment(linker, "/STACK:16777216")

#include <iostream>
#include <ctime>
#include "TypesDeclarations.h"
#include "FastSynchronizationChecker.h"
#include "Utils.h"

using namespace std;

char* getCmdOption(char** begin, char** end, const std::string& option) {
	auto itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) {
		return *itr;
	}
	return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[]) {

	if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help")) {
		printf("\n");
		printf("Usage: SynchronizationChecker.exe [-i FILENAME] [-o FILENAME]\n\n");
		printf("\t-i, --input\t\tInput file name. (Default input.txt)\n");
		printf("\t-o, --outout\t\tOutput file name. (Default stdout)\n");
		return 0;
	}

	string inputFileName = "input.txt";
	string outputFileName = "";

	if (cmdOptionExists(argv, argv + argc, "-i"))
		inputFileName = getCmdOption(argv, argv + argc, "-i");
	if (cmdOptionExists(argv, argv + argc, "--input"))
		inputFileName = getCmdOption(argv, argv + argc, "--input");

	if (cmdOptionExists(argv, argv + argc, "-o"))
		outputFileName = getCmdOption(argv, argv + argc, "-o");
	if (cmdOptionExists(argv, argv + argc, "--output"))
		outputFileName = getCmdOption(argv, argv + argc, "--output");

	if (outputFileName != "")
		freopen(outputFileName.c_str(), "wt", stdout);

	Graph g;
	int sigma;
	Utils::ReadAutomaton(g, sigma, inputFileName);

	auto result = FastSynchronizationChecker::GetInstance().IsSynchronizableFast(g, sigma);

	if (result) {
		printf("Automaton is synchronzing");
	}
	else {
		printf("Automaton is not synchronizing");
	}

	return 0;
}
