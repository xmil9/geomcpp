//
// geomcpp tests
// Tests for rectangles.
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


void testLine2HasStartPoint()
{
   {
      const std::string caseLabel = "Line2::hasStartPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(!l.hasStartPoint(), caseLabel);
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


void testLine2HasEndPoint()
{
   {
      const std::string caseLabel = "Line2::hasEndPoint";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const Line2 l{anchor, dir};
      VERIFY(!l.hasEndPoint(), caseLabel);
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

} // namespace


void testRtLine2()
{
   testLine2DefaultCtor();
   testLine2ValueCtor();
   testLine2IsPoint();
   testLine2HasStartPoint();
   testLine2StartPoint();
   testLine2HasEndPoint();
   testLine2EndPoint();
   testLine2IsPointOnLine();
}
