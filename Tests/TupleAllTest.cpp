#include <Tuple.hpp>
#include <UnitTest.hpp>

TEST_CASE(InternalBeExplicitTest)
{
	using namespace Yupei::Internal;
	using namespace Yupei;
	//static_assert(BeExplicit<tuple<int&&>, tuple<int&&>, true>::value, "");
	static_assert(NotBeExplicit<tuple<int&&>, tuple<int&&>, true>::value, "");
}