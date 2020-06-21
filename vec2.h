//
// geomcpp
// 2D vector.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include <type_traits>


namespace geom
{

///////////////////

// Select a floating point type based on a given type.
// Default to double.
template <typename T>
using FpType = std::conditional_t<
   std::is_same_v<T, float>, float,
   std::conditional_t<std::is_same_v<T, long double>, long double, double>>;


///////////////////

template <typename T> class Vec2
{
 public:
   using value_type = T;

   Vec2() = default;
   template <typename U> constexpr Vec2(U x, U y);
   template <typename U> constexpr Vec2(const Point2<U>& from, const Point2<U>& to);
   Vec2(const Vec2&) = default;
   Vec2(Vec2&&) = default;

   Vec2& operator=(const Vec2&) = default;
   Vec2& operator=(Vec2&&) = default;

   T x() const noexcept { return m_x; }
   T y() const noexcept { return m_y; }
   FpType<T> lengthSquared() const;
   FpType<T> length() const;
   template <typename U> FpType<T> dot(const Vec2<U>& w) const;

 private:
   T m_x = T(0);
   T m_y = T(0);
};


template <typename T>
template <typename U>
constexpr Vec2<T>::Vec2(U x, U y) : m_x(x), m_y(y)
{
}


template <typename T>
template <typename U>
constexpr Vec2<T>::Vec2(const Point2<U>& from, const Point2<U>& to)
: m_x(to.x() - from.x()), m_y(to.y() - from.y())
{
}


template <typename T> FpType<T> Vec2<T>::lengthSquared() const
{
   return dot(*this);
}


template <typename T> FpType<T> Vec2<T>::length() const
{
   if constexpr (std::is_same_v<FpType<T>, float>)
      return std::sqrt(lengthSquared());
   else if constexpr (std::is_same_v<FpType<T>, long double>)
      return std::sqrtl(lengthSquared());
   else
      return std::sqrt(lengthSquared());
}


// Calculates the dot product with a given vector.
// Also called inner or scalar product.
// Meaning:
//   v.dot(w) - The length of the projection of v onto w.
// Properties:
//   v.dot(w) == 0 => v and w are perpendicular
//   v.dot(w) > 0  => angle between v and w is acute, i.e abs(angle) < 90
//   v.dot(w) < 0  => angle between v and w is obtuse, i.e abs(angle) > 90
// Source:
//   http://geomalgorithms.com/vector_products.html
template <typename T>
template <typename U>
FpType<T> Vec2<T>::dot(const Vec2<U>& w) const
{
   return x() * w.x() + y() * w.y();
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Vec2<T>& a, const Vec2<U>& b)
{
   return a.x() == b.x() && a.y() == b.y();
}


template <typename T, typename U> bool operator!=(const Vec2<T>& a, const Vec2<U>& b)
{
   return !(a == b);
}

} // namespace geom


///////////////////

// Hashing.
// Inject hash implementation for Vec2 into std namespace.
namespace std
{
template <typename T> struct hash<geom::Vec2<T>>
{
   std::size_t operator()(const geom::Vec2<T>& pt) const noexcept
   {
      const std::size_t h1 = std::hash<T>{}(pt.x());
      const std::size_t h2 = std::hash<T>{}(pt.y());
      return h1 ^ (h2 << 3);
   }
};
} // namespace std
