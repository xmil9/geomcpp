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

template <typename T, template <typename> typename LineType> class Line2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

 public:
   Line2Type type() const;
   Point2<T> anchor() const noexcept;
   Vec2<T> direction() const noexcept;

   bool isPoint() const;
   std::optional<Point2<T>> startPoint() const;
   std::optional<Point2<T>> endPoint() const;

   // Returns the interpolation factor of the point.
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const;

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
   const LineType<T>& ConcreteLine() const;

 private:
   // Point that anchors the line in the coordinate system. For line types that
   // have a start point it is guaranteed to be the start point.
   Point2<value_type> m_anchor;

   // Direction of line. Whether the length of the direction vector has meaning
   // is up to each derived class.
   Vec2<value_type> m_dir;
};


template <typename T, template <typename> typename LineType>
constexpr Line2<T, LineType>::Line2(const Point2<T>& anchor, const Vec2<T>& direction)
: m_anchor{anchor}, m_dir{direction}
{
}


template <typename T, template <typename> typename LineType>
Line2Type Line2<T, LineType>::type() const
{
   return ConcreteLine().type();
}


template <typename T, template <typename> typename LineType>
Point2<T> Line2<T, LineType>::anchor() const noexcept
{
   return m_anchor;
}


template <typename T, template <typename> typename LineType>
Vec2<T> Line2<T, LineType>::direction() const noexcept
{
   return m_dir;
}


template <typename T, template <typename> typename LineType>
bool Line2<T, LineType>::isPoint() const
{
   return sutil::equal(direction().lengthSquared(), Fp(0));
}


template <typename T, template <typename> typename LineType>
std::optional<Point2<T>> Line2<T, LineType>::startPoint() const
{
   return ConcreteLine().startPoint();
}


template <typename T, template <typename> typename LineType>
std::optional<Point2<T>> Line2<T, LineType>::endPoint() const
{
   return ConcreteLine().endPoint();
}


template <typename T, template <typename> typename LineType>
std::optional<typename Line2<T, LineType>::Fp>
Line2<T, LineType>::isPointOnLine(const Point2<T>& pt) const
{
   return ConcreteLine().isPointOnLine(pt);
}


template <typename T, template <typename> typename LineType>
template <typename U>
std::optional<typename Line2<T, LineType>::Fp>
Line2<T, LineType>::isPointOnInfiniteLine(const Point2<U>& pt) const
{
   return lerpFactor(pt);
}


template <typename T, template <typename> typename LineType>
template <typename U>
std::optional<typename Line2<T, LineType>::Fp>
Line2<T, LineType>::lerpFactor(const Point2<U>& pt) const
{
   if (isPoint())
      return (pt == anchor()) ? std::make_optional(Fp(0))
                              : std::nullopt;

   const auto v = Vec2<T>{anchor(), pt};
   if (!parallel(v, direction()))
      return std::nullopt;

   // length != 0 is assured by checking whether line is a point above.
   auto factor = v.length() / direction().length();
   if (!sameDirection(v, direction()))
      factor *= -1;

   return factor;
}


template <typename T, template <typename> typename LineType>
template <typename U>
Point2<T> Line2<T, LineType>::lerp(U factor) const
{
   const Vec2 v = direction() * factor;
   return anchor() + v;
}


template <typename T, template <typename> typename LineType>
const LineType<T>& Line2<T, LineType>::ConcreteLine() const
{
   return static_cast<const LineType<T>&>(*this);
}

///////////////////

// Comparisions.

template <typename T, template <typename> typename LineT, typename U,
          template <typename> typename LineU>
bool operator==(const Line2<T, LineT>& a, const Line2<U, LineU>& b)
{
   return a.type() == b.type() && a.anchor() == b.anchor() &&
          a.direction() == b.direction();
}


template <typename T, template <typename> typename LineT, typename U,
          template <typename> typename LineU>
bool operator!=(const Line2<T, LineT>& a, const Line2<U, LineU>& b)
{
   return !(a == b);
}


template <typename T, template <typename> typename LineT, typename U,
          template <typename> typename LineU>
bool parallel(const Line2<T, LineT>& a, const Line2<U, LineU>& b)
{
   return parallel(a.direction(), b.direction().normalize());
}


// Checks if two lines are on the same inifinite line.
template <typename T, template <typename> typename LineT, typename U,
          template <typename> typename LineU>
bool coincident(const Line2<T, LineT>& a, const Line2<U, LineU>& b)
{
   return parallel(a, b) && a.isPointOnInfiniteLine(b.anchor());
}

} // namespace ct
} // namespace geom
