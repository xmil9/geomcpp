//
// geomcpp tests
// Tests for 2D lines using compile time polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "line_inf2_ct_tests.h"
#include "line_inf2_ct.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::ct;
using namespace sutil;


namespace
{
///////////////////

void testLineInf2DefaultCtor()
{
   {
      const std::string caseLabel = "[ct] LineInf2 default ctor for float";

      LineInf2<float> l;
      VERIFY((l.anchor() == Point2<float>{}), caseLabel);
      VERIFY((l.direction() == Vec2<float>{}), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2 default ctor for integer";

      LineInf2<long> l;
      VERIFY((l.anchor() == Point2<long>{}), caseLabel);
      VERIFY((l.direction() == Vec2<long>{}), caseLabel);
   }
}


void testLineInf2ValueCtor()
{
   {
      const std::string caseLabel = "[ct] LineInf2 value ctor for float";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2 value ctor for integer";

      const Point2 anchor{1, 2};
      const Vec2 dir{3, 2};
      const LineInf2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2 value ctor for constexpr definitions";

      constexpr Point2 anchor{1, 2};
      constexpr Vec2 dir{3, 2};
      // Needs to compile.
      constexpr LineInf2 l{anchor, dir};
   }
}


void testLineInf2Type()
{
   {
      const std::string caseLabel = "[ct] LineInf2::type";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY(l.type() == Line2Type::Infinite, caseLabel);
   }
}


void testLineInf2IsPoint()
{
   {
      const std::string caseLabel = "[ct] LineInf2::isPoint for line that is not a point";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY(!l.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2::isPoint for line that is a point";

      const Point2 anchor{1, 3};
      const Vec2 dir{0, 0};
      const LineInf2 l{anchor, dir};
      VERIFY(l.isPoint(), caseLabel);
   }
}


void testLineInf2StartPoint()
{
   {
      const std::string caseLabel = "[ct] LineInf2::startPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY(!l.startPoint(), caseLabel);
   }
}


void testLineInf2EndPoint()
{
   {
      const std::string caseLabel = "[ct] LineInf2::endPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY(!l.endPoint(), caseLabel);
   }
}


void testLineInf2IsPointOnLine()
{
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLineInf2IsPointOnInfiniteLine()
{
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnInfiniteLine for point that is on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 0.7 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.7), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnInfiniteLine for point that is next to line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Point2 pt{7.0, 1.0};
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(!pos, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnInfiniteLine for point that is on infinite "
         "line extension before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = -0.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -0.2), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::isPointOnInfiniteLine for point that is on infinite ray";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 1.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.isPointOnInfiniteLine(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 1.5), caseLabel);
   }
}


void testLineInf2LerpFactor()
{
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerpFactor for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 0.3 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 0.3), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerpFactor for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = 4.5 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, 4.5), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerpFactor for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const Vec2 v = -1.2 * dir;
      const Point2 pt = anchor + v;
      const auto pos = l.lerpFactor(pt);

      VERIFY(pos.has_value(), caseLabel);
      if (pos)
         VERIFY(fpEqual(*pos, -1.2), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2::lerpFactor for point not on line";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pos = l.lerpFactor(Point2{1.0, 1.0});

      VERIFY(!pos.has_value(), caseLabel);
   }
}


void testLineInf2Lerp()
{
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerp for point within length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pt = l.lerp(0.3);

      Point2 expected = anchor + 0.3 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerp for point past the length of directional vector";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pt = l.lerp(4.5);

      Point2 expected = anchor + 4.5 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] LineInf2::lerp for point before the anchor point";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pt = l.lerp(-1.2);

      Point2 expected = anchor + -1.2 * dir;
      VERIFY(pt == expected, caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2::lerp with zero";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pt = l.lerp(0.0);

      VERIFY(pt == anchor, caseLabel);
   }
   {
      const std::string caseLabel = "[ct] LineInf2::lerp with integer value";

      const Point2 anchor{3.0, 4.0};
      const Vec2 dir{2.0, 1.0};
      const LineInf2 l{anchor, dir};

      const auto pt = l.lerp(3);

      Point2 expected = anchor + 3.0 * dir;
      VERIFY(pt == expected, caseLabel);
   }
}


void testLineInf2Parallel()
{
   {
      const std::string caseLabel = "[ct] parallel() for parallel lines";

      const Vec2 dir{2.0, 1.0};
      const LineInf2 a{Point2{3.0, 4.0}, dir};
      const LineInf2 b{Point2{2.0, 1.0}, dir};

      VERIFY(parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] parallel() for non-parallel lines";

      const LineInf2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineInf2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!parallel(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] parallel() for mixed line types";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<float> b{Point2{2.0f, 1.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(parallel(a, b), caseLabel);
   }
}


void testLineInf2Coincident()
{
   {
      const std::string caseLabel = "[ct] coincident() for coincident lines";

      const Vec2 dir{2.0, 1.0};
      const LineInf2 a{Point2{3.0, 4.0}, dir};
      const LineInf2 b{Point2{5.0, 5.0}, dir};

      VERIFY(coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] coincident() for non-coincident lines";

      const LineInf2 a{Point2{3.0, 4.0}, Vec2{2.0, 1.0}};
      const LineInf2 b{Point2{2.0, 1.0}, Vec2{1.0, 3.0}};

      VERIFY(!coincident(a, b), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] coincident() for mixed line types";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<float> b{Point2{5.0f, 5.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(coincident(a, b), caseLabel);
   }
}


void testLineInf2Equality()
{
   {
      const std::string caseLabel = "[ct] Equality for equal infinite lines";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] Equality for infinite lines with different anchors";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] Equality for infinite lines with different directions";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel = "[ct] Equality for infinite lines with different "
                                    "value types but same data values";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(a == b, caseLabel);
   }
}


void testLineInf2Inequality()
{
   {
      const std::string caseLabel = "[ct] Inequality for equal infinite lines";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{3, 4}, Vec2{2, 1}};

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] Inequality for infinite lines with different anchors";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{2, 4}, Vec2{2, 1}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "[ct] Inequality for infinite lines with different directions";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<int> b{Point2{3, 4}, Vec2{2, 2}};

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel = "[ct] Inequality for infinite lines with different "
                                    "value types but same data values";

      const LineInf2<int> a{Point2{3, 4}, Vec2{2, 1}};
      const LineInf2<float> b{Point2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}};

      VERIFY(!(a != b), caseLabel);
   }
}

} // namespace


void testCtLineInf2()
{
   testLineInf2DefaultCtor();
   testLineInf2ValueCtor();
   testLineInf2Type();
   testLineInf2IsPoint();
   testLineInf2StartPoint();
   testLineInf2EndPoint();
   testLineInf2IsPointOnLine();
   testLineInf2IsPointOnInfiniteLine();
   testLineInf2LerpFactor();
   testLineInf2Lerp();
   testLineInf2Parallel();
   testLineInf2Coincident();
   testLineInf2Equality();
   testLineInf2Inequality();
}
