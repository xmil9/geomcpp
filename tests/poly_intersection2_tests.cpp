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

template <typename T> Poly2<T> makeOctagon(T x, T y, T size)
{
   Poly2<T> poly;
   poly.add(Point2<T>{x, y});
   poly.add(Point2<T>{x - size, y + size});
   poly.add(Point2<T>{x - size, y + 2 * size});
   poly.add(Point2<T>{x, y + 3 * size});
   poly.add(Point2<T>{x + size, y + 3 * size});
   poly.add(Point2<T>{x + 2 * size, y + 2 * size});
   poly.add(Point2<T>{x + 2 * size, y + size});
   poly.add(Point2<T>{x + size, y});
   return poly;
}


template <typename T> Poly2<T> makeRect(T x, T y, T sizeX, double sizeY)
{
   Poly2<T> poly;
   poly.add(Point2<T>{x, y});
   poly.add(Point2<T>{x, y + sizeY});
   poly.add(Point2<T>{x + sizeX, y + sizeY});
   poly.add(Point2<T>{x + sizeX, y});
   return poly;
}


template <typename T> Poly2<T> makeDiamond(T x, T y, T size)
{
   Poly2<T> poly;
   poly.add(Point2<T>{x, y});
   poly.add(Point2<T>{x - size, y + size});
   poly.add(Point2<T>{x, y + 2 * size});
   poly.add(Point2<T>{x + size, y + size});
   return poly;
}


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


void testIsectForPolygonWithSinglePoint()
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
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with single "
                                    "point on the other polygon's path";

      Poly2<float> P;
      P.add(Point2(4.0f, 6.0f));

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


void testIsectForPolygonWithTwoPoints()
{
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with two "
                                    "points that is outside of the other polygon";

      Poly2<float> P;
      P.add(Point2(-1.0f, -5.0f));
      P.add(Point2(8.0f, 0.0f));

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
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with two "
                                    "points that is fully inside the other polygon";

      Poly2<float> P;
      P.add(Point2(1.1f, -0.8f));
      P.add(Point2(2.5f, 0.5f));

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
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with two "
                                    "points that is intersecting the other polygon twice";

      Poly2<float> P;
      P.add(Point2(0.0f, 0.0f));
      P.add(Point2(7.0f, 4.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      expected.add(Point2(1.2894736842f, 0.736842105f));
      expected.add(Point2(2.916666667f, 1.666666667f));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel =
         "Convex polygon intersection for polygon with two "
         "points that is intersecting the other polygon once with the start point inside";

      Poly2<float> P;
      P.add(Point2(1.5f, 1.0f));
      P.add(Point2(7.0f, 4.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      expected.add(Point2(1.5f, 1.0f));
      expected.add(Point2(2.947368421f, 1.7894736842f));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel =
         "Convex polygon intersection for polygon with two "
         "points that is intersecting the other polygon once with the end point inside";

      Poly2<float> P;
      P.add(Point2(0.0f, 0.0f));
      P.add(Point2(3.0f, 5.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      expected.add(Point2(1.6153846153f, 2.692307692f));
      expected.add(Point2(3.0f, 5.0f));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with two "
                                    "points that is intersecting the other polygon once "
                                    "with the start point on an edge";

      Poly2<float> P;
      P.add(Point2(1.0f, -1.0f));
      P.add(Point2(7.0f, 4.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      expected.add(Point2(1.0f, -1.0f));
      expected.add(Point2(2.578947368f, 0.315789473f));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel = "Convex polygon intersection for polygon with two "
                                    "points that is intersecting the other polygon once "
                                    "with the end point on an edge";

      Poly2<float> P;
      P.add(Point2(0.0f, 0.0f));
      P.add(Point2(3.0f, 2.0f));

      Poly2<float> Q;
      Q.add(Point2(1.0f, -1.0f));
      Q.add(Point2(2.0f, 5.0f));
      Q.add(Point2(4.0f, 6.0f));
      Q.add(Point2(3.0f, 2.0f));
      Q.add(Point2(2.0f, -2.0f));

      Poly2<float> expected;
      expected.add(Point2(1.3125f, 0.875f));
      expected.add(Point2(3.0f, 2.0f));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}


void testIsectForGeneralPolygons()
{
   {
      const std::string caseLabel =
         "Convex polygon intersection for polygons that don't intersect";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeRect<double>(10.0, 10.0, 3.0, 3.0);

      Poly2<double> expected;
      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel = "Convex polygon intersection case 1";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeRect<double>(-1.0, 3.0, 8.0, 4.0);

      Poly2<double> expected;
      expected.add(Point2(1.0, 7.0));
      expected.add(Point2(5.0, 7.0));
      expected.add(Point2(6.0, 6.0));
      expected.add(Point2(6.0, 4.0));
      expected.add(Point2(5.0, 3.0));
      expected.add(Point2(1.0, 3.0));
      expected.add(Point2(0.0, 4.0));
      expected.add(Point2(0.0, 6.0));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel = "Convex polygon intersection case 2";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeDiamond<double>(2.5, 4.0, 3.0);

      Poly2<double> expected;
      expected.add(Point2(0.25, 6.25));
      expected.add(Point2(2.0, 8.0));
      expected.add(Point2(4.0, 8.0));
      expected.add(Point2(5.25, 6.75));
      expected.add(Point2(2.5, 4.0));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
   {
      const std::string caseLabel = "Convex polygon intersection case 3";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeDiamond<double>(3.0, 1.0, 3.0);

      Poly2<double> expected;
      expected.add(Point2(0.0, 4.0));
      expected.add(Point2(3.0, 7.0));
      expected.add(Point2(6.0, 4.0));
      expected.add(Point2(4.0, 2.0));
      expected.add(Point2(2.0, 2.0));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}


void testIsectForPolygonsTouchingAtEdge()
{
   {
      const std::string caseLabel =
         "Convex polygon intersection for polygons touching at edge";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeDiamond<double>(-1.0, -1.0, 3.0);

      Poly2<double> expected;
      expected.add(Point2(0.0, 4.0));
      expected.add(Point2(2.0, 2.0));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}


void testIsectForPolygonsTouchingAtPoint()
{
   {
      const std::string caseLabel =
         "Convex polygon intersection for polygons touching at point";

      Poly2<double> P = makeOctagon<double>(2.0, 2.0, 2.0);
      Poly2<double> Q = makeRect<double>(5.0, 1.0, 2.0, 2.0);

      Poly2<double> expected;
      expected.add(Point2(5.0, 3.0));

      VERIFY(expected == intersectConvexPolygons(P, Q), caseLabel);
      VERIFY(expected == intersectConvexPolygons(Q, P), caseLabel);
   }
}

} // namespace


///////////////////

void testPolygonIntersection2()
{
   testIsectForEmptyPolygon();
   testIsectForPolygonWithSinglePoint();
   testIsectForPolygonWithTwoPoints();
   testIsectForGeneralPolygons();
   testIsectForPolygonsTouchingAtEdge();
   testIsectForPolygonsTouchingAtPoint();
}
