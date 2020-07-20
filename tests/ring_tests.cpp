//
// geomcpp tests
// Tests for ring shapes.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "ring_tests.h"
#include "ring.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testRingDefaultCtor()
{
   {
      const std::string caseLabel = "Ring default ctor";

      const Ring<float> r;
      VERIFY(r.center() == Point2(0.0f, 0.0f), caseLabel);
      VERIFY(equal(r.innerRadius(), 0.0f), caseLabel);
      VERIFY(equal(r.outerRadius(), 0.0f), caseLabel);
   }
}

} // namespace


///////////////////

void testRing()
{
   testRingDefaultCtor();
}
