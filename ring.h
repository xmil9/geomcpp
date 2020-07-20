//
// geomcpp
// Rings
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "circle.h"
#include "rect.h"
#include "vec2.h"
#include "essentutils/fputil.h"


namespace geom
{
///////////////////

template <typename T> class Ring
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

   Ring() = default;
   constexpr Ring(const Point2<T>& center, Fp innerRadius, Fp outerRadius);
   Ring(const Ring&) = default;
   Ring(Ring&&) = default;

   Ring& operator=(const Ring&) = default;
   Ring& operator=(Ring&&) = default;

   constexpr Point2<T> center() const noexcept { return m_inner.center(); }
   constexpr Fp innerRadius() const noexcept { return m_inner.radius(); }
   constexpr Fp outerRadius() const noexcept { return m_outer.radius(); }
   constexpr Rect<T> bounds const;
   template <typename U>[[nodiscard]] Ring offset(const Vec2<U>& v) const;

   friend template <typename U, typename V>
   bool operator==(const Ring<U>& a, const Ring<V>& b);

 private:
   Circle<T> m_inner;
   Circle<T> m_outer;
};


///////////////////

// Comparisions.

template <typename U, typename V> bool operator==(const Ring<U>& a, const Ring<V>& b)
{
   return a.m_inner == b.m_inner && a.outer == b.outer;
}


template <typename U, typename V> bool operator!=(const Ring<U>& a, const Ring<V>& b)
{
   return !(a == b);
}


///////////////////

// Checks if a given point is in the ring (between or on the circles).
template <typename T, typename U>
bool isPointInRing(const Ring<T>& r, const Point2<U>& pt)
{
}

} // namespace geom
