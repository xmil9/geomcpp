//
// geomcpp tests
// Tests for intersecting 2D lines using run time polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "line_intersection2_rt_tests.h"
#include "line_intersection2_rt.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::rt;
using namespace sutil;


namespace
{
///////////////////

template <typename X, typename T = typename X::value_type>
void verifyIntersection(const std::optional<LineIntersection2<T>>& res, const X& expected,
                        const std::string& caseLabel)
{
   VERIFY(res.has_value(), caseLabel);
   if (res.has_value())
   {
      const auto x = *res;
      VERIFY(std::holds_alternative<X>(x), caseLabel);
      if (std::holds_alternative<X>(x))
         VERIFY(std::get<X>(x) == expected, caseLabel);
   }
}


///////////////////

void testIntersectTwoLineSegments2D()
{
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - both are differnt points";

      Point2 p(1.0, 2.0);
      LineSeg2 a(p, p);
      Point2 q(3.0, 1.0);
      LineSeg2 b(q, q);

      const auto res = intersect(a, b);

      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - both are the same point";

      Point2 p(1.0, 2.0);
      LineSeg2 a(p, p);
      LineSeg2 b(p, p);

      const auto res = intersect(a, b);

      verifyIntersection(res, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - one is a point and point is on other line";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      Point2 p = a.lerpPoint(0.6f);
      LineSeg2 b(p, p);

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
  // {
  //    const std::string caseLabel = "Intersect two 2d line segments - coincident and "
  //                                  "overlapp - both have same direction";

  //    Point2 startA(1.0, 2.0);
		//LineSeg2 a(startA, Point2(3.0, 3.0));
		//Point2 startB = *a.startPoint() + -0.8 * a.direction();
		//Point2 endB = *a.startPoint() + 0.4 * a.direction();
		//LineSeg2 b(startB, endB);
		//
		//auto res = intersect(a, b);
  //    verifyIntersection(res, LineSeg2{startA, endB}, caseLabel);
  // }
}

} // namespace


void testRtLineIntersection2()
{
   testIntersectTwoLineSegments2D();
}
