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


void testIsConvexPath()
{
   {
      const std::string caseLabel = "isConvexPath for no points";

      const std::vector<Point2<double>> path;
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for one point";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for two points";

      const std::vector<Point2<int>> path{Point2{1, 2}, Point2{5, 3}};
      VERIFY(isConvexPath<int>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for three points";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}, Point2{5.0, 3.0},
                                             Point2{3.0, 2.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for cw points";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}, Point2{3.0, 1.0},
                                             Point2{4.0, 4.0}, Point2{2.0, 3.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for ccw points";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}, Point2{3.0, 0.0},
                                             Point2{4.0, -2.0}, Point2{2.0, -3.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for points with crossing edges";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}, Point2{3.0, 0.0},
                                             Point2{4.0, -2.0}, Point2{5.0, 2.0}};
      VERIFY(!isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for points with concave edges";

      const std::vector<Point2<double>> path{Point2{1.0, 2.0}, Point2{3.0, 1.0},
                                             Point2{4.0, 3.0}, Point2{2.0, 2.0},
                                             Point2{0.0, 5.0}};
      VERIFY(!isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel = "isConvexPath for points in straight line";

      const std::vector<Point2<double>> path{Point2{1.0, 0.0}, Point2{2.0, 1.0},
                                             Point2{3.0, 2.0}, Point2{4.0, 3.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
   {
      const std::string caseLabel =
         "isConvexPath for path with duplicate consecutive points";

      const std::vector<Point2<double>> path{Point2{1.0, 0.0}, Point2{2.0, 1.0},
                                             Point2{2.0, 1.0}, Point2{3.0, 2.0},
                                             Point2{4.0, 3.0}};
      VERIFY(isConvexPath<double>(path.begin(), path.end()), caseLabel);
   }
}

} // namespace


///////////////////

void testGeometryUtilities()
{
   testCalcPathBounds();
   testIsConvexPath();
}
