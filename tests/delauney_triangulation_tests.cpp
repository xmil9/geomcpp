//
// geomcpp tests
// Tests for Delauney triangulation.
//
// Sept-2020, Michael Lindner
// MIT license
//
#include "delauney_triangulation_tests.h"
#include "delauney_triangulation.h"
#include "point2.h"
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


void testTriangulateWithOnePoint()
{
   {
      const std::string caseLabel = "Delauney triangulation with one point";

      using Fp = float;

      const std::vector<Point2<Fp>> samples{Point2<Fp>{1.0, 2.0}};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(triangles.empty(), caseLabel);
   }
}


void testTriangulateWithTwoPoints()
{
   {
      const std::string caseLabel = "Delauney triangulation with two points";

      using Fp = double;

      const std::vector<Point2<Fp>> samples{Point2<Fp>{1.0, 2.0}, Point2<Fp>{6.0, -3.0}};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(triangles.empty(), caseLabel);
   }
}


void testTriangulateWithThreePoints()
{
   {
      const std::string caseLabel = "Delauney triangulation with three points";

      using Fp = double;

      const Point2<Fp> a{1.0, 2.0};
      const Point2<Fp> b{6.0, -3.0};
      const Point2<Fp> c{-2.0, -1.0};
      const std::vector<Point2<Fp>> samples{a, b, c};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(triangles.size() == 1, caseLabel);
      if (triangles.size() == 1)
      {
         const auto& t = triangles[0];
         VERIFY(t.hasVertex(a), caseLabel);
         VERIFY(t.hasVertex(b), caseLabel);
         VERIFY(t.hasVertex(c), caseLabel);
      }
   }
}


void testTriangulateWithFourPointsAsRect()
{
   {
      const std::string caseLabel = "Delauney triangulation with four points as rect";

      using Fp = double;

      const std::vector<Point2<Fp>> samples{
         {1.0, 10.0}, {5.0, 10.0}, {1.0, 5.0}, {1.0, 1.0}};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(triangles.size() == 2, caseLabel);
      VERIFY(DelauneyTriangulation<Fp>::isDelauneyConditionSatisfied(triangles),
             caseLabel);
   }
}


void testTriangulateWithTenPoints()
{
   {
      const std::string caseLabel = "Delauney triangulation with ten points";

      using Fp = double;

      const std::vector<Point2<Fp>> samples{{2.0, 1.0},  {5.0, 2.0}, {2.0, 4.0},
                                            {5.0, 4.0},  {9.0, 5.0}, {5.0, 7.0},
                                            {10.0, 7.0}, {3.0, 8.0}, {1.0, 10.0}};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();

      VERIFY(DelauneyTriangulation<Fp>::isDelauneyConditionSatisfied(triangles),
             caseLabel);
   }
}


void testAccessDelauneyTriangles()
{
   {
      const std::string caseLabel = "Access Delauney triangles after triangulation";

      using Fp = double;

      const std::vector<Point2<Fp>> samples{
         {2.0, 1.0}, {5.0, 2.0}, {2.0, 4.0}, {5.0, 4.0}, {9.0, 5.0}};
      DelauneyTriangulation<Fp> dt{samples};
      const std::vector<Triangle<Fp>> triangles = dt.triangulate();
      const std::vector<DelauneyTriangle<Fp>> delauneyTriangles = dt.delauneyTriangles();

      VERIFY(triangles.size() == delauneyTriangles.size(), caseLabel);
   }
}


void testIsDelauneyConditionSatisfied()
{
   {
      const std::string caseLabel =
         "DelauneyTriangulation::isDelauneyConditionSatisfied for one triangle";

      using Fp = double;

      const std::vector<Triangle<Fp>> triangles{Triangle<Fp>{
         Point2<Fp>{1.0, 2.0}, Point2<Fp>{6.0, -3.0}, Point2<Fp>{-2.0, -1.0}}};

      VERIFY(DelauneyTriangulation<Fp>::isDelauneyConditionSatisfied(triangles),
             caseLabel);
   }
   {
      const std::string caseLabel = "DelauneyTriangulation::isDelauneyConditionSatisfied "
                                    "for triangles that fulfill the condition";

      using Fp = double;

      const std::vector<Triangle<Fp>> triangles{
         Triangle<Fp>{Point2<Fp>{1.0, 2.0}, Point2<Fp>{6.0, -3.0},
                      Point2<Fp>{7.0, 5.0}},
         Triangle<Fp>{Point2<Fp>{1.0, 2.0}, Point2<Fp>{-5.0, 4.0},
                      Point2<Fp>{-3.0, -2.0}}};

      VERIFY(DelauneyTriangulation<Fp>::isDelauneyConditionSatisfied(triangles),
             caseLabel);
   }
   {
      const std::string caseLabel = "DelauneyTriangulation::isDelauneyConditionSatisfied "
                                    "for triangles that don't fulfill the condition";

      using Fp = double;

      const std::vector<Triangle<Fp>> triangles{
         Triangle<Fp>{Point2<Fp>{1.0, 2.0}, Point2<Fp>{6.0, -3.0},
                      Point2<Fp>{7.0, 5.0}},
         Triangle<Fp>{Point2<Fp>{1.0, 2.0}, Point2<Fp>{7.0, 4.0},
                      Point2<Fp>{5.0, 10.0}}};

      VERIFY(!DelauneyTriangulation<Fp>::isDelauneyConditionSatisfied(triangles),
             caseLabel);
   }
}

} // namespace


void testDelauneyTriangulation()
{
   testTriangulateWithNoPoints();
   testTriangulateWithOnePoint();
   testTriangulateWithTwoPoints();
   testTriangulateWithThreePoints();
   testTriangulateWithFourPointsAsRect();
   testTriangulateWithTenPoints();
   testAccessDelauneyTriangles();
   testIsDelauneyConditionSatisfied();
}
