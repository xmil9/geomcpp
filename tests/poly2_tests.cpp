//
// geomcpp tests
// Tests for rectangles.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "poly2_tests.h"
#include "poly2.h"
#include "test_util.h"
#include "essentutils/fputil.h"
#include <vector>

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testPoly2DefaultCtor()
{
   {
      const std::string caseLabel = "Poly2 default ctor";

      const Poly2<float> poly;
      VERIFY(poly.size() == 0, caseLabel);
   }
}


void testPoly2SequenceCtor()
{
   {
      const std::string caseLabel = "Poly2 sequence ctor";

      std::vector<Point2<double>> pts{Point2{1.0, 2.0}, Point2{-3.0, 4.0},
                                      Point2{7.0, -2.0}, Point2{3.0, 1.0}};
      const Poly2<double> poly(pts.begin(), pts.end());

      VERIFY(poly.size() == pts.size(), caseLabel);
      for (std::size_t i = 0; i < poly.size(); ++i)
         VERIFY(poly[i] == pts[i], caseLabel);
   }
   {
      const std::string caseLabel = "Poly2 sequence ctor for empty sequence";

      std::vector<Point2<double>> empty;
      const Poly2<double> poly(empty.begin(), empty.end());

      VERIFY(poly.size() == 0, caseLabel);
   }
}


void testPoly2InitializerListCtor()
{
   {
      const std::string caseLabel = "Poly2 initializer list ctor";

      std::initializer_list<Point2<float>> ilist = {
         Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f}, Point2{7.0f, -2.0f},
         Point2{3.0f, 1.0f}};
      const Poly2<float> poly{ilist};

      VERIFY(poly.size() == ilist.size(), caseLabel);
      std::size_t i = 0;
      for (const auto& pt : ilist)
         VERIFY(poly[i++] == pt, caseLabel);
   }
}

} // namespace


///////////////////

void testPoly2()
{
   testPoly2DefaultCtor();
   testPoly2SequenceCtor();
   testPoly2InitializerListCtor();
}
