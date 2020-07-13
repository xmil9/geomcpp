//
// geomcpp tests
// Tests for triangles.
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "triangle_tests.h"
#include "line_seg2_ct.h"
#include "triangle.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testTriangleDefaultCtor()
{
   {
      const std::string caseLabel = "Triangle default ctor";

      Triangle<float> t;
      VERIFY((t[0] == Point2{0.0f, 0.0f}), caseLabel);
      VERIFY((t[1] == Point2{0.0f, 0.0f}), caseLabel);
      VERIFY((t[2] == Point2{0.0f, 0.0f}), caseLabel);
   }
}


void testTriangleValueCtor()
{
   {
      const std::string caseLabel = "Triangle construct with ccw points";

      const Point2 a(1, 2);
      const Point2 b(3, 4);
      const Point2 c(7, 1);
      const Triangle t(a, b, c);

      VERIFY(t[0] == a, caseLabel);
      VERIFY(t[1] == b, caseLabel);
      VERIFY(t[2] == c, caseLabel);
   }
   {
      const std::string caseLabel = "Triangle construct with cw points";

      const Point2 a(1.0, 2.0);
      const Point2 b(3.0, 4.0);
      const Point2 c(2.0, 7.0);
      const Triangle t(a, b, c);

      VERIFY(t[0] == a, caseLabel);
      VERIFY(t[1] == c, caseLabel);
      VERIFY(t[2] == b, caseLabel);
   }
}


void testTriangleSubscriptOperator()
{
   {
      const std::string caseLabel = "Triangle subscript operator";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(7, 1);
      const Triangle t(a, b, c);

      VERIFY(t[0] == a, caseLabel);
      VERIFY(t[1] == b, caseLabel);
      VERIFY(t[2] == c, caseLabel);
   }
}


void testTriangleHasVertex()
{
   {
      const std::string caseLabel = "Triangle::hasVertex when point is a vertex";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(7, 1);
      const Triangle t(a, b, c);

      VERIFY(t.hasVertex(a), caseLabel);
      VERIFY(t.hasVertex(b), caseLabel);
      VERIFY(t.hasVertex(c), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::hasVertex when point is not a vertex";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(7, 1);
      const Triangle t(a, b, c);

      VERIFY(!t.hasVertex(Point2{4.0, 2.0}), caseLabel);
   }
}


void testTriangleVertexArray()
{
   {
      const std::string caseLabel = "Triangle::vertexArray";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(7, 1);
      const Triangle t(a, b, c);

      VERIFY(t.vertexArray()[0] == a, caseLabel);
      VERIFY(t.vertexArray()[1] == b, caseLabel);
      VERIFY(t.vertexArray()[2] == c, caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::vertexArray for empty triangle";

      const Triangle<int> t;

      VERIFY((t.vertexArray()[0] == Point2<int>{}), caseLabel);
      VERIFY((t.vertexArray()[1] == Point2<int>{}), caseLabel);
      VERIFY((t.vertexArray()[2] == Point2<int>{}), caseLabel);
   }
}


void testTriangleEdge()
{
   {
      const std::string caseLabel = "Triangle::edge with ccw points";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(7, 1);
      const Triangle t(a, b, c);

      VERIFY((t.edge(0) == ct::LineSeg2{a, b}), caseLabel);
      VERIFY((t.edge(1) == ct::LineSeg2{b, c}), caseLabel);
      VERIFY((t.edge(2) == ct::LineSeg2{c, a}), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::edge with cw points";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(2, 7);
      const Triangle t(a, b, c);

      VERIFY((t.edge(0) == ct::LineSeg2{a, c}), caseLabel);
      VERIFY((t.edge(1) == ct::LineSeg2{c, b}), caseLabel);
      VERIFY((t.edge(2) == ct::LineSeg2{b, a}), caseLabel);
   }
}


void testTriangleIsPoint()
{
   {
      const std::string caseLabel = "Triangle::isPoint when triangle is a point";

      const Point2<float> a(1, 2);
      const Triangle t(a, a, a);

      VERIFY(t.isPoint(), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::isPoint when triangle is not a point";

      const Point2<double> a(1, 2);
      const Point2<double> b(3, 4);
      const Point2<double> c(10, 6);
      const Triangle t(a, b, c);

      VERIFY(!t.isPoint(), caseLabel);
   }
}


void testTriangleIsLine()
{
   {
      const std::string caseLabel = "Triangle::isLine when triangle is a line";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(5, 6);
      const Triangle t(a, b, c);

      VERIFY(t.isLine(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Triangle::isLine when triangle is a line with two equal points";

      const Point2<float> a(1, 2);
      const Point2<float> b(1, 2);
      const Point2<float> c(5, 6);
      const Triangle t(a, b, c);

      VERIFY(t.isLine(), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::isLine when triangle is not a line";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(10, 6);
      const Triangle t(a, b, c);

      VERIFY(!t.isLine(), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::isLine when triangle is a point";

      const Point2<float> a(1, 2);
      const Triangle t(a, a, a);

      VERIFY(!t.isLine(), caseLabel);
   }
}


void testTriangleIsDegenerate()
{
   {
      const std::string caseLabel = "Triangle::isDegenerate when triangle is a line";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(5, 6);
      const Triangle t(a, b, c);

      VERIFY(t.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel =
         "Triangle::isDegenerate when triangle is not degenerate";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(10, 6);
      const Triangle t(a, b, c);

      VERIFY(!t.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::isDegenerate when triangle is a point";

      const Point2<float> a(1, 2);
      const Triangle t(a, a, a);

      VERIFY(t.isDegenerate(), caseLabel);
   }
}


void testTriangleArea()
{
   {
      const std::string caseLabel = "Triangle::area for right angled triangle";

      const Point2<double> a(1, 1);
      const Point2<double> b(4, 1);
      const Point2<double> c(1, 3);
      const Triangle t(a, b, c);

      VERIFY(equal(t.area(), 3.0), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::area for general triangle";

      const Point2<double> a(1, 1);
      const Point2<double> b(4, 1);
      const Point2<double> c(2, 7);
      const Triangle t(a, b, c);

      VERIFY(equal(t.area(), 9.0), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::area for degenerated triangle";

      const Point2<double> a(1, 2);
      const Point2<double> b(3, 4);
      const Point2<double> c(5, 6);
      const Triangle t(a, b, c);

      VERIFY(equal(t.area(), 0.0), caseLabel);
   }
}


void testTriangleCalcCircumcircle()
{
   {
      const std::string caseLabel =
         "Triangle::calcCircumcircle for triangle that is a point";

      const Point2<float> a(1, 2);
      const Triangle t(a, a, a);

      const auto ccircle = t.calcCircumcircle();
      VERIFY(ccircle.has_value(), caseLabel);
      if (ccircle)
         VERIFY((*ccircle == Circle<float>{a, 0}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Triangle::calcCircumcircle for triangle that is a line";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(5, 6);
      const Triangle t(a, b, c);

      const auto ccircle = t.calcCircumcircle();
      VERIFY(!ccircle, caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::calcCircumcircle for equilateral triangle "
                                    "with base midpoint at origin";

      constexpr Point2<float> a(2, 0);
      constexpr Point2<float> b(-2, 0);
      constexpr Point2<float> c(0, 2);
      const Triangle t(a, b, c);

      const auto ccircle = t.calcCircumcircle();
      VERIFY(ccircle.has_value(), caseLabel);
      if (ccircle)
         VERIFY((*ccircle == Circle<float>{Point2<float>(0, 0), 2.0f}), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::calcCircumcircle for general triangle";

      constexpr Point2<double> center(3, 8);
      constexpr double radius = 4;
      const Circle c(center, radius);

      const Point2 p1 = c.pointAtAngle(1.0);
      const Point2 p2 = c.pointAtAngle(3.0);
      const Point2 p3 = c.pointAtAngle(5.0);

      const Triangle t(p1, p2, p3);

      const auto ccircle = t.calcCircumcircle();
      VERIFY(ccircle.has_value(), caseLabel);
      if (ccircle)
         VERIFY((*ccircle == Circle{center, radius}), caseLabel);
   }
}


void testTriangleCalcCircumcenter()
{
   {
      const std::string caseLabel =
         "Triangle::calcCircumcenter for triangle that is a point";

      const Point2<float> a(1, 2);
      const Triangle t(a, a, a);

      const auto ccenter = t.calcCircumcenter();
      VERIFY(ccenter.has_value(), caseLabel);
      if (ccenter)
         VERIFY(*ccenter == a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Triangle::calcCircumcenter for triangle that is a line";

      const Point2<float> a(1, 2);
      const Point2<float> b(3, 4);
      const Point2<float> c(5, 6);
      const Triangle t(a, b, c);

      const auto ccenter = t.calcCircumcenter();
      VERIFY(!ccenter, caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::calcCircumcenter for equilateral triangle "
                                    "with base midpoint at origin";

      constexpr Point2<float> a(2, 0);
      constexpr Point2<float> b(-2, 0);
      constexpr Point2<float> c(0, 2);
      const Triangle t(a, b, c);

      const auto ccenter = t.calcCircumcenter();
      VERIFY(ccenter.has_value(), caseLabel);
      if (ccenter)
         VERIFY(*ccenter == Point2<float>(0, 0), caseLabel);
   }
   {
      const std::string caseLabel = "Triangle::calcCircumcenter for general triangle";

      constexpr Point2<double> center(3, 8);
      constexpr double radius = 4;
      const Circle c(center, radius);

      const Point2 p1 = c.pointAtAngle(1.0);
      const Point2 p2 = c.pointAtAngle(3.0);
      const Point2 p3 = c.pointAtAngle(5.0);

      const Triangle t(p1, p2, p3);

      const auto ccenter = t.calcCircumcenter();
      VERIFY(ccenter.has_value(), caseLabel);
      if (ccenter)
         VERIFY(*ccenter == center, caseLabel);
   }
}


void testTriangleEquality()
{
   {
      const std::string caseLabel = "Equality for equal triangles";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);
      const Triangle b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal triangles";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);

      constexpr Point2<float> q1(1, 2);
      constexpr Point2<float> q2(-2, 4);
      constexpr Point2<float> q3(-9, -3);
      const Triangle b(q1, q2, q3);

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal triangles with different value types";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);

      constexpr Point2<int> q1(2, 0);
      constexpr Point2<int> q2(-2, 0);
      constexpr Point2<int> q3(0, 2);
      const Triangle b(q1, q2, q3);

      VERIFY(a == b, caseLabel);
   }
}


void testTriangleInequality()
{
   {
      const std::string caseLabel = "Inequality for equal triangles";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);
      const Triangle b = a;
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal triangles";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);

      constexpr Point2<float> q1(1, 2);
      constexpr Point2<float> q2(-2, 4);
      constexpr Point2<float> q3(-9, -3);
      const Triangle b(q1, q2, q3);

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal triangles with different value types";

      constexpr Point2<float> p1(2, 0);
      constexpr Point2<float> p2(-2, 0);
      constexpr Point2<float> p3(0, 2);
      const Triangle a(p1, p2, p3);

      constexpr Point2<int> q1(2, 0);
      constexpr Point2<int> q2(-2, 0);
      constexpr Point2<int> q3(0, 2);
      const Triangle b(q1, q2, q3);

      VERIFY(!(a != b), caseLabel);
   }
}

} // namespace


void testTriangle()
{
   testTriangleDefaultCtor();
   testTriangleValueCtor();
   testTriangleSubscriptOperator();
   testTriangleHasVertex();
   testTriangleVertexArray();
   testTriangleEdge();
   testTriangleIsPoint();
   testTriangleIsLine();
   testTriangleIsDegenerate();
   testTriangleArea();
   testTriangleCalcCircumcircle();
   testTriangleCalcCircumcenter();
   testTriangleEquality();
   testTriangleInequality();
}
