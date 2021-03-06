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


void testPoly2Size()
{
   {
      const std::string caseLabel = "Poly2::size";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f},
                              Point2{7.0f, -2.0f}, Point2{3.0f, 1.0f}};

      VERIFY(poly.size() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::size for empty polygon";

      const Poly2<float> poly;

      VERIFY(poly.size() == 0, caseLabel);
   }
}


void testPoly2SubscriptOperator()
{
   {
      const std::string caseLabel = "Poly2::operator[]";

      Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f}, Point2{7.0f, -2.0f},
                        Point2{3.0f, 1.0f}};

      VERIFY(poly[0] == Point2(1.0f, 2.0f), caseLabel);
      VERIFY(poly[1] == Point2(-3.0f, 4.0f), caseLabel);
      VERIFY(poly[2] == Point2(7.0f, -2.0f), caseLabel);
      VERIFY(poly[3] == Point2(3.0f, 1.0f), caseLabel);

      poly[0] = Point2{100.0f, -100.0f};

      VERIFY(poly[0] == Point2(100.0f, -100.0f), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::operator[] const";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f},
                              Point2{7.0f, -2.0f}, Point2{3.0f, 1.0f}};

      VERIFY(poly[0] == Point2(1.0f, 2.0f), caseLabel);
      VERIFY(poly[1] == Point2(-3.0f, 4.0f), caseLabel);
      VERIFY(poly[2] == Point2(7.0f, -2.0f), caseLabel);
      VERIFY(poly[3] == Point2(3.0f, 1.0f), caseLabel);
   }
}


void testPoly2Contains()
{
   {
      const std::string caseLabel = "Poly2::contains";

      Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f}, Point2{7.0f, -2.0f},
                        Point2{3.0f, 1.0f}};

      VERIFY(poly.contains(Point2(1.0f, 2.0f)) != poly.end(), caseLabel);
      VERIFY(poly.contains(Point2(3.0f, 1.0f)) != poly.end(), caseLabel);
      VERIFY(poly.contains(Point2(111.0f, 222.0f)) == poly.end(), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::contains and edit point through iterator";

      Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f}, Point2{7.0f, -2.0f},
                        Point2{3.0f, 1.0f}};

      auto pos = poly.contains(Point2(-3.0f, 4.0f));
      *pos = Point2(111.0f, 222.0f);
      VERIFY(poly.contains(Point2(111.0f, 222.0f)) != poly.end(), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::contains const";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{-3.0f, 4.0f},
                              Point2{7.0f, -2.0f}, Point2{3.0f, 1.0f}};

      VERIFY(poly.contains(Point2(1.0f, 2.0f)) != poly.end(), caseLabel);
      VERIFY(poly.contains(Point2(3.0f, 1.0f)) != poly.end(), caseLabel);
      VERIFY(poly.contains(Point2(111.0f, 222.0f)) == poly.end(), caseLabel);
   }
}


void testPoly2BeginEnd()
{
   {
      const std::string caseLabel = "Poly2::begin/end non-const";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      for (auto it = poly.begin(); it != poly.end(); ++it)
         *it = Point2{it->x() + 1, it->y() - 1};

      VERIFY(poly[0] == Point2(2, 1), caseLabel);
      VERIFY(poly[1] == Point2(-2, 3), caseLabel);
      VERIFY(poly[2] == Point2(8, -3), caseLabel);
      VERIFY(poly[3] == Point2(4, 0), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::begin/end const";

      const Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      std::size_t i = 0;
      for (auto it = poly.begin(); it != poly.end(); ++it, ++i)
         VERIFY(*it == poly[i], caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::begin/end for empty polygon";

      Poly2<short> poly;

      VERIFY(poly.begin() == poly.end(), caseLabel);
   }
}


void testPoly2Add()
{
   {
      const std::string caseLabel = "Poly2::add";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      const Point2 added{100, 200};
      poly.add(added);

      VERIFY(poly.size() == 5, caseLabel);
      VERIFY(poly[4] == added, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::add first vertex";

      Poly2<int> poly;

      const Point2 added{100, 200};
      poly.add(added);

      VERIFY(poly.size() == 1, caseLabel);
      VERIFY(poly[0] == added, caseLabel);
   }
}


void testPoly2Insert()
{
   {
      const std::string caseLabel = "Poly2::insert at beginning";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      const Point2 inserted{100, 200};
      poly.insert(inserted, 0);

      VERIFY(poly.size() == 5, caseLabel);
      VERIFY(poly[0] == inserted, caseLabel);
      VERIFY(poly[1] == Point2(1, 2), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::insert in middle";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      const Point2 inserted{100, 200};
      poly.insert(inserted, 2);

      VERIFY(poly.size() == 5, caseLabel);
      VERIFY(poly[1] == Point2(-3, 4), caseLabel);
      VERIFY(poly[2] == inserted, caseLabel);
      VERIFY(poly[3] == Point2(7, -2), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::insert at end";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      const Point2 inserted{100, 200};
      poly.insert(inserted, 4);

      VERIFY(poly.size() == 5, caseLabel);
      VERIFY(poly[3] == Point2(3, 1), caseLabel);
      VERIFY(poly[4] == inserted, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::insert past the end";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      const Point2 inserted{100, 200};
      poly.insert(inserted, 20);

      VERIFY(poly.size() == 5, caseLabel);
      VERIFY(poly[3] == Point2(3, 1), caseLabel);
      VERIFY(poly[4] == inserted, caseLabel);
   }
}


void testPoly2NumEdges()
{
   {
      const std::string caseLabel = "Poly2::numEdges";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};
      VERIFY(poly.numEdges() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::numEdges for empty polygon";

      Poly2<int> poly;
      VERIFY(poly.numEdges() == 0, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::numEdges for one vertex";

      Poly2<int> poly{Point2{1, 2}};
      VERIFY(poly.numEdges() == 0, caseLabel);
   }
}


void testPoly2Edge()
{
   {
      const std::string caseLabel = "Poly2::edge";

      Poly2<int> poly{Point2{1, 2}, Point2{-3, 4}, Point2{7, -2}, Point2{3, 1}};

      VERIFY(poly.edge(0) == Poly2<int>::Edge(Point2{1, 2}, Point2{-3, 4}), caseLabel);
      VERIFY(poly.edge(1) == Poly2<int>::Edge(Point2{-3, 4}, Point2{7, -2}), caseLabel);
      VERIFY(poly.edge(2) == Poly2<int>::Edge(Point2{7, -2}, Point2{3, 1}), caseLabel);
      VERIFY(poly.edge(3) == Poly2<int>::Edge(Point2{3, 1}, Point2{1, 2}), caseLabel);
   }
}


void testPoly2Bounds()
{
   {
      const std::string caseLabel = "Poly2::bounds";

      Poly2<int> poly{Point2{1, 2}, Point2{2, -2}, Point2{3, 2}, Point2{4, -2}};

      const auto bounds = poly.bounds();
      VERIFY(bounds.has_value(), caseLabel);
      if (bounds)
         VERIFY(*poly.bounds() == Rect(1, -2, 4, 2), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::bounds for empty polygon";

      Poly2<float> poly;

      const auto bounds = poly.bounds();
      VERIFY(!bounds.has_value(), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::bounds for polygon with single vertex";

      Poly2<double> poly{Point2{1.0, 2.0}};

      const auto bounds = poly.bounds();
      VERIFY(bounds.has_value(), caseLabel);
      if (bounds)
         VERIFY(*poly.bounds() == Rect<double>(poly[0], poly[0]), caseLabel);
   }
}


void testPoly2Reversed()
{
   {
      const std::string caseLabel = "Poly2::reversed";

      const Poly2<int> poly{Point2{1, 2}, Point2{3, 0}, Point2{4, -2}, Point2{2, -3}};
      const Poly2<int> rev = poly.reversed();

      const Poly2<int> expected = {Point2{2, -3}, Point2{4, -2}, Point2{3, 0},
                                   Point2{1, 2}};
      VERIFY(rev == expected, caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::reversed for empty polygon";

      const Poly2<int> empty;
      const Poly2<int> rev = empty.reversed();

      VERIFY(rev == empty, caseLabel);
   }
}


void testPoly2IsConvex()
{
   {
      const std::string caseLabel = "Poly2::isConvex for convex polygon";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{3.0f, 0.0f}, Point2{4.0f, -2.0f},
                              Point2{2.0f, -3.0f}};

      VERIFY(poly.isConvex(), caseLabel);
   }
   {
      const std::string caseLabel = "Poly2::isConvex for concave polygon";

      const Poly2<float> poly{Point2{1.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                              Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};

      VERIFY(!poly.isConvex(), caseLabel);
   }
}


void testPoly2Equality()
{
   {
      const std::string caseLabel = "Equality for equal polygons";

      const Poly2<float> a{Point2{1.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      const auto b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal polygons";

      const Poly2<float> a{Point2{1.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      const Poly2<float> b{Point2{2.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      VERIFY(!(a == b), caseLabel);
   }
}


void testPoly2Inequality()
{
   {
      const std::string caseLabel = "Equality for equal polygons";

      const Poly2<float> a{Point2{1.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      const auto b = a;
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal polygons";

      const Poly2<float> a{Point2{1.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      const Poly2<float> b{Point2{2.0f, 0.0f}, Point2{3.0f, 1.0f}, Point2{4.0f, 3.0f},
                           Point2{2.0f, 2.0f}, Point2{0.0f, 5.0f}};
      VERIFY(a != b, caseLabel);
   }
}


void testIsPointInsideConvexPolygon()
{
   {
      const std::string caseLabel =
         "isPointInsideConvexPolygon for polygon with one vertex";

      const Poly2<int> poly{Point2{1, 2}};
      VERIFY(isPointInsideConvexPolygon(poly, Point2(1, 2)), caseLabel);
      VERIFY(!isPointInsideConvexPolygon(poly, Point2(2, 2)), caseLabel);
   }
   {
      const std::string caseLabel =
         "isPointInsideConvexPolygon for polygon with two vertices";

      const Poly2<float> poly{Point2{1.0f, 1.0f}, Point2{2.0f, 2.0f}};
      VERIFY(isPointInsideConvexPolygon(poly, Point2(1.5f, 1.5f)), caseLabel);
      VERIFY(!isPointInsideConvexPolygon(poly, Point2(2.0f, 3.0f)), caseLabel);
   }
   {
      const std::string caseLabel = "isPointInsideConvexPolygon for ccw convex polygon";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{3.0f, 0.0f}, Point2{4.0f, -2.0f},
                              Point2{2.0f, -3.0f}};
      VERIFY(isPointInsideConvexPolygon(poly, Point2(2.0f, -1.0f)), caseLabel);
      VERIFY(!isPointInsideConvexPolygon(poly, Point2(2.0f, 3.0f)), caseLabel);
   }
   {
      const std::string caseLabel = "isPointInsideConvexPolygon for cw convex polygon";

      const Poly2<float> poly{Point2{1.0f, 2.0f}, Point2{2.0f, -3.0f},
                              Point2{4.0f, -2.0f}, Point2{3.0f, 0.0f}};
      VERIFY(isPointInsideConvexPolygon(poly, Point2(2.0f, -1.0f)), caseLabel);
      VERIFY(!isPointInsideConvexPolygon(poly, Point2(2.0f, 3.0f)), caseLabel);
   }
   {
      const std::string caseLabel = "isPointInsideConvexPolygon for point on edge";

      const Poly2<double> octagon{Point2{6.0, 8.0},  Point2{8.0, 8.0}, Point2{10.0, 6.0},
                                  Point2{10.0, 4.0}, Point2{8.0, 2.0}, Point2{6.0, 2.0},
                                  Point2{4.0, 4.0},  Point2{4.0, 6.0}};
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(7.0, 8.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(9.0, 7.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(10.0, 5.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(9.0, 3.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(7.0, 2.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(5.0, 5.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(4.0, 5.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(5.0, 7.0)), caseLabel);
   }
   {
      const std::string caseLabel = "isPointInsideConvexPolygon for point on vertex";

      const Poly2<double> octagon{Point2{6.0, 8.0},  Point2{8.0, 8.0}, Point2{10.0, 6.0},
                                  Point2{10.0, 4.0}, Point2{8.0, 2.0}, Point2{6.0, 2.0},
                                  Point2{4.0, 4.0},  Point2{4.0, 6.0}};
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(6.0, 8.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(8.0, 8.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(10.0, 6.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(10.0, 4.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(8.0, 2.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(6.0, 2.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(4.0, 4.0)), caseLabel);
      VERIFY(isPointInsideConvexPolygon(octagon, Point2(4.0, 6.0)), caseLabel);
   }
}

} // namespace


///////////////////

void testPoly2()
{
   testPoly2DefaultCtor();
   testPoly2SequenceCtor();
   testPoly2InitializerListCtor();
   testPoly2Size();
   testPoly2SubscriptOperator();
   testPoly2Contains();
   testPoly2BeginEnd();
   testPoly2Add();
   testPoly2Insert();
   testPoly2NumEdges();
   testPoly2Edge();
   testPoly2Bounds();
   testPoly2Reversed();
   testPoly2IsConvex();
   testPoly2Equality();
   testPoly2Inequality();
   testIsPointInsideConvexPolygon();
}
