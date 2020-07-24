//
// geomcpp
// Intersection of 2D lines using compile time/dynamic polymorphism.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "interval_dec.h"
#include "line_inf2_ct.h"
#include "line_ray2_ct.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include <cassert>
#include <limits>
#include <optional>
#include <variant>


namespace geom
{
// Use [c]ompile [t]ime polymorphism for line class hierarchy.
namespace ct
{
///////////////////

// Union of possible outcomes when intersecting two lines.
template <typename T>
using LineIntersection2 = std::variant<Point2<T>, LineSeg2<T>, LineRay2<T>, LineInf2<T>>;


///////////////////

namespace internals
{

template <typename T> constexpr T PosInf = std::numeric_limits<T>::max();
template <typename T> constexpr T NegInf = std::numeric_limits<T>::lowest();


// Returns the result for the reduced problem of intersecting a point with a line.
template <typename T, typename Line>
std::optional<LineIntersection2<T>> intersectPointLine(const Point2<T>& pt, const Line& l)
{
   if (l.isPointOnLine(pt))
      return std::make_optional(pt);
   return std::nullopt;
}


// Intersects two given lines where at least one line degenerated into a point.
template <typename LineA, typename LineB>
std::optional<LineIntersection2<typename LineA::value_type>>
intersectDegenerateLines(const LineA& a, const LineB& b)
{
   // Also covers case of both lines being points.
   if (a.isPoint())
      return intersectPointLine(a.anchor(), b);

   assert(b.isPoint());
   return intersectPointLine(b.anchor(), a);
}


// Creates the result for the intersection of two coincident lines that overlap at
// a given interval that contains the interpolation factors relative to a given
// reference line.
template <typename FP, typename Line>
std::optional<LineIntersection2<typename Line::value_type>>
makeCoincidentIntersection(const dec::Interval<FP>& overlap, const Line& refLine)
{
   using Value = typename Line::value_type;

   if (overlap == dec::EmptyInterval<FP>)
      return std::nullopt;

   std::size_t numInfiniteEnds = 0;
   if (overlap.start() == NegInf<FP>)
      ++numInfiniteEnds;
   if (overlap.end() == PosInf<FP>)
      ++numInfiniteEnds;

   switch (numInfiniteEnds)
   {
   case 0:
      if (sutil::equal(overlap.start(), overlap.end()))
      {
         return std::make_optional(refLine.lerp(overlap.start()));
      }
      else
      {
         const LineSeg2<Value> res{refLine.lerp(overlap.start()),
                                   refLine.lerp(overlap.end())};
         return std::make_optional(res);
      }

   case 1:
      if (overlap.start() == NegInf<FP>)
      {
         const LineRay2<Value> res{refLine.lerp(overlap.end()), -refLine.direction()};
         return std::make_optional(res);
      }
      else
      {
         assert(overlap.end() == PosInf<FP>);
         const LineRay2<Value> res{refLine.lerp(overlap.start()), refLine.direction()};
         return std::make_optional(res);
      }

   case 2:
      return std::make_optional(LineInf2<Value>{refLine.anchor(), refLine.direction()});

   default:
      assert(false && "Unexpected number of line ends. :)");
      return std::nullopt;
   }
}


// Intersects two given lines that are coincident (lie on the same infinite line).
template <typename LineA, typename LineB>
std::optional<LineIntersection2<typename LineA::value_type>>
intersectCoincidentLines(const LineA& a, const LineB& b)
{
   using Fp = sutil::FpType<typename LineA::value_type>;
   using dec::Interval;

   // Use the interpolation values of the lines' start and end points to
   // create intervals that represent the lines on the infinite line
   // that they share. Express the interval of line b with respect to
   // line a, so that the interpolation value ranges can be compared.
   // When line b is infinite or a ray watch out to choose the correct
   // inifinite interpolation values depending on whether b is in the same
   // or the opposite direction of a.
   Fp begin = a.startPoint() ? 0 : NegInf<Fp>;
   Fp end = a.endPoint() ? 1 : PosInf<Fp>;
   const Interval<Fp> aIval{begin, end, IntervalType::Closed};

   const bool haveSameDir = sameDirection(b.direction(), a.direction());
   begin = haveSameDir ? NegInf<Fp> : PosInf<Fp>;
   if (const auto startPt = b.startPoint())
      if (const auto startFactor = a.lerpFactor(*startPt))
         begin = *startFactor;

   end = haveSameDir ? PosInf<Fp> : NegInf<Fp>;
   if (const auto endPt = b.endPoint())
      if (const auto endFactor = a.lerpFactor(*endPt))
         end = *endFactor;

   const Interval<Fp> bIval{begin, end, IntervalType::Closed};

   // Calculate the overlap of the two intervals.
   const Interval<Fp> overlap = intersect(aIval, bIval);

   return makeCoincidentIntersection(overlap, a);
}


// Calculates the interpolation factors of the intersection point for two
// given lines.
// Source: http://geomalgorithms.com/a05-_intersect-1.html
template <typename LineA, typename LineB>
std::pair<typename LineA::Fp, typename LineB::Fp>
calcLerpFactorsOfIntersectionPoint(const LineA& a, const LineB& b)
{
   assert(!parallel(a, b));

   using Value = typename LineA::value_type;
   using Fp = typename LineA::Fp;

   const Vec2<Value> u = a.direction();
   const Vec2<Value> v = b.direction();
   const Vec2<Value> w{b.anchor(), a.anchor()};

   // Calc interpolation factors of the intersection point relative to each
   // line.
   // The divisions are safe because the denominators would only be zero
   // if the lines were parallel which should be checked before calling
   // this method.
   const Fp factorA = (v.y() * w.x() - v.x() * w.y()) / perpDot(v, u);
   const Fp factorB = perpDot(u, w) / perpDot(u, v);

   return std::make_pair(factorA, factorB);
}


// Checks if the point for a given interpolation factor is on the line and not
// beyond the start or end points.
template <typename FP, typename Line>
bool isInterpolatedPointOnLine(FP lerpFactor, const Line& l)
{
   if (l.startPoint() && sutil::less<FP>(lerpFactor, 0))
      return false;
   if (l.endPoint() && sutil::greater<FP>(lerpFactor, 1))
      return false;
   return true;
}


// Intersects two given lines that have no special relation to each other.
// Source: http://geomalgorithms.com/a05-_intersect-1.html
template <typename LineA, typename LineB>
std::optional<LineIntersection2<typename LineA::value_type>>
intersectSkewLines(const LineA& a, const LineB& b)
{
   const auto lerpFactors = calcLerpFactorsOfIntersectionPoint(a, b);

   // The lines only intersect if the parametric values of the intersection
   // points lie on each line (not beyond the start or end points).
   if (isInterpolatedPointOnLine(lerpFactors.first, a) &&
       isInterpolatedPointOnLine(lerpFactors.second, b))
   {
      return std::make_optional(a.lerp(lerpFactors.first));
   }
   return std::nullopt;
}

} // namespace internals


///////////////////

// Intersect two given lines.
template <typename LineA, typename LineB>
std::optional<LineIntersection2<typename LineA::value_type>> intersect(const LineA& a,
                                                                       const LineB& b)
{
   static_assert(std::is_same_v<typename LineA::value_type, typename LineB::value_type>);

   if (a.isPoint() || b.isPoint())
      return internals::intersectDegenerateLines(a, b);

   if (coincident(a, b))
      return internals::intersectCoincidentLines(a, b);
   else if (parallel(a, b))
      return std::nullopt;

   return internals::intersectSkewLines(a, b);
}

} // namespace ct
} // namespace geom
