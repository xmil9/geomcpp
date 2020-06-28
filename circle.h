//
// geomcpp
// Circle.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "rect.h"
#include "vec2.h"
#include "essentutils/fputil.h"
#include <cmath>


namespace geom
{
///////////////////

template <typename T> class Circle
{
 public:
   using value_type = T;

   Circle() = default;
   template <typename U> constexpr Circle(const Point2<T>& center, U radius);
   Circle(const Circle&) = default;
   Circle(Circle&&) = default;

   Circle& operator=(const Circle&) = default;
   Circle& operator=(Circle&&) = default;

   Point2<T> center() const noexcept { return m_center; }
   T radius() const noexcept { return m_radius; }
   bool isPoint() const;
   Rect<T> bounds() const;
   template <typename U>[[nodiscard]] Circle offset(const Vec2<U>& v) const;
   template <typename U> bool isPointInCircle(const Point2<U>& pt) const;
   template <typename U> bool isPointOnCircle(const Point2<U>& pt) const;
   template <typename U> bool isPointInsideCircle(const Point2<U>& pt) const;
   template <typename U> Point2<T> pointAtAngle(U angleInRadians) const;

 private:
   Point2<T> m_center;
   T m_radius = T(0);
};


template <typename T>
template <typename U>
constexpr Circle<T>::Circle(const Point2<T>& center, U radius)
: m_center{center}, m_radius{static_cast<T>(radius)}
{
}


template <typename T> bool Circle<T>::isPoint() const
{
   return sutil::equal(radius(), T(0));
}


template <typename T> Rect<T> Circle<T>::bounds() const
{
   return Rect{m_center.x - radius(), m_center.y - radius(), m_center.x + radius(),
               m_center.y + radius()};
}


template <typename T>
template <typename U>
Circle<T> Circle<T>::offset(const Vec2<U>& v) const
{
   return Circle{center() + v, radius()};
}


// Checks if a given point is in the circle (inside or on the circle).
template <typename T>
template <typename U>
bool Circle<T>::isPointInCircle(const Point2<U>& pt) const
{
   return sutil::lessEqual(distSquared(pt, center()), radius() * radius());
}


// Checks if a given point is on the circle.
template <typename T>
template <typename U>
bool Circle<T>::isPointOnCircle(const Point2<U>& pt) const
{
   return sutil::equal(distSquared(pt, center()), radius() * radius());
}


// Checks if a given point is strictly inside the circle (not on the circle).
template <typename T>
template <typename U>
bool Circle<T>::isPointInsideCircle(const Point2<U>& pt) const
{
   return sutil::less(distSquared(pt, center()), radius() * radius());
}


template <typename T>
template <typename U>
Point2<T> Circle<T>::pointAtAngle(U angleInRadians) const
{
   using FP = sutil::FpType<T>;
   const FP angle = static_cast<FP>(angleInRadians);
   const FP r = static_cast<FP>(radius());
   const FP x = m_center.x() + r * std::cos(angle);
   const FP y = m_center.y() + r * std::sin(angle);
   return Point2<T>(x, y);
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Circle<T>& a, const Circle<U>& b)
{
   return a.center() == b.center() && sutil::equal(a.radius(), b.radius());
}


template <typename T, typename U> bool operator!=(const Circle<T>& a, const Circle<U>& b)
{
   return !(a == b);
}

} // namespace geom
