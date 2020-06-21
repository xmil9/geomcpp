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
      VERIFY(v.x() == 0.0, caseLabel);
      VERIFY(v.y() == 0.0, caseLabel);
   }
}


void testVec2Length()
{
   {
      const std::string caseLabel = "Vec2::length for float";

      const Vec2<float> v{2.0f, 3.0f};
      VERIFY(fpEqual(v.length(), 3.60555124f), caseLabel);
   }
}

} // namespace


///////////////////

void testVector2D()
{
   testVec2DefaultCtor();
   testVec2Length();
}
