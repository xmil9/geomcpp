//
// geomcpp tests
// Tests for interval functionality.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "interval_tests.h"
#include "interval_tec.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace geom::tec;
using namespace sutil;


namespace
{
///////////////////

void testCtor()
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

void testLength()
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


void testIsEmpty()
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


void testContains()
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


void testLeftEnd()
{
   {
      const std::string caseLabel = "Interval::leftEnd";

      VERIFY((Interval<short, Closed>(200, 300).leftEnd() == IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float, Open>(200.1f, 300.2f).leftEnd() == IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).leftEnd() == IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<double, RightOpen>(200.0, 300.3).leftEnd() == IntervalEnd::Closed),
             caseLabel);
   }
}


void testRightEnd()
{
   {
      const std::string caseLabel = "Interval::rightEnd";

      VERIFY((Interval<short, Closed>(200, 300).rightEnd() == IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<float, Open>(200.1f, 300.2f).rightEnd() == IntervalEnd::Open),
             caseLabel);
      VERIFY((Interval<int, LeftOpen>(200, 300).rightEnd() == IntervalEnd::Closed),
             caseLabel);
      VERIFY((Interval<double, RightOpen>(200.0, 300.3).rightEnd() == IntervalEnd::Open),
             caseLabel);
   }
}


void testType()
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


void testOperatorBool()
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


void testNegationOperator()
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
      const std::string caseLabel = "Intersect disjoint intervals";

      const Interval<double, Closed> a{1.0, 10.4};
      const Interval<double, Open> b{20.1, 22.09};

      SomeInterval<double> res = intersect(a, b);

      VERIFY(std::holds_alternative<OpenInterval<double>>(res), caseLabel);

      const OpenInterval<double> resIv = std::get<OpenInterval<double>>(res);
      VERIFY(resIv.isEmpty(), caseLabel);
   }
}

} // namespace


///////////////////

void testInterval()
{
   testCtor();
   testLength();
   testIsEmpty();
   testContains();
   testLeftEnd();
   testRightEnd();
   testType();
   testOperatorBool();
   testNegationOperator();

   testIntersect();
}
