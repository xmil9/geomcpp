//
// geomcpp tests
// Tests for Delauney triangles.
//
// Sept-2020, Michael Lindner
// MIT license
//
#include "delauney_triangle_tests.h"
#include "delauney_triangle.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testTriangleAccessor()
{
   {
      const std::string caseLabel = "DelauneyTriangle::vertex accessor";

      using Fp = float;

		constexpr Point2<Fp> a(1.0f, 2.0f);
		constexpr Point2<Fp> b(3.0f, 4.0f);
		constexpr Point2<Fp> c(7.0f, 1.0f);
		const Triangle<Fp> t(a, b, c);
		const DelauneyTriangle<Fp> dt(t);
			
		const Triangle<Fp> tr = dt.triangle();

		VERIFY(t == tr, caseLabel);
   }
}

} // namespace


void testDelauneyTriangle()
{
   testTriangleAccessor();
}
