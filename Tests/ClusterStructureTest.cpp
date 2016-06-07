#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utils.h"
#include "../ShitCode/ClusterGraph.h"
#include "../ShitCode/CondensationBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define DEBUG_PRINT

namespace Tests {
	TEST_CLASS(ClusterStructureTest) {
	public:
		const std::string outputPath = "..\\";
		const std::string resourcePath = "..\\Tests\\";

		TEST_METHOD(TestSimpleGraph) {
			Graph g;
			int sigma;
			Utils::ReadAutomaton(g, sigma, resourcePath + "simpleGraph.txt");

			auto letters = std::vector<int>{0, 1};
			auto condensation = CondensationBuilder::GetInstance().BuildCondensation(g, letters);
			auto clusterStructureA = ClusterStructure(g, letters[0]);

			Assert::IsTrue(clusterStructureA.IsSingleHighestTree());
			Assert::AreEqual(1, clusterStructureA.GetClusterCount());
			Assert::IsTrue(clusterStructureA.IsSingleHighestTree());
			Assert::AreEqual(2, clusterStructureA.GetStablePair().GetP());
			Assert::AreEqual(3, clusterStructureA.GetStablePair().GetQ());
			Assert::IsFalse(clusterStructureA.DoesHighestOneCrownIntersectsWithCondensationMinComponent(condensation));
			Assert::IsTrue(clusterStructureA.IsBigCluster(0));
			
			Assert::AreEqual(-1, clusterStructureA.GetVertexInfos()[0].height);
			Assert::AreEqual(0, clusterStructureA.GetVertexInfos()[0].treeIndex);
			Assert::AreEqual(2, clusterStructureA.GetVertexInfos()[6].height);
			Assert::AreEqual(3, clusterStructureA.GetVertexInfos()[6].treeIndex);
		}
	};
}
