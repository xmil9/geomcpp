//
// geomcpp
// A 2D line using run time/dynamic polymorphism.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_types.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [r]un [t]ime polymorphism for line class hierarchy.
namespace rt
{
///////////////////

// A line extends infinitely into both directions.
template <typename T> class Line2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

   Line2() = default;
   constexpr Line2(const Point2<T>& anchor, const Vec2<T>& direction);

   virtual Line2Type type() const { return Line2Type::Infinite; }
   Point2<T> anchor() const noexcept { return m_anchor; }
   Vec2<T> direction() const noexcept { return m_dir; }

   bool isPoint() const;
   virtual std::optional<Point2<T>> startPoint() const { return std::nullopt; }
   virtual std::optional<Point2<T>> endPoint() const { return std::nullopt; }

   // Returns the interpolation factor of the point.
   virtual std::optional<Fp> isPointOnLine(const Point2<T>& pt) const;

   // Checks if a given point is on the infinite extension of the line.
   // Returns the interpolation factor of the point.
   template <typename U>
   std::optional<Fp> isPointOnInfiniteLine(const Point2<U>& pt) const;

   // Calculates the interpolation factor of a given point along the line.
   template <typename U> std::optional<Fp> calcLerpFactor(const Point2<U>& pt) const;

   // Interpolates a point at a given factor along the line.
   template <typename U> Point2<T> lerpPoint(U factor) const;

 private:
   // Point that anchors the line in the coordinate system. For line types that
   // have a start point it is guaranteed to be the start point.
   Point2<T> m_anchor;

   // Direction of line. Whether the length of the direction vector has meaning
   // is up to each derived class.
   Vec2<T> m_dir;
};


template <typename T>
constexpr Line2<T>::Line2(const Point2<T>& anchor, const Vec2<T>& direction)
: m_anchor{anchor}, m_dir{direction}
{
}


template <typename T> bool Line2<T>::isPoint() const
{
   return sutil::equal(direction().lengthSquared(), Fp(0));
}


template <typename T>
std::optional<typename Line2<T>::Fp> Line2<T>::isPointOnLine(const Point2<T>& pt) const
{
   return isPointOnInfiniteLine(pt);
}


template <typename T>
template <typename U>
std::optional<typename Line2<T>::Fp>
Line2<T>::isPointOnInfiniteLine(const Point2<U>& pt) const
{
   return calcLerpFactor(pt);
}


template <typename T>
template <typename U>
std::optional<typename Line2<T>::Fp> Line2<T>::calcLerpFactor(const Point2<U>& pt) const
{
   if (isPoint())
      return (pt == anchor()) ? std::make_optional(Fp(0)) : std::nullopt;

   const auto v = Vec2<T>{anchor(), pt};
   if (!v.isParallel(direction()))
      return std::nullopt;

   // length != 0 is assured by checking whether line is a point above.
   auto factor = v.length() / direction().length();
   if (!v.hasSameDirection(direction()))
      factor *= -1;

   return factor;
}


template <typename T> template <typename U> Point2<T> Line2<T>::lerpPoint(U factor) const
{
   const Vec2 v = direction() * factor;
   return anchor() + v;
}


///////////////////

template <typename T, typename U> bool parallel(const Line2<T>& a, const Line2<U>& b)
{
   return a.direction().isParallel(b.direction().normalize());
}


// Checks if two lines are on the same inifinite line.
template <typename T, typename U> bool coincident(const Line2<T>& a, const Line2<U>& b)
{
   return parallel(a, b) && a.isPointOnInfiniteLine(b.anchor());
}


// template <typename T, typename U>
// LineIntersection2D.Result intersect(const Line2<T>& a, const Line2<T>& b) const;

} // namespace rt
} // namespace geom
