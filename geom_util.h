//
// geomcpp
// Geometry utilities.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "rect.h"
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

} // namespace geom
