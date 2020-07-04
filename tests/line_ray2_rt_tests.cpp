//
// geomcpp tests
// Tests for 2D line rays using run time polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "line_ray2_rt_tests.h"
#include "line_ray2_rt.h"
#include "line_seg2_rt.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::rt;
using namespace sutil;


namespace
{
///////////////////

void testLineRay2DefaultCtor()
{
   {
      const std::string caseLabel = "LineRay2 default ctor for float";

      LineRay2<float> l;
      VERIFY((l.anchor() == Point2<float>{}), caseLabel);
      VERIFY((l.direction() == Vec2<float>{}), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2 default ctor for integer";

      LineRay2<long> l;
      VERIFY((l.anchor() == Point2<long>{}), caseLabel);
      VERIFY((l.direction() == Vec2<long>{}), caseLabel);
   }
}


void testLineRay2StartDirectionCtor()
{
   {
      const std::string caseLabel = "LineRay2 start-direction ctor for float";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineRay2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2 start-direction ctor for integer";

      const Point2 anchor{1, 2};
      const Vec2 dir{3, 2};
      const LineRay2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
}


void testLineRay2Type()
{
   {
      const std::string caseLabel = "LineRay2::type";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineRay2 l{anchor, dir};
      VERIFY(l.type() == Line2Type::Ray, caseLabel);
   }
}


void testLineRay2IsPoint()
{
   {
      const std::string caseLabel = "LineRay2::isPoint for line that is not a point";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineRay2 l{anchor, dir};
      VERIFY(!l.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::isPoint for line that is a point";

      const Point2 anchor{1, 3};
      const Vec2 dir{0, 0};
      const LineRay2 l{anchor, dir};
      VERIFY(l.isPoint(), caseLabel);
   }
}


void testLineRay2StartPoint()
{
   {
      const std::string caseLabel = "LineRay2::startPoint";

      const Point2 start{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineRay2 l{start, dir};
      VERIFY(l.startPoint() == start, caseLabel);
   }
}


void testLineRay2EndPoint()
{
   {
      const std::string caseLabel = "LineRay2::endPoint";

      const Point2 start{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineRay2 l{start, dir};

      VERIFY(!l.endPoint(), caseLabel);
   }
}


void testLineRay2IsPointOnLine()
{
   {
      const std::string caseLabel = "LineRay2::isPointOnLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::isPointOnLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::isPointOnLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::isPointOnLine for point that is on "
                                    "infinite extension past the end point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::isPointOnLine for start point";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{start, dir};

      const auto pos = l.isPointOnLine(start);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.0), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::isPointOnLine for end point";

      const Point2 start{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{start, dir};

      const auto pos = l.isPointOnLine(start + dir);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.0), caseLabel);
   }
}


void testLineRay2IsPointOnInfiniteLine()
{
   {
      const std::string caseLabel =
         "LineRay2::isPointOnInfiniteLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::isPointOnInfiniteLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::isPointOnInfiniteLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::isPointOnInfiniteLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLineRay2CalcLerpFactor()
{
   {
      const std::string caseLabel =
         "LineRay2::calcLerpFactor for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 0.3 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.3), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::calcLerpFactor for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = 4.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 4.5), caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::calcLerpFactor for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const Vec2 v = -1.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.calcLerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -1.2), caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::calcLerpFactor for point not on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pos = l.calcLerpFactor(Point2{1.0, 1.0});

      VERIFY(!pos.has_value(), caseLabel);
   }
}


void testLineRay2LerpPoint()
{
   {
      const std::string caseLabel =
         "LineRay2::lerpPoint for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pt = l.lerpPoint(0.3);

      Point2 expected = anchor + 0.3 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::lerpPoint for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pt = l.lerpPoint(4.5);

      Point2 expected = anchor + 4.5 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "LineRay2::lerpPoint for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pt = l.lerpPoint(-1.2);

      Point2 expected = anchor + -1.2 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::lerpPoint with zero";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pt = l.lerpPoint(0.0);

      VERIFY(pt == anchor, caseLabel);
   }
   {
      const std::string caseLabel = "LineRay2::lerpPoint with integer value";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineRay2 l{anchor, dir};

      const auto pt = l.lerpPoint(3);

      Point2 expected = anchor + 3.0 * dir;
      VERIFY(pt == expected, caseLabel);
   }
}


void testLineRay2Parallel()
{
   {
      const std::string caseLabel = "parallel() for parallel line rays";

      const Vec2 dir{2.0, 1.0};
      const LineRay2 a{Point2{3.0, 4.0}, dir};
      const LineRay2 b{Point2{2.0, 1.0}, dir};
      
      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for non-parallel line rays";

      const LineRay2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineRay2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};
      
      VERIFY(!parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for mixed line ray types";

      const LineRay2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineRay2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};
      
      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for mixed line classes";

      const LineRay2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};
      
      VERIFY(parallel(a, b), caseLabel);
   }
}


void testLineRay2Coincident()
{
   {
      const std::string caseLabel = "coincident() for coincident line rays";

      const Vec2 dir{2.0, 1.0};
      const LineRay2 a{Point2{3.0, 4.0}, dir};
      const LineRay2 b{Point2{5.0, 5.0}, dir};
      
      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for non-coincident line rays";

      const LineRay2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineRay2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};
      
      VERIFY(!coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for mixed line ray types";

      const LineRay2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineRay2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};
      
      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for mixed line classes";

      const LineRay2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineSeg2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};
      
      VERIFY(coincident(a, b), caseLabel);
   }
}

} // namespace


void testRtLineRay2()
{
   testLineRay2DefaultCtor();
   testLineRay2StartDirectionCtor();
   testLineRay2IsPoint();
   testLineRay2StartPoint();
   testLineRay2EndPoint();
   testLineRay2IsPointOnLine();
   testLineRay2IsPointOnInfiniteLine();
   testLineRay2CalcLerpFactor();
   testLineRay2LerpPoint();
   testLineRay2Parallel();
   testLineRay2Coincident();
}
