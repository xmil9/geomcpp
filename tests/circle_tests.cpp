//
// geomcpp tests
// Tests for circles.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "circle_tests.h"
#include "circle.h"
#include "test_util.h"
#include "essentutils/fputil.h"
#define _USE_MATH_DEFINES
#include <math.h> 

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testCircleDefaultCtor()
{
   {
      const std::string caseLabel = "Circle default ctor";

      const Circle<float> c;
      VERIFY((c.center() == Point2{0.0f, 0.0f}), caseLabel);
      VERIFY(equal(c.radius(),0.0f), caseLabel);
   }
}


void testCircleValueCtor()
{
   {
      const std::string caseLabel = "Circle value ctor";

      const Point2 center{2, 3};
      const Circle c{center, 1};
      VERIFY(c.center() == center, caseLabel);
      VERIFY(c.radius() == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Circle value ctor for constexpr definitions";

      constexpr Point2 center{2, 3};
      // Needs to compile.
      constexpr Circle c{center, 1};
   }
}


void testCircleIsPoint()
{
   {
      const std::string caseLabel = "Circle::isPoint when circle is a point";

      const Point2 center{2, 3};
      const Circle c{center, 0};
      VERIFY(c.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPoint when circle is not a point";

      const Point2 center{2.2, 3.3};
      const Circle c{center, 0.01};
      VERIFY(!c.isPoint(), caseLabel);
   }
}


void testCircleBounds()
{
   {
      const std::string caseLabel = "Circle::bounds";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY((c.bounds() == Rect{-2.0, -1.0, 4.0, 5.0}), caseLabel);
   }
}


void testCircleOffset()
{
   {
      const std::string caseLabel = "Circle::offset";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      const Circle moved = c.offset(Vec2{2, 1});
      VERIFY((moved.center() == Point2{3.0, 3.0}), caseLabel);
      VERIFY((moved.radius() == c.radius()), caseLabel);
   }
}


void testCircleIsPointInCircle()
{
   {
      const std::string caseLabel = "Circle::isPointInCircle for point inside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(c.isPointInCircle(Point2{2, 2}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointInCircle for point outside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(!c.isPointInCircle(Point2{2, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointInCircle for point exactly on circle";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(c.isPointInCircle(Point2{1, 5}), caseLabel);
   }
}


void testCircleIsPointOnCircle()
{
   {
      const std::string caseLabel = "Circle::isPointOnCircle for point inside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(!c.isPointOnCircle(Point2{2, 2}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointOnCircle for point outside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(!c.isPointOnCircle(Point2{2, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointOnCircle for point exactly on circle";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(c.isPointOnCircle(Point2{1, 5}), caseLabel);
   }
}


void testCircleIsPointInsideCircle()
{
   {
      const std::string caseLabel = "Circle::isPointInsideCircle for point inside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(c.isPointInsideCircle(Point2{2, 2}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointInsideCircle for point outside";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(!c.isPointInsideCircle(Point2{2, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::isPointInsideCircle for point exactly on circle";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY(!c.isPointInsideCircle(Point2{1, 5}), caseLabel);
   }
}


void testCirclePointAtAngle()
{
   {
      const std::string caseLabel = "Circle::pointAtAngle for zero radians";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY((c.pointAtAngle(0.0) == Point2{4.0, 2.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::pointAtAngle for 1/2 pi radians";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY((c.pointAtAngle(0.5 * M_PI) == Point2{1.0, 5.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::pointAtAngle for pi radians";

      const Circle c{Point2{1, 2}, 3};
      VERIFY((c.pointAtAngle(M_PI) == Point2{-2, 2}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::pointAtAngle for 3/2 pi radians";

      const Circle c{Point2{1.0, 2.0}, 3.0};
      VERIFY((c.pointAtAngle(1.5 * M_PI) == Point2{1.0, -1.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Circle::pointAtAngle for 2pi radians";

      const Circle c{Point2{1.0f, 2.0f}, 3.0f};
      VERIFY((c.pointAtAngle(2.0 * M_PI) == Point2{4.0f, 2.00000048f}), caseLabel);
   }
}

} // namespace


///////////////////

void testCircle()
{
   testCircleDefaultCtor();
   testCircleValueCtor();
   testCircleIsPoint();
   testCircleBounds();
   testCircleOffset();
   testCircleIsPointInCircle();
   testCircleIsPointOnCircle();
   testCircleIsPointInsideCircle();
   testCirclePointAtAngle();
}
