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

// 2D point.
template <typename T> class Point2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

   Point2() = default;
   constexpr Point2(T x, T y);
   Point2(const Point2&) = default;
   Point2(Point2&&) = default;

   Point2& operator=(const Point2&) = default;
   Point2& operator=(Point2&&) = default;

   constexpr T x() const noexcept { return m_x; }
   constexpr T y() const noexcept { return m_y; }
   Point2 operator-() const;
   template <typename U>[[nodiscard]] Point2 offset(U dx, U dy) const;
   template <typename U>[[nodiscard]] Point2 scale(U factor) const;

 private:
   T m_x = T(0);
   T m_y = T(0);
};


template <typename T> constexpr Point2<T>::Point2(T x, T y) : m_x{x}, m_y{y}
{
}


template <typename T> Point2<T> Point2<T>::operator-() const
{
   return Point2(-x(), -y());
}


template <typename T> template <typename U> Point2<T> Point2<T>::offset(U dx, U dy) const
{
   return Point2(static_cast<T>(x() + dx), static_cast<T>(y() + dy));
}


template <typename T> template <typename U> Point2<T> Point2<T>::scale(U factor) const
{
   return Point2(static_cast<T>(x() * factor), static_cast<T>(y() * factor));
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Point2<T>& a, const Point2<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return sutil::equal<Common>(static_cast<Common>(a.x()), static_cast<Common>(b.x())) &&
          sutil::equal<Common>(static_cast<Common>(a.y()), static_cast<Common>(b.y()));
}


template <typename T, typename U> bool operator!=(const Point2<T>& a, const Point2<U>& b)
{
   return !(a == b);
}


// Functor for less-than comparison of points. Ordering is meaningless. The operator
// is only used to allow points to be stored in a set.
template <typename T>
struct pointLess
{
   bool operator()(const Point2<T>& a, const Point2<T>& b) const
   {
      return (a.x() < b.x() || (sutil::equal(a.x(), b.x()) && a.y() < b.y()));
   }
};


///////////////////

// Distance between points.

template <typename T, typename U>
inline std::common_type_t<T, U> distSquared(const Point2<T>& a, const Point2<U>& b)
{
   using R = std::common_type_t<T, U>;
   const R dx = static_cast<R>(b.x()) - static_cast<R>(a.x());
   const R dy = static_cast<R>(b.y()) - static_cast<R>(a.y());
   return dx * dx + dy * dy;
}


template <typename T, typename U>
std::common_type_t<sutil::FpType<T>, sutil::FpType<U>> dist(const Point2<T>& a,
                                                            const Point2<U>& b)
{
   using R = std::common_type_t<sutil::FpType<T>, sutil::FpType<U>>;
   return sutil::sqrt<sutil::FpType<R>>(distSquared(a, b));
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
