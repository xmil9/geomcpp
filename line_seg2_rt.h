//
// geomcpp
// 2D line segment using run time/dynamic polymorphism.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_rt.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [r]un [t]ime polymorphism for line class hierarchy.
namespace rt
{
///////////////////

// A line segment has both a start and and end point.
template <typename T> class LineSeg2 : public Line2<T>
{
 public:
   using value_type = typename Line2<T>::value_type;
   using Fp = typename Line2<T>::Fp;
   using ParametricValue = typename Line2<T>::ParametricValue;

   LineSeg2() = default;
   LineSeg2(const Point2<T>& start, const Point2<T>& end);
   LineSeg2(const Point2<T>& start, const Vec2<T>& direction);

   bool hasStartPoint() const override { return true; }
   std::optional<Point2<T>> startPoint() const override { return anchor(); }
   bool hasEndPoint() const override { return true; }
   std::optional<Point2<T>> endPoint() const override;
   std::optional<ParametricValue> isPointOnLine(const Point2<T>& pt) const override;

   Point2<T> midPoint() const;
   Fp lengthSquared() const;
   Fp length() const;
};


template <typename T>
LineSeg2<T>::LineSeg2(const Point2<T>& start, const Point2<T>& end)
: Line2(start, Vec2(start, end))
{
}


template <typename T>
LineSeg2<T>::LineSeg2(const Point2<T>& start, const Vec2<T>& direction)
: Line2(start, direction)
{
}


template <typename T> std::optional<Point2<T>> LineSeg2<T>::endPoint() const
{
   return anchor() + direction();
}


template <typename T>
std::optional<typename LineSeg2<T>::ParametricValue>
LineSeg2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto paramVal = calcParametricValue(pt);
   if (paramVal && sutil::fpGreaterEqual(*paramVal, 0) && sutil::fpLessEqual(paramVal, 1))
      return paramVal;
   return std::nullopt;
}


template <typename T> Point2<T> LineSeg2<T>::midPoint() const
{
   return startPoint().offset(direction() * typename Line2<T>::Fp(0.5));
}


template <typename T> typename LineSeg2<T>::Fp LineSeg2<T>::lengthSquared() const
{
   return direction().lengthSquared();
}


template <typename T> typename LineSeg2<T>::Fp LineSeg2<T>::length() const
{
   return direction().length();
}

} // namespace rt
} // namespace geom
