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
      const std::string caseLabel = "LineInf2 default ctor for float";

      LineInf2<float> l;
      VERIFY((l.anchor() == Point2<float>{}), caseLabel);
      VERIFY((l.direction() == Vec2<float>{}), caseLabel);
   }
   {
      const std::string caseLabel = "LineInf2 default ctor for integer";

      LineInf2<long> l;
      VERIFY((l.anchor() == Point2<long>{}), caseLabel);
      VERIFY((l.direction() == Vec2<long>{}), caseLabel);
   }
}


void testLineInf2ValueCtor()
{
   {
      const std::string caseLabel = "LineInf2 value ctor for float";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
   {
      const std::string caseLabel = "LineInf2 value ctor for integer";

      const Point2 anchor{1, 2};
      const Vec2 dir{3, 2};
      const LineInf2 l{anchor, dir};
      VERIFY((l.anchor() == anchor), caseLabel);
      VERIFY((l.direction() == dir), caseLabel);
   }
}


void testLineInf2Type()
{
   {
      const std::string caseLabel = "LineInf2::type";

      const Point2 anchor{1.0f, 2.0f};
      const Vec2 dir{3.0f, 2.0f};
      const LineInf2 l{anchor, dir};
      VERIFY(l.type() == Line2Type::Infinite, caseLabel);
   }
}

} // namespace


void testCtLineInf2()
{
   testLineInf2DefaultCtor();
   testLineInf2ValueCtor();
   testLineInf2Type();
   //testLine2IsPoint();
   //testLine2StartPoint();
   //testLine2EndPoint();
   //testLine2IsPointOnLine();
   //testLine2IsPointOnInfiniteLine();
   //testLine2LerpFactor();
   //testLine2Lerp();
   //testLine2Parallel();
   //testLine2Coincident();
   //testLine2Equality();
   //testLine2Inequality();
}
