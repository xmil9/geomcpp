//
// geomcpp
// 2D vector.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "essentutils/fputil.h"
#include <type_traits>


namespace geom
{

///////////////////

template <typename T> class Vec2
{
 public:
   using value_type = T;

   Vec2() = default;
   constexpr Vec2(T x, T y);
   template <typename U> constexpr Vec2(const Point2<U>& from, const Point2<U>& to);
   Vec2(const Vec2&) = default;
   Vec2(Vec2&&) = default;

   Vec2& operator=(const Vec2&) = default;
   Vec2& operator=(Vec2&&) = default;

   T x() const noexcept { return m_x; }
   T y() const noexcept { return m_y; }
   sutil::FpType<T> lengthSquared() const;
   sutil::FpType<T> length() const;
   [[nodiscard]] Vec2 normalize() const;
   template <typename U>[[nodiscard]] Vec2 scale(U factor) const;

   template <typename U> sutil::FpType<T> dot(const Vec2<U>& w) const;
   template <typename U> sutil::FpType<T> perpDot(const Vec2<U>& w) const;

 private:
   T m_x = T(0);
   T m_y = T(0);
};


template <typename T> constexpr Vec2<T>::Vec2(T x, T y) : m_x{x}, m_y{y}
{
}


template <typename T>
template <typename U>
constexpr Vec2<T>::Vec2(const Point2<U>& from, const Point2<U>& to)
: m_x{static_cast<T>(to.x() - from.x())}, m_y{static_cast<T>(to.y() - from.y())}
{
}


template <typename T> sutil::FpType<T> Vec2<T>::lengthSquared() const
{
   return dot(*this);
}


template <typename T> sutil::FpType<T> Vec2<T>::length() const
{
   return sutil::sqrt<sutil::FpType<T>>(lengthSquared());
}


template <typename T> Vec2<T> Vec2<T>::normalize() const
{
   const auto len = length();
   if (sutil::equal(len, sutil::FpType<T>(0)))
      return *this;
   return scale(sutil::FpType<T>(1) / len);
}


template <typename T> template <typename U> Vec2<T> Vec2<T>::scale(U factor) const
{
   return Vec2(static_cast<T>(x() * factor), static_cast<T>(y() * factor));
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
sutil::FpType<T> Vec2<T>::dot(const Vec2<U>& w) const
{
   return x() * w.x() + y() * w.y();
}


// Calculates the perp dot product with a given vector.
// Also called external or outer product.
// Named because it is the same as the dot product of the perpendicular vector
// to the first vector and the second vector: perpDot(v, w) = dot(perp(v), w)
// Meaning:
//   v.perpDot(w) - The signed length of the 3D cross product between v and w.
// Properties:
//   v.perpDot(w) == 0 => v and w have same or opposite directions
//   v.perpDot(w) > 0  =>
// 		cartesian CS: w is ccw of v when facing into direction of v
// 		screen CS   : w is cw of v when facing into direction of v
//   v.perpDot(w) < 0  =>
// 		cartesian CS: w is cw of v when facing into direction of v
// 		screen CS   : w is ccw of v when facing into direction of v
// Other usage:
//   Gives the (signed) area of the 2D parallelogram spanned by 'this' and the
//   given vector.
// Source:
//   http://geomalgorithms.com/vector_products.html
template <typename T>
template <typename U>
sutil::FpType<T> Vec2<T>::perpDot(const Vec2<U>& w) const
{
   return x() * w.y() - y() * w.x();
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


///////////////////

template <typename T, typename U>
sutil::FpType<T> operator*(const Vec2<T>& a, const Vec2<U>& b)
{
   return a.dot(b);
}


template <typename T, typename U>
sutil::FpType<T> perpDot(const Vec2<T>& a, const Vec2<U>& b)
{
   return a.perpDot(b);
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
