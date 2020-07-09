//
// geomcpp
// 2D line segments using compile time/static polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "line2_base_ct.h"
#include "point2.h"
#include "vec2.h"
#include <optional>


namespace geom
{
// Use [c]ompile [t]ime polymorphism for line class hierarchy.
namespace ct
{
///////////////////

template <typename T> class LineSeg2 : public Line2Base<LineSeg2<T>>
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

 public:
   LineSeg2() = default;
   constexpr LineSeg2(const Point2<T>& start, const Point2<T>& end);
   constexpr LineSeg2(const Point2<T>& start, const Vec2<T>& direction);

   Line2Type type() const;
   std::optional<Point2<T>> startPoint() const;
   std::optional<Point2<T>> endPoint() const;
   std::optional<Fp> isPointOnLine(const Point2<T>& pt) const;

   Point2<T> midPoint() const;
   Fp lengthSquared() const;
   Fp length() const;
};


template <typename T>
constexpr LineSeg2<T>::LineSeg2(const Point2<T>& start, const Point2<T>& end)
: Line2Base<LineSeg2<T>>{start, Vec2<T>{start, end}}
{
}


template <typename T>
constexpr LineSeg2<T>::LineSeg2(const Point2<T>& start, const Vec2<T>& direction)
: Line2Base<LineSeg2<T>>{anchor, direction}
{
}


template <typename T> Line2Type LineSeg2<T>::type() const
{
   return Line2Type::Segment;
}


template <typename T> std::optional<Point2<T>> LineSeg2<T>::startPoint() const
{
   return this->anchor();
}


template <typename T> std::optional<Point2<T>> LineSeg2<T>::endPoint() const
{
   return this->anchor() + this->direction();
}


template <typename T>
std::optional<typename LineSeg2<T>::Fp>
LineSeg2<T>::isPointOnLine(const Point2<T>& pt) const
{
   const auto factor = this->lerpFactor(pt);
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

} // namespace ct
} // namespace geom
