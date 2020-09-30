//
// geomcpp tests
// Tests for Delauney triangulation.
//
// Sept-2020, Michael Lindner
// MIT license
//
#include "delauney_triangulation_tests.h"
#include "delauney_triangulation.h"
#include "test_util.h"
#include "essentutils/fputil.h"
#include <vector>

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testTriangulateWithNoPoints()
{
   {
      const std::string caseLabel = "Delauney triangulation with no points";

      using Fp = float;

      const std::vector<Point2<Fp>> noSamples;
      DelauneyTriangulation<Fp> dt{noSamples};
		const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(triangles.empty(), caseLabel);
   }
}

} // namespace


void testDelauneyTriangulation()
{
   testTriangulateWithNoPoints();
}
