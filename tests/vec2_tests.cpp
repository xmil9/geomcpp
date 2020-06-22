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

} // namespace


///////////////////

void testVector2D()
{
   testVec2DefaultCtor();
   testVec2ValueCtor();
   testVec2PointCtor();
   testVec2LengthSquared();
   testVec2Length();
}
