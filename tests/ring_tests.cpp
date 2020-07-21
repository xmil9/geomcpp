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


void testRingCtor()
{
   {
      const std::string caseLabel = "Ring ctor";

      const Point2 center{1.0f, 2.0f};
      const Ring r{center, 0.5f, 1.2f};
      VERIFY(r.center() == center, caseLabel);
      VERIFY(equal(r.innerRadius(), 0.5f), caseLabel);
      VERIFY(equal(r.outerRadius(), 1.2f), caseLabel);
   }
   {
      const std::string caseLabel = "Ring ctor for integer coordinate type";

      const Point2 center{1, 2};
      const Ring r{center, 2, 3};
      VERIFY(r.center() == center, caseLabel);
      VERIFY(equal(r.innerRadius(), 2.0), caseLabel);
      VERIFY(equal(r.outerRadius(), 3.0), caseLabel);
   }
   {
      const std::string caseLabel = "Ring ctor for denormalized radii";

      const Point2 center{1.0f, 2.0f};
      const Ring r{center, 1.2f, 0.5f};
      VERIFY(r.center() == center, caseLabel);
      VERIFY(equal(r.innerRadius(), 0.5f), caseLabel);
      VERIFY(equal(r.outerRadius(), 1.2f), caseLabel);
   }
}


void testRingBounds()
{
   {
      const std::string caseLabel = "Ring bounds";

      const Point2 center{1.0, 2.0};
      const Ring r{center, 3.0, 4.0};

      const Rect expected{-3.0, -2.0, 5.0, 6.0};
      VERIFY(r.bounds() == expected, caseLabel);
   }
}


void testRingOffset()
{
   {
      const std::string caseLabel = "Ring offset";

      const Point2 center{1.0, 2.0};
      const Ring r{center, 3.0, 4.0};
      const Ring off = r.offset(Vec2{2.0, 1.0});

      VERIFY(off.center() == Point2(3.0, 3.0), caseLabel);
      VERIFY(off.innerRadius() == r.innerRadius(), caseLabel);
      VERIFY(off.outerRadius() == r.outerRadius(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Ring offset with vector that has another coordinate type";

      const Point2 center{1.0, 2.0};
      const Ring r{center, 3.0, 4.0};
      const Ring off = r.offset(Vec2{2, 1});

      VERIFY(off.center() == Point2(3.0, 3.0), caseLabel);
      VERIFY(off.innerRadius() == r.innerRadius(), caseLabel);
      VERIFY(off.outerRadius() == r.outerRadius(), caseLabel);
   }
}


void testRingEquality()
{
   {
      const std::string caseLabel = "Equality for equal rings";

      const Point2 center{1.0f, 2.0f};
      const Ring<float> a{center, 1.2f, 4.5f};
      const Ring<float> b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rings whose inner radii are unequal beyond the "
         "epsilon threshold";

      const Point2 center{1.0, 2.0};
      const Ring<double> a{center, 1.23456789000000000001, 3.2};
      const Ring<double> b{center, 1.23456789000000000002, 3.2};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rings whose inner radii are unequal beyond the "
         "epsilon threshold";

      const Point2 center{1.0, 2.0};
      const Ring<double> a{center, 0.3, 1.23456789000000000001};
      const Ring<double> b{center, 0.3, 1.23456789000000000002};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal rings";

      const Point2 center{1.0f, 2.0f};
      const Ring<float> a{center, 1.2f, 2.2f};
      const Ring<float> b{center, 1.2f, 2.3f};
      VERIFY(!(a == b), caseLabel);
   }
}


void testRingInequality()
{
   {
      const std::string caseLabel = "Inequality for equal rings";

      const Point2 center{1.0f, 2.0f};
      const Ring<float> a{center, 1.2f, 4.5f};
      const Ring<float> b = a;
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rings whose inner radii are unequal beyond the "
         "epsilon threshold";

      const Point2 center{1.0, 2.0};
      const Ring<double> a{center, 1.23456789000000000001, 3.2};
      const Ring<double> b{center, 1.23456789000000000002, 3.2};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rings whose inner radii are unequal beyond the "
         "epsilon threshold";

      const Point2 center{1.0, 2.0};
      const Ring<double> a{center, 0.3, 1.23456789000000000001};
      const Ring<double> b{center, 0.3, 1.23456789000000000002};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal rings";

      const Point2 center{1.0f, 2.0f};
      const Ring<float> a{center, 1.2f, 2.2f};
      const Ring<float> b{center, 1.2f, 2.3f};
      VERIFY(a != b, caseLabel);
   }
}

} // namespace


///////////////////

void testRing()
{
   testRingDefaultCtor();
   testRingCtor();
   testRingBounds();
   testRingOffset();
   testRingEquality();
   testRingInequality();
}
