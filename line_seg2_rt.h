//
// geomcpp
// 2D line segment using run time/dynamic polymorphism.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_rt.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [r]un [t]ime polymorphism for line class hierarchy.
namespace rt
{
///////////////////

// A line segment has both a start and and end point.
template <typename T> class LineSeg2 : public Line2<T>
{
 public:
   using value_type = typename Line2<T>::value_type;
   using Fp = typename Line2<T>::Fp;

   LineSeg2() = default;
   LineSeg2(const Point2<T>& start, const Point2<T>& end);
   LineSeg2(const Point2<T>& start, const Vec2<T>& direction);

   std::optional<Point2<T>> startPoint() const override { return this->anchor(); }
   std::optional<Point2<T>> endPoint() const override;
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const override;

   Point2<T> midPoint() const;
   Fp lengthSquared() const;
   Fp length() const;
};


template <typename T>
LineSeg2<T>::LineSeg2(const Point2<T>& start, const Point2<T>& end)
: Line2<T>(start, Vec2<T>(start, end))
{
}


template <typename T>
LineSeg2<T>::LineSeg2(const Point2<T>& start, const Vec2<T>& direction)
: Line2<T>(start, direction)
{
}


template <typename T> std::optional<Point2<T>> LineSeg2<T>::endPoint() const
{
   return this->anchor() + this->direction();
}


template <typename T>
std::optional<typename LineSeg2<T>::Fp>
LineSeg2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto factor = this->calcLerpFactor(pt);
   if (factor && sutil::fpGreaterEqual<Fp>(*factor, 0) &&
       sutil::fpLessEqual<Fp>(*factor, 1))
   {
      return factor;
   }
   return std::nullopt;
}


template <typename T> Point2<T> LineSeg2<T>::midPoint() const
{
   return *startPoint() + (this->direction() * Fp(0.5));
}


template <typename T> typename LineSeg2<T>::Fp LineSeg2<T>::lengthSquared() const
{
   return this->direction().lengthSquared();
}


template <typename T> typename LineSeg2<T>::Fp LineSeg2<T>::length() const
{
   return this->direction().length();
}

} // namespace rt
} // namespace geom
