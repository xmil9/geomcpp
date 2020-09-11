//
// geomcpp tests
// Tests for Delauney triangles.
//
// Sept-2020, Michael Lindner
// MIT license
//
#include "delauney_triangle_tests.h"
#include "delauney_triangle.h"
#include "line_seg2_ct.h"
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
      const std::string caseLabel = "DelauneyTriangle::triangle accessor";

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


void testSubscriptOperator()
{
   {
      const std::string caseLabel = "DelauneyTriangle::operator[]";

      using Fp = float;

      constexpr Point2 a(1.0f, 2.0f);
      constexpr Point2 b(3.0f, 4.0f);
      constexpr Point2 c(7.0f, 1.0f);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      VERIFY(a == dt[0], caseLabel);
      VERIFY(b == dt[1], caseLabel);
      VERIFY(c == dt[2], caseLabel);
   }
}


void testFindVertex()
{
   {
      const std::string caseLabel = "DelauneyTriangle::findVertex for existing vertex";

      using Fp = float;

      constexpr Point2 a(1.0f, 2.0f);
      constexpr Point2 b(3.0f, 4.0f);
      constexpr Point2 c(7.0f, 1.0f);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      auto res = dt.findVertex(a);
      VERIFY(res.has_value() && *res == 0, caseLabel);
      res = dt.findVertex(b);
      VERIFY(res.has_value() && *res == 1, caseLabel);
      res = dt.findVertex(c);
      VERIFY(res.has_value() && *res == 2, caseLabel);
   }
   {
      const std::string caseLabel =
         "DelauneyTriangle::findVertex for not existing vertex";

      using Fp = float;

      constexpr Point2 a(1.0f, 2.0f);
      constexpr Point2 b(3.0f, 4.0f);
      constexpr Point2 c(7.0f, 1.0f);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      auto res = dt.findVertex(Point2{3.4f, 7.7f});
      VERIFY(!res.has_value(), caseLabel);
   }
}


void testEdgeAccessor()
{
   {
      const std::string caseLabel = "DelauneyTriangle::edge accessor";

      using Fp = double;

      constexpr Point2 a(1.0, 2.0);
      constexpr Point2 b(3.0, 4.0);
      constexpr Point2 c(7.0, 1.0);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      VERIFY(dt.edge(0) == ct::LineSeg2(a, b), caseLabel);
      VERIFY(dt.edge(1) == ct::LineSeg2(b, c), caseLabel);
      VERIFY(dt.edge(2) == ct::LineSeg2(c, a), caseLabel);
   }
}


void testIsPointInCircumcircle()
{
   {
      const std::string caseLabel =
         "DelauneyTriangle::isPointInCircumcircle for point in circumcircle";

      using Fp = double;

      constexpr Point2 a(1.0, 2.0);
      constexpr Point2 b(3.0, 4.0);
      constexpr Point2 c(7.0, 1.0);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      VERIFY(dt.isPointInCircumcircle(Point2(6.0, 2.0)), caseLabel);
   }
   {
      const std::string caseLabel =
         "DelauneyTriangle::isPointInCircumcircle for point not in circumcircle";

      using Fp = double;

      constexpr Point2 a(1.0, 2.0);
      constexpr Point2 b(3.0, 4.0);
      constexpr Point2 c(7.0, 1.0);
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);

      VERIFY(!dt.isPointInCircumcircle(Point2(10.0, 2.0)), caseLabel);
   }
}


void testCircumcenter()
{
   {
      const std::string caseLabel = "DelauneyTriangle::circumcenter";

      using Fp = double;

		constexpr Circle circle(Point2(1.0, 2.0), 3.0);
      const Point2 a{circle.pointAtAngle(1.0)};
      const Point2 b{circle.pointAtAngle(1.3)};
      const Point2 c{circle.pointAtAngle(2.1)};
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);
			
		VERIFY(circle.center() == dt.circumcenter(), caseLabel);
   }
}


void testCircumcircleRadius()
{
   {
      const std::string caseLabel = "DelauneyTriangle::circumcircleRadius";

      using Fp = double;

		constexpr Circle circle(Point2(1.0, 2.0), 3.0);
      const Point2 a{circle.pointAtAngle(1.0)};
      const Point2 b{circle.pointAtAngle(1.3)};
      const Point2 c{circle.pointAtAngle(2.1)};
      const Triangle t(a, b, c);
      const DelauneyTriangle dt(t);
		
		VERIFY(equal(dt.circumcircleRadius(), 3.0), caseLabel);
   }
}

} // namespace


void testDelauneyTriangle()
{
   testTriangleAccessor();
   testSubscriptOperator();
   testFindVertex();
   testEdgeAccessor();
   testIsPointInCircumcircle();
   testCircumcenter();
   testCircumcircleRadius();
}
