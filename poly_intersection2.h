//
// geomcpp
// Intersection of 2D polygons.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_ct.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include "poly2.h"

namespace geom
{

namespace internals
{
///////////////////

// Indicates which polygon's edge is inside the other polygon.
enum class InsideFlag
{
   PInside,
   QInside,
   Unknown
};

// Traversal along a given poygon starting at a given point.
template <typename T> class Traversal
{
 public:
   Traversal(const Poly2<T>& poly, int start, InsideFlag inside)
   {
      m_poly = poly;
      m_ptIdx = start;
      m_curPt = poly[ptIdx];
      m_curEdge = poly.edge(edgeIndex(ptIdx));
      m_insideFlag = inside;
   }

   Point2<T> point() const { return curPt; }
   ct::LineSeg2<T> edge() const { return curEdge; }

   // Advance to next point and edge.
   void advance()
   {
      m_ptIdx = (m_ptIdx + 1) % m_poly.size();
      curPt = poly[ptIdx];
      curEdge = poly.edge(edgeIndex(ptIdx));
   }

   // If this traversed polygon is the inside one, add its current point to
   // the output.
   void collectPointIfInside(InsideFlag curInside, Poly2<T>& out)
   {
      if (curInside == insideFlag)
         addUniquePoint(out, curPt);
   }

   // Checks if a given point lies on the side of the current edge that is
   // towards the inside of the (ccw) polygon. Being on the edge is also
   // considered 'inside'.
   bool isPointOnInside(const Point2<T>& pt)
   {
      const Vec2<T> v(curEdge.startPoint(), pt);
      return sutil::lessEqual<Fp>(perpDot(curEdge.direction(), v), 0.0);
   }

   bool isEdgeCcwOrCollinear(const LineSeg2<T>& e)
   {
      return sutil::lessEqual<Fp>(perpDot(curEdge.direction(), e.direction()), 0.0);
   }

   // Returns the index of the edge that the algorithm associates with a
   // given point index.
   int edgeIndex(int ptIdx) { return (ptIdx != 0) ? ptIdx - 1 : poly.numEdges() - 1; }

 private:
   using Fp = sutil::FpType<T>;

 private:
   Poly2<T> m_poly;
   int m_ptIdx;
   Point2<T> m_curPt;
   ct::LineSeg2<T> m_curEdge;
   // Inside flag value for this polygon.
   InsideFlag m_insideFlag;
};


// Advances the traversal state of one of the polygons.
template <typename T>
void advance(Traversal<T>& p, Traversal<T>& q, InsideFlag curInside, Poly2<T>& out)
{
   Traversal<T> rear = q.isEdgeCcwOrCollinear(p.edge())
                          ? (q.isPointOnInside(p.point()) ? q : p)
                          : (p.isPointOnInside(q.point()) ? p : q);

   rear.collectPointIfInside(curInside, out);
   rear.advance();
}


// Intersects a given point with a given polygon and returns the result as
// a polygon.
template <typename T>
Poly2<T> intersectWithPoint(const Point2<T>& pt, const Poly2<T>& poly)
{
   Poly2<T> resultPoly;
   if (isPointInsideConvexPolygon(poly, pt))
      resultPoly.add(pt);
   return resultPoly;
}


// Adds a given point to a given polygon if the polygon does not contain it already.
template <typename T> void addUniquePoint(Poly2<T>& poly, const Point2<T>& pt)
{
   const bool hasVertex = poly.contains(pt) != poly.end();
   if (!hasVertex)
      poly.add(pt);
}


// Inserts a given point to a given polygon if the polygon does not contain it
// already.
template <typename T>
void insertUniquePoint(Poly2<T>& poly, const Point2<T>& pt, std::size_t idx)
{
   const bool hasVertex = poly.contains(pt) != poly.end();
   if (!hasVertex)
      poly.insert(pt, idx);
}

// Checks if a given convex polygon's orientation is counter-clockwise.
// Assumes the polygon is not degenerate.
template <typename T> bool isCcw(const Poly2<T>& poly)
{
   // Since we know it's a convex polygon we only have to check the
   // orientation of the first two edges.
   return poly.edge(0).direction().isCcw(poly.edge(1).direction());
}


// Changes the orientation of a given polygon to counter-clockwise.
template <typename T> Poly2<T> makeCcw(const Poly2<T>& poly)
{
   if (!isCcw(poly))
      return poly.reversed();
   return poly;
}


// Intersects a given line with a given polygon and returns the result as
// a polygon.
template <typename T, template <typename> typename LineT>
Poly2<T> intersectWithLine(const ct::Line2<T, LineT>& line, const Poly2<T>& poly)
{
   Poly2<T> resultPoly;

   // Find intersections of the line and the polygon's edges.
   for (std::size_t i = 0; i < poly.numEdges(); ++i)
   {
      const auto x = intersect(line, poly.edge(i));
      if (x && std::holds_alternative<Point2<T>>(*x)))
      {
			addUniquePoint(resultPoly, std::get<Point2<T>>(*x));
		}
      else if (x && std::holds_alternative<ct::LineSeg2<T>>(*x)))
         {
            const ct::LineSeg2<T>& xLine = std::get<ct::LineSeg2<T>>(*x);
            addUniquePoint(resultPoly, xLine.startPoint());
            addUniquePoint(resultPoly, xLine.endPoint());
         }
   }

   // Add the line end points that are inside the polygon to the result.
   // Note that for the case of two intersection points we know already
   // that none of the line end points can be inside the polygon.
   const std::size_t numVert = resultPoly.size();
   if (numVert == 0 || numVert == 1)
   {
      const std::optional<Point2<T>> vert = (numVert == 1) ? resultPoly[0] : std::nullopt;
      const std::optional<Point2<T>> lineStart = line.startPoint();
      const std::optional<Point2<T>> lineEnd = line.endPoint();

      if ((vert != lineStart && *vert != *lineStart) &&
          isPointInsideConvexPolygon(poly, *lineStart))
      {
         insertUniquePoint(resultPoly, *lineStart, 0);
      }
      if ((vert != lineEnd && *vert != *lineEnd) &&
          isPointInsideConvexPolygon(poly, *lineEnd))
      {
         addUniquePoint(resultPoly, *lineEnd);
      }
   }

   return resultPoly;
}

} // namespace internals


///////////////////

// Intersects two convex polygons.
// Source: https://www.cs.jhu.edu/~misha/Spring16/ORourke82.pdf
template <typename T> Poly2<T> intersect(const Poly2<T>& PIn, const Poly2<T>& QIn)
{
   using internals::InsideFlag;
   using internals::Traversal;

   Poly2<T> resultPoly;

   // Special cases.
   if (PIn.size() == 0 || QIn.size() == 0)
      return resultPoly;
   if (PIn.size() == 1)
      return intersectWithPoint(PIn.vertex(0), QIn);
   if (QIn.size() == 1)
      return intersectWithPoint(QIn.vertex(0), PIn);
   if (PIn.size() == 2)
      return intersectWithLine(PIn.edge(0), QIn);
   if (QIn.size() == 2)
      return intersectWithLine(QIn.edge(0), PIn);
   if (!PIn.isConvex() || !QIn.isConvex())
      return resultPoly;

   Poly2<T> P = makeCcw(PIn);
   Poly2<T> Q = makeCcw(QIn);

   std::size_t maxIter = 2 * (P.numEdges() + Q.numEdges());
   std::size_t numIter = 0;

   std::optional<Point2<T>> firstIsectPt;
   int firstIsectFoundIter = -1;

   Traversal<T> p(P, 1, InsideFlag::PInside);
   Traversal<T> q(Q, 1, InsideFlag::QInside);
   InsideFlag curInside = InsideFlag::Unknown;

   while (numIter <= maxIter)
   {
      // Find intersection.
      const auto x = ct::intersect(p.edge(), q.edge());
      // Collinear edges, i.e. intersections that result in a line, are
      // interpreted as 'no intersection'.
      if (x && std::holds_alternative<Point2<T>>(*x))
      {
         Point2<T> isectPt = std::get<Point2<T>>(*x);
         if (firstIsectPt)
         {
            // Keep track of first intersection and the iteration it was
            // found in to detect a complete loop around the polygons.
            firstIsectPt = isectPt;
            firstIsectFoundIter = numIter;
         }
         else if (isectPt == firstIsectPt && firstIsectFoundIter != numIter - 1)
         {
            // First intersection reached again. Stop.
            return resultPoly;
         }

         addUniquePoint(resultPoly, isectPt);

         if (q.isPointOnInside(p.point()))
            curInside = InsideFlag::PInside;
         else
            curInside = InsideFlag::QInside;
      }

      // Advance.
      advance(p, q, curInside, resultPoly);
      ++numIter;
   }

   // The polygons either don't intersect at all or one is completely within
   // the other.
   if (isPointInsideConvexPolygon(Q, p.point()))
      return P;
   else if (isPointInsideConvexPolygon(P, q.point()))
      return Q;
   return Poly2{};
}

} // namespace geom
