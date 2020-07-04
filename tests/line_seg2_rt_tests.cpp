//
// geomcpp tests
// Tests for 2D line segments using run time polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "line_seg2_rt_tests.h"
#include "line_seg2_rt.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::rt;
using namespace sutil;


namespace
{
///////////////////

void testLineSeg2DefaultCtor()
{
   {
      const std::string caseLabel = "LineSeg2 default ctor for float";

      LineSeg2<float> l;
      VERIFY((l.anchor() == Point2<float>{}), caseLabel);
      VERIFY((l.direction() == Vec2<float>{}), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2 default ctor for integer";

      LineSeg2<long> l;
      VERIFY((l.anchor() == Point2<long>{}), caseLabel);
      VERIFY((l.direction() == Vec2<long>{}), caseLabel);
   }
}


void testLineSeg2StartEndCtor()
{
   {
      const std::string caseLabel = "LineSeg2 start-end point ctor for float";

      const Point2 start{1.0f, 2.0f};
      const Point2 end{0.0f, 7.0f};
      const LineSeg2 l{start, end};
      VERIFY((l.startPoint() == start), caseLabel);
      VERIFY((l.endPoint() == end), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2 start-end point ctor for integer";

      const Point2 start{1, 2};
      const Point2 end{0, 7};
      const LineSeg2 l{start, end};
      VERIFY((l.startPoint() == start), caseLabel);
      VERIFY((l.endPoint() == end), caseLabel);
   }
}


void testLineSeg2StartDirectionCtor()
{
   {
      const std::string caseLabel = "LineSeg2 start-direction ctor for float";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineSeg2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2 start-direction ctor for integer";

      const Point2 anchor{1, 2};
      const Vec2 dir{3, 2};
      const LineSeg2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
}


void testLineSeg2Type()
{
   {
      const std::string caseLabel = "LineSeg2::type";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineSeg2 l{anchor, dir};
      VERIFY(l.type() == Line2Type::Segment, caseLabel);
   }
}


void testLineSeg2IsPoint()
{
   {
      const std::string caseLabel = "LineSeg2::isPoint for line that is not a point";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineSeg2 l{anchor, dir};
      VERIFY(!l.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::isPoint for line that is a point";

      const Point2 anchor{1, 3};
      const Vec2 dir{0, 0};
      const LineSeg2 l{anchor, dir};
      VERIFY(l.isPoint(), caseLabel);
   }
}


void testLineSeg2StartPoint()
{
   {
      const std::string caseLabel = "LineSeg2::startPoint";

      const Point2 start{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineSeg2 l{start, dir};
      VERIFY(l.startPoint() == start, caseLabel);
   }
}


void testLineSeg2EndPoint()
{
   {
      const std::string caseLabel = "LineSeg2::endPoint";

      const Point2 start{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineSeg2 l{start, dir};
      VERIFY(l.endPoint() == start + dir, caseLabel);
   }
}


void testLineSeg2IsPointOnLine()
{
   {
      const std::string caseLabel = "LineSeg2::isPointOnLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::isPointOnLine for point that is on "
                                    "infinite extension past the end point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::isPointOnLine for start point";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{start, dir};

      const auto pos = l.isPointOnLine(start);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.0), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::isPointOnLine for end point";

      const Point2 start{3.0, 4.0};
      const Point2 end{5.6, 99.3};
      const LineSeg2 l{start, end};

      const auto pos = l.isPointOnLine(end);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.0), caseLabel);
   }
}


void testLineSeg2IsPointOnInfiniteLine()
{
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnInfiniteLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnInfiniteLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnInfiniteLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::isPointOnInfiniteLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLineSeg2CalcLerpFactor()
{
   {
      const std::string caseLabel =
         "LineSeg2::calcLerpFactor for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 0.3 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.3), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::calcLerpFactor for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = 4.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 4.5), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::calcLerpFactor for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const Vec2 v = -1.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -1.2), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::calcLerpFactor for point not on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pos = l.calcLerpFactor(Point2{1.0, 1.0});

      VERIFY(!pos.has_value(), caseLabel);
   }
}


void testLineSeg2LerpPoint()
{
   {
      const std::string caseLabel =
         "LineSeg2::lerpPoint for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pt = l.lerpPoint(0.3);

      Point2 expected = anchor + 0.3 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::lerpPoint for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pt = l.lerpPoint(4.5);

      Point2 expected = anchor + 4.5 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineSeg2::lerpPoint for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pt = l.lerpPoint(-1.2);

      Point2 expected = anchor + -1.2 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::lerpPoint with zero";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pt = l.lerpPoint(0.0);

      VERIFY(pt == anchor, caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::lerpPoint with integer value";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineSeg2 l{anchor, dir};

      const auto pt = l.lerpPoint(3);

      Point2 expected = anchor + 3.0 * dir;
      VERIFY(pt == expected, caseLabel);
   }
}


void testLineSeg2MidPoint()
{
   {
      const std::string caseLabel = "LineSeg2::midPoint";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{6.0, 2.0};
      const LineSeg2 l{start, dir};

      const auto pt = l.midPoint();
      VERIFY((pt == Point2{6.0, 5.0}), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::midPoint when line is point";

      const Point2 start{3.0, 4.0};
      const LineSeg2 l{start, Vec2<double>{}};

      const auto pt = l.midPoint();
      VERIFY((pt == start), caseLabel);
   }
}


void testLineSeg2LengthSquared()
{
   {
      const std::string caseLabel = "LineSeg2::lengthSquared";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{-1.0, 3.0};
      const LineSeg2 l{start, dir};

      VERIFY(equal(l.lengthSquared(), 10.0), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::lengthSquared for line that is a point";

      const Point2 start{3.0, 4.0};
      const LineSeg2 l{start, Vec2<double>{}};

      VERIFY(equal(l.lengthSquared(), 0.0), caseLabel);
   }
}


void testLineSeg2Length()
{
   {
      const std::string caseLabel = "LineSeg2::length";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{-1.0, 3.0};
      const LineSeg2 l{start, dir};

      VERIFY(equal(l.length(), std::sqrt(10.0)), caseLabel);
   }
   {
      const std::string caseLabel = "LineSeg2::length for line that is a point";

      const Point2 start{3.0, 4.0};
      const LineSeg2 l{start, Vec2<double>{}};

      VERIFY(equal(l.length(), 0.0), caseLabel);
   }
}


void testLineSeg2Parallel()
{
   {
      const std::string caseLabel = "parallel() for parallel line segments";

      const Vec2 dir{2.0, 1.0};
      const LineSeg2 a{Point2{3.0, 4.0}, dir};
      const LineSeg2 b{Point2{2.0, 1.0}, dir};

      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for non-parallel line segments";

      const LineSeg2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineSeg2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for mixed line segment types";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for mixed line classes";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(parallel(a, b), caseLabel);
   }
}


void testLineSeg2Coincident()
{
   {
      const std::string caseLabel = "coincident() for coincident line segments";

      const Vec2 dir{2.0, 1.0};
      const LineSeg2 a{Point2{3.0, 4.0}, dir};
      const LineSeg2 b{Point2{5.0, 5.0}, dir};

      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for non-coincident line segments";

      const LineSeg2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineSeg2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for mixed line segment types";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for mixed line classes";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(coincident(a, b), caseLabel);
   }
}


void testLineSeg2Equality()
{
   {
      const std::string caseLabel = "Equality for equal line segments";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for line segments with different anchors";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for line segments with different directions";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for line segments with different value types but same data values";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality between line segment and infinite line";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(!(a == b), caseLabel);
   }
}


void testLineSeg2Inequality()
{
   {
      const std::string caseLabel = "Inequality for equal line segments";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for line segments with different anchors";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for line segments with different directions";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for line segments with different value types but same data values";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality between line segment and infinite line";

      const LineSeg2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(a != b, caseLabel);
   }
}

} // namespace


void testRtLineSeg2()
{
   testLineSeg2DefaultCtor();
   testLineSeg2StartEndCtor();
   testLineSeg2StartDirectionCtor();
   testLineSeg2Type();
   testLineSeg2IsPoint();
   testLineSeg2StartPoint();
   testLineSeg2EndPoint();
   testLineSeg2IsPointOnLine();
   testLineSeg2IsPointOnInfiniteLine();
   testLineSeg2CalcLerpFactor();
   testLineSeg2LerpPoint();
   testLineSeg2MidPoint();
   testLineSeg2LengthSquared();
   testLineSeg2Length();
   testLineSeg2Parallel();
   testLineSeg2Coincident();
   testLineSeg2Equality();
   testLineSeg2Inequality();
}
