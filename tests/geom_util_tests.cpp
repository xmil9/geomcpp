//
// geomcpp tests
// Tests for geometry utilities.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "geom_util_tests.h"
#include "geom_util.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testCalcPathBounds()
{
   {
      const std::string caseLabel = "calcPathBounds for no points";

      const std::vector<Point2<int>> empty;
      const auto bounds = calcPathBounds<int>(empty.begin(), empty.end());

      VERIFY(!bounds.has_value(), caseLabel);
   }
   {
      const std::string caseLabel = "calcPathBounds for one point";

      const Point2<float> pt{1.2f, 3.4f};
      const std::vector<Point2<float>> path{pt};
      const auto bounds = calcPathBounds<float>(path.begin(), path.end());

      VERIFY(bounds.has_value(), caseLabel);
      if (bounds)
      {
         VERIFY(bounds->leftTop() == pt, caseLabel);
         VERIFY(bounds->rightBottom() == pt, caseLabel);
      }
   }
   {
      const std::string caseLabel = "calcPathBounds for multiple points";

      const std::vector<Point2<double>> path = {Point2{2.0, 3.0}, Point2{2.3, 3.6},
                                                Point2{-3.4, 7.8}, Point2{0.0, 2.0},
                                                Point2{1.9, 9.1}};
      const auto bounds = calcPathBounds<double>(path.begin(), path.end());

      VERIFY(bounds.has_value(), caseLabel);
      if (bounds)
      {
         VERIFY(bounds->left() == -3.4, caseLabel);
         VERIFY(bounds->top() == 2.0, caseLabel);
         VERIFY(bounds->right() == 2.3, caseLabel);
         VERIFY(bounds->bottom() == 9.1, caseLabel);
      }
   }
}

} // namespace


///////////////////

void testGeometryUtilities()
{
   testCalcPathBounds();
}