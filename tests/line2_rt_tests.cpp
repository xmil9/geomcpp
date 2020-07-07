//
// geomcpp tests
// Tests for 2D lines using run time polymorphism.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "line2_rt_tests.h"
#include "line2_rt.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::rt;
using namespace sutil;


namespace
{
///////////////////

void testLine2DefaultCtor()
{
   {
      const std::string caseLabel = "Line2 default ctor for float";

      Line2<float> l;
      VERIFY((l.anchor() == Point2<float>{}), caseLabel);
      VERIFY((l.direction() == Vec2<float>{}), caseLabel);
   }
   {
      const std::string caseLabel = "Line2 default ctor for integer";

      Line2<long> l;
      VERIFY((l.anchor() == Point2<long>{}), caseLabel);
      VERIFY((l.direction() == Vec2<long>{}), caseLabel);
   }
}


void testLine2ValueCtor()
{
   {
      const std::string caseLabel = "Line2 value ctor for float";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "Line2 value ctor for integer";

      const Point2 anchor{1, 2};
      const Vec2 dir{3, 2};
      const Line2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
}


void testLine2Type()
{
   {
      const std::string caseLabel = "Line2::type";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(l.type() == Line2Type::Infinite, caseLabel);
   }
}


void testLine2IsPoint()
{
   {
      const std::string caseLabel = "Line2::isPoint for line that is not a point";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(!l.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "Line2::isPoint for line that is a point";

      const Point2 anchor{1, 3};
      const Vec2 dir{0, 0};
      const Line2 l{anchor, dir};
      VERIFY(l.isPoint(), caseLabel);
   }
}


void testLine2StartPoint()
{
   {
      const std::string caseLabel = "Line2::startPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(!l.startPoint(), caseLabel);
   }
}


void testLine2EndPoint()
{
   {
      const std::string caseLabel = "Line2::endPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(!l.endPoint(), caseLabel);
   }
}


void testLine2IsPointOnLine()
{
   {
      const std::string caseLabel = "Line2::isPointOnLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel = "Line2::isPointOnLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel = "Line2::isPointOnLine for point that is on infinite "
                                    "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::isPointOnLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLine2IsPointOnInfiniteLine()
{
   {
      const std::string caseLabel =
         "Line2::isPointOnInfiniteLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::isPointOnInfiniteLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::isPointOnInfiniteLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::isPointOnInfiniteLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLine2LerpFactor()
{
   {
      const std::string caseLabel =
         "Line2::lerpFactor for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 0.3 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.3), caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::lerpFactor for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = 4.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 4.5), caseLabel);
   }
   {
      const std::string caseLabel = "Line2::lerpFactor for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const Vec2 v = -1.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -1.2), caseLabel);
   }
   {
      const std::string caseLabel = "Line2::lerpFactor for point not on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pos = l.lerpFactor(Point2{1.0, 1.0});

      VERIFY(!pos.has_value(), caseLabel);
   }
}


void testLine2Lerp()
{
   {
      const std::string caseLabel =
         "Line2::lerp for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pt = l.lerp(0.3);

      Point2 expected = anchor + 0.3 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "Line2::lerp for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pt = l.lerp(4.5);

      Point2 expected = anchor + 4.5 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Line2::lerp for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pt = l.lerp(-1.2);

      Point2 expected = anchor + -1.2 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Line2::lerp with zero";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pt = l.lerp(0.0);

      VERIFY(pt == anchor, caseLabel);
   }
   {
      const std::string caseLabel = "Line2::lerp with integer value";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const Line2 l{anchor, dir};

      const auto pt = l.lerp(3);

      Point2 expected = anchor + 3.0 * dir;
      VERIFY(pt == expected, caseLabel);
   }
}


void testLine2Parallel()
{
   {
      const std::string caseLabel = "parallel() for parallel lines";

      const Vec2 dir{2.0, 1.0};
      const Line2 a{Point2{3.0, 4.0}, dir};
      const Line2 b{Point2{2.0, 1.0}, dir};

      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for non-parallel lines";

      const Line2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const Line2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "parallel() for mixed line types";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(parallel(a, b), caseLabel);
   }
}


void testLine2Coincident()
{
   {
      const std::string caseLabel = "coincident() for coincident lines";

      const Vec2 dir{2.0, 1.0};
      const Line2 a{Point2{3.0, 4.0}, dir};
      const Line2 b{Point2{5.0, 5.0}, dir};

      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for non-coincident lines";

      const Line2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const Line2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "coincident() for mixed line types";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(coincident(a, b), caseLabel);
   }
}


void testLine2Equality()
{
   {
      const std::string caseLabel = "Equality for equal infinite lines";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for infinite lines with different anchors";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for infinite lines with different directions";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for infinite lines with different value types but same data values";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(a == b, caseLabel);
   }
}


void testLine2Inequality()
{
   {
      const std::string caseLabel = "Inequality for equal infinite lines";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for infinite lines with different anchors";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for infinite lines with different directions";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for infinite lines with different value types but same data values";

      const Line2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const Line2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(!(a != b), caseLabel);
   }
}

} // namespace


void testRtLine2()
{
   testLine2DefaultCtor();
   testLine2ValueCtor();
   testLine2Type();
   testLine2IsPoint();
   testLine2StartPoint();
   testLine2EndPoint();
   testLine2IsPointOnLine();
   testLine2IsPointOnInfiniteLine();
   testLine2LerpFactor();
   testLine2Lerp();
   testLine2Parallel();
   testLine2Coincident();
   testLine2Equality();
   testLine2Inequality();
}
