//
// geomcpp tests
// Tests for interval functionality.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "interval_tec_tests.h"
#include "interval_tec.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::tec;
using namespace sutil;


namespace
{
///////////////////

void testEndpointEqual()
{
   {
      const std::string caseLabel = "Equality for equal open integer endpoints";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(a == b, caseLabel);
      VERIFY(b == a, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for equal closed floating point endpoints";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.22};
      VERIFY(a == b, caseLabel);
      VERIFY(b == a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for open integer endpoints with different values";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{-2};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for closed floating point endpoints with different values";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.33};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for integer endpoints with different inclusion types";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> b{2};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for floating point endpoints with different inclusion types";

      const Endpoint<double, EndType<RightEnd, OpenEnd>> a{10.11};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{10.11};
      VERIFY(!(a == b), caseLabel);
      VERIFY(!(b == a), caseLabel);
   }
}


void testEndpointUnequal()
{
   {
      const std::string caseLabel = "Inequality for equal open integer endpoints";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(!(a != b), caseLabel);
      VERIFY(!(b != a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal closed floating point endpoints";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.22};
      VERIFY(!(a != b), caseLabel);
      VERIFY(!(b != a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for open integer endpoints with different values";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{-2};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for closed floating point endpoints with different values";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.33};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for integer endpoints with different inclusion types";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> b{2};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for floating point endpoints with different inclusion types";

      const Endpoint<double, EndType<RightEnd, OpenEnd>> a{10.11};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{10.11};
      VERIFY(a != b, caseLabel);
      VERIFY(b != a, caseLabel);
   }
}


void testEndpointLess()
{
   {
      const std::string caseLabel =
         "Less-than for open integer endpoints with different values";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{3};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for closed floating point endpoints with different values";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.23};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for open integer endpoints with different inclusions";

      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(a < b, caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-than for closed floating point endpoints with different inclusions";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, OpenEnd>> b{2.22};
      VERIFY(b < a, caseLabel);
      VERIFY(!(a < b), caseLabel);
   }
   {
      const std::string caseLabel = "Less-than for equal open integer endpoints";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{2};
      VERIFY(!(a < b), caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
   {
      const std::string caseLabel = "Less-than for equal closed floating point endpoints";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.45};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.45};
      VERIFY(!(a < b), caseLabel);
      VERIFY(!(b < a), caseLabel);
   }
}


void testEndpointLessEqual()
{
   {
      const std::string caseLabel =
         "Less-equal-than for open integer endpoints with different values";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{3};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for closed floating point endpoints with different values";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.23};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for open integer endpoints with different inclusions";

      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(a <= b, caseLabel);
      VERIFY(!(b <= a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for closed floating point endpoints with different inclusions";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, OpenEnd>> b{2.22};
      VERIFY(b <= a, caseLabel);
      VERIFY(!(a <= b), caseLabel);
   }
   {
      const std::string caseLabel = "Less-equal-than for equal open integer endpoints";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{2};
      VERIFY(a <= b, caseLabel);
      VERIFY(b <= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Less-equal-than for equal closed floating point endpoints";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.45};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.45};
      VERIFY(a <= b, caseLabel);
      VERIFY(b <= a, caseLabel);
   }
}


void testEndpointGreater()
{
   {
      const std::string caseLabel =
         "Greater-than for open integer endpoints with different values";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{3};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for closed floating point endpoints with different values";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.23};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for open integer endpoints with different inclusions";

      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(!(a > b), caseLabel);
      VERIFY(b > a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for closed floating point endpoints with different inclusions";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, OpenEnd>> b{2.22};
      VERIFY(!(b > a), caseLabel);
      VERIFY(a > b, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-than for equal open integer endpoints";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{2};
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(b > a), caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-than for equal closed floating point endpoints";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.45};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.45};
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(b > a), caseLabel);
   }
}


void testEndpointGreaterEqual()
{
   {
      const std::string caseLabel =
         "Greater-equal-than for open integer endpoints with different values";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{3};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for closed floating point endpoints with different values";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, ClosedEnd>> b{2.23};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for open integer endpoints with different inclusions";

      const Endpoint<int, EndType<LeftEnd, ClosedEnd>> a{2};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{2};
      VERIFY(!(a >= b), caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-equal-than for closed floating point "
                                    "endpoints with different inclusions";

      const Endpoint<double, EndType<RightEnd, ClosedEnd>> a{2.22};
      const Endpoint<double, EndType<RightEnd, OpenEnd>> b{2.22};
      VERIFY(!(b >= a), caseLabel);
      VERIFY(a >= b, caseLabel);
   }
   {
      const std::string caseLabel = "Greater-equal-than for equal open integer endpoints";

      const Endpoint<int, EndType<RightEnd, OpenEnd>> a{2};
      const Endpoint<int, EndType<RightEnd, OpenEnd>> b{2};
      VERIFY(a >= b, caseLabel);
      VERIFY(b >= a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Greater-equal-than for equal closed floating point endpoints";

      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> a{2.45};
      const Endpoint<double, EndType<LeftEnd, ClosedEnd>> b{2.45};
      VERIFY(a >= b, caseLabel);
      VERIFY(b >= a, caseLabel);
   }
}


void testEndpointOverlapping()
{
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the same orientation";

      const Endpoint<int, EndType<LeftEnd, OpenEnd>> a{-1000};
      const Endpoint<int, EndType<LeftEnd, OpenEnd>> b{30000000};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel = "overlapping() for endpoints with the different "
                                    "orientations and overlapping value ranges";

      const Endpoint<float, EndType<LeftEnd, OpenEnd>> a{1.1f};
      const Endpoint<float, EndType<RightEnd, OpenEnd>> b{3.6f};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with one endpoint closed";

      const Endpoint<float, EndType<LeftEnd, OpenEnd>> a{1};
      const Endpoint<float, EndType<RightEnd, ClosedEnd>> b{1};
      VERIFY(!overlapping(a, b), caseLabel);
      VERIFY(!overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with both endpoints closed";

      const Endpoint<float, EndType<LeftEnd, ClosedEnd>> a{1};
      const Endpoint<float, EndType<RightEnd, ClosedEnd>> b{1};
      VERIFY(overlapping(a, b), caseLabel);
      VERIFY(overlapping(b, a), caseLabel);
   }
   {
      const std::string caseLabel =
         "overlapping() for endpoints with the different "
         "orientations and touching value ranges with both endpoints open";

      const Endpoint<float, EndType<LeftEnd, OpenEnd>> a{1};
      const Endpoint<float, EndType<RightEnd, OpenEnd>> b{1};
      VERIFY(!overlapping(a, b), caseLabel);
      VERIFY(!overlapping(b, a), caseLabel);
   }
}


///////////////////

void testIntervalCtor()
{
   {
      const std::string caseLabel = "Interval ctor for double";

      Interval<double, Closed> iv(1.1, 1.2);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for float";

      Interval<float, Open> iv(1.1f, 1.2f);
      VERIFY(iv.start() == 1.1f, caseLabel);
      VERIFY(iv.end() == 1.2f, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for int";

      Interval<int, RightOpen> iv(1, 3);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor in constexpr context";

      constexpr Interval<double, LeftOpen> iv(1.1, 1.2);
      VERIFY(iv.start() == 1.1, caseLabel);
      VERIFY(iv.end() == 1.2, caseLabel);
   }
   {
      const std::string caseLabel = "Interval ctor for denormalized (reversed) values";

      Interval<int, RightOpen> iv(3, 1);
      VERIFY(iv.start() == 1, caseLabel);
      VERIFY(iv.end() == 3, caseLabel);
   }
}

void testIntervalLength()
{
   {
      const std::string caseLabel = "Interval::length";

      VERIFY(fpEqual(Interval<double, Closed>(1.1, 1.2).length(), 0.1), caseLabel);
      VERIFY((Interval<int, Open>(200, 300).length() == 100), caseLabel);
      VERIFY((Interval<char, RightOpen>(60, 72).length() == 12), caseLabel);
      VERIFY(fpEqual(Interval<float, LeftOpen>(1.1f, 1.2f).length(), 0.1f), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::length for empty interval";

      VERIFY(fpEqual(Interval<double, Closed>(1.1, 1.1).length(), 0.0), caseLabel);
      VERIFY((Interval<int, Open>(200, 200).length() == 0), caseLabel);
      VERIFY((EmptyInterval<float>.length() == 0.0), caseLabel);
   }
}


void testIntervalIsEmpty()
{
   {
      const std::string caseLabel = "Interval::isEmpty for closed interval";

      VERIFY((!Interval<int, Closed>(200, 300).isEmpty()), caseLabel);
      VERIFY((!Interval<double, Closed>(20.2, 20.2).isEmpty()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::isEmpty for open interval";

      VERIFY((!Interval<int, Open>(200, 300).isEmpty()), caseLabel);
      VERIFY((Interval<double, Open>(20.2, 20.2).isEmpty()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::isEmpty for half-open interval";

      VERIFY((!Interval<int, RightOpen>(200, 300).isEmpty()), caseLabel);
      VERIFY((Interval<double, RightOpen>(20.2, 20.2).isEmpty()), caseLabel);
      VERIFY((!Interval<int, LeftOpen>(200, 300).isEmpty()), caseLabel);
      VERIFY((Interval<double, LeftOpen>(20.2, 20.2).isEmpty()), caseLabel);
   }
}


void testIntervalContains()
{
   {
      const std::string caseLabel = "Interval::contains for closed interval";

      VERIFY((Interval<int, Closed>(200, 300).contains(250)), caseLabel);
      VERIFY((!Interval<int, Closed>(200, 300).contains(400)), caseLabel);
      VERIFY((!Interval<int, Closed>(200, 300).contains(199)), caseLabel);
      VERIFY((Interval<int, Closed>(200, 300).contains(200)), caseLabel);
      VERIFY((Interval<int, Closed>(200, 300).contains(300)), caseLabel);
      VERIFY((Interval<double, Closed>(20.2, 20.3).contains(20.23)), caseLabel);
      VERIFY((!Interval<double, Closed>(20.2, 20.3).contains(20.31)), caseLabel);
      VERIFY((!Interval<double, Closed>(20.2, 20.3).contains(20.19)), caseLabel);
      VERIFY((Interval<double, Closed>(20.2, 20.3).contains(20.2)), caseLabel);
      VERIFY((Interval<double, Closed>(20.2, 20.3).contains(20.3)), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for open interval";

      VERIFY((Interval<int, Open>(200, 300).contains(250)), caseLabel);
      VERIFY((!Interval<int, Open>(200, 300).contains(400)), caseLabel);
      VERIFY((!Interval<int, Open>(200, 300).contains(199)), caseLabel);
      VERIFY((!Interval<int, Open>(200, 300).contains(200)), caseLabel);
      VERIFY((!Interval<int, Open>(200, 300).contains(300)), caseLabel);
      VERIFY((Interval<double, Open>(20.2, 20.3).contains(20.23)), caseLabel);
      VERIFY((!Interval<double, Open>(20.2, 20.3).contains(20.31)), caseLabel);
      VERIFY((!Interval<double, Open>(20.2, 20.3).contains(20.19)), caseLabel);
      VERIFY((!Interval<double, Open>(20.2, 20.3).contains(20.2)), caseLabel);
      VERIFY((!Interval<double, Open>(20.2, 20.3).contains(20.3)), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for left-open interval";

      VERIFY((Interval<long, LeftOpen>(200, 300).contains(250)), caseLabel);
      VERIFY((!Interval<long, LeftOpen>(200, 300).contains(400)), caseLabel);
      VERIFY((!Interval<long, LeftOpen>(200, 300).contains(199)), caseLabel);
      VERIFY((!Interval<long, LeftOpen>(200, 300).contains(200)), caseLabel);
      VERIFY((Interval<long, LeftOpen>(200, 300).contains(300)), caseLabel);
      VERIFY((Interval<float, LeftOpen>(20.2f, 20.3f).contains(20.23f)), caseLabel);
      VERIFY((!Interval<float, LeftOpen>(20.2f, 20.3f).contains(20.31f)), caseLabel);
      VERIFY((!Interval<float, LeftOpen>(20.2f, 20.3f).contains(20.19f)), caseLabel);
      VERIFY((!Interval<float, LeftOpen>(20.2f, 20.3f).contains(20.2f)), caseLabel);
      VERIFY((Interval<float, LeftOpen>(20.2f, 20.3f).contains(20.3f)), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::contains for right-open interval";

      VERIFY((Interval<short, RightOpen>(200, 300).contains(250)), caseLabel);
      VERIFY((!Interval<short, RightOpen>(200, 300).contains(400)), caseLabel);
      VERIFY((!Interval<short, RightOpen>(200, 300).contains(199)), caseLabel);
      VERIFY((Interval<short, RightOpen>(200, 300).contains(200)), caseLabel);
      VERIFY((!Interval<short, RightOpen>(200, 300).contains(300)), caseLabel);
      VERIFY((Interval<float, RightOpen>(20.2f, 20.3f).contains(20.23f)), caseLabel);
      VERIFY((!Interval<float, RightOpen>(20.2f, 20.3f).contains(20.31f)), caseLabel);
      VERIFY((!Interval<float, RightOpen>(20.2f, 20.3f).contains(20.19f)), caseLabel);
      VERIFY((Interval<float, RightOpen>(20.2f, 20.3f).contains(20.2f)), caseLabel);
      VERIFY((!Interval<float, RightOpen>(20.2f, 20.3f).contains(20.3f)), caseLabel);
   }
}


void testIntervalLeftEnd()
{
   {
      const std::string caseLabel = "Interval::leftEndType";

      VERIFY((Interval<short, Closed>(200, 300).leftEndType() == IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float, Open>(200.1f, 300.2f).leftEndType() == IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).leftEndType() == IntervalEnd::Open),
             caseLabel);
      VERIFY(
         (Interval<double, RightOpen>(200.0, 300.3).leftEndType() == IntervalEnd::Closed),
         caseLabel);
   }
}


void testIntervalRightEnd()
{
   {
      const std::string caseLabel = "Interval::rightEndType";

      VERIFY((Interval<short, Closed>(200, 300).rightEndType() == IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float, Open>(200.1f, 300.2f).rightEndType() == IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).rightEndType() == IntervalEnd::Closed),
             caseLabel);
      VERIFY(
         (Interval<double, RightOpen>(200.0, 300.3).rightEndType() == IntervalEnd::Open),
         caseLabel);
   }
}


void testIntervalType()
{
   {
      const std::string caseLabel = "Interval::type";

      VERIFY((Interval<short, Closed>(200, 300).type() == IntervalType::Closed),
             caseLabel);
      VERIFY((Interval<float, Open>(200.1f, 300.2f).type() == IntervalType::Open),
             caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).type() == IntervalType::LeftOpen),
             caseLabel);
      VERIFY(
         (Interval<double, RightOpen>(200.0, 300.3).type() == IntervalType::RightOpen),
         caseLabel);
   }
}


void testIntervalOperatorBool()
{
   {
      const std::string caseLabel = "Interval::operator bool() for closed interval";

      VERIFY((Interval<int, Closed>(200, 300).operator bool()), caseLabel);
      VERIFY((Interval<double, Closed>(20.2, 20.2).operator bool()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator bool() for open interval";

      VERIFY((Interval<int, Open>(200, 300).operator bool()), caseLabel);
      VERIFY((!Interval<double, Open>(20.2, 20.2).operator bool()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator bool() for half-open interval";

      VERIFY((Interval<int, RightOpen>(200, 300).operator bool()), caseLabel);
      VERIFY((!Interval<double, RightOpen>(20.2, 20.2).operator bool()), caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).operator bool()), caseLabel);
      VERIFY((!Interval<double, LeftOpen>(20.2, 20.2).operator bool()), caseLabel);
   }
}


void testIntervalNegationOperator()
{
   {
      const std::string caseLabel = "Interval::operator!() for closed interval";

      VERIFY((!Interval<int, Closed>(200, 300).operator!()), caseLabel);
      VERIFY((!Interval<double, Closed>(20.2, 20.2).operator!()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator!() for open interval";

      VERIFY((!Interval<int, Open>(200, 300).operator!()), caseLabel);
      VERIFY((Interval<double, Open>(20.2, 20.2).operator!()), caseLabel);
   }
   {
      const std::string caseLabel = "Interval::operator!() for half-open interval";

      VERIFY((!Interval<int, RightOpen>(200, 300).operator!()), caseLabel);
      VERIFY((Interval<double, RightOpen>(20.2, 20.2).operator!()), caseLabel);
      VERIFY((!Interval<int, LeftOpen>(200, 300).operator!()), caseLabel);
      VERIFY((Interval<double, LeftOpen>(20.2, 20.2).operator!()), caseLabel);
   }
}


void testIntersect()
{
   {
      const std::string caseLabel = "Intersect [1, 10] and [20, 22]";

      const Interval<int, Closed> a{1, 10};
      const Interval<int, Open> b{20, 22};

      const SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<int>>(res), caseLabel);

      const OpenInterval<int> resIv = std::get<OpenInterval<int>>(res);
      VERIFY(resIv.isEmpty(), caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<OpenInterval<int>>(resFlipped), caseLabel);

      const OpenInterval<int> resIvFlipped = std::get<OpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.isEmpty(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect disjoint intervals that have the same "
                                    "endpoint but the endpoint is open for one interval";

      const Interval<int, RightOpen> a{1, 10};
      const Interval<int, Closed> b{10, 22};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<int>>(res), caseLabel);

      const OpenInterval<int> resIv = std::get<OpenInterval<int>>(res);
      VERIFY(resIv.isEmpty(), caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<OpenInterval<int>>(resFlipped), caseLabel);

      const OpenInterval<int> resIvFlipped = std::get<OpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.isEmpty(), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with fully contained open interval";

      const Interval<double, Closed> a{1.0, 10.4};
      const Interval<double, Open> b{2.1, 8.09};

      SomeInterval<double> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<double>>(res), caseLabel);

      const auto resIv = std::get<OpenInterval<double>>(res);
      VERIFY(resIv.start() == 2.1, caseLabel);
      VERIFY(resIv.end() == 8.09, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<double> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<OpenInterval<double>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<OpenInterval<double>>(resFlipped);
      VERIFY(resIvFlipped.start() == 2.1, caseLabel);
      VERIFY(resIvFlipped.end() == 8.09, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with fully contained left-open interval";

      const Interval<int, Open> a{100, 200};
      const Interval<int, LeftOpen> b{120, 150};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(res), caseLabel);

      const auto resIv = std::get<LeftOpenInterval<int>>(res);
      VERIFY(resIv.start() == 120, caseLabel);
      VERIFY(resIv.end() == 150, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<LeftOpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.start() == 120, caseLabel);
      VERIFY(resIvFlipped.end() == 150, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with fully contained right-open interval";

      const Interval<int, Open> a{100, 200};
      const Interval<int, RightOpen> b{120, 150};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<RightOpenInterval<int>>(res), caseLabel);

      const auto resIv = std::get<RightOpenInterval<int>>(res);
      VERIFY(resIv.start() == 120, caseLabel);
      VERIFY(resIv.end() == 150, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<RightOpenInterval<int>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<RightOpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.start() == 120, caseLabel);
      VERIFY(resIvFlipped.end() == 150, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with fully contained closed interval";

      const Interval<float, Closed> a{100.1f, 200.12f};
      const Interval<float, Closed> b{120.1f, 150.12f};

      SomeInterval<float> res = intersect(a, b);

      VERIFY(std::holds_alternative<ClosedInterval<float>>(res), caseLabel);

      const auto resIv = std::get<ClosedInterval<float>>(res);
      VERIFY(resIv.start() == 120.1f, caseLabel);
      VERIFY(resIv.end() == 150.12f, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<float> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<ClosedInterval<float>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<ClosedInterval<float>>(resFlipped);
      VERIFY(resIvFlipped.start() == 120.1f, caseLabel);
      VERIFY(resIvFlipped.end() == 150.12f, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect overlapping open intervals";

      const Interval<double, Open> a{1.0, 10.4};
      const Interval<double, Open> b{-2.1, 8.09};

      SomeInterval<double> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<double>>(res), caseLabel);

      const auto resIv = std::get<OpenInterval<double>>(res);
      VERIFY(resIv.start() == 1.0, caseLabel);
      VERIFY(resIv.end() == 8.09, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<double> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<OpenInterval<double>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<OpenInterval<double>>(resFlipped);
      VERIFY(resIvFlipped.start() == 1.0, caseLabel);
      VERIFY(resIvFlipped.end() == 8.09, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect overlapping closed intervals";

      const Interval<double, Closed> a{-10.0, -0.4};
      const Interval<double, Closed> b{-1.1, 8.09};

      SomeInterval<double> res = intersect(a, b);

      VERIFY(std::holds_alternative<ClosedInterval<double>>(res), caseLabel);

      const auto resIv = std::get<ClosedInterval<double>>(res);
      VERIFY(resIv.start() == -1.1, caseLabel);
      VERIFY(resIv.end() == -0.4, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<double> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<ClosedInterval<double>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<ClosedInterval<double>>(resFlipped);
      VERIFY(resIvFlipped.start() == -1.1, caseLabel);
      VERIFY(resIvFlipped.end() == -0.4, caseLabel);
   }
   {
      const std::string caseLabel =
         "Intersect overlapping closed and left-open intervals";

      const Interval<int, Closed> a{0, 10};
      const Interval<int, LeftOpen> b{2, 12};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(res), caseLabel);

      const auto resIv = std::get<LeftOpenInterval<int>>(res);
      VERIFY(resIv.start() == 2, caseLabel);
      VERIFY(resIv.end() == 10, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<LeftOpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.start() == 2, caseLabel);
      VERIFY(resIvFlipped.end() == 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect overlapping open and right-open intervals";

      const Interval<int, Open> a{2, 12};
      const Interval<int, RightOpen> b{0, 10};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<int>>(res), caseLabel);

      const auto resIv = std::get<OpenInterval<int>>(res);
      VERIFY(resIv.start() == 2, caseLabel);
      VERIFY(resIv.end() == 10, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<OpenInterval<int>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<OpenInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.start() == 2, caseLabel);
      VERIFY(resIvFlipped.end() == 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect overlapping left- and right-open "
                                    "intervals creating a closed interval";

      const Interval<int, LeftOpen> a{0, 10};
      const Interval<int, RightOpen> b{2, 12};

      SomeInterval<int> res = intersect(a, b);

      VERIFY(std::holds_alternative<ClosedInterval<int>>(res), caseLabel);

      const auto resIv = std::get<ClosedInterval<int>>(res);
      VERIFY(resIv.start() == 2, caseLabel);
      VERIFY(resIv.end() == 10, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<ClosedInterval<int>>(resFlipped), caseLabel);

      const auto resIvFlipped = std::get<ClosedInterval<int>>(resFlipped);
      VERIFY(resIvFlipped.start() == 2, caseLabel);
      VERIFY(resIvFlipped.end() == 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect intervals that have the same start value "
                                    "but one's left end is closed";

      const Interval<int, Open> a{2, 12};
      const Interval<int, Closed> b{2, 10};

      SomeInterval<int> res = intersect(a, b);

      // The left end of the intersection should be open because when the start values
      // are equal the open interval is evaluates to 'greater' when compared.
      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(res), caseLabel);

      const auto resIv = std::get<LeftOpenInterval<int>>(res);
      VERIFY(resIv.start() == 2, caseLabel);
      VERIFY(resIv.end() == 10, caseLabel);

      // Flip the arguments. The outcome should be he same.
      SomeInterval<int> resFlipped = intersect(b, a);

      VERIFY(std::holds_alternative<LeftOpenInterval<int>>(resFlipped), caseLabel);

      const auto resIvFLipped = std::get<LeftOpenInterval<int>>(resFlipped);
      VERIFY(resIvFLipped.start() == 2, caseLabel);
      VERIFY(resIvFLipped.end() == 10, caseLabel);
   }
}


void testUnite()
{
}

} // namespace


///////////////////

void testTecInterval()
{
   testEndpointEqual();
   testEndpointUnequal();
   testEndpointLess();
   testEndpointLessEqual();
   testEndpointGreater();
   testEndpointGreaterEqual();
   testEndpointOverlapping();

   testIntervalCtor();
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
