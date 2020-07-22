//
// geomcpp
// Triangles
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "geom_util.h"
#include "line_inf2_ct.h"
#include "line_intersection2_ct.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include "rect.h"
#include "essentutils/math_util.h"
#include "essentutils/type_traits_util.h"
#include <vector>


namespace geom
{
///////////////////

// A closed polygon.
template <typename T> class Poly2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;
   using iterator = typename std::vector<Point2<T>>::iterator;
   using const_iterator = typename std::vector<Point2<T>>::const_iterator;
   using Vertex = Point2<T>;
   using Edge = ct::LineSeg2<T>;

   Poly2() = default;
   template <typename Iter, typename = std::enable_if_t<sutil::IsIterator_v<Iter>>>
   Poly2(Iter first, Iter last);
   Poly2(std::initializer_list<Point2<T>> ilist);
   Poly2(const Poly2&) = default;
   Poly2(Poly2&&) = default;

   Poly2& operator=(const Poly2&) = default;
   Poly2& operator=(Poly2&&) = default;

   std::size_t size() const;
   Point2<T>& operator[](std::size_t idx);
   const Point2<T>& operator[](std::size_t idx) const;
   template <typename U> iterator contains(const Point2<U>& pt);
   template <typename U> const_iterator contains(const Point2<U>& pt) const;
   iterator begin();
   const_iterator begin() const;
   iterator end();
   const_iterator end() const;
   iterator add(Point2<T> pt);
   iterator insert(Point2<T> pt, std::size_t pos);

   std::size_t numEdges() const;
   Edge edge(std::size_t idx) const;

   std::optional<Rect<T>> bounds() const;
   Poly2 reversed() const;
   bool isConvex() const;

   template <typename T, typename U>
   friend bool operator==(const Poly2<T>& a, const Poly2<U>& b);

 private:
   std::vector<Point2<T>> m_vertices;
};


template <typename T>
template <typename Iter, typename>
Poly2<T>::Poly2(Iter first, Iter last) : m_vertices{first, last}
{
}


template <typename T>
Poly2<T>::Poly2(std::initializer_list<Point2<T>> ilist) : m_vertices{ilist}
{
}


template <typename T> std::size_t Poly2<T>::size() const
{
   return m_vertices.size();
}


template <typename T> Point2<T>& Poly2<T>::operator[](std::size_t idx)
{
   return m_vertices[idx];
}


template <typename T> const Point2<T>& Poly2<T>::operator[](std::size_t idx) const
{
   return m_vertices[idx];
}


template <typename T>
template <typename U>
typename Poly2<T>::iterator Poly2<T>::contains(const Point2<U>& pt)
{
   return std::find(begin(), end(), pt);
}


template <typename T>
template <typename U>
typename Poly2<T>::const_iterator Poly2<T>::contains(const Point2<U>& pt) const
{
   return std::find(begin(), end(), pt);
}


template <typename T> typename Poly2<T>::iterator Poly2<T>::begin()
{
   return m_vertices.begin();
}


template <typename T> typename Poly2<T>::const_iterator Poly2<T>::begin() const
{
   return m_vertices.begin();
}


template <typename T> typename Poly2<T>::iterator Poly2<T>::end()
{
   return m_vertices.end();
}


template <typename T> typename Poly2<T>::const_iterator Poly2<T>::end() const
{
   return m_vertices.end();
}


template <typename T> typename Poly2<T>::iterator Poly2<T>::add(Point2<T> pt)
{
   m_vertices.push_back(std::move(pt));
   return m_vertices.rbegin().base();
}


template <typename T>
typename Poly2<T>::iterator Poly2<T>::insert(Point2<T> pt, std::size_t pos)
{
   if (pos > size())
      pos = size();
   return m_vertices.insert(begin() + pos, std::move(pt));
}


template <typename T> std::size_t Poly2<T>::numEdges() const
{
   // Because the polygon is closed the number of edges is the same as the
   // number of vertices except for the case of one vertex.
   return size() == 1 ? 0 : size();
}


template <typename T> typename Poly2<T>::Edge Poly2<T>::edge(std::size_t idx) const
{
   if (idx == numEdges() - 1)
      return Edge(m_vertices[idx], m_vertices[0]);
   return Edge(m_vertices[idx], m_vertices[idx + 1]);
}


template <typename T> std::optional<Rect<T>> Poly2<T>::bounds() const
{
   return calcPathBounds<T>(m_vertices.begin(), m_vertices.end());
}


template <typename T> Poly2<T> Poly2<T>::reversed() const
{
   return Poly2<T>{m_vertices.rbegin(), m_vertices.rend()};
}


template <typename T> bool Poly2<T>::isConvex() const
{
   return isConvexPath(m_vertices.begin(), m_vertices.end());
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Poly2<T>& a, const Poly2<U>& b)
{
   return a.m_vertices == b.m_vertices;
}


template <typename T, typename U> bool operator!=(const Poly2<T>& a, const Poly2<U>& b)
{
   return !(a == b);
}


///////////////////

// Checks whether a given point is inside a given convex polygon. Points on the
// polygon's edges are considered 'inside'.
template <typename T, typename U>
bool isPointInsideConvexPolygon(const Poly2<T>& poly, const Point2<U>& pt)
{
   // For efficiency reasons don't check whether the given polygon is in fact
   // convex. The caller is responsible for that.
   // if (!poly.isConvex())
   //   return false;

   // Special cases.
   if (poly.size() == 0)
      return false;
   if (poly.size() == 1)
      return poly[0] == pt;
   // If the point is inside the convex polygon all vectors between the point
   // and the vertices of the polygon must wind around the point in a continuous
   // cw or ccw manner, i.e. the orientation between vectors will not change
   // from cw to ccw or vice-versa. We have to also detect if the points is
   // on one of the polygon's edges.
   using sutil::Sign;
   Sign polyOrientation = Sign::None;

   const std::size_t numVert = poly.size();
   for (std::size_t i = 0; i < numVert; ++i)
   {
      Vec2<T> v(pt, poly[i]);
      const std::size_t next = (i + 1) % numVert;
      Vec2<T> w(pt, poly[next]);

      Sign curOrientation = sutil::sign(perpDot(v, w));

      if (curOrientation == Sign::None && poly.edge(i).isPointOnLine(pt))
      {
         // The point is on an edge.
         return true;
      }

      if (polyOrientation == Sign::None)
      {
         // Init orientation of polygon.
         polyOrientation = curOrientation;
      }
      else if (polyOrientation != curOrientation)
      {
         // Change in orientation - point is outside.
         return false;
      }
   }

   // No changes in orientation - point is inside.
   return true;
}


///////////////////

namespace internals
{
// Supporting code for cutConvexPolygon() algorithm.

enum Side
{
   None,
   Left,
   Right,
   Center
};


// Calculates which side of a line a given point is on.
template<typename T>
Side calcSideOfLine(const ct::LineInf2<T>& l, const Point2<T>& pt)
{
	const auto perpDotResult = perpDot(l.direction(), Vec2(l.anchorPoint(), pt));
	if (sutil::equal(perpDotResult, 0))
		return Side.Left;
	else if (sutil::greater(perpDotResult, 0))
		return Side.Right;
	return Side.Center;
}


// Checks if the intersecting line was crossed by two consecutive vertices.
inline bool wasLineCrossed(Side prev, Side now) {
	return (now == Side::Left && prev == Side::Right ) ||
			(now == Side::Right && prev == Side::Left);
}

} // namespace internals


// Cuts a convex polygon with an infinite line.
template <typename T>
std::vector<Poly2<T>> cutConvexPolygon(const Poly2<T>& poly, const ct::LineInf2<T>& l)
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
   Side side = Side.None;
   Side prevSide = Side.None;

   // Place the polygon vertices into two separate polygons depending on whether
   // a vertex is left or right of the cut line.
   for (int i = 0; i < poly.size(); ++i)
   {
      Point2<T> pt = poly[i];

      // First determine the side the current vertex is on.
      prevSide = side;
      side = internals::calcSideOfLine(l, pt);

      // If the vertices switched from one side of the line to the other, we need
      // to find the intersection point and add it to both output polygons. Since
      // we are building the output polygons as we traverse the vertices the
      // intersection point has to be added to the outout polygons before the
      // current vertex otherwise the vertices will be out of order.
      if (internals::wasLineCrossed(prevSide, side))
      {
         ct::LineSeg2<T> edge(poly[i - 1], pt);
         const auto x = ct::intersect(l, edge);
         if (x && std::holds_alternative<Point2<T>>(x))
         {
            leftPoly.add(*x);
            rightPoly.add(*x);
         }
      }

      // Now we can add the current vertex to the output polygons depending on
      // which side of the line it is on.
      if (side == Side.Left)
      {
         leftPoly.add(pt);
         haveStrictlyLeftPoints = true;
      }
      else if (side == Side.Right)
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
      Point2 pt = poly[0];
      prevSide = side;
      side = internals::calcSideOfLine(l, pt);

      if (internals::wasLineCrossed(prevSide, side))
      {
         const ct::LineSeg2<T> edge(poly[poly.size() - 1], pt);
         const auto x = ct::intersect(l, edge);
         if (x && std::holds_alternative<Point2<T>>(x))
         {
            leftPoly.add(*x);
            rightPoly.add(*x);
         }
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
