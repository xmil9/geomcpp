//
// geomcpp tests
// Tests for cutting polygons.
//
// Aug-2020, Michael Lindner
// MIT license
//
#include "poly_line_cut2_tests.h"
#include "line_inf2_ct.h"
#include "poly_line_cut2.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


///////////////////

void testPolygonLineCutting2()
{
   {
      const std::string caseLabel = "cutConvexPolygon for empty polygon";

      ct::LineInf2<double> l(Point2(1.0, 2.0), Vec2(2.0, 1.0));
      Poly2<double> p;

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() > 0)
         VERIFY(res[0].size() == 0, caseLabel);
   }
   {
      const std::string caseLabel = "cutConvexPolygon for polygon with single point";

      ct::LineInf2<int> l(Point2(1, 2), Vec2(2, 1));
      Poly2<int> p{Point2{1, 1}};

      const std::vector<Poly2<int>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() > 0)
         VERIFY(res[0] == p, caseLabel);
   }
   {
      const std::string caseLabel =
         "cutConvexPolygon for polygon with two points crossing the line";

      ct::LineInf2<double> l(Point2(1.0, 2.0), Vec2(2.0, 1.0));
      Poly2<double> p{Point2{1.0, 1.0}, Point2{2.0, 5.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 2, caseLabel);
      if (res.size() == 2)
      {
         VERIFY(res[0].size() == 2, caseLabel);
         VERIFY(res[1].size() == 2, caseLabel);
      }
   }
   {
      const std::string caseLabel =
         "cutConvexPolygon for polygon with two points on the same side of the line";

      ct::LineInf2<float> l(Point2(1.0f, 2.0f), Vec2(2.0f, 1.0f));
      Poly2<float> p{Point2{3.0f, 4.0f}, Point2{2.0f, 5.0f}};

      const std::vector<Poly2<float>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() == 1)
         VERIFY(res[0] == p, caseLabel);
   }
   {
      const std::string caseLabel =
         "cutConvexPolygon for polygon with two points on the line";

      ct::LineInf2<double> l(Point2(1.0, 2.0), Vec2(2.0, 1.0));
      Poly2<double> p{Point2{3.0, 3.0}, Point2{5.0, 4.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() == 1)
         VERIFY(res[0] == p, caseLabel);
   }
   {
      const std::string caseLabel =
         "cutConvexPolygon for polygon intersected by the line";

      ct::LineInf2<double> l(Point2(1.0, 2.0), Vec2(2.0, 1.0));
      Poly2<double> p{Point2{1.0, -1.0}, Point2{7.0, 1.0}, Point2{6.0, 2.0},
                      Point2{4.0, 6.0}, Point2{2.0, 5.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 2, caseLabel);
      if (res.size() == 2)
      {
         VERIFY(res[0].size() == 5, caseLabel);
         VERIFY(res[1].size() == 4, caseLabel);
      }
   }
   {
      const std::string caseLabel =
         "cutConvexPolygon for polygon not intersected by the line";

      ct::LineInf2<double> l(Point2(10.0, 10.0), Vec2(2.0, 1.0));
      Poly2<double> p{Point2{1.0, -1.0}, Point2{7.0, 1.0}, Point2{6.0, 2.0},
                      Point2{4.0, 6.0}, Point2{2.0, 5.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() == 1)
         VERIFY(res[0] == p, caseLabel);
   }
   {
      const std::string caseLabel = "cutConvexPolygon for polygon touched by the line";

      ct::LineInf2<double> l(Point2(7.0, 1.0), Vec2(-1.0, -1.0));
      Poly2<double> p{Point2{1.0, -1.0}, Point2{7.0, 1.0}, Point2{6.0, 2.0},
                      Point2{4.0, 6.0}, Point2{2.0, 5.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 1, caseLabel);
      if (res.size() == 1)
         VERIFY(res[0] == p, caseLabel);
   }
   {
      const std::string caseLabel = "cutConvexPolygon for polygon intersected by the "
                                    "line with all but one point on the same side";

      ct::LineInf2<double> l(Point2(6.0, -2.0), Vec2(1.0, 4.0));
      Poly2<double> p{Point2{1.0, -1.0}, Point2{8.0, 1.0}, Point2{6.0, 2.0},
                      Point2{4.0, 6.0}, Point2{2.0, 5.0}};

      const std::vector<Poly2<double>> res = cutConvexPolygon(p, l);

      VERIFY(res.size() == 2, caseLabel);
      if (res.size() == 2)
      {
         VERIFY(res[0].size() == 3, caseLabel);
         VERIFY(res[1].size() == 6, caseLabel);
      }
   }
}
