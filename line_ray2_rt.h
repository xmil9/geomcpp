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

   LineRay2() = default;
   LineRay2(const Point2<T>& start, const Vec2<T>& direction);

   Line2Type type() const override { return Line2Type::Ray; }
   std::optional<Point2<T>> startPoint() const override { return this->anchor(); }
   std::optional<Point2<T>> endPoint() const override { return std::nullopt; }
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const override;
};


template <typename T>
LineRay2<T>::LineRay2(const Point2<T>& start, const Vec2<T>& direction)
: Line2<T>(start, direction)
{
}


template <typename T>
std::optional<typename LineRay2<T>::Fp>
LineRay2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto factor = this->calcLerpFactor(pt);
   if (factor && sutil::fpGreaterEqual<Fp>(*factor, 0))
      return factor;
   return std::nullopt;
}

} // namespace rt
} // namespace geom
