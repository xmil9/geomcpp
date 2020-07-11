//
// geomcpp
// 2D infinitle lines using run time/dynamic polymorphism.
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

template <typename T> class LineInf2 : public Line2<T>
{
 public:
   using value_type = typename Line2<T>::value_type;
   using Fp = typename Line2<T>::Fp;

   LineInf2() = default;
   constexpr LineInf2(const Point2<T>& anchor, const Vec2<T>& direction);

   Line2Type type() const override { return Line2Type::Infinite; }
   std::optional<Point2<T>> startPoint() const override { return std::nullopt; }
   std::optional<Point2<T>> endPoint() const override { return std::nullopt; }
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const override;
};


template <typename T>
constexpr LineInf2<T>::LineInf2(const Point2<T>& anchor, const Vec2<T>& direction)
: Line2<T>(anchor, direction)
{
}


template <typename T>
std::optional<typename LineInf2<T>::Fp>
LineInf2<T>::isPointOnLine(const Point2<T>& pt) const
{
   return this->isPointOnInfiniteLine(pt);
}

} // namespace rt
} // namespace geom
