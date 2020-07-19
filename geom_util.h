//
// geomcpp
// Geometry utilities.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "geom_types.h"
#include "rect.h"
#include "vec2.h"
#include <iterator>
#include <optional>


namespace geom
{
///////////////////

// Calculates a minimal rectangle around given points.
template <typename T, typename PointIter>
std::optional<Rect<T>> calcPathBounds(PointIter first, PointIter last)
{
   // No points.
   if (first == last)
      return std::nullopt;

   // One point.
   const Point2<T>& prev = *first;
   if (++first == last)
      return std::make_optional(Rect<T>(prev, prev));

   Rect<T> bounds{prev, *first};
   while (++first != last)
   {
      if (first->x() < bounds.left())
         bounds.setLeft(first->x());
      if (first->y() < bounds.top())
         bounds.setTop(first->y());
      if (first->x() > bounds.right())
         bounds.setRight(first->x());
      if (first->y() > bounds.bottom())
         bounds.setBottom(first->y());
   }

   return std::make_optional(bounds);
}


// Checks if given points form a convex path.
// Convex path - All edges bend in the same direction and don't cross.
template <typename PointIter> bool isConvexPath(PointIter first, PointIter last)
{
   using Coord = typename std::iterator_traits<PointIter>::value_type::value_type;
   using Fp = sutil::FpType<Coord>;

   const std::size_t numPts = std::distance(first, last);
   if (numPts <= 3)
      return true;

   std::optional<Vec2<Coord>> prev;
   std::optional<Vec2<Coord>> next;
   Orientation orient = Orientation::None;

   for (PointIter it = first; it != last; ++it)
   {
      PointIter nextIt = (it + 1 == last) ? first : it + 1;
      prev = next;
      next = std::make_optional(Vec2<Coord>(*it, *nextIt));
      if (!prev)
         continue;

      auto val = perpDot(*prev, *next);
      if (sutil::equal<Fp>(val, 0))
         continue;

      if (orient == Orientation::None)
         orient = sutil::greater<Fp>(val, 0) ? Orientation::Cw : Orientation::Ccw;

      // Orientation cannot change for convex paths.
      Orientation edgeOrient =
         sutil::greater<Fp>(val, 0) ? Orientation::Cw : Orientation::Ccw;
      if (edgeOrient != orient)
         return false;
   }

   return true;
}

} // namespace geom
