//
// geomcpp
// A 2D line using run time/dynamic polymorphism.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [r]un [t]ime polymorphism for line class hierarchy.
namespace rt
{
///////////////////

// A line extends infinitely into both directions.
template <typename T> class Line2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;
   using ParametricValue = Fp;

   Line2() = default;
   constexpr Line2(const Point2<T>& anchor, const Vec2<T>& direction);

   Point2<T> anchorPoint() const noexcept { return m_anchor; }
   Vec2<T> direction() const noexcept { return m_dir; }

   bool isPoint() const;
   virtual bool hasStartPoint() const { return false; }
   virtual std::optional<Point2<T>> startPoint() const { return std::nullopt; }
   virtual bool hasEndPoint() const  { return false; }   
   virtual std::optional<Point2<T>> endPoint() const { return std::nullopt; }

   virtual std::optional<ParametricValue> isPointOnLine(const Point2<T>& pt) const;
   // Checks if a given point is on the infinite extension of the line.
   std::optional<ParametricValue> isPointOnInfiniteLine(const Point2<T>& pt) const;
   // Calculates the parametric value of a given point along the line.
   std::optional<ParametricValue> calcParametricValue(const Point2<T>& pt) const;
   // Returns the point at a given parametric value along the line.
   template <typename U> Point2<T> calcPointAt(U parametricVal) const;

 protected:
   // Point that anchors the line in the coordinate system. For line types that
   // have a start point it is guaranteed to be the start point.
   Point2<T> m_anchor;
   // Direction of line. Whether the length of the direction vector has meaning
   // is up to each derived class.
   Vec2 m_dir;
};


template <typename T>
constexpr Line2<T>::Line2(const Point2<T>& anchor, const Vec2<T>& direction)
   : m_anchor{anchor}, m_dir{direction}
{
}


template <typename T> bool Line2<T>::isPoint() const
{
   return sutil::equal(direction().lengthSquared() == FP(0));
}


template <typename T>
std::optional<typename Line2<T>::ParametricValue>
Line2<T>::isPointOnLine(const Point2<T>& pt) const
{
   return isPointOnInfiniteLine(pt);
}


template <typename T>
std::optional<typename Line2<T>::ParametricValue>
Line2<T>::isPointOnInfiniteLine(const Point2<T>& pt) const
{
   return calcParametricValue(pt);
}


template <typename T>
std::optional<typename Line2<T>::ParametricValue>
Line2<T>::calcParametricValue(const Point2<T>& pt) const
{
   if (isPoint())
      return (pt == anchor) ? ParametricValue(0) : std::nullopt;

   const Vec2<T> v = Vec2{anchor, pt};
   if (!v.isParallel(dir))
      return std::nullopt;

   // length != 0 is assured by checking whether line is a point above.
   const auto parametricVal = v.length() / dir.length();
   if (!v.hasSameDirection(dir))
      parametricVal *= ParametricValue(-1);

   return parametricVal;
}


template <typename T>
template <typename U>
Point2<T> Line2<T>::calcPointAt(U parametricVal) const
{
   const Vec2 v = dir.scale(parametricVal);
   return anchor().offset(v);
}


///////////////////

template <typename T, typename U> bool parallel(const Line2<T>& a, const Line2<U>& b)
{
   return a.direction().isParallel(b.direction().normalize());
}


// Checks if two lines are on the same inifinite line.
template <typename T, typename U> bool coincident(const Line2<T>& a, const Line2<U>& b)
{
   return parallel(a, b) && a.isPointOnInfiniteLine(b.anchorPoint());
}


// template <typename T, typename U>
// LineIntersection2D.Result intersect(const Line2<T>& a, const Line2<T>& b) const;

} // namespace rt
} // namespace geom
