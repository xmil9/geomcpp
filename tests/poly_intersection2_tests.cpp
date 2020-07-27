//
// geomcpp tests
// Tests for polygon intersection algorithm.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "poly_intersection2_tests.h"
#include "poly_intersection2.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testIsectForEmptyPolygon()
{
   {
      const std::string caseLabel = "Convex polygon intersection for empty polygon";

      Poly2<int> P;
      Poly2<int> Q;
      Q.add(Point2(1, -1));
      Q.add(Point2(2, 5));
      Q.add(Point2(4, 6));
      Q.add(Point2(3, 2));
      Q.add(Point2(2, -2));

      Poly2<int> expected;
      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}


void testIsectForPolygonWithSinglePointOutsideTheOther()
{
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with single "
                                    "point outside of the other polygon";

      Poly2<float> P;
      P.add(Point2(10.0f, 10.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}


void testIsectForPolygonWithSinglePointInsideTheOther()
{
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with single "
                                    "point inside of the other polygon";

      Poly2<float> P;
      P.add(Point2(2.0f, 0.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected = P;
      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}

} // namespace


///////////////////

void testPolygonIntersection2()
{
   testIsectForEmptyPolygon();
   testIsectForPolygonWithSinglePointOutsideTheOther();
   testIsectForPolygonWithSinglePointInsideTheOther();
}
