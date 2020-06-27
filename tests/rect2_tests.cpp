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
}


void testRect2Equality()
{
}


void testRect2Inequality()
{
}


void testRect2Intersect()
{
}


void testRect2Unite()
{
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
