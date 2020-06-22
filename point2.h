//
// geomcpp
// 2D point.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "essentutils/fputil.h"
#include <functional>
#include <type_traits>


namespace geom
{

///////////////////

template <typename T> class Point2
{
 public:
   using value_type = T;

   Point2() = default;
   template <typename U> constexpr Point2(U x, U y);
   Point2(const Point2&) = default;
   Point2(Point2&&) = default;

   Point2& operator=(const Point2&) = default;
   Point2& operator=(Point2&&) = default;

   T x() const noexcept { return m_x; }
   T y() const noexcept { return m_y; }
   template <typename U> Point2<T> offset(U x, U y) const;
   template <typename U> Point2<T> scale(U factor) const;

 private:
   T m_x = T(0);
   T m_y = T(0);
};


template <typename T>
template <typename U>
constexpr Point2<T>::Point2(U x, U y) : m_x(x), m_y(y)
{
}


template <typename T> template <typename U> Point2<T> Point2<T>::offset(U x, U y) const
{
   return Point2(x() + x, y() + y);
}


template <typename T> template <typename U> Point2<T> Point2<T>::scale(U factor) const
{
   return Point2(x() * factor, y() * factor);
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Point2<T>& a, const Point2<U>& b)
{
   return a.x() == b.x() && a.y() == b.y();
}


template <typename T, typename U> bool operator!=(const Point2<T>& a, const Point2<U>& b)
{
   return !(a == b);
}


///////////////////

// Distance between points.

template <typename T> inline T distSquared(const Point2<T>& a, const Point2<T>& b)
{
   const T dx = b.x() - a.x();
   const T dy = b.y() - a.y();
   return dx * dx + dy * dy;
}


template <typename T>
sutil::FpType<T> dist(const Point2<T>& a, const Point2<T>& b)
{
   return sutil::sqrt<sutil::FpType<T>>(distSquared(a, b));
}

} // namespace geom


///////////////////

// Hashing.
// Inject hash implementation for Point2 into std namespace.
namespace std
{
template <typename T> struct hash<geom::Point2<T>>
{
   std::size_t operator()(const geom::Point2<T>& pt) const noexcept
   {
      const std::size_t h1 = std::hash<T>{}(pt.x());
      const std::size_t h2 = std::hash<T>{}(pt.y());
      return h1 ^ (h2 << 1);
   }
};
} // namespace std
