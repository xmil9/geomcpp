//
// geomcpp tests
// Tests for 2D points.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "point2_tests.h"
#include "point2.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testPoint2DefaultCtor()
{
   {
      const std::string caseLabel = "Point2 default ctor for float";

      Point2<float> p;
      VERIFY(p.x() == 0.0f, caseLabel);
      VERIFY(p.y() == 0.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Point2 default ctor for double";

      Point2<double> p;
      VERIFY(p.x() == 0.0, caseLabel);
      VERIFY(p.y() == 0.0, caseLabel);
   }
   {
      const std::string caseLabel = "Point2 default ctor for integer";

      Point2<int> p;
      VERIFY(p.x() == 0, caseLabel);
      VERIFY(p.y() == 0, caseLabel);
   }
}


void testPoint2ValueCtor()
{
   {
      const std::string caseLabel = "Point2 value ctor for float";

      Point2 p{1.0f, 2.2f};
      VERIFY(p.x() == 1.0f, caseLabel);
      VERIFY(p.y() == 2.2f, caseLabel);
   }
   {
      const std::string caseLabel = "Point2 value ctor for double";

      Point2 p{1.0, 2.2};
      VERIFY(p.x() == 1.0, caseLabel);
      VERIFY(p.y() == 2.2, caseLabel);
   }
   {
      const std::string caseLabel = "Point2 value ctor for integer";

      Point2 p{1, 2};
      VERIFY(p.x() == 1, caseLabel);
      VERIFY(p.y() == 2, caseLabel);
   }
   {
      const std::string caseLabel = "Point2 value ctor for float with integer values";

      Point2<float> p{1, 2};
      VERIFY(p.x() == 1.0f, caseLabel);
      VERIFY(p.y() == 2.0f, caseLabel);
   }
}


void testPoint2Offset()
{
   {
      const std::string caseLabel = "Point2::offset for float";

      const Point2<float> p{2.0f, 3.0f};
      const auto off = p.offset(2.5f, 0.2f);
      VERIFY(equal(off.x(), 4.5f), caseLabel);
      VERIFY(equal(off.y(), 3.2f), caseLabel);
   }
   {
      const std::string caseLabel = "Point2::offset for double with integer factor";

      const Point2<double> p{2.0, 3.0};
      const auto off = p.offset(2, 1);
      VERIFY(equal(off.x(), 4.0), caseLabel);
      VERIFY(equal(off.y(), 4.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Point2::offset for integer with floating point factor";

      const Point2<int> p{2, 3};
      const auto off = p.offset(2.6, -1.4);
      // Coordinates get truncated to integers.
      VERIFY(equal(off.x(), static_cast<int>(2 + 2.6)), caseLabel);
      VERIFY(equal(off.y(), static_cast<int>(3 + -1.4)), caseLabel);
   }
   {
      const std::string caseLabel = "Point2::offset with zero";

      const Point2<double> p{2.0, 3.0};
      const auto off = p.offset(0.0, 0.0);
      VERIFY(equal(off.x(), 2.0), caseLabel);
      VERIFY(equal(off.y(), 3.0), caseLabel);
   }
}


void testPoint2Scale()
{
   {
      const std::string caseLabel = "Point2::scale for float";

      const Point2<float> p{2.0f, 3.0f};
      const auto scaled = p.scale(2.5f);
      VERIFY(equal(scaled.x(), 5.0f), caseLabel);
      VERIFY(equal(scaled.y(), 7.5f), caseLabel);
   }
   {
      const std::string caseLabel = "Point2::scale for double with integer factor";

      const Point2<double> p{2.0, 3.0};
      const auto scaled = p.scale(2);
      VERIFY(equal(scaled.x(), 4.0), caseLabel);
      VERIFY(equal(scaled.y(), 6.0), caseLabel);
   }
   {
      const std::string caseLabel =
         "Point2::scale for integer with floating point factor";

      const Point2<int> p{2, 3};
      const auto scaled = p.scale(2.6);
      // Coordinates get truncated to integers.
      VERIFY(equal(scaled.x(), static_cast<int>(2 * 2.6)), caseLabel);
      VERIFY(equal(scaled.y(), static_cast<int>(3 * 2.6)), caseLabel);
   }
   {
      const std::string caseLabel = "Point2::scale with zero";

      const Point2<double> p{2.0, 3.0};
      const auto scaled = p.scale(0.0);
      VERIFY(equal(scaled.x(), 0.0), caseLabel);
      VERIFY(equal(scaled.y(), 0.0), caseLabel);
   }
}


void testPoint2Equality()
{
   {
      const std::string caseLabel = "Equality for equal float points";

      const Point2<float> a{0.011f, -345.78f};
      const Point2<float> b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for equal integer points";

      const Point2<int> a{-3, 9};
      const Point2<int> b{-3, 9};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double points whose x coordinates are unequal beyond the "
         "epsilon threshold";

      const Point2<double> a{1.23456789000000000001, 3.2};
      const Point2<double> b{1.23456789000000000002, 3.2};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double points whose y coordinates are unequal beyond the "
         "epsilon threshold";

      const Point2<double> a{3.2, 1.23456789000000000001};
      const Point2<double> b{3.2, 1.23456789000000000002};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal points";

      const Point2<float> a{0.011f, -345.78f};
      const Point2<float> b{7.6f, -2.2f};
      VERIFY(!(a == b), caseLabel);
   }
}


void testPoint2Inequality()
{
   {
      const std::string caseLabel = "Inequality for unequal float points";

      const Point2<float> a{0.011f, -345.78f};
      const Point2<float> b{0.011f, -345.0f};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal integer points";

      const Point2<int> a{-3, 9};
      const Point2<int> b{3, 9};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double points whose x coordinates are unequal beyond the "
         "epsilon threshold";

      const Point2<double> a{1.23456789000000000001, 3.2};
      const Point2<double> b{1.23456789000000000002, 3.2};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double points whose y coordinates are unequal beyond the "
         "epsilon threshold";

      const Point2<double> a{3.2, 1.23456789000000000001};
      const Point2<double> b{3.2, 1.23456789000000000002};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for equal points";

      const Point2<float> a{0.011f, -345.78f};
      const Point2<float> b = a;
      VERIFY(!(a != b), caseLabel);
   }
}

} // namespace


///////////////////

void testPoint2D()
{
   testPoint2DefaultCtor();
   testPoint2ValueCtor();
   testPoint2Offset();
   testPoint2Scale();
   testPoint2Equality();
   testPoint2Inequality();
}
