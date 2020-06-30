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

} // namespace


void testRtLine2()
{
   testLine2DefaultCtor();
   testLine2ValueCtor();
}
