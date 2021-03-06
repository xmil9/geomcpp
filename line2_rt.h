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

   virtual Line2Type type() const { return Line2Type::Infinite; }
   Point2<T> anchor() const noexcept { return m_anchor; }
   Vec2<T> direction() const noexcept { return m_dir; }

   bool isPoint() const;
   virtual std::optional<Point2<T>> startPoint() const = 0;
   virtual std::optional<Point2<T>> endPoint() const = 0;

   // Returns the interpolation factor of the point.
   virtual std::optional<Fp> isPointOnLine(const Point2<T>& pt) const = 0;

   // Checks if a given point is on the infinite extension of the line.
   // Returns the interpolation factor of the point.
   template <typename U>
   std::optional<Fp> isPointOnInfiniteLine(const Point2<U>& pt) const;

   // Calculates the interpolation factor of a given point along the line.
   template <typename U> std::optional<Fp> lerpFactor(const Point2<U>& pt) const;

   // Interpolates a point at a given factor along the line.
   template <typename U> Point2<T> lerp(U factor) const;

 protected:
   Line2() = default;
   constexpr Line2(const Point2<T>& anchor, const Vec2<T>& direction);

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
   return lerpFactor(pt);
}


template <typename T>
template <typename U>
std::optional<typename Line2<T>::Fp> Line2<T>::lerpFactor(const Point2<U>& pt) const
{
   if (isPoint())
      return (pt == anchor()) ? std::make_optional(Fp(0)) : std::nullopt;

   const auto v = Vec2<T>{anchor(), pt};
   if (!parallel(v, direction()))
      return std::nullopt;

   // length != 0 is assured by checking whether line is a point above.
   auto factor = v.length() / direction().length();
   if (!sameDirection(v, direction()))
      factor *= -1;

   return factor;
}


template <typename T> template <typename U> Point2<T> Line2<T>::lerp(U factor) const
{
   const Vec2 v = direction() * factor;
   return anchor() + v;
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Line2<T>& a, const Line2<U>& b)
{
   return a.type() == b.type() && a.anchor() == b.anchor() &&
          a.direction() == b.direction();
}


template <typename T, typename U> bool operator!=(const Line2<T>& a, const Line2<U>& b)
{
   return !(a == b);
}


template <typename T, typename U> bool parallel(const Line2<T>& a, const Line2<U>& b)
{
   return parallel(a.direction(), b.direction().normalize());
}


// Checks if two lines are on the same inifinite line.
template <typename T, typename U> bool coincident(const Line2<T>& a, const Line2<U>& b)
{
   return parallel(a, b) && a.isPointOnInfiniteLine(b.anchor());
}

} // namespace rt
} // namespace geom
