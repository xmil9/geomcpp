//
// geomcpp
// Base class for 2D lines using compile time/static polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_types.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [c]ompile [t]ime polymorphism for line class hierarchy.
namespace ct
{
///////////////////

template <typename LineConcrete> class Line2Base
{
 public:
   using value_type = typename LineConcrete::value_type;
   using Fp = typename LineConcrete::Fp;

 public:
   Line2Base() = default;
   constexpr Line2Base(const Point2<value_type>& anchor,
                       const Vec2<value_type>& direction);

   Line2Type type() const;
   Point2<value_type> anchor() const noexcept { return m_anchor; }
   Vec2<value_type> direction() const noexcept { return m_dir; }

   bool isPoint() const;
   std::optional<Point2<value_type>> startPoint() const;
   std::optional<Point2<value_type>> endPoint() const;

   // Returns the interpolation factor of the point.
   std::optional<Fp> isPointOnLine(const Point2<value_type>& pt) const;

   // Checks if a given point is on the infinite extension of the line.
   // Returns the interpolation factor of the point.
   template <typename U>
   std::optional<Fp> isPointOnInfiniteLine(const Point2<U>& pt) const;

   // Calculates the interpolation factor of a given point along the line.
   template <typename U> std::optional<Fp> lerpFactor(const Point2<U>& pt) const;

   // Interpolates a point at a given factor along the line.
   template <typename U> Point2<value_type> lerp(U factor) const;

 private:
   const LineConcrete& ConcreteLine() const;

 private:
   // Point that anchors the line in the coordinate system. For line types that
   // have a start point it is guaranteed to be the start point.
   Point<value_type> m_anchor;

   // Direction of line. Whether the length of the direction vector has meaning
   // is up to each derived class.
   Vec2<value_type> m_dir;
};


template <typename LineConcrete>
constexpr Line2Base<LineConcrete>::Line2Base(const Point2<value_type>& anchor,
                                             const Vec2<value_type>& direction)
: m_anchor{anchor}, m_dir{direction}
{
}


template <typename LineConcrete> Line2Type Line2Base<LineConcrete>::type() const
{
   return ConcreteLine().type();
}


template <typename LineConcrete> bool Line2Base<LineConcrete>::isPoint() const
{
   return sutil::equal(direction().lengthSquared(), Fp(0));
}


template <typename LineConcrete>
std::optional<Point2<typename LineConcrete::value_type>>
Line2Base<LineConcrete>::startPoint() const
{
   return ConcreteLine().startPoint();
}


template <typename LineConcrete>
std::optional<Point2<typename LineConcrete::value_type>>
Line2Base<LineConcrete>::endPoint() const
{
   return ConcreteLine().endPoint();
}


template <typename LineConcrete>
std::optional<typename LineConcrete::Fp>
Line2Base<LineConcrete>::isPointOnLine(const Point2<value_type>& pt) const
{
   return ConcreteLine().isPointOnLine(pt);
}


template <typename LineConcrete>
template <typename U>
std::optional<typename LineConcrete::Fp>
Line2Base<LineConcrete>::isPointOnInfiniteLine(const Point2<U>& pt) const
{
   return lerpFactor(pt);
}


template <typename LineConcrete>
template <typename U>
std::optional<typename LineConcrete::Fp>
Line2Base<LineConcrete>::lerpFactor(const Point2<U>& pt) const
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


template <typename LineConcrete>
template <typename U>
Point2<typename LineConcrete::value_type> Line2Base<LineConcrete>::lerp(U factor) const
{
   const Vec2 v = direction() * factor;
   return anchor() + v;
}


template <typename LineConcrete>
const LineConcrete& Line2Base<LineConcrete>::ConcreteLine() const
{
   return static_cast<LineConcrete&>(*this);
}


///////////////////

// Comparisions.

template <typename T, typename U>
bool operator==(const Line2Base<T>& a, const Line2Base<U>& b)
{
   return a.type() == b.type() && a.anchor() == b.anchor() &&
          a.direction() == b.direction();
}


template <typename T, typename U>
bool operator!=(const Line2Base<T>& a, const Line2Base<U>& b)
{
   return !(a == b);
}


template <typename T, typename U>
bool parallel(const Line2Base<T>& a, const Line2Base<U>& b)
{
   return parallel(a.direction(), b.direction().normalize());
}


// Checks if two lines are on the same inifinite line.
template <typename T, typename U>
bool coincident(const Line2Base<T>& a, const Line2Base<U>& b)
{
   return parallel(a, b) && a.isPointOnInfiniteLine(b.anchor());
}

} // namespace ct
} // namespace geom
