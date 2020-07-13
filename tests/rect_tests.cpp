//
// geomcpp tests
// Tests for rectangles.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "rect_tests.h"
#include "rect.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testRectDefaultCtor()
{
   {
      const std::string caseLabel = "Rect default ctor";

      Rect<float> r;
      VERIFY(r.left() == 0.0f, caseLabel);
      VERIFY(r.top() == 0.0f, caseLabel);
      VERIFY(r.right() == 0.0f, caseLabel);
      VERIFY(r.bottom() == 0.0f, caseLabel);
   }
}


void testRectValueCtor()
{
   {
      const std::string caseLabel = "Rect value ctor";

      Rect r{1, 2, 3, 4};
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect value ctor for float with integer values";

      Rect<float> r{3, 4, 1, 2};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect value ctor for denormalized coordinates";

      Rect r{3, 4, 1, 2};
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
}


void testRectPointCtor()
{
   {
      const std::string caseLabel = "Rect point ctor for float points";

      const Rect<float> r{Point2{1.0f, 2.0f}, Point2{3.0f, 4.0f}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect point ctor for points of other value type";

      const Rect<float> r{Point2{1, 2}, Point2{3, 4}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect point ctor for denormalized points";

      const Rect<float> r{Point2{3.0f, 4.0f}, Point2{1.0f, 2.0f}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect point ctor for constexpr declaration";

      constexpr Rect<float> r{Point2{3.0f, 4.0f}, Point2{1.0f, 2.0f}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
}


void testRectSetters()
{
   {
      const std::string caseLabel = "Rect coordinate setters for normalized coordinates";

      Rect<double> r;
      r.setLeft(-1.0);
      r.setTop(-2.0);
      r.setRight(3.0);
      r.setBottom(4.0);

      VERIFY(r.left() == -1.0, caseLabel);
      VERIFY(r.top() == -2.0, caseLabel);
      VERIFY(r.right() == 3.0, caseLabel);
      VERIFY(r.bottom() == 4.0, caseLabel);
   }
   {
      const std::string caseLabel = "Rect coordinate setters for denormalized left";

      Rect r{1, 2, 3, 4};
      r.setLeft(7);
      VERIFY(r.left() == 3, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 7, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect coordinate setters for denormalized top";

      Rect r{1, 2, 3, 4};
      r.setTop(7);
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 4, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 7, caseLabel);
   }
   {
      const std::string caseLabel = "Rect coordinate setters for denormalized right";

      Rect r{1, 2, 3, 4};
      r.setRight(-1);
      VERIFY(r.left() == -1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 1, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect coordinate setters for denormalized bottom";

      Rect r{1, 2, 3, 4};
      r.setBottom(-1);
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == -1, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 2, caseLabel);
   }
}


void testRectIsDegenerate()
{
   {
      const std::string caseLabel = "Rect::isDegenerate for non-degenerate rect";

      const Rect r{1, 2, 3, 4};
      VERIFY(!r.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel = "Rect::isDegenerate for degenerate rect";

      const Rect rh{1, 2, 1, 4};
      VERIFY(rh.isDegenerate(), caseLabel);
      const Rect rv{1, 2, 3, 2};
      VERIFY(rv.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel = "Rect::isDegenerate for empty rect";

      const Rect<float> r;
      VERIFY(r.isDegenerate(), caseLabel);
   }
}


void testRectWidth()
{
   {
      const std::string caseLabel = "Rect width";

      Rect r{1, 2, 30, 4};
      VERIFY(r.width() == 29, caseLabel);
   }
}


void testRectHeight()
{
   {
      const std::string caseLabel = "Rect width";

      Rect r{1.0, 2.0, 3.0, 40.0};
      VERIFY(sutil::equal(r.height(), 38.0), caseLabel);
   }
}


void testRectLeftTop()
{
   {
      const std::string caseLabel = "Rect::leftTop";

      Rect r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.leftTop() == Point2{1.0, 2.0}), caseLabel);
   }
}


void testRectRightTop()
{
   {
      const std::string caseLabel = "Rect::rightTop";

      Rect r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.rightTop() == Point2{3.0, 2.0}), caseLabel);
   }
}


void testRectLeftBottom()
{
   {
      const std::string caseLabel = "Rect::leftBottom";

      Rect r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.leftBottom() == Point2{1.0, 4.0}), caseLabel);
   }
}


void testRectRightBottom()
{
   {
      const std::string caseLabel = "Rect::rightBottom";

      Rect r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.rightBottom() == Point2{3.0, 4.0}), caseLabel);
   }
}


void testRectCenter()
{
   {
      const std::string caseLabel = "Rect::center";

      Rect r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.center() == Point2{2.0, 3.0}), caseLabel);
   }
}


void testRectIsPointInRect()
{
   {
      const std::string caseLabel = "Is point in rect for point inside";

      Rect r{-1, -2, 5, 7};
      VERIFY(r.isPointInRect(Point2{2, 4}), caseLabel);
      VERIFY(r.isPointInRect(Point2{3, -1}), caseLabel);
      VERIFY(r.isPointInRect(Point2{-0.2, 6.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Is point in rect for point outside";

      Rect r{-1, -2, 5, 7};
      VERIFY(!r.isPointInRect(Point2{-2, 4}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{0, -4}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{8.0, 4.0}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{3, 10}), caseLabel);
   }
   {
      const std::string caseLabel = "Is point in rect for point on edge";

      Rect r{-1, -2, 5, 7};
      VERIFY(r.isPointInRect(Point2{2, -2}), caseLabel);
      VERIFY(r.isPointInRect(Point2{5, 2}), caseLabel);
      VERIFY(r.isPointInRect(Point2{0.0, 7.0}), caseLabel);
      VERIFY(r.isPointInRect(Point2{-1, 1}), caseLabel);
   }
}


void testRectInflate()
{
   {
      const std::string caseLabel = "Rect::inflate for positive offset";

      Rect r{1, 2, 3, 7};
      r.inflate(2);
      VERIFY(equal(r.left(), -1), caseLabel);
      VERIFY(equal(r.top(), 0), caseLabel);
      VERIFY(equal(r.right(), 5), caseLabel);
      VERIFY(equal(r.bottom(), 9), caseLabel);
   }
   {
      const std::string caseLabel = "Rect::inflate for negative offset";

      Rect r{1, 2, 5, 7};
      r.inflate(-1);
      VERIFY(equal(r.left(), 2), caseLabel);
      VERIFY(equal(r.top(), 3), caseLabel);
      VERIFY(equal(r.right(), 4), caseLabel);
      VERIFY(equal(r.bottom(), 6), caseLabel);
   }
   {
      const std::string caseLabel = "Rect::inflate for offset that denormalizes rect";

      Rect r{1, 2, 4, 5};
      r.inflate(-3);
      VERIFY(equal(r.left(), 1), caseLabel);
      VERIFY(equal(r.top(), 2), caseLabel);
      VERIFY(equal(r.right(), 4), caseLabel);
      VERIFY(equal(r.bottom(), 5), caseLabel);
   }
}


void testRectEquality()
{
   {
      const std::string caseLabel = "Equality for equal rects";

      const Rect<float> a{0.011f, -345.78f, 1.2f, 4.5f};
      const Rect<float> b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rects whose left coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect<double> a{1.23456789000000000001, 3.2, 3.4, 5.8};
      const Rect<double> b{1.23456789000000000002, 3.2, 3.4, 5.8};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rects whose top coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect<double> a{3.2, 1.23456789000000000001, 3.4, 5.8};
      const Rect<double> b{3.2, 1.23456789000000000002, 3.4, 5.8};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal rects";

      const Rect<float> a{0.011f, -345.78f, 1.2f, 4.8f};
      const Rect<float> b{7.6f, -2.2f, 12.2f, 14.8f};
      VERIFY(!(a == b), caseLabel);
   }
}


void testRectInequality()
{
   {
      const std::string caseLabel = "Inequality for equal rects";

      const Rect<float> a{0.011f, -345.78f, 1.2f, 4.5f};
      const Rect<float> b = a;
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rects whose left coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect<double> a{1.23456789000000000001, 3.2, 3.4, 5.8};
      const Rect<double> b{1.23456789000000000002, 3.2, 3.4, 5.8};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rects whose top coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect<double> a{3.2, 1.23456789000000000001, 3.4, 5.8};
      const Rect<double> b{3.2, 1.23456789000000000002, 3.4, 5.8};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal rects";

      const Rect<float> a{0.011f, -345.78f, 1.2f, 4.8f};
      const Rect<float> b{7.6f, -2.2f, 12.2f, 14.8f};
      VERIFY(a != b, caseLabel);
   }
}


void testRectIntersect()
{
   {
      const std::string caseLabel = "Intersect vertically overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{-3, 1, 7, 10};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect{-1, 1, 5, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect horizontally overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{2, -4, 10, 8};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect{2, -2, 5, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect diagonally overlapping rects (case 1)";

      const Rect a{-1.0, -2.0, 5.0, 7.0};
      const Rect b{-3.0, -5.0, 3.0, 4.0};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect{-1.0, -2.0, 3.0, 4.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect diagonally overlapping rects (case 2)";

      const Rect a{-1.0, -2.0, 5.0, 7.0};
      const Rect b{1.0, 3.0, 7.0, 10.0};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect{1.0, 3.0, 5.0, 7.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect nested rects";

      const Rect a{-1, -2, 5, 7};
      const Rect inner{1, 0, 3, 2};
		const Rect isect = intersect(a, inner);
      VERIFY((isect == inner), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect vertically non-overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{2, 10, 7, 14};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect<int>{}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect horizontally non-overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{8, 2, 11, 14};
		const Rect isect = intersect(a, b);
      VERIFY((isect == Rect<int>{}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect same rects";

      const Rect a{-1, -2, 5, 7};
		const Rect isect = intersect(a, a);
      VERIFY((isect == a), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with empty rect";

      const Rect a{-1, -2, 5, 7};
      const Rect<int> empty{};
      const Rect isect = intersect(a, empty);
      VERIFY((isect == empty), caseLabel);
   }
}


void testRectUnite()
{
   {
      const std::string caseLabel = "Unite vertically overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{-3, 1, 7, 10};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-3, -2, 7, 10}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite horizontally overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{2, -4, 10, 8};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-1, -4, 10, 8}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite diagonally overlapping rects (case 1)";

      const Rect a{-1.0, -2.0, 5.0, 7.0};
      const Rect b{-3.0, -5.0, 3.0, 4.0};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-3.0, -5.0, 5.0, 7.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite diagonally overlapping rects (case 2)";

      const Rect a{-1.0, -2.0, 5.0, 7.0};
      const Rect b{1.0, 3.0, 7.0, 10.0};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-1.0, -2.0, 7.0, 10.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite nested rects";

      const Rect outer{-1, -2, 5, 7};
      const Rect b{1, 0, 3, 2};
		const Rect u = unite(outer, b);
      VERIFY((u == outer), caseLabel);
   }
   {
      const std::string caseLabel = "Unite vertically non-overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{2, 10, 7, 14};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-1, -2, 7, 14}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite horizontally non-overlapping rects";

      const Rect a{-1, -2, 5, 7};
      const Rect b{8, 2, 11, 14};
		const Rect u = unite(a, b);
      VERIFY((u == Rect{-1, -2, 11, 14}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite same rects";

      const Rect a{-1, -2, 5, 7};
		const Rect u = unite(a, a);
      VERIFY((u == a), caseLabel);
   }
   {
      const std::string caseLabel = "Unite with empty rect";

      const Rect a{-1, -2, 5, 7};
      const Rect<int> empty{};
      const Rect u = unite(a, empty);
      VERIFY((u == a), caseLabel);
   }
}

} // namespace


///////////////////

void testRect()
{
   testRectDefaultCtor();
   testRectValueCtor();
   testRectPointCtor();
   testRectSetters();
   testRectIsDegenerate();
   testRectWidth();
   testRectHeight();
   testRectLeftTop();
   testRectCenter();
   testRectIsPointInRect();
   testRectInflate();
   testRectEquality();
   testRectInequality();
   testRectIntersect();
   testRectUnite();
}
