//
// geomcpp tests
// Tests for data-encoded intervals.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "interval_dec_tests.h"
#include "interval_dec.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::dec;
using namespace sutil;


namespace
{

///////////////////

void testEndpointEqual()
{
   {
      const std::string caseLabel = "Equality for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(a == b, caseLabel);
      VERIFY(b == a, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for equal closed floating point endpoints";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(a == b, caseLabel);
      VERIFY(b == a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{-2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.33, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for integer endpoints with different inclusion types";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for floating point endpoints with different inclusion types";

      const Endpoint<double> a{10.11, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<double> b{10.11, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
}


void testEndpointUnequal()
{
   {
      const std::string caseLabel = "Inequality for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(!(a != b), caseLabel);
      VERIFY(!(b != a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal closed floating point endpoints";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(!(a != b), caseLabel);
      VERIFY(!(b != a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{-2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.33, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for integer endpoints with different inclusion types";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for floating point endpoints with different inclusion types";

      const Endpoint<double> a{10.11, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<double> b{10.11, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
}


void testEndpointLess()
{
   {
      const std::string caseLabel =
         "Less-than for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{3, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.23, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for open integer endpoints with different inclusions";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for closed floating point endpoints with different inclusions";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(b < a, caseLabel);
      VERIFY(!(a < b), caseLabel);
   }
   {
      const std::string caseLabel = "Less-than for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!(a < b), caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel = "Less-than for equal closed floating point endpoints";

      const Endpoint<double> a{2.45, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.45, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(!(a < b), caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
}


void testEndpointLessEqual()
{
   {
      const std::string caseLabel =
         "Less-equal-than for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{3, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.23, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for open integer endpoints with different inclusions";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for closed floating point endpoints with different inclusions";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(b <= a, caseLabel);
      VERIFY(!(a <= b), caseLabel);
   }
   {
      const std::string caseLabel = "Less-equal-than for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(a <= b, caseLabel);
      VERIFY(b <= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for equal closed floating point endpoints";

      const Endpoint<double> a{2.45, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.45, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(a <= b, caseLabel);
      VERIFY(b <= a, caseLabel);
   }
}


void testEndpointGreater()
{
   {
      const std::string caseLabel =
         "Greater-than for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{3, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.23, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for open integer endpoints with different inclusions";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for closed floating point endpoints with different inclusions";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!(b > a), caseLabel);
      VERIFY(a > b, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-than for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(b > a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for equal closed floating point endpoints";

      const Endpoint<double> a{2.45, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.45, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(b > a), caseLabel);
   }
}


void testEndpointGreaterEqual()
{
   {
      const std::string caseLabel =
         "Greater-equal-than for open integer endpoints with different values";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{3, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for closed floating point endpoints with different values";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.23, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for open integer endpoints with different inclusions";

      const Endpoint<int> a{2, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<int> b{2, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-equal-than for closed floating point "
                                    "endpoints with different inclusions";

      const Endpoint<double> a{2.22, IntervalSide::Right, IntervalEnd::Closed};
      const Endpoint<double> b{2.22, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!(b >= a), caseLabel);
      VERIFY(a >= b, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-equal-than for equal open integer endpoints";

      const Endpoint<int> a{2, IntervalSide::Right, IntervalEnd::Open};
      const Endpoint<int> b{2, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(a >= b, caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for equal closed floating point endpoints";

      const Endpoint<double> a{2.45, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<double> b{2.45, IntervalSide::Left, IntervalEnd::Closed};
      VERIFY(a >= b, caseLabel);
      VERIFY(b >= a, caseLabel);
   }
}


void testEndpointOverlapping()
{
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the same orientation";

      const Endpoint<int> a{-1000, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<int> b{30000000, IntervalSide::Left, IntervalEnd::Open};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel = "overlapping() for endpoints with the different "
                                    "orientations and overlapping value ranges";

      const Endpoint<float> a{1.1f, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<float> b{3.6f, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with one endpoint closed";

      const Endpoint<float> a{1, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<float> b{1, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(!overlapping(a, b), caseLabel);
      VERIFY(!overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with both endpoints closed";

      const Endpoint<float> a{1, IntervalSide::Left, IntervalEnd::Closed};
      const Endpoint<float> b{1, IntervalSide::Right, IntervalEnd::Closed};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with both endpoints open";

      const Endpoint<float> a{1, IntervalSide::Left, IntervalEnd::Open};
      const Endpoint<float> b{1, IntervalSide::Right, IntervalEnd::Open};
      VERIFY(!overlapping(a, b), caseLabel);
      VERIFY(!overlapping(b, a), caseLabel);
   }
}


///////////////////

void testIntervalCtorForIntervalType()
{
   {
      const std::string caseLabel = "Interval type ctor for double";

      Interval<double> iv(1.1, 1.2, IntervalType::Closed);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Closed, caseLabel);
   }
   {
      const std::string caseLabel = "Interval type ctor for float";

      Interval<float> iv(1.1f, 1.2f, IntervalType::Open);
      VERIFY(iv.start() == 1.1f, caseLabel);
      VERIFY(iv.end() == 1.2f, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Open, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for int";

      Interval<int> iv(1, 3, IntervalType::RightOpen);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor in constexpr context";

      constexpr Interval<double> iv(1.1, 1.2, IntervalType::LeftOpen);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Open, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Closed, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for denormalized (reversed) values";

      Interval<int> iv(3, 1, IntervalType::RightOpen);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
}


void testIntervalCtorForEndTypes()
{
   {
      const std::string caseLabel = "Interval end types ctor for double";

      Interval<double> iv(1.1, 1.2, IntervalEnd::Closed, IntervalEnd::Closed);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Closed, caseLabel);
   }
   {
      const std::string caseLabel = "Interval type ctor for float";

      Interval<float> iv(1.1f, 1.2f, IntervalEnd::Open, IntervalEnd::Open);
      VERIFY(iv.start() == 1.1f, caseLabel);
      VERIFY(iv.end() == 1.2f, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Open, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for int";

      Interval<int> iv(1, 3, IntervalEnd::Closed, IntervalEnd::Open);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor in constexpr context";

      constexpr Interval<double> iv(1.1, 1.2, IntervalEnd::Open, IntervalEnd::Closed);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Open, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Closed, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for denormalized (reversed) values";

      Interval<int> iv(3, 1, IntervalEnd::Closed, IntervalEnd::Open);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
      VERIFY(iv.leftEndType() == IntervalEnd::Closed, caseLabel);
      VERIFY(iv.rightEndType() == IntervalEnd::Open, caseLabel);
   }
}


void testIntervalLength()
{
   {
      const std::string caseLabel = "Interval::length";

      VERIFY(fpEqual(Interval<double>(1.1, 1.2, IntervalType::Closed).length(), 0.1),
             caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::Open).length() == 100), caseLabel);
      VERIFY((Interval<char>(60, 72, IntervalType::RightOpen).length() == 12), caseLabel);
      VERIFY(fpEqual(Interval<float>(1.1f, 1.2f, IntervalType::LeftOpen).length(), 0.1f),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::length for empty interval";

      VERIFY(fpEqual(Interval<double>(1.1, 1.1, IntervalType::Closed).length(), 0.0),
             caseLabel);
      VERIFY((Interval<int>(200, 200, IntervalType::Open).length() == 0), caseLabel);
      VERIFY((EmptyInterval<float>.length() == 0.0), caseLabel);
   }
}


void testIntervalIsEmpty()
{
   {
      const std::string caseLabel = "Interval::isEmpty for closed interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::Closed).isEmpty()), caseLabel);
      VERIFY((!Interval<double>(20.2, 20.2, IntervalType::Closed).isEmpty()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::isEmpty for open interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::Open).isEmpty()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::Open).isEmpty()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::isEmpty for half-open interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::RightOpen).isEmpty()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::RightOpen).isEmpty()),
             caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::LeftOpen).isEmpty()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::LeftOpen).isEmpty()), caseLabel);
   }
}


void testIntervalContains()
{
   {
      const std::string caseLabel = "Interval::contains for closed interval";

      VERIFY((Interval<int>(200, 300, IntervalType::Closed).contains(250)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Closed).contains(400)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Closed).contains(199)), caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::Closed).contains(200)), caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::Closed).contains(300)), caseLabel);
      VERIFY((Interval<double>(20.2, 20.3, IntervalType::Closed).contains(20.23)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Closed).contains(20.31)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Closed).contains(20.19)),
             caseLabel);
      VERIFY((Interval<double>(20.2, 20.3, IntervalType::Closed).contains(20.2)),
             caseLabel);
      VERIFY((Interval<double>(20.2, 20.3, IntervalType::Closed).contains(20.3)),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for open interval";

      VERIFY((Interval<int>(200, 300, IntervalType::Open).contains(250)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Open).contains(400)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Open).contains(199)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Open).contains(200)), caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::Open).contains(300)), caseLabel);
      VERIFY((Interval<double>(20.2, 20.3, IntervalType::Open).contains(20.23)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Open).contains(20.31)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Open).contains(20.19)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Open).contains(20.2)),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.3, IntervalType::Open).contains(20.3)),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for left-open interval";

      VERIFY((Interval<long>(200, 300, IntervalType::LeftOpen).contains(250)), caseLabel);
      VERIFY((!Interval<long>(200, 300, IntervalType::LeftOpen).contains(400)),
             caseLabel);
      VERIFY((!Interval<long>(200, 300, IntervalType::LeftOpen).contains(199)),
             caseLabel);
      VERIFY((!Interval<long>(200, 300, IntervalType::LeftOpen).contains(200)),
             caseLabel);
      VERIFY((Interval<long>(200, 300, IntervalType::LeftOpen).contains(300)), caseLabel);
      VERIFY((Interval<float>(20.2f, 20.3f, IntervalType::LeftOpen).contains(20.23f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::LeftOpen).contains(20.31f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::LeftOpen).contains(20.19f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::LeftOpen).contains(20.2f)),
             caseLabel);
      VERIFY((Interval<float>(20.2f, 20.3f, IntervalType::LeftOpen).contains(20.3f)),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for right-open interval";

      VERIFY((Interval<short>(200, 300, IntervalType::RightOpen).contains(250)),
             caseLabel);
      VERIFY((!Interval<short>(200, 300, IntervalType::RightOpen).contains(400)),
             caseLabel);
      VERIFY((!Interval<short>(200, 300, IntervalType::RightOpen).contains(199)),
             caseLabel);
      VERIFY((Interval<short>(200, 300, IntervalType::RightOpen).contains(200)),
             caseLabel);
      VERIFY((!Interval<short>(200, 300, IntervalType::RightOpen).contains(300)),
             caseLabel);
      VERIFY((Interval<float>(20.2f, 20.3f, IntervalType::RightOpen).contains(20.23f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::RightOpen).contains(20.31f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::RightOpen).contains(20.19f)),
             caseLabel);
      VERIFY((Interval<float>(20.2f, 20.3f, IntervalType::RightOpen).contains(20.2f)),
             caseLabel);
      VERIFY((!Interval<float>(20.2f, 20.3f, IntervalType::RightOpen).contains(20.3f)),
             caseLabel);
   }
}


void testIntervalLeftEnd()
{
   {
      const std::string caseLabel = "Interval::leftEndType";

      VERIFY((Interval<short>(200, 300, IntervalType::Closed).leftEndType() ==
              IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float>(200.1f, 300.2f, IntervalType::Open).leftEndType() ==
              IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::LeftOpen).leftEndType() ==
              IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<double>(200.0, 300.3, IntervalType::RightOpen).leftEndType() ==
              IntervalEnd::Closed),
             caseLabel);
   }
}


void testIntervalRightEnd()
{
   {
      const std::string caseLabel = "Interval::rightEndType";

      VERIFY((Interval<short>(200, 300, IntervalType::Closed).rightEndType() ==
              IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float>(200.1f, 300.2f, IntervalType::Open).rightEndType() ==
              IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::LeftOpen).rightEndType() ==
              IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<double>(200.0, 300.3, IntervalType::RightOpen).rightEndType() ==
              IntervalEnd::Open),
             caseLabel);
   }
}


void testIntervalType()
{
   {
      const std::string caseLabel = "Interval::type";

      VERIFY(
         (Interval<short>(200, 300, IntervalType::Closed).type() == IntervalType::Closed),
         caseLabel);
      VERIFY((Interval<float>(200.1f, 300.2f, IntervalType::Open).type() ==
              IntervalType::Open),
             caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::LeftOpen).type() ==
              IntervalType::LeftOpen),
             caseLabel);
      VERIFY((Interval<double>(200.0, 300.3, IntervalType::RightOpen).type() ==
              IntervalType::RightOpen),
             caseLabel);
   }
}


void testIntervalOperatorBool()
{
   {
      const std::string caseLabel = "Interval::operator bool() for closed interval";

      VERIFY((Interval<int>(200, 300, IntervalType::Closed).operator bool()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::Closed).operator bool()),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator bool() for open interval";

      VERIFY((Interval<int>(200, 300, IntervalType::Open).operator bool()), caseLabel);
      VERIFY((!Interval<double>(20.2, 20.2, IntervalType::Open).operator bool()),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator bool() for half-open interval";

      VERIFY((Interval<int>(200, 300, IntervalType::RightOpen).operator bool()),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.2, IntervalType::RightOpen).operator bool()),
             caseLabel);
      VERIFY((Interval<int>(200, 300, IntervalType::LeftOpen).operator bool()),
             caseLabel);
      VERIFY((!Interval<double>(20.2, 20.2, IntervalType::LeftOpen).operator bool()),
             caseLabel);
   }
}


void testIntervalNegationOperator()
{
   {
      const std::string caseLabel = "Interval::operator!() for closed interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::Closed).operator!()), caseLabel);
      VERIFY((!Interval<double>(20.2, 20.2, IntervalType::Closed).operator!()),
             caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator!() for open interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::Open).operator!()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::Open).operator!()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator!() for half-open interval";

      VERIFY((!Interval<int>(200, 300, IntervalType::RightOpen).operator!()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::RightOpen).operator!()),
             caseLabel);
      VERIFY((!Interval<int>(200, 300, IntervalType::LeftOpen).operator!()), caseLabel);
      VERIFY((Interval<double>(20.2, 20.2, IntervalType::LeftOpen).operator!()),
             caseLabel);
   }
}

void testIntersect()
{
   {
      const std::string caseLabel = "Intersect [1, 10] and [20, 22]";

      const Interval<int> a{1, 10, IntervalType::Closed};
      const Interval<int> b{20, 22, IntervalType::Closed};

      VERIFY(intersect(a, b).isEmpty(), caseLabel);
      VERIFY(intersect(b, a).isEmpty(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and (10, 22]";

      const Interval<int> a{1, 10, IntervalType::Closed};
      const Interval<int> b{10, 22, IntervalType::LeftOpen};

      VERIFY(intersect(a, b).isEmpty(), caseLabel);
      VERIFY(intersect(b, a).isEmpty(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10) and [10, 22]";

      const Interval<int> a{1, 10, IntervalType::RightOpen};
      const Interval<int> b{10, 22, IntervalType::Closed};

      VERIFY(intersect(a, b).isEmpty(), caseLabel);
      VERIFY(intersect(b, a).isEmpty(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and [10, 22]";

      const Interval<int> a{1, 10, IntervalType::Closed};
      const Interval<int> b{10, 22, IntervalType::Closed};

      const Interval<int> expected{10, 10, IntervalType::Closed};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and [5, 8]";

      const Interval<int> a{1, 10, IntervalType::Closed};
      const Interval<int> b{5, 8, IntervalType::Closed};

      const Interval<int> expected{5, 8, IntervalType::Closed};
      VERIFY(intersect(a, b) == expected , caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 8)";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 8, IntervalType::RightOpen};

      const Interval<int> expected{5, 8, IntervalType::RightOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 10)";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 10, IntervalType::RightOpen};

      const Interval<int> expected{5, 10, IntervalType::RightOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 10]";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 10, IntervalType::Closed};

      const Interval<int> expected{5, 10, IntervalType::Closed};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and [5, 10)";

      const Interval<int> a{1, 10, IntervalType::Open};
      const Interval<int> b{5, 10, IntervalType::RightOpen};

      const Interval<int> expected{5, 10, IntervalType::RightOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and (5, 12)";

      const Interval<int> a{1, 10, IntervalType::Open};
      const Interval<int> b{5, 12, IntervalType::Open};

      const Interval<int> expected{5, 10, IntervalType::Open};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and (5, 12)";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 12, IntervalType::Open};

      const Interval<int> expected{5, 10, IntervalType::LeftOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 12)";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 12, IntervalType::RightOpen};

      const Interval<int> expected{5, 10, IntervalType::Closed};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and [5, 12)";

      const Interval<int> a{1, 10, IntervalType::Open};
      const Interval<int> b{5, 12, IntervalType::RightOpen};

      const Interval<int> expected{5, 10, IntervalType::RightOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and (1, 10)";

      const Interval<int> a{1, 10, IntervalType::Open};
      const Interval<int> b{1, 10, IntervalType::Open};

      const Interval<int> expected{1, 10, IntervalType::Open};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (-1.11, 10.78] and (5.01, 12.83)";

      const Interval<double> a{-1.11, 10.78, IntervalType::LeftOpen};
      const Interval<double> b{5.01, 12.83, IntervalType::Open};

      const Interval<double> expected{5.01, 10.78, IntervalType::LeftOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [-10.45, -1.003] and (-8.7, -2.0]";

      const Interval<double> a{-10.45, -1.003, IntervalType::Closed};
      const Interval<double> b{-8.7, -2.0, IntervalType::LeftOpen};

      const Interval<double> expected{-8.7, -2.0, IntervalType::LeftOpen};
      VERIFY(intersect(a, b) == expected, caseLabel);
      VERIFY(intersect(b, a) == expected, caseLabel);
   }
}


void testUnite()
{
   {
      const std::string caseLabel = "Unite [1, 10] and [20, 22]";

      const Interval<int> a{1, 10, IntervalType::Closed};
      const Interval<int> b{20, 22, IntervalType::Closed};

      const Interval<int> expected{1, 22, IntervalType::Closed};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite (1, 10] and [20, 22)";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{20, 22, IntervalType::RightOpen};

      const Interval<int> expected{1, 22, IntervalType::Open};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite (1, 10] and (20, 22]";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{20, 22, IntervalType::LeftOpen};

      const Interval<int> expected{1, 22, IntervalType::LeftOpen};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite (1, 10] and (5, 22]";

      const Interval<int> a{1, 10, IntervalType::LeftOpen};
      const Interval<int> b{5, 22, IntervalType::LeftOpen};

      const Interval<int> expected{1, 22, IntervalType::LeftOpen};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite [-1, 10] and (5, 22)";

      const Interval<int> a{-1, 10, IntervalType::Closed};
      const Interval<int> b{5, 22, IntervalType::Open};

      const Interval<int> expected{-1, 22, IntervalType::RightOpen};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite [-1, 30] and (5, 22)";

      const Interval<int> a{-1, 30, IntervalType::Closed};
      const Interval<int> b{5, 22, IntervalType::Open};

      const Interval<int> expected{-1, 30, IntervalType::Closed};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite [-1, 22] and (5, 22)";

      const Interval<int> a{-1, 22, IntervalType::Closed};
      const Interval<int> b{5, 22, IntervalType::Open};

      const Interval<int> expected{-1, 22, IntervalType::Closed};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite [-1, 22) and (5, 22)";

      const Interval<int> a{-1, 22, IntervalType::RightOpen};
      const Interval<int> b{5, 22, IntervalType::Open};

      const Interval<int> expected{-1, 22, IntervalType::RightOpen};
      VERIFY(unite(a, b) == expected, caseLabel);
      VERIFY(unite(b, a) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite [-1.1, 22.2) and (-1.1, 22.2)";

      const Interval<double> a{-1.1, 22.2, IntervalType::RightOpen};
      const Interval<double> b{-1.1, 22.2, IntervalType::Open};

      const Interval<double> expected{-1.1, 22.2, IntervalType::RightOpen};
      VERIFY(unite(b, a) == expected, caseLabel);
      VERIFY(unite(a, b) == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Unite (-1.1, 22.2) and (-1.1, 22.2]";

      const Interval<double> a{-1.1, 22.2, IntervalType::Open};
      const Interval<double> b{-1.1, 22.2, IntervalType::LeftOpen};

      const Interval<double> expected{-1.1, 22.2, IntervalType::LeftOpen};
      VERIFY(unite(b, a) == expected, caseLabel);
      VERIFY(unite(a, b) == expected, caseLabel);
   }
}

} // namespace


///////////////////

void testDecInterval()
{
   testEndpointEqual();
   testEndpointUnequal();
   testEndpointLess();
   testEndpointLessEqual();
   testEndpointGreater();
   testEndpointGreaterEqual();
   testEndpointOverlapping();

   testIntervalCtorForIntervalType();
   testIntervalCtorForEndTypes();
   testIntervalLength();
   testIntervalIsEmpty();
   testIntervalContains();
   testIntervalLeftEnd();
   testIntervalRightEnd();
   testIntervalType();
   testIntervalOperatorBool();
   testIntervalNegationOperator();

   testIntersect();
   testUnite();
}
