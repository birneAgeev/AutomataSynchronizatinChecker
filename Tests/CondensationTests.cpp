#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SynchronizationChecker/CondensationBuilder.h"
#include "Utils.h"
#include "../SynchronizationChecker/Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//#define DEBUG_PRINT

namespace Tests {
	TEST_CLASS(CondensationTests) {
	public:
		const std::string outputPath = "..\\";
		const std::string resourcePath = "..\\Tests\\";

		TEST_METHOD(TestConnectedSingleMinGraph) {
			Graph g;
			int sigma;
			Utils::ReadAutomaton(g, sigma, resourcePath + "simpleGraph.txt");

			std::vector<int> letters{0, 1};
			auto condensation = CondensationBuilder::GetInstance().BuildCondensation(g, letters);

#ifdef DEBUG_PRINT
			Utils::PrintInGraphViz(g, outputPath + "output");
			Utils::PrintInGraphViz(condensation.condensation, outputPath + "condensation");
#endif

			Assert::AreEqual(condensation.initialVerticiesColoring, std::vector<int>{5, 5, 5, 4, 4, 4, 4, 3, 2, 1, 0});
			Assert::IsTrue(condensation.IsSingleMin());
			Assert::AreEqual(condensation.GetMinComponentIndex(), 5);
		}

		TEST_METHOD(TestNonConnectedGraph) {
			Graph g;
			int sigma;
			Utils::ReadAutomaton(g, sigma, resourcePath + "nonConnectedGraph.txt");

			std::vector<int> letters{0, 1};
			auto condensation = CondensationBuilder::GetInstance().BuildCondensation(g, letters);

#ifdef DEBUG_PRINT
			Utils::PrintInGraphViz(g, outputPath + "output");
			Utils::PrintInGraphViz(condensation.condensation, outputPath + "condensation");
#endif

			Assert::AreEqual(condensation.initialVerticiesColoring, std::vector<int>{1, 1, 1, 0, 0});
			Assert::IsFalse(condensation.IsSingleMin());
		}

		TEST_METHOD(TestConnectedMultipleMinGraph) {
			Graph g;
			int sigma;
			Utils::ReadAutomaton(g, sigma, resourcePath + "connectedMultipleMinGraph.txt");

			std::vector<int> letters{0, 1};
			auto condensation = CondensationBuilder::GetInstance().BuildCondensation(g, letters);

#ifdef DEBUG_PRINT
			Utils::PrintInGraphViz(g, outputPath + "output");
			Utils::PrintInGraphViz(condensation.condensation, outputPath + "condensation");
#endif

			Assert::AreEqual(condensation.initialVerticiesColoring, std::vector<int>{0, 2, 1});
			Assert::IsFalse(condensation.IsSingleMin());
		}
	};
}
