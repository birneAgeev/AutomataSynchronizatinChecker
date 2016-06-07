#include "stdafx.h"
#include <string>
#include <vector>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template <>
			static std::wstring ToString<std::vector<int>>(const std::vector<int>& a) {
				std::wstring ans;
				for (size_t i = 0; i < a.size(); ++i) {
					ans += std::to_wstring(a[i]) + (i != a.size() - 1 ? L", " : L"");
				}
				return ans;
			};
		}
	}
}
