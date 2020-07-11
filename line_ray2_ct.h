//
// geomcpp
// 2D line rays using compile time/static polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_ct.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [c]ompile [t]ime polymorphism for line class hierarchy.
namespace ct
{
///////////////////

template <typename T> class LineRay2 : public Line2<T, LineRay2>
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

 public:
   LineRay2() = default;
   constexpr LineRay2(const Point2<T>& start, const Vec2<T>& direction);

   Line2Type type() const;
   std::optional<Point2<T>> startPoint() const;
   std::optional<Point2<T>> endPoint() const;
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const;
};


template <typename T>
constexpr LineRay2<T>::LineRay2(const Point2<T>& start, const Vec2<T>& direction)
: Line2<T, LineRay2>{start, direction}
{
}


template <typename T> Line2Type LineRay2<T>::type() const
{
   return Line2Type::Ray;
}


template <typename T> std::optional<Point2<T>> LineRay2<T>::startPoint() const
{
   return this->anchor();
}


template <typename T> std::optional<Point2<T>> LineRay2<T>::endPoint() const
{
   return std::nullopt;
}


template <typename T>
std::optional<typename LineRay2<T>::Fp>
LineRay2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto factor = this->lerpFactor(pt);
   if (factor && sutil::fpGreaterEqual<Fp>(*factor, 0))
      return factor;
   return std::nullopt;
}

} // namespace ct
} // namespace geom
