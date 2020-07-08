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
   {
      const std::string caseLabel = "Vec2 point ctor for mixed point types";

      Vec2<float> v{Point2{2, 3}, Point2{4.0, 7.0}};
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
      const std::string caseLabel = "Dot product for two vectors in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(dot(v, w), 19.5), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product for two vectors in opposite directions";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-2.0, -3.0};
      VERIFY(fpEqual(dot(v, w), -13.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product for two vectors with second perpendicular to left";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{-3.0f, 2.0f};
      VERIFY(equal(dot(v, w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product for two vectors with second perpendicular to right";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(equal(dot(v, w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product of vectors with second to right at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{4, 3};
      VERIFY(equal(dot(v, w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product of vectors with second to left at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(dot(v, w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product of vectors with second to right at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(equal(dot(v, w), -21.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product of vectors with second to left at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-2.0f, 1.0f};
      VERIFY(equal(dot(v, w), -3.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product of vectors with with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(dot(v, w), -3.0f), caseLabel);
      VERIFY(equal(dot(w, v), -3.0), caseLabel);
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
      const std::string caseLabel =
         "Perpendicular dot product for vectors in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(equal(perpDot(v, w), 0.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors in opposite direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-2.0, -3.0};
      VERIFY(fpEqual(perpDot(v, w), 0.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors perpendicular to left";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{-3.0f, 2.0f};
      VERIFY(equal(perpDot(v, w), 13.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors perpendicular to right";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(equal(perpDot(v, w), -13.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors to right at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{4, 3};
      VERIFY(equal(perpDot(v, w), -3.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors to left at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(equal(perpDot(v, w), 3.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors to right at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(equal(perpDot(v, w), -3.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors to left at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-2.0f, 1.0f};
      VERIFY(equal(perpDot(v, w), 9.0f), caseLabel);
   }
   {
      const std::string caseLabel =
         "Perpendicular dot product for vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(equal(perpDot(v, w), 9.0f), caseLabel);
      VERIFY(equal(perpDot(w, v), -9.0), caseLabel);
   }
}


void testVec2Perpendicular()
{
   {
      const std::string caseLabel = "Perpendicular for perpendicular vectors";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-3.0, 2.0};
      VERIFY(perpendicular(v, w), caseLabel);
      VERIFY(perpendicular(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "Perpendicular for not perpendicular vectors";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<int> w{2, 4};
      VERIFY(!perpendicular(v, w), caseLabel);
      VERIFY(!perpendicular(w, v), caseLabel);
   }
}


void testVec2Orthogonal()
{
   {
      const std::string caseLabel = "Orthogonal for orthogonal vectors";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{-3.0, 2.0};
      VERIFY(orthogonal(v, w), caseLabel);
      VERIFY(orthogonal(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "Orthogonal for not orthogonal vectors";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<int> w{2, 4};
      VERIFY(!orthogonal(v, w), caseLabel);
      VERIFY(!orthogonal(w, v), caseLabel);
   }
}


void testVec2SameDirection()
{
   {
      const std::string caseLabel = "SameDirection for vectors with same direction";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{4.0f, 4.0f};
      VERIFY(sameDirection(v, w), caseLabel);
      VERIFY(sameDirection(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "SameDirection for vectors with opposite directions";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-4, -4};
      VERIFY(!sameDirection(v, w), caseLabel);
      VERIFY(!sameDirection(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "SameDirection for vectors with different directions";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(!sameDirection(v, w), caseLabel);
      VERIFY(!sameDirection(w, v), caseLabel);
   }
}


void testVec2Parallel()
{
   {
      const std::string caseLabel = "Parallel for vectors with same direction";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{4.0f, 4.0f};
      VERIFY(parallel(v, w), caseLabel);
      VERIFY(parallel(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "Parallel for vectors with opposite directions";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-4, -4};
      VERIFY(parallel(v, w), caseLabel);
      VERIFY(parallel(w, v), caseLabel);
   }
   {
      const std::string caseLabel = "Parallel for vectors with different directions";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(!parallel(v, w), caseLabel);
      VERIFY(!parallel(w, v), caseLabel);
   }
}


void testVec2AcuteAngle()
{
   {
      const std::string caseLabel = "AcuteAngle() for vectors with acute angle";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{3.0f, 4.0f};
      VERIFY(acuteAngle(v, w), caseLabel);
   }
   {
      const std::string caseLabel = "AcuteAngle() for vectors with obtuse angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-3, -4};
      VERIFY(!acuteAngle(v, w), caseLabel);
   }
   {
      const std::string caseLabel = "AcuteAngle() for vectors with right angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-2.0f, 2.0f};
      VERIFY(!acuteAngle(v, w), caseLabel);
   }
}


void testVec2ObtuseAngle()
{
   {
      const std::string caseLabel = "ObtuseAngle() for vectors with acute angle";

      const Vec2<float> v{2.0f, 2.0f};
      const Vec2<float> w{3.0f, 4.0f};
      VERIFY(!obtuseAngle(v, w), caseLabel);
   }
   {
      const std::string caseLabel = "ObtuseAngle() for vectors with obtuse angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<int> w{-3, -4};
      VERIFY(obtuseAngle(v, w), caseLabel);
   }
   {
      const std::string caseLabel = "ObtuseAngle() for vectors with right angle";

      const Vec2<double> v{2.0, 2.0};
      const Vec2<float> w{-2.0f, 2.0f};
      VERIFY(!obtuseAngle(v, w), caseLabel);
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


void testVec2Subtraction()
{
   {
      const std::string caseLabel = "Vec2 subtraction";

      const Vec2<double> v = Vec2(2.0, -3.0);
      const Vec2<int> w = Vec2(1, 5);
      const auto res = v - w;

      static_assert(std::is_same_v<decltype(res)::value_type, double>);
      VERIFY(res.x() == 1.0, caseLabel);
      VERIFY(res.y() == -8.0, caseLabel);
   }
}


void testVec2Addition()
{
   {
      const std::string caseLabel = "Vec2 addition";

      const Vec2<double> v = Vec2(2.0, -3.0);
      const Vec2<int> w = Vec2(1, 5);
      const auto res = v + w;

      static_assert(std::is_same_v<decltype(res)::value_type, double>);
      VERIFY(res.x() == 3.0, caseLabel);
      VERIFY(res.y() == 2.0, caseLabel);
   }
}


void testVec2MultiplicationWithScalarAsSecondOperand()
{
   {
      const std::string caseLabel = "Vec2 multiplication with scalar as second operand";

      const Vec2<double> v = Vec2(2.0, -3.0);
      const auto res = v * 1.5;

      static_assert(std::is_same_v<decltype(res)::value_type, double>);
      VERIFY(sutil::equal(res.x(), 3.0), caseLabel);
      VERIFY(sutil::equal(res.y(), -4.5), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2 multiplication with scalar as second operand for mixed types";

      const Vec2<int> v = Vec2(2, -3);
      const auto res = v * 1.5;

      static_assert(std::is_same_v<decltype(res)::value_type, int>);
      VERIFY(sutil::equal(res.x(), 3), caseLabel);
      VERIFY(sutil::equal(res.y(), -4), caseLabel);
   }
}


void testVec2MultiplicationWithScalarAsFirstOperand()
{
   {
      const std::string caseLabel = "Vec2 multiplication with scalar as first operand";

      const Vec2<double> v = Vec2(2.0, -3.0);
      const auto res = 1.5 * v;

      static_assert(std::is_same_v<decltype(res)::value_type, double>);
      VERIFY(sutil::equal(res.x(), 3.0), caseLabel);
      VERIFY(sutil::equal(res.y(), -4.5), caseLabel);
   }
   {
      const std::string caseLabel =
         "Vec2 multiplication with scalar as first operand for mixed types";

      const Vec2<int> v = Vec2(2, -3);
      const auto res = 1.5 * v;

      static_assert(std::is_same_v<decltype(res)::value_type, int>);
      VERIFY(sutil::equal(res.x(), 3), caseLabel);
      VERIFY(sutil::equal(res.y(), -4), caseLabel);
   }
}


void testVec2DivisionByScalar()
{
   {
      const std::string caseLabel = "Vec2 division by scalar";

      const Vec2<double> v = Vec2(2.0, -3.0);
      const auto res = v / 2.0;

      static_assert(std::is_same_v<decltype(res)::value_type, double>);
      VERIFY(sutil::equal(res.x(), 1.0), caseLabel);
      VERIFY(sutil::equal(res.y(), -1.5), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 division by scalar";

      const Vec2<int> v = Vec2(2, -3);
      const auto res = v / 2.0;

      static_assert(std::is_same_v<decltype(res)::value_type, int>);
      VERIFY(sutil::equal(res.x(), 1), caseLabel);
      VERIFY(sutil::equal(res.y(), -1), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2 division by zero";

      VERIFY_THROW(
         [&]() {
            const Vec2<double> v = Vec2(2.0, -3.0);
            const auto res = v / 0.0;
         },
         std::runtime_error, caseLabel);
   }
}


void testVec2Equality()
{
   {
      const std::string caseLabel = "Equality for equal float vectors";

      const Vec2<float> a{0.011f, -345.78f};
      const Vec2<float> b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for equal integer vectors";

      const Vec2<int> a{-3, 9};
      const Vec2<int> b{-3, 9};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double vectors whose x coordinates are unequal beyond the "
         "epsilon threshold";

      const Vec2<double> a{1.23456789000000000001, 3.2};
      const Vec2<double> b{1.23456789000000000002, 3.2};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double vectors whose y coordinates are unequal beyond the "
         "epsilon threshold";

      const Vec2<double> a{3.2, 1.23456789000000000001};
      const Vec2<double> b{3.2, 1.23456789000000000002};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal vectors";

      const Vec2<float> a{0.011f, -345.78f};
      const Vec2<float> b{7.6f, -2.2f};
      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for vectors with different value types but same data values";

      const Vec2<float> a{2.0f, -3.0f};
      const Vec2<int> b{2, -3};
      VERIFY(a == b, caseLabel);
   }
}


void testVec2Inequality()
{
   {
      const std::string caseLabel = "Inequality for unequal float vectors";

      const Vec2<float> a{0.011f, -345.78f};
      const Vec2<float> b{0.011f, -345.0f};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal integer vectors";

      const Vec2<int> a{-3, 9};
      const Vec2<int> b{3, 9};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double vectors whose x coordinates are unequal beyond the "
         "epsilon threshold";

      const Vec2<double> a{1.23456789000000000001, 3.2};
      const Vec2<double> b{1.23456789000000000002, 3.2};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double vectors whose y coordinates are unequal beyond the "
         "epsilon threshold";

      const Vec2<double> a{3.2, 1.23456789000000000001};
      const Vec2<double> b{3.2, 1.23456789000000000002};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for equal vectors";

      const Vec2<float> a{0.011f, -345.78f};
      const Vec2<float> b = a;
      VERIFY(!(a != b), caseLabel);
   }
}


void testPointVectorAddition()
{
   {
      const std::string caseLabel = "Add vector to point";

      const Point2<float> p{3.2f, -4.1f};
      const Vec2<float> v{0.3f, -0.1f};

      const auto res = p + v;
      VERIFY(equal(res.x(), 3.5f), caseLabel);
      VERIFY(equal(res.y(), -4.2f), caseLabel);
   }
}


void testVectorPointAddition()
{
   {
      const std::string caseLabel = "Add point to vector";

      const Point2<float> p{3.2f, -4.1f};
      const Vec2<float> v{0.3f, -0.1f};

      const auto res = v + p;
      VERIFY(equal(res.x(), 3.5f), caseLabel);
      VERIFY(equal(res.y(), -4.2f), caseLabel);
   }
}


void testPointSubtraction()
{
   {
      const std::string caseLabel = "Subtract points";

      const Point2<float> p{3.2f, -4.1f};
      const Point2<float> q{0.3f, -0.1f};

      const auto res = p - q;
      VERIFY(equal(res.x(), 2.9f), caseLabel);
      VERIFY(equal(res.y(), -4.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Subtract points for mixed types";

      const Point2<int> p{3, -4};
      const Point2<float> q{0.3f, -0.1f};

      const auto res = p - q;
      VERIFY(equal(res.x(), 2.7f), caseLabel);
      VERIFY(equal(res.y(), -3.9f), caseLabel);
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
   testVec2Perpendicular();
   testVec2Orthogonal();
   testVec2SameDirection();
   testVec2Parallel();
   testVec2AcuteAngle();
   testVec2ObtuseAngle();
   testVec2IsCcw();
   testVec2IsCw();
   testVec2CcwNormal();
   testVec2CwNormal();
   testVec2Negation();
   testVec2Subtraction();
   testVec2Addition();
   testVec2MultiplicationWithScalarAsSecondOperand();
   testVec2MultiplicationWithScalarAsFirstOperand();
   testVec2DivisionByScalar();
   testVec2Equality();
   testVec2Inequality();

   testPointVectorAddition();
   testVectorPointAddition();
   testPointSubtraction();
}
