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

template <typename IvType, typename Value>
void verifyInterval(const SomeInterval<Value>& ivVar, Value start, Value end,
                    const std::string& caseLabel)
{
   const bool hasExpectedType = std::holds_alternative<IvType>(ivVar);
   VERIFY(hasExpectedType, caseLabel);
   if (hasExpectedType)
   {
      const auto iv = std::get<IvType>(ivVar);
      VERIFY(iv.start() == start, caseLabel);
      VERIFY(iv.end() == end, caseLabel);
   }
}


template <typename IvType, typename Value>
void verifyEmptyInterval(const SomeInterval<Value>& ivVar, const std::string& caseLabel)
{
   const bool hasExpectedType = std::holds_alternative<IvType>(ivVar);
   VERIFY(hasExpectedType, caseLabel);
   if (hasExpectedType)
   {
      const auto iv = std::get<IvType>(ivVar);
      VERIFY(iv.isEmpty(), caseLabel);
   }
}


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

      verifyEmptyInterval<OpenInterval<int>>(intersect(a, b), caseLabel);
      verifyEmptyInterval<OpenInterval<int>>(intersect(b, a), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and (10, 22]";

      const Interval<int, Closed> a{1, 10};
      const Interval<int, LeftOpen> b{10, 22};

      verifyEmptyInterval<OpenInterval<int>>(intersect(a, b), caseLabel);
      verifyEmptyInterval<OpenInterval<int>>(intersect(b, a), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10) and [10, 22]";

      const Interval<int, RightOpen> a{1, 10};
      const Interval<int, Closed> b{10, 22};

      verifyEmptyInterval<OpenInterval<int>>(intersect(a, b), caseLabel);
      verifyEmptyInterval<OpenInterval<int>>(intersect(b, a), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and [10, 22]";

      const Interval<int, Closed> a{1, 10};
      const Interval<int, Closed> b{10, 22};

      verifyInterval<ClosedInterval<int>>(intersect(a, b), 10, 10, caseLabel);
      verifyInterval<ClosedInterval<int>>(intersect(b, a), 10, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [1, 10] and [5, 8]";

      const Interval<int, Closed> a{1, 10};
      const Interval<int, Closed> b{5, 8};

      verifyInterval<ClosedInterval<int>>(intersect(a, b), 5, 8, caseLabel);
      verifyInterval<ClosedInterval<int>>(intersect(b, a), 5, 8, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 8)";

      const Interval<int, LeftOpen> a{1, 10};
      const Interval<int, RightOpen> b{5, 8};

      verifyInterval<RightOpenInterval<int>>(intersect(a, b), 5, 8, caseLabel);
      verifyInterval<RightOpenInterval<int>>(intersect(b, a), 5, 8, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 10)";

      const Interval<int, LeftOpen> a{1, 10};
      const Interval<int, RightOpen> b{5, 10};

      verifyInterval<RightOpenInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<RightOpenInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 10]";

      const Interval<int, LeftOpen> a{1, 10};
      const Interval<int, Closed> b{5, 10};

      verifyInterval<ClosedInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<ClosedInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and [5, 10)";

      const Interval<int, Open> a{1, 10};
      const Interval<int, RightOpen> b{5, 10};

      verifyInterval<RightOpenInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<RightOpenInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and (5, 12)";

      const Interval<int, Open> a{1, 10};
      const Interval<int, Open> b{5, 12};

      verifyInterval<OpenInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<OpenInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and (5, 12)";

      const Interval<int, LeftOpen> a{1, 10};
      const Interval<int, Open> b{5, 12};

      verifyInterval<LeftOpenInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<LeftOpenInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10] and [5, 12)";

      const Interval<int, LeftOpen> a{1, 10};
      const Interval<int, RightOpen> b{5, 12};

      verifyInterval<ClosedInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<ClosedInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and [5, 12)";

      const Interval<int, Open> a{1, 10};
      const Interval<int, RightOpen> b{5, 12};

      verifyInterval<RightOpenInterval<int>>(intersect(a, b), 5, 10, caseLabel);
      verifyInterval<RightOpenInterval<int>>(intersect(b, a), 5, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (1, 10) and (1, 10)";

      const Interval<int, Open> a{1, 10};
      const Interval<int, Open> b{1, 10};

      verifyInterval<OpenInterval<int>>(intersect(a, b), 1, 10, caseLabel);
      verifyInterval<OpenInterval<int>>(intersect(b, a), 1, 10, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect (-1.11, 10.78] and (5.01, 12.83)";

      const Interval<double, LeftOpen> a{-1.11, 10.78};
      const Interval<double, Open> b{5.01, 12.83};

      verifyInterval<LeftOpenInterval<double>>(intersect(a, b), 5.01, 10.78, caseLabel);
      verifyInterval<LeftOpenInterval<double>>(intersect(b, a), 5.01, 10.78, caseLabel);
   }
   {
      const std::string caseLabel = "Intersect [-10.45, -1.003] and (-8.7, -2.0]";

      const Interval<double, Closed> a{-10.45, -1.003};
      const Interval<double, LeftOpen> b{-8.7, -2.0};

      verifyInterval<LeftOpenInterval<double>>(intersect(a, b), -8.7, -2.0, caseLabel);
      verifyInterval<LeftOpenInterval<double>>(intersect(b, a), -8.7, -2.0, caseLabel);
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
