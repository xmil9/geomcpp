//
// geomcpp
// Cutting of 2D polygons.
//
// Aug-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line_inf2_ct.h"
#include "line_intersection2_ct.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include "poly2.h"
#include "vec2.h"
#include "essentutils/fputil.h"
#include <variant>
#include <vector>


namespace geom
{

namespace internals
{
///////////////////

// Sides that a point can be on relative to a line.
enum class Side
{
   None,
   Left,
   Right,
   Center
};


// Calculates which side of a line a given point is on.
template <typename T> Side calcSideOfLine(const ct::LineInf2<T>& l, const Point2<T>& pt)
{
   using Fp = sutil::FpType<T>;

   const auto perpDotResult = perpDot(l.direction(), Vec2<T>(l.anchor(), pt));
   if (sutil::less<Fp>(perpDotResult, 0))
      return Side::Left;
   else if (sutil::greater<Fp>(perpDotResult, 0))
      return Side::Right;
   return Side::Center;
}


// Checks if the intersecting line was crossed by two consecutive vertices.
bool wasLineCrossed(Side prev, Side now)
{
   return (now == Side::Left && prev == Side::Right) ||
          (now == Side::Right && prev == Side::Left);
}


// Adds the intersection point of the cutting line and a polygon edge to the output
// polygons.
template <typename T>
void collectIntersection(const ct::LineInf2<T>& line, const ct::LineSeg2<T> edge,
                         Poly2<T>& leftPoly, Poly2<T>& rightPoly)
{
   const auto x = ct::intersect(line, edge);
   if (x && std::holds_alternative<Point2<T>>(*x))
   {
      leftPoly.add(std::get<Point2<T>>(*x));
      rightPoly.add(std::get<Point2<T>>(*x));
   }
}

} // namespace internals


///////////////////

// Cuts a convex polygon by an infinite line.
template <typename T>
std::vector<Poly2<T>> cutConvexPolygon(const Poly2<T>& poly, const ct::LineInf2<T>& line)
{
   using internals::Side;

   std::vector<Poly2<T>> res;

   // Create the output polygons.
   Poly2<T> leftPoly;
   Poly2<T> rightPoly;
   // Keep track of whether any points lie strictly on each side. Helps identifying
   // cases where the line only touches the polygon.
   bool haveStrictlyLeftPoints = false;
   bool haveStrictlyRightPoints = false;
   // Keep track of changes from one side of the line to another.
   Side side = Side::None;
   Side prevSide = Side::None;

   // Place the polygon vertices into two separate polygons depending on whether
   // a vertex is left or right of the cut line.
   for (int i = 0; i < poly.size(); ++i)
   {
      const Point2<T> pt = poly[i];

      // First determine the side the current vertex is on.
      prevSide = side;
      side = internals::calcSideOfLine(line, pt);

      // If the vertices switched from one side of the line to the other, we need
      // to find the intersection point and add it to both output polygons. Since
      // we are building the output polygons as we traverse the vertices the
      // intersection point has to be added to the outout polygons before the
      // current vertex otherwise the vertices will be out of order.
      if (internals::wasLineCrossed(prevSide, side))
      {
         ct::LineSeg2<T> edge(poly[i - 1], pt);
         internals::collectIntersection(line, edge, leftPoly, rightPoly);
      }

      // Now we can add the current vertex to the output polygons depending on
      // which side of the line it is on.
      if (side == Side::Left)
      {
         leftPoly.add(pt);
         haveStrictlyLeftPoints = true;
      }
      else if (side == Side::Right)
      {
         rightPoly.add(pt);
         haveStrictlyRightPoints = true;
      }
      else
      {
         leftPoly.add(pt);
         rightPoly.add(pt);
      }
   }

   // Process the edge that closes the polygon (only for non-degenerate polygons).
   if (poly.size() > 2)
   {
      const Point2<T> firstPt = poly[0];
      const Point2<T> lastPt = poly[poly.size() - 1];
      prevSide = side;
      side = internals::calcSideOfLine(line, firstPt);

      if (internals::wasLineCrossed(prevSide, side))
      {
         const ct::LineSeg2<T> edge(lastPt, firstPt);
         internals::collectIntersection(line, edge, leftPoly, rightPoly);
      }
   }

   // Prepare the list of output polygons.
   // Special case: The input polygon was empty.
   if (leftPoly.size() == 0 && rightPoly.size() == 0)
   {
      res.push_back(leftPoly);
   }
   // Special case: The entire (degenerate) polygon lies on the line.
   else if (!haveStrictlyLeftPoints && !haveStrictlyRightPoints)
   {
      res.push_back(leftPoly);
   }
   // Normal case: Polygon is not degenerate.
   // Keep the output polygons that have any dedicated (not shared) points.
   else
   {
      if (leftPoly.size() > 0 && haveStrictlyLeftPoints)
         res.push_back(leftPoly);
      if (rightPoly.size() > 0 && haveStrictlyRightPoints)
         res.push_back(rightPoly);
   }

   return res;
}

} // namespace geom
