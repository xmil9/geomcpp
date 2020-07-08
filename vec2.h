//
// geomcpp
// 2D vector.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coordsys.h"
#include "point2.h"
#include "essentutils/fputil.h"
#include <stdexcept>
#include <type_traits>


namespace geom
{
///////////////////

// Mathematical 2D vector.
template <typename T> class Vec2
{
 public:
   using value_type = T;

   Vec2() = default;
   constexpr Vec2(T x, T y);
   // To use the ctor for two points explicitly specify the vector's value type,
   // otherwise the coordinate ctor might be selected.
   template <typename U, typename V>
   constexpr Vec2(const Point2<U>& from, const Point2<V>& to);
   Vec2(const Vec2&) = default;
   Vec2(Vec2&&) = default;

   Vec2& operator=(const Vec2&) = default;
   Vec2& operator=(Vec2&&) = default;

   T x() const noexcept { return m_x; }
   T y() const noexcept { return m_y; }
   Vec2 operator-() const;
   sutil::FpType<T> lengthSquared() const;
   sutil::FpType<T> length() const;
   [[nodiscard]] Vec2 normalize() const;
   template <typename U>[[nodiscard]] Vec2 scale(U factor) const;

   template <typename U> bool isPerpendicular(const Vec2<U>& w) const;
   template <typename U> bool isOrthogonal(const Vec2<U>& w) const;
   template <typename U> bool hasSameDirection(const Vec2<U>& w) const;
   template <typename U> bool isParallel(const Vec2<U>& w) const;
   template <typename U> bool hasAcuteAngle(const Vec2<U>& w) const;
   template <typename U> bool hasObtuseAngle(const Vec2<U>& w) const;
   template <typename U>
   bool isCcw(const Vec2<U>& w, CoordSys cs = CoordSys::Screen) const;
   template <typename U>
   bool isCw(const Vec2<U>& w, CoordSys cs = CoordSys::Screen) const;

   Vec2 ccwNormal(CoordSys cs = CoordSys::Screen) const;
   Vec2 cwNormal(CoordSys cs = CoordSys::Screen) const;

 private:
   T m_x = T(0);
   T m_y = T(0);
};


// Forward declare dot product.
template <typename T, typename U>
sutil::FpType<T> dot(const Vec2<T>& a, const Vec2<U>& b);


template <typename T> constexpr Vec2<T>::Vec2(T x, T y) : m_x{x}, m_y{y}
{
}


template <typename T>
template <typename U, typename V>
constexpr Vec2<T>::Vec2(const Point2<U>& from, const Point2<V>& to)
: m_x{static_cast<T>(to.x() - from.x())}, m_y{static_cast<T>(to.y() - from.y())}
{
}


template <typename T> Vec2<T> Vec2<T>::operator-() const
{
   return Vec2(-x(), -y());
}


template <typename T> sutil::FpType<T> Vec2<T>::lengthSquared() const
{
   return dot(*this, *this);
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


template <typename T>
template <typename U>
bool Vec2<T>::isPerpendicular(const Vec2<U>& w) const
{
   return sutil::equal(dot(*this, w), sutil::FpType<T>(0));
}


// Orthogonal describes the same concept as perpendicular but can be applied to
// other geometric objects, too. For lines it is the same as perpendicular.
template <typename T>
template <typename U>
bool Vec2<T>::isOrthogonal(const Vec2<U>& w) const
{
   return isPerpendicular(w);
}


template <typename T>
template <typename U>
bool Vec2<T>::hasSameDirection(const Vec2<U>& w) const
{
   return isParallel(w) && hasAcuteAngle(w);
}


// Could be pointing in the same or opposite direction.
template <typename T>
template <typename U>
bool Vec2<T>::isParallel(const Vec2<U>& w) const
{
   return sutil::equal(perpDot(*this, w), sutil::FpType<T>(0));
}


// Checks if the angle between 'this' and a given vector is < 90.
template <typename T>
template <typename U>
bool Vec2<T>::hasAcuteAngle(const Vec2<U>& w) const
{
   return sutil::greater(dot(*this, w), sutil::FpType<T>(0));
}


// Checks if the angle between 'this' and a given vector is > 90.
template <typename T>
template <typename U>
bool Vec2<T>::hasObtuseAngle(const Vec2<U>& w) const
{
   return sutil::less(dot(*this, w), sutil::FpType<T>(0));
}


// Checks if a given vector is counter-clockwise of 'this' when facing into
// the direction of 'this'. This depends on the coordinate system used.
template <typename T>
template <typename U>
bool Vec2<T>::isCcw(const Vec2<U>& w, CoordSys cs) const
{
   if (cs == CoordSys::Screen)
      return sutil::less(perpDot(*this, w), sutil::FpType<T>(0));
   return sutil::greater(perpDot(*this, w), sutil::FpType<T>(0));
}


// Checks if a given vector is clockwise of 'this' when facing into the
// direction of 'this'. This depends on the coordinate system used.
template <typename T>
template <typename U>
bool Vec2<T>::isCw(const Vec2<U>& w, CoordSys cs) const
{
   if (cs == CoordSys::Screen)
      return sutil::greater(perpDot(*this, w), sutil::FpType<T>(0));
   return sutil::less(perpDot(*this, w), sutil::FpType<T>(0));
}


// Returns a vector that is a counter-clockwise perpendicular to 'this'. This
// depends on the coordinate system used.
template <typename T> Vec2<T> Vec2<T>::ccwNormal(CoordSys cs) const
{
   if (cs == CoordSys::Screen)
      return Vec2(y(), -x());
   return Vec2(-y(), x());
}


// Returns a vector that is a clockwise perpendicular to 'this'. This depends
// on the coordinate system used.
template <typename T> Vec2<T> Vec2<T>::cwNormal(CoordSys cs) const
{
   if (cs == CoordSys::Screen)
      return Vec2(-y(), x());
   return Vec2(y(), -x());
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Vec2<T>& a, const Vec2<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return sutil::equal<Common>(static_cast<Common>(a.x()), static_cast<Common>(b.x())) &&
          sutil::equal<Common>(static_cast<Common>(a.y()), static_cast<Common>(b.y()));
}


template <typename T, typename U> bool operator!=(const Vec2<T>& a, const Vec2<U>& b)
{
   return !(a == b);
}


///////////////////

// Calculates the dot product of two given vectors.
// Also called inner or scalar product.
// Meaning:
//   dot(v, w) - The length of the projection of v onto w.
// Properties:
//   dot(v, w) == 0 => v and w are perpendicular
//   dot(v, w) > 0  => angle between v and w is acute, i.e abs(angle) < 90
//   dot(v, w) < 0  => angle between v and w is obtuse, i.e abs(angle) > 90
// Source:
//   http://geomalgorithms.com/vector_products.html
template <typename T, typename U> sutil::FpType<T> dot(const Vec2<T>& a, const Vec2<U>& b)
{
   return static_cast<sutil::FpType<T>>(a.x() * b.x() + a.y() * b.y());
}


// Dot product as operator*.
template <typename T, typename U>
sutil::FpType<T> operator*(const Vec2<T>& a, const Vec2<U>& b)
{
   return dot(a, b);
}


// Calculates the perp dot product with a given vector.
// Also called external or outer product and in 3d space cross product.
// Named because it is the same as the dot product of the perpendicular vector
// to the first vector and the second vector: perpDot(v, w) = dot(perp(v), w)
// Meaning:
//   perpDot(v, w) - The signed length of the 3D cross product between v and w.
// Properties:
//   perpDot(v, w) == 0 => v and w have same or opposite directions
//   perpDot(v, w) > 0  =>
// 		cartesian CS: w is ccw of v when facing into direction of v
// 		screen CS   : w is cw of v when facing into direction of v
//   perpDot(v, w) < 0  =>
// 		cartesian CS: w is cw of v when facing into direction of v
// 		screen CS   : w is ccw of v when facing into direction of v
// Other usage:
//   Gives the (signed) area of the 2D parallelogram spanned by 'this' and the
//   given vector.
// Source:
//   http://geomalgorithms.com/vector_products.html
template <typename T, typename U>
sutil::FpType<T> perpDot(const Vec2<T>& a, const Vec2<U>& b)
{
   return static_cast<sutil::FpType<T>>(a.x() * b.y() - a.y() * b.x());
}


// Vector subtraction.
template <typename T, typename U>
Vec2<std::common_type_t<T, U>> operator-(const Vec2<T>& a, const Vec2<U>& b)
{
   using R = std::common_type_t<T, U>;
   return Vec2<R>(static_cast<R>(a.x()) - static_cast<R>(b.x()),
                  static_cast<R>(a.y()) - static_cast<R>(b.y()));
}


// Vector addition.
template <typename T, typename U>
Vec2<std::common_type_t<T, U>> operator+(const Vec2<T>& a, const Vec2<U>& b)
{
   using R = std::common_type_t<T, U>;
   return Vec2<R>(static_cast<R>(a.x()) + static_cast<R>(b.x()),
                  static_cast<R>(a.y()) + static_cast<R>(b.y()));
}


// Scale vector by scalar.
template <typename T, typename S> Vec2<T> operator*(const Vec2<T>& v, S scalar)
{
   return v.scale(scalar);
}


// Scale vector by scalar (reverse operands).
template <typename T, typename S> Vec2<T> operator*(S scalar, const Vec2<T>& v)
{
   return v.scale(scalar);
}


// Divide vector by scalar.
template <typename T, typename S> Vec2<T> operator/(const Vec2<T>& v, S scalar)
{
   if (scalar == S(0))
      throw std::runtime_error("Division by zero.");
   using FP = sutil::FpType<T>;
   return v.scale(FP(1.0) / scalar);
}


///////////////////

// Offset point by vector.
template <typename T, typename U>
Point2<T> operator+(const Point2<T>& pt, const Vec2<U>& v)
{
   return pt.offset(v.x(), v.y());
}


// Offset point by vector (reverse operands).
template <typename T, typename U>
Point2<T> operator+(const Vec2<U>& v, const Point2<T>& pt)
{
   return pt.offset(v.x(), v.y());
}


// Subtract points yielding a vector.
template <typename T, typename U>
Vec2<std::common_type_t<T, U>> operator-(const Point2<T>& p, const Point2<U>& q)
{
   return Vec2<std::common_type_t<T, U>>(q, p);
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
