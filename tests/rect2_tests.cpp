//
// geomcpp tests
// Tests for 2D rectangles.
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "rect2_tests.h"
#include "rect2.h"
#include "test_util.h"
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testRect2DefaultCtor()
{
   {
      const std::string caseLabel = "Rect2 default ctor";

      Rect2<float> r;
      VERIFY(r.left() == 0.0f, caseLabel);
      VERIFY(r.top() == 0.0f, caseLabel);
      VERIFY(r.right() == 0.0f, caseLabel);
      VERIFY(r.bottom() == 0.0f, caseLabel);
   }
}


void testRect2ValueCtor()
{
   {
      const std::string caseLabel = "Rect2 value ctor";

      Rect2 r{1, 2, 3, 4};
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 value ctor for float with integer values";

      Rect2<float> r{3, 4, 1, 2};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 value ctor for denormalized coordinates";

      Rect2 r{3, 4, 1, 2};
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
}


void testRect2PointCtor()
{
   {
      const std::string caseLabel = "Rect2 point ctor for float points";

      const Rect2<float> r{Point2{1.0f, 2.0f}, Point2{3.0f, 4.0f}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 point ctor for points of other value type";

      const Rect2<float> r{Point2{1, 2}, Point2{3, 4}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 point ctor for denormalized points";

      const Rect2<float> r{Point2{3.0f, 4.0f}, Point2{1.0f, 2.0f}};
      VERIFY(r.left() == 1.0f, caseLabel);
      VERIFY(r.top() == 2.0f, caseLabel);
      VERIFY(r.right() == 3.0f, caseLabel);
      VERIFY(r.bottom() == 4.0f, caseLabel);
   }
}


void testRect2Setters()
{
   {
      const std::string caseLabel = "Rect2 coordinate setters for normalized coordinates";

      Rect2<double> r;
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
      const std::string caseLabel = "Rect2 coordinate setters for denormalized left";

      Rect2 r{1, 2, 3, 4};
      r.setLeft(7);
      VERIFY(r.left() == 3, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 7, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 coordinate setters for denormalized top";

      Rect2 r{1, 2, 3, 4};
      r.setTop(7);
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == 4, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 7, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 coordinate setters for denormalized right";

      Rect2 r{1, 2, 3, 4};
      r.setRight(-1);
      VERIFY(r.left() == -1, caseLabel);
      VERIFY(r.top() == 2, caseLabel);
      VERIFY(r.right() == 1, caseLabel);
      VERIFY(r.bottom() == 4, caseLabel);
   }
   {
      const std::string caseLabel = "Rect2 coordinate setters for denormalized bottom";

      Rect2 r{1, 2, 3, 4};
      r.setBottom(-1);
      VERIFY(r.left() == 1, caseLabel);
      VERIFY(r.top() == -1, caseLabel);
      VERIFY(r.right() == 3, caseLabel);
      VERIFY(r.bottom() == 2, caseLabel);
   }
}


void testRect2IsDegenerate()
{
   {
      const std::string caseLabel = "Rect2::isDegenerate for non-degenerate rect";

      const Rect2 r{1, 2, 3, 4};
      VERIFY(!r.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel = "Rect2::isDegenerate for degenerate rect";

      const Rect2 rh{1, 2, 1, 4};
      VERIFY(rh.isDegenerate(), caseLabel);
      const Rect2 rv{1, 2, 3, 2};
      VERIFY(rv.isDegenerate(), caseLabel);
   }
   {
      const std::string caseLabel = "Rect2::isDegenerate for empty rect";

      const Rect2<float> r;
      VERIFY(r.isDegenerate(), caseLabel);
   }
}


void testRect2Width()
{
   {
      const std::string caseLabel = "Rect2 width";

      Rect2 r{1, 2, 30, 4};
      VERIFY(r.width() == 29, caseLabel);
   }
}


void testRect2Height()
{
   {
      const std::string caseLabel = "Rect2 width";

      Rect2 r{1.0, 2.0, 3.0, 40.0};
      VERIFY(sutil::equal(r.height(), 38.0), caseLabel);
   }
}


void testRect2LeftTop()
{
   {
      const std::string caseLabel = "Rect2::leftTop";

      Rect2 r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.leftTop() == Point2{1.0, 2.0}), caseLabel);
   }
}


void testRect2RightTop()
{
   {
      const std::string caseLabel = "Rect2::rightTop";

      Rect2 r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.rightTop() == Point2{3.0, 2.0}), caseLabel);
   }
}


void testRect2LeftBottom()
{
   {
      const std::string caseLabel = "Rect2::leftBottom";

      Rect2 r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.leftBottom() == Point2{1.0, 4.0}), caseLabel);
   }
}


void testRect2RightBottom()
{
   {
      const std::string caseLabel = "Rect2::rightBottom";

      Rect2 r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.rightBottom() == Point2{3.0, 4.0}), caseLabel);
   }
}


void testRect2Center()
{
   {
      const std::string caseLabel = "Rect2::center";

      Rect2 r{1.0, 2.0, 3.0, 4.0};
      VERIFY((r.center() == Point2{2.0, 3.0}), caseLabel);
   }
}


void testRect2IsPointInRect()
{
   {
      const std::string caseLabel = "Is point in rect for point inside";

      Rect2 r{-1, -2, 5, 7};
      VERIFY(r.isPointInRect(Point2{2, 4}), caseLabel);
      VERIFY(r.isPointInRect(Point2{3, -1}), caseLabel);
      VERIFY(r.isPointInRect(Point2{-0.2, 6.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Is point in rect for point outside";

      Rect2 r{-1, -2, 5, 7};
      VERIFY(!r.isPointInRect(Point2{-2, 4}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{0, -4}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{8.0, 4.0}), caseLabel);
      VERIFY(!r.isPointInRect(Point2{3, 10}), caseLabel);
   }
   {
      const std::string caseLabel = "Is point in rect for point on edge";

      Rect2 r{-1, -2, 5, 7};
      VERIFY(r.isPointInRect(Point2{2, -2}), caseLabel);
      VERIFY(r.isPointInRect(Point2{5, 2}), caseLabel);
      VERIFY(r.isPointInRect(Point2{0.0, 7.0}), caseLabel);
      VERIFY(r.isPointInRect(Point2{-1, 1}), caseLabel);
   }
}


void testRect2Inflate()
{
   {
      const std::string caseLabel = "Rect2::inflate for positive offset";

      Rect2 r{1, 2, 3, 7};
      r.inflate(2);
      VERIFY(equal(r.left(), -1), caseLabel);
      VERIFY(equal(r.top(), 0), caseLabel);
      VERIFY(equal(r.right(), 5), caseLabel);
      VERIFY(equal(r.bottom(), 9), caseLabel);
   }
   {
      const std::string caseLabel = "Rect2::inflate for negative offset";

      Rect2 r{1, 2, 5, 7};
      r.inflate(-1);
      VERIFY(equal(r.left(), 2), caseLabel);
      VERIFY(equal(r.top(), 3), caseLabel);
      VERIFY(equal(r.right(), 4), caseLabel);
      VERIFY(equal(r.bottom(), 6), caseLabel);
   }
   {
      const std::string caseLabel = "Rect2::inflate for offset that denormalizes rect";

      Rect2 r{1, 2, 4, 5};
      r.inflate(-3);
      VERIFY(equal(r.left(), 1), caseLabel);
      VERIFY(equal(r.top(), 2), caseLabel);
      VERIFY(equal(r.right(), 4), caseLabel);
      VERIFY(equal(r.bottom(), 5), caseLabel);
   }
}


void testRect2Equality()
{
   {
      const std::string caseLabel = "Equality for equal rects";

      const Rect2<float> a{0.011f, -345.78f, 1.2f, 4.5f};
      const Rect2<float> b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rects whose left coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect2<double> a{1.23456789000000000001, 3.2, 3.4, 5.8};
      const Rect2<double> b{1.23456789000000000002, 3.2, 3.4, 5.8};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Equality for equal double rects whose top coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect2<double> a{3.2, 1.23456789000000000001, 3.4, 5.8};
      const Rect2<double> b{3.2, 1.23456789000000000002, 3.4, 5.8};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Equality for unequal rects";

      const Rect2<float> a{0.011f, -345.78f, 1.2f, 4.8f};
      const Rect2<float> b{7.6f, -2.2f, 12.2f, 14.8f};
      VERIFY(!(a == b), caseLabel);
   }
}


void testRect2Inequality()
{
   {
      const std::string caseLabel = "Inequality for equal rects";

      const Rect2<float> a{0.011f, -345.78f, 1.2f, 4.5f};
      const Rect2<float> b = a;
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rects whose left coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect2<double> a{1.23456789000000000001, 3.2, 3.4, 5.8};
      const Rect2<double> b{1.23456789000000000002, 3.2, 3.4, 5.8};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Inequality for equal double rects whose top coordinates are unequal beyond the "
         "epsilon threshold";

      const Rect2<double> a{3.2, 1.23456789000000000001, 3.4, 5.8};
      const Rect2<double> b{3.2, 1.23456789000000000002, 3.4, 5.8};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel = "Inequality for unequal rects";

      const Rect2<float> a{0.011f, -345.78f, 1.2f, 4.8f};
      const Rect2<float> b{7.6f, -2.2f, 12.2f, 14.8f};
      VERIFY(a != b, caseLabel);
   }
}


void testRect2Intersect()
{
   {
      const std::string caseLabel = "Intersect vertically overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{-3, 1, 7, 10};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2{-1, 1, 5, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect horizontally overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{2, -4, 10, 8};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2{2, -2, 5, 7}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect diagonally overlapping rects (case 1)";

      const Rect2 a{-1.0, -2.0, 5.0, 7.0};
      const Rect2 b{-3.0, -5.0, 3.0, 4.0};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2{-1.0, -2.0, 3.0, 4.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect diagonally overlapping rects (case 2)";

      const Rect2 a{-1.0, -2.0, 5.0, 7.0};
      const Rect2 b{1.0, 3.0, 7.0, 10.0};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2{1.0, 3.0, 5.0, 7.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect nested rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 inner{1, 0, 3, 2};
		const Rect2 isect = intersect(a, inner);
      VERIFY((isect == inner), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect vertically non-overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{2, 10, 7, 14};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2<int>{}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect horizontally non-overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{8, 2, 11, 14};
		const Rect2 isect = intersect(a, b);
      VERIFY((isect == Rect2<int>{}), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect same rects";

      const Rect2 a{-1, -2, 5, 7};
		const Rect2 isect = intersect(a, a);
      VERIFY((isect == a), caseLabel);
   }
   {
      const std::string caseLabel = "Intersect with empty rect";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2<int> empty{};
      const Rect2 isect = intersect(a, empty);
      VERIFY((isect == empty), caseLabel);
   }
}


void testRect2Unite()
{
   {
      const std::string caseLabel = "Unite vertically overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{-3, 1, 7, 10};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-3, -2, 7, 10}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite horizontally overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{2, -4, 10, 8};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-1, -4, 10, 8}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite diagonally overlapping rects (case 1)";

      const Rect2 a{-1.0, -2.0, 5.0, 7.0};
      const Rect2 b{-3.0, -5.0, 3.0, 4.0};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-3.0, -5.0, 5.0, 7.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite diagonally overlapping rects (case 2)";

      const Rect2 a{-1.0, -2.0, 5.0, 7.0};
      const Rect2 b{1.0, 3.0, 7.0, 10.0};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-1.0, -2.0, 7.0, 10.0}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite nested rects";

      const Rect2 outer{-1, -2, 5, 7};
      const Rect2 b{1, 0, 3, 2};
		const Rect2 u = unite(outer, b);
      VERIFY((u == outer), caseLabel);
   }
   {
      const std::string caseLabel = "Unite vertically non-overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{2, 10, 7, 14};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-1, -2, 7, 14}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite horizontally non-overlapping rects";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2 b{8, 2, 11, 14};
		const Rect2 u = unite(a, b);
      VERIFY((u == Rect2{-1, -2, 11, 14}), caseLabel);
   }
   {
      const std::string caseLabel = "Unite same rects";

      const Rect2 a{-1, -2, 5, 7};
		const Rect2 u = unite(a, a);
      VERIFY((u == a), caseLabel);
   }
   {
      const std::string caseLabel = "Unite with empty rect";

      const Rect2 a{-1, -2, 5, 7};
      const Rect2<int> empty{};
      const Rect2 u = unite(a, empty);
      VERIFY((u == a), caseLabel);
   }
}

} // namespace


///////////////////

void testRect2D()
{
   testRect2DefaultCtor();
   testRect2ValueCtor();
   testRect2PointCtor();
   testRect2Setters();
   testRect2IsDegenerate();
   testRect2Width();
   testRect2Height();
   testRect2LeftTop();
   testRect2Center();
   testRect2IsPointInRect();
   testRect2Inflate();
   testRect2Equality();
   testRect2Inequality();
   testRect2Intersect();
   testRect2Unite();
}
