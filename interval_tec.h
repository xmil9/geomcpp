//
// geomcpp
// Mathematical interval encoding the open/closed state as part of the type.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "interval_types.h"
#include "essentutils/fputil.h"
#include <algorithm>
#include <type_traits>
#include <variant>


namespace geom
{
// (t)ype-(e)n(c)oded
namespace tec
{

///////////////////

// Types for open and closed interval ends.
struct OpenEnd
{
};
struct ClosedEnd
{
};

// Direction of interval ends.
struct LeftEnd
{
};
struct RightEnd
{
};

// Each end type is define by its orientation and inclusion properties.
template <typename Orient, typename Incl> struct EndType
{
   using Orientation = Orient;
   using Inclusion = Incl;
};


///////////////////

// Endpoint of an interval.
template <typename Value, typename ET> struct Endpoint
{
   Value val = Value(0);
};


// Implements a helper concept for comparing endpoints.
// When endpoint values are equal the ordering of the endpoints is determined
// by extending the endpoint's value towards the outside of its orientation.
// If the endpoint is closed the value is extended by one, if the endpoint is
// open the value is not extended.
template <typename Value, typename ET> Value extendedValue(Endpoint<Value, ET> ep)
{
   if constexpr (std::is_same_v<typename ET::Inclusion, ClosedEnd>)
   {
      if constexpr (std::is_same_v<typename ET::Orientation, LeftEnd>)
         return ep.val - Value(1);
      else
         return ep.val + Value(1);
   }
   else
   {
      return ep.val;
   }
}


// Define ordering for endpoints that have the same direction.
// The ordering has the following principles:
// - First compare the values.
// - If the values are unequal use their order.
// - If the values are equal, then the end type is examined:
//    - Closed end types are thought of as to extend the value range by 1 to the
//      outside direction of the end point.
//    - Open end types are thought of as to not extend the value range.
//    - The extended values are compared.
template <typename Value, typename ET1, typename ET2>
bool operator==(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   static_assert(std::is_same_v<typename ET1::Orientation, typename ET2::Orientation>,
                 "Cannot compare endpoints with different orientation.");
   return sutil::equal(a.val, b.val) &&
          std::is_same_v<typename ET1::Inclusion, typename ET2::Inclusion>;
}

template <typename Value, typename ET1, typename ET2>
bool operator!=(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   return !(a == b);
}

template <typename Value, typename ET1, typename ET2>
bool operator<(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   static_assert(std::is_same_v<typename ET1::Orientation, typename ET2::Orientation>,
                 "Cannot compare endpoints with different orientation.");
   return sutil::less(a.val, b.val) ||
          (sutil::equal(a.val, b.val) && sutil::less(extendedValue(a), extendedValue(b)));
}

template <typename Value, typename ET1, typename ET2>
bool operator<=(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   return a < b || a == b;
}

template <typename Value, typename ET1, typename ET2>
bool operator>(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   return !(a <= b);
}

template <typename Value, typename ET1, typename ET2>
bool operator>=(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   return !(a < b);
}


// Checks if two endpoints have overlapping value ranges.
template <typename Value, typename ET1, typename ET2>
bool overlapping(Endpoint<Value, ET1> a, Endpoint<Value, ET2> b)
{
   if (std::is_same_v<typename ET1::Orientation, typename ET2::Orientation>)
   {
      // Endpoints with the same orientation always overlap.
      return true;
   }
   else
   {
      Value left = a.val;
      Value right = b.val;
      if constexpr (std::is_same_v<typename ET1::Orientation, RightEnd>)
      {
         left = b.val;
         right = a.val;
      }

      // To overlap the left endpoint needs to be less than the right endpoint
      // or, if their values are equal, they both need to be closed.
      return sutil::less(left, right) ||
             (sutil::equal(left, right) &&
              std::is_same_v<typename ET1::Inclusion, ClosedEnd> &&
              std::is_same_v<typename ET2::Inclusion, ClosedEnd>);
   }
}


///////////////////

// Pair of end types for the left and right endpoints of an interval.
template <typename LeftET, typename RightET> struct EndTypePair
{
   using Left = LeftET;
   using Right = RightET;
};

// Combinations of end types for each interval type.
using Open = EndTypePair<EndType<LeftEnd, OpenEnd>, EndType<RightEnd, OpenEnd>>;
using LeftOpen = EndTypePair<EndType<LeftEnd, OpenEnd>, EndType<RightEnd, ClosedEnd>>;
using RightOpen = EndTypePair<EndType<LeftEnd, ClosedEnd>, EndType<RightEnd, OpenEnd>>;
using Closed = EndTypePair<EndType<LeftEnd, ClosedEnd>, EndType<RightEnd, ClosedEnd>>;


// Interval with type-encoded end types.
template <typename Value, typename ETs> class Interval
{
 public:
   using value_type = Value;
   using EndTypes = ETs;
   using LeftEndType = typename ETs::Left;
   using RightEndType = typename ETs::Right;

   constexpr Interval(Value start, Value end);
   Interval(const Interval&) = default;
   Interval(Interval&&) = default;

   Interval& operator=(const Interval&) = default;
   Interval& operator=(Interval&&) = default;
   explicit operator bool() const { return !isEmpty(); }
   bool operator!() const { return !operator bool(); }

   Value start() const noexcept { return m_left.val; }
   Value end() const noexcept { return m_right.val; }
   Endpoint<Value, LeftEndType> leftEndpoint() const noexcept { return m_left; }
   Endpoint<Value, RightEndType> rightEndpoint() const noexcept { return m_right; }
   constexpr IntervalEnd leftEndType() const noexcept;
   constexpr IntervalEnd rightEndType() const noexcept;

   constexpr IntervalType type() const noexcept;
   Value length() const noexcept;
   bool isEmpty() const noexcept;
   bool contains(Value val) const noexcept;

 private:
   bool containsLeft(Value val) const noexcept;
   bool containsRight(Value val) const noexcept;

 private:
   Endpoint<Value, LeftEndType> m_left;
   Endpoint<Value, RightEndType> m_right;
};


template <typename Value, typename ETs>
constexpr Interval<Value, ETs>::Interval(Value start, Value end)
: m_left{std::min(start, end)}, m_right{std::max(start, end)}
{
}


template <typename Value, typename ETs>
constexpr IntervalEnd Interval<Value, ETs>::leftEndType() const noexcept
{
   if constexpr (std::is_same_v<typename LeftEndType::Inclusion, ClosedEnd>)
      return IntervalEnd::Closed;
   else
      return IntervalEnd::Open;
}


template <typename Value, typename ETs>
constexpr IntervalEnd Interval<Value, ETs>::rightEndType() const noexcept
{
   if constexpr (std::is_same_v<typename RightEndType::Inclusion, ClosedEnd>)
      return IntervalEnd::Closed;
   else
      return IntervalEnd::Open;
}


template <typename Value, typename ETs>
constexpr IntervalType Interval<Value, ETs>::type() const noexcept
{
   if constexpr (std::is_same_v<ETs, Open>)
      return IntervalType::Open;
   else if constexpr (std::is_same_v<ETs, LeftOpen>)
      return IntervalType::LeftOpen;
   else if constexpr (std::is_same_v<ETs, RightOpen>)
      return IntervalType::RightOpen;
   else
      return IntervalType::Closed;
}

template <typename Value, typename ETs>
Value Interval<Value, ETs>::length() const noexcept
{
   // The mathmatical definition of the length of an interval is the absolute
   // difference of its endpoints.
   return end() - start();
}

template <typename Value, typename ETs>
bool Interval<Value, ETs>::isEmpty() const noexcept
{
   switch (type())
   {
   case IntervalType::Open:
      // Open intervals of integer values have a special case. If the start and end
      // values are consecutive numbers then the interval is empty because both values
      // are excluded.
      if constexpr (std::is_integral_v<Value>)
         return length() <= 1;
      else
         return sutil::equal(start(), end());

   case IntervalType::Closed:
      // Closed intervals contain at least one point (two unless start and endpoint
      // are the same).
      return false;

   case IntervalType::LeftOpen:
   case IntervalType::RightOpen:
      // Half-open intervals are only empty if start and end point are the same.
      return sutil::equal(start(), end());

   default:
      assert(false && "Unknown interval type.");
      return false;
   }
}


template <typename Value, typename ETs>
bool Interval<Value, ETs>::contains(Value val) const noexcept
{
   return containsLeft(val) && containsRight(val);
}


template <typename Value, typename ETs>
bool Interval<Value, ETs>::containsLeft(Value val) const noexcept
{
   if constexpr (std::is_same_v<typename LeftEndType::Inclusion, ClosedEnd>)
      return sutil::greaterEqual(val, start());
   else
      return sutil::greater(val, start());
}


template <typename Value, typename ETs>
bool Interval<Value, ETs>::containsRight(Value val) const noexcept
{
   if constexpr (std::is_same_v<typename RightEndType::Inclusion, ClosedEnd>)
      return sutil::lessEqual(val, end());
   else
      return sutil::less(val, end());
}


///////////////////

// Alias templates for each template types.
template <typename Value> using ClosedInterval = Interval<Value, Closed>;
template <typename Value> using OpenInterval = Interval<Value, Open>;
template <typename Value> using LeftOpenInterval = Interval<Value, LeftOpen>;
template <typename Value> using RightOpenInterval = Interval<Value, RightOpen>;

// Variable template for an empty interval.
template <typename Value> constexpr OpenInterval<Value> EmptyInterval{Value{0}, Value{0}};

// Union of all interval types.
template <typename Value>
using SomeInterval = std::variant<OpenInterval<Value>, LeftOpenInterval<Value>,
                                  RightOpenInterval<Value>, ClosedInterval<Value>>;


///////////////////

// Equality of intervals.

template <typename Value, typename ETsA, typename ETsB>
bool operator==(const Interval<Value, ETsA>& a, const Interval<Value, ETsB>& b)
{
   return a.leftEndpoint() == b.leftEndpoint() && a.rightEndpoint() == b.rightEndpoint();
}


template <typename Value, typename ETsA, typename ETsB>
bool operator!=(const Interval<Value, ETsA>& a, const Interval<Value, ETsB>& b)
{
   return !(a == b);
}


///////////////////

template <typename Value, typename ETs1, typename ETs2>
SomeInterval<Value> intersectOrderedIntervals(const Interval<Value, ETs1>& first,
                                              const Interval<Value, ETs2>& second)
{
   if (!overlapping(first.rightEndpoint(), second.leftEndpoint()))
   {
      // Disjoint intervals.
      return EmptyInterval<Value>;
   }
   else if (first.rightEndpoint() >= second.rightEndpoint())
   {
      // Second interval is fully contained in first.
      return second;
   }

   // Overlapping.
   return Interval<Value, EndTypePair<typename ETs2::Left, typename ETs1::Right>>(
      second.start(), first.end());
}


template <typename Value, typename ETsA, typename ETsB>
SomeInterval<Value> intersect(const Interval<Value, ETsA>& a,
                              const Interval<Value, ETsB>& b)
{
   if (a.leftEndpoint() <= b.leftEndpoint())
      return intersectOrderedIntervals(a, b);
   return intersectOrderedIntervals(b, a);
}


///////////////////

template <typename Value, typename ETs1, typename ETs2>
SomeInterval<Value> uniteOrderedIntervals(const Interval<Value, ETs1>& first,
                                          const Interval<Value, ETs2>& second)
{
   if (first.rightEndpoint() >= second.rightEndpoint())
      return first;

   return Interval<Value, EndTypePair<typename ETs1::Left, typename ETs2::Right>>(
      first.start(), second.end());
}


template <typename Value, typename ETsA, typename ETsB>
SomeInterval<Value> unite(const Interval<Value, ETsA>& a, const Interval<Value, ETsB>& b)
{
   if (a.leftEndpoint() <= b.leftEndpoint())
      return uniteOrderedIntervals(a, b);
   return uniteOrderedIntervals(b, a);
}

} // namespace tec
} // namespace geom