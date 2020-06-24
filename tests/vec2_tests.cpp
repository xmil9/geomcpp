//
// geomcpp tests
// Tests for 2D vectors.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "vec2_tests.h"
#include "vec2.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{

///////////////////

void testVec2DefaultCtor()
{
   {
      const std::string caseLabel = "Vec2 default ctor for float";

      Vec2<float> v;
      VERIFY(v.x() == 0.0f, caseLabel);
      VERIFY(v.y() == 0.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 default ctor for double";

      Vec2<double> v;
      VERIFY(v.x() == 0.0, caseLabel);
      VERIFY(v.y() == 0.0, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 default ctor for integer";

      Vec2<int> v;
      VERIFY(v.x() == 0, caseLabel);
      VERIFY(v.y() == 0, caseLabel);
   }
}


void testVec2ValueCtor()
{
   {
      const std::string caseLabel = "Vec2 value ctor for float";

      Vec2 v{1.0f, 2.2f};
      VERIFY(v.x() == 1.0f, caseLabel);
      VERIFY(v.y() == 2.2f, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 value ctor for double";

      Vec2 v{1.0, 2.2};
      VERIFY(v.x() == 1.0, caseLabel);
      VERIFY(v.y() == 2.2, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 value ctor for integer";

      Vec2 v{1, 2};
      VERIFY(v.x() == 1, caseLabel);
      VERIFY(v.y() == 2, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 value ctor for float with integer values";

      Vec2<float> v{1, 2};
      VERIFY(v.x() == 1.0f, caseLabel);
      VERIFY(v.y() == 2.0f, caseLabel);
   }
}


void testVec2PointCtor()
{
   {
      const std::string caseLabel = "Vec2 point ctor for float";

      Vec2<float> v{Point2{2.0f, 3.0f}, Point2{4.0f, 7.0f}};
      VERIFY(v.x() == 2.0f, caseLabel);
      VERIFY(v.y() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 point ctor for double";

      Vec2<double> v{Point2{2.0, 3.0}, Point2{4.0, 7.0}};
      VERIFY(v.x() == 2.0, caseLabel);
      VERIFY(v.y() == 4.0, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 point ctor for integer";

      Vec2<int> v{Point2{2, 3}, Point2{4, 7}};
      VERIFY(v.x() == 2, caseLabel);
      VERIFY(v.y() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 point ctor for float with integer point values";

      Vec2<float> v{Point2{2, 3}, Point2{4, 7}};
      VERIFY(v.x() == 2.0f, caseLabel);
      VERIFY(v.y() == 4.0f, caseLabel);
   }
}


void testVec2LengthSquared()
{
   {
      const std::string caseLabel = "Vec2::lengthSquared for float";

      const Vec2<float> v{2.0f, 3.0f};
      VERIFY(equal(v.lengthSquared(), 13.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared for double";

      const Vec2<double> v{2.0, 3.0};
      VERIFY(equal(v.lengthSquared(), 13.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared for integer";

      const Vec2<int> v{2, 3};
      VERIFY(v.lengthSquared() == 13, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared when length is zero";

      const Vec2<double> v{0.0, 0.0};
      VERIFY(equal(v.lengthSquared(), 0.0), caseLabel);
   }
}


void testVec2Length()
{
   {
      const std::string caseLabel = "Vec2::length for float";

      const Vec2<float> v{2.0f, 3.0f};
      VERIFY(equal(v.length(), 3.60555124f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length for double";

      const Vec2<double> v{2.0, 3.0};
      VERIFY(equal(v.length(), 3.60555124), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length for integer";

      const Vec2<int> v{2, 3};
      VERIFY(equal(v.length(), 3.60555124), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length when length is zero";

      const Vec2<double> v{0.0, 0.0};
      VERIFY(equal(v.length(), 0.0), caseLabel);
   }
}


void testVec2Scale()
{
   {
      const std::string caseLabel = "Vec2::scale for float";

      const Vec2<float> v{2.0f, 3.0f};
      const auto scaled = v.scale(2.5f);
      VERIFY(equal(scaled.x(), 5.0f), caseLabel);
      VERIFY(equal(scaled.y(), 7.5f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::scale for double with integer factor";

      const Vec2<double> v{2.0, 3.0};
      const auto scaled = v.scale(2);
      VERIFY(equal(scaled.x(), 4.0), caseLabel);
      VERIFY(equal(scaled.y(), 6.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::scale for integer with floating point factor";

      const Vec2<int> v{2, 3};
      const auto scaled = v.scale(2.6);
      // Coordinates get truncated to integers.
      VERIFY(equal(scaled.x(), static_cast<int>(2 * 2.6)), caseLabel);
      VERIFY(equal(scaled.y(), static_cast<int>(3 * 2.6)), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::scale with zero";

      const Vec2<double> v{2.0, 3.0};
      const auto scaled = v.scale(0.0);
      VERIFY(equal(scaled.x(), 0.0), caseLabel);
      VERIFY(equal(scaled.y(), 0.0), caseLabel);
   }
}


void testVec2Normalize()
{
   {
      const std::string caseLabel = "Vec2::normalize for float";

      const Vec2<float> v{4.0f, 3.0f};
      const auto normed = v.normalize();
      VERIFY(equal(normed.x(), 4.0f / v.length()), caseLabel);
      VERIFY(equal(normed.y(), 3.0f / v.length()), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::normalize for integer";

      const Vec2<long> v{4L, 3L};
      const auto normed = v.normalize();
      VERIFY(equal(normed.x(), static_cast<long>(4L / v.length())), caseLabel);
      VERIFY(equal(normed.y(), static_cast<long>(3L / v.length())), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::normalize for vector with zero length";

      const Vec2<double> v{0.0, 0.0};
      const auto normed = v.normalize();
      VERIFY(equal(normed.x(), 0.0), caseLabel);
      VERIFY(equal(normed.y(), 0.0), caseLabel);
   }
}


void testVec2Dot()
{
   {
      const std::string caseLabel = "Vec2::dot for other vector in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(v.dot(w), 19.5), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector in opposite direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-2.0, -3.0};
      VERIFY(fpEqual(v.dot(w), -13.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector perpendicular to left";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{-3.0f, 2.0f};
      VERIFY(equal(v.dot(w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector perpendicular to right";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(equal(v.dot(w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to right at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{4, 3};
      VERIFY(equal(v.dot(w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to left at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(v.dot(w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to right at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(equal(v.dot(w), -21.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to left at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-2.0f, 1.0f};
      VERIFY(equal(v.dot(w), -3.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(v.dot(w), -3.0f), caseLabel);
      VERIFY(equal(w.dot(v), -3.0), caseLabel);
   }
}


void testVec2DotOperator()
{
   {
      const std::string caseLabel = "Dot product operator for vectors in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(v * w, 19.5), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product operator for perpendicular vectors";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(equal(v * w, 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product operator for vectors at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(v * w, 21.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product operator for vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(v * w, -3.0f), caseLabel);
      VERIFY(equal(w * v, -3.0), caseLabel);
   }
}


void testVec2PerpDot()
{
   {
      const std::string caseLabel = "Vec2::perpDot for other vector in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(v.perpDot(w), 0.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector in opposite direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-2.0, -3.0};
      VERIFY(fpEqual(v.perpDot(w), 0.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector perpendicular to left";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{-3.0f, 2.0f};
      VERIFY(equal(v.perpDot(w), 13.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector perpendicular to right";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(equal(v.perpDot(w), -13.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector to right at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{4, 3};
      VERIFY(equal(v.perpDot(w), -3.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector to left at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(v.perpDot(w), 3.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector to right at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(equal(v.perpDot(w), -3.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vector to left at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-2.0f, 1.0f};
      VERIFY(equal(v.perpDot(w), 9.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::perpDot for other vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(v.perpDot(w), 9.0f), caseLabel);
      VERIFY(equal(w.perpDot(v), -9.0), caseLabel);
   }
}


void testVec2PerpDotFreeFunction()
{
   {
      const std::string caseLabel =
         "Perpendicular dot product free function for vectors in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(perpDot(v, w), 0.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product free function for perpendicular vectors";

      const Vec2<float> v{2.0, 3.0};
      const Vec2<float> w{-2.0, -3.0};
      VERIFY(fpEqual(perpDot(v, w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product free function for vectors at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(perpDot(v, w), 3.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product free function for vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(perpDot(v, w), 9.0f), caseLabel);
      VERIFY(equal(perpDot(w, v), -9.0), caseLabel);
   }
}


void testVec2IsPerpendicular()
{
   {
      const std::string caseLabel = "Vec2::isPerpendicular for perpendicular vector";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-3.0, 2.0};
      VERIFY(v.isPerpendicular(w), caseLabel);
      VERIFY(w.isPerpendicular(v), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isPerpendicular for not perpendicular vector";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<int> w{2, 4};
      VERIFY(!v.isPerpendicular(w), caseLabel);
      VERIFY(!w.isPerpendicular(v), caseLabel);
   }
}


void testVec2IsOrthogonal()
{
   {
      const std::string caseLabel = "Vec2::isOrthogonal for orthogonal vector";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-3.0, 2.0};
      VERIFY(v.isOrthogonal(w), caseLabel);
      VERIFY(w.isOrthogonal(v), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isOrthogonal for not orthogonal vector";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<int> w{2, 4};
      VERIFY(!v.isOrthogonal(w), caseLabel);
      VERIFY(!w.isOrthogonal(v), caseLabel);
   }
}


void testVec2HasSameDirection()
{
   {
      const std::string caseLabel =
         "Vec2::hasSameDirection for vector with same direction";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{4.0f, 4.0f};
      VERIFY(v.hasSameDirection(w), caseLabel);
      VERIFY(w.hasSameDirection(v), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::hasSameDirection for vector with opposite direction";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-4, -4};
      VERIFY(!v.hasSameDirection(w), caseLabel);
      VERIFY(!w.hasSameDirection(v), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::hasSameDirection for vector with different direction";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(!v.hasSameDirection(w), caseLabel);
      VERIFY(!w.hasSameDirection(v), caseLabel);
   }
}


void testVec2IsParallel()
{
   {
      const std::string caseLabel = "Vec2::isParallel for vector with same direction";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{4.0f, 4.0f};
      VERIFY(v.isParallel(w), caseLabel);
      VERIFY(w.isParallel(v), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isParallel for vector with opposite direction";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-4, -4};
      VERIFY(v.isParallel(w), caseLabel);
      VERIFY(w.isParallel(v), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2::isParallel for vector with different direction";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(!v.isParallel(w), caseLabel);
      VERIFY(!w.isParallel(v), caseLabel);
   }
}


void testVec2HasAcuteAngle()
{
   {
      const std::string caseLabel = "Vec2::hasAcuteAngle for vector with acute angle";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{3.0f, 4.0f};
      VERIFY(v.hasAcuteAngle(w), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::hasAcuteAngle for vector with obtuse angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-3, -4};
      VERIFY(!v.hasAcuteAngle(w), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::hasAcuteAngle for vector with right angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-2.0f, 2.0f};
      VERIFY(!v.hasAcuteAngle(w), caseLabel);
   }
}


void testVec2HasObtuseAngle()
{
   {
      const std::string caseLabel = "Vec2::hasObtuseAngle for vector with acute angle";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{3.0f, 4.0f};
      VERIFY(!v.hasObtuseAngle(w), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::hasObtuseAngle for vector with obtuse angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-3, -4};
      VERIFY(v.hasObtuseAngle(w), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::hasObtuseAngle for vector with right angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-2.0f, 2.0f};
      VERIFY(!v.hasObtuseAngle(w), caseLabel);
   }
}


void testVec2IsCcw()
{
   {
      const std::string caseLabel = "Vec2::isCcw for ccw vector";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{2.0f, 1.0f};
      VERIFY(v.isCcw(w, CoordSys::Screen), caseLabel);
      VERIFY(!v.isCcw(w, CoordSys::Cartesian), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isCcw for cw vector";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{1.0f, 2.0f};
      VERIFY(!v.isCcw(w, CoordSys::Screen), caseLabel);
      VERIFY(v.isCcw(w, CoordSys::Cartesian), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isCcw for vector in same direction";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{3, 3};
      VERIFY(!v.isCcw(w, CoordSys::Screen), caseLabel);
      VERIFY(!v.isCcw(w, CoordSys::Cartesian), caseLabel);
   }
}


void testVec2IsCw()
{
   {
      const std::string caseLabel = "Vec2::isCw for ccw vector";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{2.0f, 1.0f};
      VERIFY(!v.isCw(w, CoordSys::Screen), caseLabel);
      VERIFY(v.isCw(w, CoordSys::Cartesian), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isCw for cw vector";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{1.0f, 2.0f};
      VERIFY(v.isCw(w, CoordSys::Screen), caseLabel);
      VERIFY(!v.isCw(w, CoordSys::Cartesian), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::isCw for vector in same direction";

      Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{3, 3};
      VERIFY(!v.isCw(w, CoordSys::Screen), caseLabel);
      VERIFY(!v.isCw(w, CoordSys::Cartesian), caseLabel);
   }
}


void testVec2CcwNormal()
{
   {
      const std::string caseLabel = "Vec2::ccwNormal";

		const Vec2<float> v = Vec2(2.0f, 3.0f);

      const Vec2<float> ccwScreen = v.ccwNormal(CoordSys::Screen);
		VERIFY(v.isCcw(ccwScreen, CoordSys::Screen), caseLabel);

      const Vec2<float> ccwCart = v.ccwNormal(CoordSys::Cartesian);
		VERIFY(v.isCcw(ccwCart, CoordSys::Cartesian), caseLabel);
   }
}


void testVec2CwNormal()
{
   {
      const std::string caseLabel = "Vec2::cwNormal";

		const Vec2<int> v = Vec2(2, 3);

      const Vec2<int> cwScreen = v.cwNormal(CoordSys::Screen);
		VERIFY(v.isCw(cwScreen, CoordSys::Screen), caseLabel);

      const Vec2<int> cwCart = v.cwNormal(CoordSys::Cartesian);
		VERIFY(v.isCw(cwCart, CoordSys::Cartesian), caseLabel);
   }
}


void testVec2Negation()
{
   {
      const std::string caseLabel = "Vec2 negation";

		const auto v = Vec2(2.0, -3.0);
		const auto negated = -v;
		VERIFY(negated.x() == -v.x(), caseLabel);
		VERIFY(negated.y() == -v.y(), caseLabel);
   }
}

} // namespace


///////////////////

void testVector2D()
{
   testVec2DefaultCtor();
   testVec2ValueCtor();
   testVec2PointCtor();
   testVec2LengthSquared();
   testVec2Length();
   testVec2Scale();
   testVec2Normalize();
   testVec2Dot();
   testVec2DotOperator();
   testVec2PerpDot();
   testVec2PerpDotFreeFunction();
   testVec2IsPerpendicular();
   testVec2IsOrthogonal();
   testVec2HasSameDirection();
   testVec2IsParallel();
   testVec2HasAcuteAngle();
   testVec2HasObtuseAngle();
   testVec2IsCcw();
   testVec2IsCw();
   testVec2CcwNormal();
   testVec2CwNormal();
   testVec2Negation();
}
