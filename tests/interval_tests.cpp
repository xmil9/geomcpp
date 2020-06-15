//
// geomcpp tests
// Tests for interval functionality.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "interval_tests.h"
#include "interval.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
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

} // namespace


///////////////////

void testInterval()
{
   testCtor();
   testLength();
   testIsEmpty();
   testContains();
}
