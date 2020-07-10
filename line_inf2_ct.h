//
// geomcpp
// 2D infinite lines using compile time/static polymorphism.
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

template <typename T> class LineInf2 : public Line2<T, LineInf2>
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

 public:
   LineInf2() = default;
   constexpr LineInf2(const Point2<T>& anchor, const Vec2<T>& direction);

   Line2Type type() const;
   std::optional<Point2<T>> startPoint() const;
   std::optional<Point2<T>> endPoint() const;
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const;
};


template <typename T>
constexpr LineInf2<T>::LineInf2(const Point2<T>& anchor, const Vec2<T>& direction)
: Line2<T, LineInf2<T>>{anchor, direction}
{
}


template <typename T> Line2Type LineInf2<T>::type() const
{
   return Line2Type::Infinite;
}


template <typename T> std::optional<Point2<T>> LineInf2<T>::startPoint() const
{
   return std::nullopt;
}


template <typename T> std::optional<Point2<T>> LineInf2<T>::endPoint() const
{
   return std::nullopt;
}


template <typename T>
std::optional<typename LineInf2<T>::Fp>
LineInf2<T>::isPointOnLine(const Point2<T>& pt) const
{
   return isPointOnInfiniteLine(pt);
}

} // namespace ct
} // namespace geom
