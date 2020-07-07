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
   {
      const std::string caseLabel = "Intersect two 2d line segments - coincident and "
                                    "overlapping with both in same direction";

      Point2 startA(1.0, 2.0);
      LineSeg2 a(startA, Point2(3.0, 3.0));
      Point2 startB = *a.startPoint() + -0.8 * a.direction();
      Point2 endB = *a.startPoint() + 0.4 * a.direction();
      LineSeg2 b(startB, endB);

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{startA, endB}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line segments - coincident and "
                                    "overlapping with opposite directions";

      Point2 startA(1.0f, 2.0f);
      LineSeg2 a(startA, Point2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8f * a.direction();
      Point2 endB = *a.startPoint() + -2.4f * a.direction();
      LineSeg2 b(startB, endB);

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{startA, startB}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - coincident but disjoint";

      Point2 startA(1.0f, 2.0f);
      LineSeg2 a(startA, Point2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + -3.8f * a.direction();
      Point2 endB = *a.startPoint() + -0.5f * a.direction();
      LineSeg2 b(startB, endB);

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line segments - same line";

      Point2 start(1, 2);
      Point2 end(3, 3);
      LineSeg2 a(start, end);
      LineSeg2 b(start, end);

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{start, end}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - parallel but not coincident";

      Point2 startA(1.0, 2.0);
      LineSeg2 a(startA, Vec2(3.0, 3.0));
      LineSeg2 b(Point2(4.0, 2.0), a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line segments - intersecting";

      Point2 x(3.0, 4.0);
      // Build the lines so that they intersect at the intersection point.
      Vec2 dirA(1.0, 2.0);
      Point2 startA = x + -dirA;
      Point2 endA = x + 0.7 * dirA;
      LineSeg2 a(startA, endA);
      Vec2 dirB(5.0, 3.0);
      Point2 startB = x + -1.2 * dirB;
      Point2 endB = x + 1.7 * dirB;
      LineSeg2 b(startB, endB);

      auto res = intersect(a, b);
      verifyIntersection(res, x, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line segments - intersecting at origin";

      LineSeg2 a(Point2(-1.0f, 0.0f), Point2(3.0f, 0.0f));
      LineSeg2 b(Point2(0.0f, 2.0f), Point2(0.0f, -2.0f));

      auto res = intersect(a, b);
      verifyIntersection(res, Point2{0.0f, 0.0f}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line segments - not intersecting";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      LineSeg2 b(Point2(0.0f, 2.0f), Point2(0.0f, -2.0f));

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
}


void testIntersectLineSegmentAndRay2D()
{
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - both are differnt points";

      Point2 p(1.0, 2.0);
      LineSeg2 a(p, p);
      Point2 q(3.0, 1.0);
      LineRay2 b(q, Vec2{0.0, 0.0});

      const auto res = intersect(a, b);

      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - both are the same point";

      Point2 p(1.0, 2.0);
      LineSeg2 a(p, p);
      LineRay2 b(p, Vec2{0.0, 0.0});

      const auto res = intersect(a, b);

      verifyIntersection(res, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - ray is a point and is on segment";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      Point2 p = a.lerpPoint(0.6f);
      LineRay2 b(p, Vec2{0.0f, 0.0f});

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - segment is a point and is on ray";

      LineRay2 b(Point2(1.0, 2.0), Vec2(2.0, 3.0));
      Point2 p = b.lerpPoint(0.6);
      LineSeg2 a(p, p);

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - segment is a point and is not on ray";

      LineRay2 b(Point2(1.0, 2.0), Vec2(2.0, 3.0));
      Point2 p(0.0, 0.0);
      LineSeg2 a(p, p);

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and ray - coincident and "
                                    "overlapping (same direction)";

      Point2 startA(1.0L, 2.0L);
      LineSeg2 a(startA, Point2(3.0L, 3.0L));
      Point2 startB = *a.startPoint() + 0.8L * a.direction();
      LineRay2 b(startB, 2.5L * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{startB, *a.endPoint()}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and ray - coincident and "
                                    "overlapping (opposite direction)";

      Point2 startA(1.0f, 2.0f);
      LineSeg2 a(startA, Point2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8 * a.direction();
      LineRay2 b(startB, -2.5 * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{startA, startB}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - coincident but disjoint";

      Point2 startA(1.0, 2.0);
      LineSeg2 a(startA, Point2(3.0, 3.0));
      Point2 startB = *a.startPoint() + 1.8 * a.direction();
      LineRay2 b(startB, 2.5 * a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - same start points";

      Point2 start(1, 2);
      Point2 end(3, 3);
      LineSeg2 a(start, end);
      LineRay2 b(start, a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - parallel but not coincident";

      Point2 startA(1.0, 2.0);
      LineSeg2 a(startA, Point2(3.0, 3.0));
      LineRay2 b(Point2(4.0, 2.0), a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and ray - intersecting";

      Point2 x(3.0, 4.0);
      // Build the lines so that they intersect at the intersection point.
      Vec2 dirA(1.0, 2.0);
      Point2 startA = x + -dirA;
      Point2 endA = x + 0.7 * dirA;
      LineSeg2 a(startA, endA);
      Vec2 dirB(5.0, 3.0);
      Point2 startB = x + -1.2 * dirB;
      Point2 ptOnB = x + 1.7 * dirB;
      LineRay2 b(startB, Vec2<double>{startB, ptOnB});

      auto res = intersect(a, b);
      verifyIntersection(res, x, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - intersecting at origin";

      LineSeg2 a(Point2(-1.0f, 0.0f), Point2(3.0f, 0.0f));
      LineRay2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      verifyIntersection(res, Point2{0.0f, 0.0f}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and ray - not intersecting";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      LineRay2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
}


void testIntersectLineSegmentAndInfiniteLine2D()
{
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - both are differnt points";

      Point2 p(1, 2);
      LineSeg2 a(p, p);
      Point2 q(2, 1);
      Line2 b(q, Vec2{0, 0});

      const auto res = intersect(a, b);

      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - both are the same point";

      Point2 p(1.0, 2.0);
      LineSeg2 a(p, p);
      Line2 b(p, Vec2{0.0, 0.0});

      const auto res = intersect(a, b);

      verifyIntersection(res, p, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and infinite line - line "
                                    "is a point and is on segment";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      Point2 p = a.lerpPoint(0.6f);
      Line2 b(p, Vec2{0.0f, 0.0f});

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and infinite line - "
                                    "segment is a point and is on line";

      Line2 b(Point2(1.0, 2.0), Vec2(2.0, 3.0));
      Point2 p = b.lerpPoint(0.6);
      LineSeg2 a(p, p);

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line segment and infinite line - "
                                    "segment is a point and is not on line";

      Line2 b(Point2(1.0, 2.0), Vec2(2.0, 3.0));
      Point2 p(0.0, 0.0);
      LineSeg2 a(p, p);

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - coincident and "
         "in same direction";

      Point2 startA(1.0L, 2.0L);
      LineSeg2 a(startA, Point2(3.0L, 3.0L));
      Point2 startB = *a.startPoint() + 0.8L * a.direction();
      Line2 b(startB, 2.5L * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - coincident and "
         "in opposite directions";

      Point2 startA(1.0f, 2.0f);
      LineSeg2 a(startA, Point2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8 * a.direction();
      Line2 b(startB, -2.5 * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - parallel but not coincident";

      Point2 startA(1.0, 2.0);
      LineSeg2 a(startA, Point2(3.0, 3.0));
      Line2 b(Point2(4.0, 2.0), a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - intersecting";

      Point2 x(3.0, 4.0);
      // Build the lines so that they intersect at the intersection point.
      Vec2 dirA(1.0, 2.0);
      Point2 startA = x + -dirA;
      Point2 endA = x + 0.7 * dirA;
      LineSeg2 a(startA, endA);
      Vec2 dirB(5.0, 3.0);
      Point2 startB = x + -1.2 * dirB;
      Point2 ptOnB = x + 1.7 * dirB;
      Line2 b(startB, Vec2<double>{startB, ptOnB});

      auto res = intersect(a, b);
      verifyIntersection(res, x, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - intersecting at origin";

      LineSeg2 a(Point2(-1.0f, 0.0f), Point2(3.0f, 0.0f));
      Line2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      verifyIntersection(res, Point2{0.0f, 0.0f}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line segment and infinite line - not intersecting";

      LineSeg2 a(Point2(1.0f, 2.0f), Point2(3.0f, 3.0f));
      Line2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
}


void testIntersectTwoLineRays2D()
{
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - both are differnt points";

      Point2 p(1.0, 2.0);
      LineRay2 a(p, Vec2<double>{});
      Point2 q(2.0, 1.0);
      LineRay2 b(q, Vec2<double>{});

      const auto res = intersect(a, b);
      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - both are the same point";

      Point2 p(1.0, 2.0);
      LineRay2 a(p, Vec2<double>{});
      LineRay2 b(p, Vec2<double>{});

      const auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - one is a point and point is on other ray";

      LineRay2 a(Point2(1.0f, 2.0f), Vec2(3.0f, 3.0f));
      Point2 p = a.lerpPoint(0.6f);
      LineRay2 b(p, Vec2<float>{});

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - one is a point and point is not on other ray";

      LineRay2 a(Point2(1.0f, 2.0f), Vec2(2.0f, 3.0f));
      Point2 p{0.0f, 0.0f};
      LineRay2 b(p, Vec2<float>{});

      auto res = intersect(a, b);
      VERIFY(!res.has_value(), caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line rays - coincident and "
                                    "overlapping in same direction";

      Point2 startA(1.0f, 2.0f);
      LineRay2 a(startA, Vec2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8f * a.direction();
      Vec2 dirB = 2.5f * a.direction();
      LineRay2 b(startB, dirB);

      auto res = intersect(a, b);
      verifyIntersection(res, LineRay2{startB, a.direction()}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line rays - coincident and "
                                    "overlapping with opposite directions";

      Point2 startA(1.0f, 2.0f);
      LineRay2 a(startA, Vec2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8f * a.direction();
      Vec2 dirB = -2.5f * a.direction();
      LineRay2 b(startB, dirB);

      auto res = intersect(a, b);
      verifyIntersection(res, LineSeg2{startA, startB}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line rays - coincident and "
                                    "overlapping at start points";

      Point2 start(1.0f, 2.0f);
      LineRay2 a(start, Vec2(3.0f, 3.0f));
      LineRay2 b(start, -2.5f * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, start, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - coincident but disjoint";

      Point2 startA(1.0f, 2.0f);
      LineRay2 a(startA, Vec2(3.0f, 3.0f));
      Point2 startB = -*a.startPoint() + -3.8f * a.direction();
      LineRay2 b(startB, -2.5f * a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - parallel (not coincident)";

      LineRay2 a(Point2(1, 2), Vec2(3, 3));
      LineRay2 b(Point2(2, 4), a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line rays - intersecting";

      Point2 x(3.0, 4.0);
      // Build the lines so that they intersect at the intersection point.
      Vec2 dirA(1.0, 2.0);
      Point2 startA = x + -dirA;
      LineRay2 a(startA, dirA);
      Vec2 dirB(5.0, 3.0);
      Point2 startB = x + -1.2 * dirB;
      LineRay2 b(startB, dirB);

      auto res = intersect(a, b);
      verifyIntersection(res, x, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect two 2d line rays - intersecting at origin";

      LineRay2 a(Point2(-1.0f, 0.0f), Vec2(3.0f, 0.0f));
      LineRay2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      verifyIntersection(res, Point2{0.0f, 0.0f}, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect two 2d line rays - not intersecting";

      LineRay2 a(Point2(1.0f, 2.0f), Vec2(3.0f, 3.0f));
      LineRay2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
}


void testIntersectLineRayAndInfiniteLine2D()
{
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - both are differnt points";

      Point2 p(1, 2);
      LineRay2 a(p, Vec2{0, 0});
      Point2 q(2, 1);
      Line2 b(q, Vec2{0, 0});

      const auto res = intersect(a, b);

      VERIFY(!res.has_value(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - both are the same point";

      Point2 p(1.0, 2.0);
      LineRay2 a(p, Vec2{0.0, 0.0});
      Line2 b(p, Vec2{0.0, 0.0});

      const auto res = intersect(a, b);

      verifyIntersection(res, p, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line ray and infinite line - one "
                                    "is a point and is on other line";

      LineRay2 a(Point2(1.0f, 2.0f), Vec2(3.0f, 3.0f));
      Point2 p = a.lerpPoint(0.6f);
      Line2 b(p, Vec2{0.0f, 0.0f});

      auto res = intersect(a, b);
      verifyIntersection(res, p, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      verifyIntersection(reversedRes, p, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect 2d line ray and infinite line - "
                                    "one is a point and is not on other line";

      Line2 b(Point2(1.0, 2.0), Vec2(2.0, 3.0));
      Point2 p(0.0, 0.0);
      LineRay2 a(p, Vec2{0.0, 0.0});

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);

      // Reverse order.
      auto reversedRes = intersect(b, a);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - coincident and "
         "in same direction";

      Point2 startA(1.0L, 2.0L);
      LineRay2 a(startA, Vec2(3.0L, 3.0L));
      Point2 startB = *a.startPoint() + 0.8L * a.direction();
      Line2 b(startB, 2.5L * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - coincident and "
         "in opposite directions";

      Point2 startA(1.0f, 2.0f);
      LineRay2 a(startA, Vec2(3.0f, 3.0f));
      Point2 startB = *a.startPoint() + 0.8 * a.direction();
      Line2 b(startB, -2.5 * a.direction());

      auto res = intersect(a, b);
      verifyIntersection(res, a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - parallel but not coincident";

      Point2 startA(1.0, 2.0);
      LineRay2 a(startA, Vec2(3.0, 3.0));
      Line2 b(Point2(4.0, 2.0), a.direction());

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - intersecting";

      Point2 x(3.0, 4.0);
      // Build the lines so that they intersect at the intersection point.
      Vec2 dirA(1.0, 2.0);
      Point2 startA = x + -dirA;
      LineRay2 a(startA, dirA);
      Vec2 dirB(5.0, 3.0);
      Point2 startB = x + -1.2 * dirB;
      Line2 b(startB, dirB);

      auto res = intersect(a, b);
      verifyIntersection(res, x, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - intersecting at origin";

      LineRay2 a(Point2(-1.0f, 0.0f), Vec2(3.0f, 0.0f));
      Line2 b(Point2(0.0f, 2.0f), Vec2(0.0f, -2.0f));

      auto res = intersect(a, b);
      verifyIntersection(res, Point2{0.0f, 0.0f}, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect 2d line ray and infinite line - not intersecting";

      LineRay2 a(Point2(1.0f, 2.0f), Vec2(3.0f, 3.0f));
      Line2 b(Point2(0.0f, 2.0f), Vec2(1.0f, -2.0f));

      auto res = intersect(a, b);
      VERIFY(!res, caseLabel);
   }
}

} // namespace


void testRtLineIntersection2()
{
   testIntersectTwoLineSegments2D();
   testIntersectLineSegmentAndRay2D();
   testIntersectLineSegmentAndInfiniteLine2D();
   testIntersectTwoLineRays2D();
   testIntersectLineRayAndInfiniteLine2D();
}
