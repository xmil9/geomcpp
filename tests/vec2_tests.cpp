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
      VERIFY(fpEqual(v.lengthSquared(), 13.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared for double";

      const Vec2<double> v{2.0, 3.0};
      VERIFY(fpEqual(v.lengthSquared(), 13.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared for integer";

      const Vec2<int> v{2, 3};
      VERIFY(v.lengthSquared() == 13, caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::lengthSquared when length is zero";

      const Vec2<double> v{0.0, 0.0};
      VERIFY(fpEqual(v.lengthSquared(), 0.0), caseLabel);
   }
}


void testVec2Length()
{
   {
      const std::string caseLabel = "Vec2::length for float";

      const Vec2<float> v{2.0f, 3.0f};
      VERIFY(fpEqual(v.length(), 3.60555124f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length for double";

      const Vec2<double> v{2.0, 3.0};
      VERIFY(fpEqual(v.length(), 3.60555124), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length for integer";

      const Vec2<int> v{2, 3};
      VERIFY(fpEqual(v.length(), 3.60555124), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::length when length is zero";

      const Vec2<double> v{0.0, 0.0};
      VERIFY(fpEqual(v.length(), 0.0), caseLabel);
   }
}


void testVec2Dot()
{
   {
      const std::string caseLabel = "Vec2::dot for other vector in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(fpEqual(v.dot(w), 19.5), caseLabel);
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
      VERIFY(fpEqual(v.dot(w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector perpendicular to right";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(fpEqual(v.dot(w), 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to right at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{4, 3};
      VERIFY(fpEqual(v.dot(w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to left at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(fpEqual(v.dot(w), 21.0), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to right at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-3.0f, -4.0f};
      VERIFY(fpEqual(v.dot(w), -21.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vector to left at obtuse angle";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<float> w{-2.0f, 1.0f};
      VERIFY(fpEqual(v.dot(w), -3.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Vec2::dot for other vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(fpEqual(v.dot(w), -3.0f), caseLabel);
      VERIFY(fpEqual(w.dot(v), -3.0), caseLabel);
   }
}


void testVec2DotOperator()
{
   {
      const std::string caseLabel = "Dot product operator for vectors in same direction";

      const Vec2<double> v{2.0, 3.0};
      const Vec2<double> w{3.0, 4.5};
      VERIFY(fpEqual(v * w, 19.5), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product operator for perpendicular vectors";

      const Vec2<float> v{2.0f, 3.0f};
      const Vec2<float> w{3.0f, -2.0f};
      VERIFY(fpEqual(v * w, 0.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Dot product operator for vectors at acute angle";

      const Vec2<int> v{3, 3};
      const Vec2<int> w{3, 4};
      VERIFY(fpEqual(v * w, 21.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Dot product operator for vectors with mixed value types";

      const Vec2<float> v{3.0f, 3.0f};
      const Vec2<int> w{-2, 1};
      VERIFY(fpEqual(v * w, -3.0f), caseLabel);
      VERIFY(fpEqual(w * v, -3.0), caseLabel);
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
   testVec2Dot();
   testVec2DotOperator();
}
