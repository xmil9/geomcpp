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
   constexpr Rect<T> bounds() const;
   template <typename U>[[nodiscard]] Ring offset(const Vec2<U>& v) const;

   template <typename U, typename V>
   friend bool operator==(const Ring<U>& a, const Ring<V>& b);
   template <typename U, typename V>
   friend bool isPointInRing(const Ring<U>& r, const Point2<V>& pt);

 private:
   Circle<T> m_inner;
   Circle<T> m_outer;
};


template <typename T>
constexpr Ring<T>::Ring(const Point2<T>& center, Fp innerRadius, Fp outerRadius)
: m_inner{center, static_cast<T>(innerRadius)}, m_outer{center,
                                                        static_cast<T>(outerRadius)}
{
   if (innerRadius > outerRadius)
      std::swap(m_inner, m_outer);
}


template <typename T> constexpr Rect<T> Ring<T>::bounds() const
{
   return Rect<T>(
      m_inner.center().x() - m_outer.radius(), m_inner.center().y() - m_outer.radius(),
      m_inner.center().x() + m_outer.radius(), m_inner.center().y() + m_outer.radius());
}


template <typename T>
template <typename U>
Ring<T> Ring<T>::offset(const Vec2<U>& v) const
{
   return Ring<T>(m_inner.center() + v, m_inner.radius(), m_outer.radius());
}


///////////////////

// Comparisions.

template <typename U, typename V> bool operator==(const Ring<U>& a, const Ring<V>& b)
{
   return a.m_inner == b.m_inner && a.m_outer == b.m_outer;
}


template <typename U, typename V> bool operator!=(const Ring<U>& a, const Ring<V>& b)
{
   return !(a == b);
}


///////////////////

// Checks if a given point is in the ring (between or on the circles).
template <typename U, typename V>
bool isPointInRing(const Ring<U>& r, const Point2<V>& pt)
{
   return isPointInCircle(r.m_outer, pt) && !isPointInsideCircle(r.m_inner, pt);
}

} // namespace geom
