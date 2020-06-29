//
// geomcpp
// 2D line ray using run time/dynamic polymorphism.
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

// A line ray has a start and exends to infinity into a given direction.
template <typename T> class LineRay2 : public Line2<T>
{
 public:
   using value_type = typename Line2<T>::value_type;
   using Fp = typename Line2<T>::Fp;
   using ParametricValue = typename Line2<T>::ParametricValue;

   LineRay2() = default;
   LineRay2(const Point2<T>& start, const Vec2<T>& direction);

   bool hasStartPoint() const override { return true; }
   std::optional<Point2<T>> startPoint() const override { return anchor(); }
   bool hasEndPoint() const override { return false; }
   std::optional<Point2<T>> endPoint() const override { return std::nullopt; }
   std::optional<ParametricValue> isPointOnLine(const Point2<T>& pt) const override;
};


template <typename T>
LineRay2<T>::LineRay2(const Point2<T>& start, const Vec2<T>& direction)
: Line2(start, direction)
{
}


template <typename T>
std::optional<typename Line2<T>::ParametricValue>
LineRay2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto paramVal = calcParametricValue(pt);
   if (paramVal && sutil::fpGreaterEqual(*paramVal, 0))
      return paramVal;
   return std::nullopt;
}

} // namespace rt
} // namespace geom
