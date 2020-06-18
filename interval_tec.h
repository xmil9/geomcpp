//
// geomcpp
// ClosedInterval of numbers.
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
struct OpenEndType
{
};
struct ClosedEndType
{
};


// Comparision for strictness of two end types.
template <typename EndTypeA, typename EndTypeB>
inline constexpr bool isStricterEndType =
   std::is_same_v<EndTypeA, OpenEndType>&& std::is_same_v<EndTypeB, ClosedEndType>;


// Pair of end types for the left and right endpoints of an interval.
template <typename LeftEndType, typename RightEndType> struct EndTypePair
{
   using Left = LeftEndType;
   using Right = RightEndType;
};

// Combinations of end types for each interval type.
using Closed = EndTypePair<ClosedEndType, ClosedEndType>;
using Open = EndTypePair<OpenEndType, OpenEndType>;
using LeftOpen = EndTypePair<OpenEndType, ClosedEndType>;
using RightOpen = EndTypePair<ClosedEndType, OpenEndType>;


///////////////////

// Interval with type-encoded end types.
template <typename Value, typename EndTypes> class Interval
{
 public:
   using value_type = Value;
   using EndTypes_t = EndTypes;

   constexpr Interval(Value start, Value end);
   Interval(const Interval&) = default;
   Interval(Interval&&) = default;

   Interval& operator=(const Interval&) = default;
   Interval& operator=(Interval&&) = default;
   explicit operator bool() const;
   bool operator!() const;

   Value start() const noexcept;
   Value end() const noexcept;
   Value length() const noexcept;
   bool isEmpty() const noexcept;
   bool contains(Value val) const noexcept;
   IntervalEnd leftEnd() const noexcept;
   IntervalEnd rightEnd() const noexcept;
   IntervalType type() const noexcept;

 private:
   bool isLeftIncluded(Value val) const;
   bool isRightIncluded(Value val) const;

 private:
   std::pair<Value, Value> m_endPoints;
};


template <typename Value, typename EndTypes>
constexpr Interval<Value, EndTypes>::Interval(Value start, Value end)
: m_endPoints{std::min(start, end), std::max(start, end)}
{
}

template <typename Value, typename EndTypes>
Interval<Value, EndTypes>::operator bool() const
{
   return !isEmpty();
}


template <typename Value, typename EndTypes>
bool Interval<Value, EndTypes>::operator!() const
{
   return !operator bool();
}


template <typename Value, typename EndTypes>
Value Interval<Value, EndTypes>::start() const noexcept
{
   return m_endPoints.first;
}


template <typename Value, typename EndTypes>
Value Interval<Value, EndTypes>::end() const noexcept
{
   return m_endPoints.second;
}


template <typename Value, typename EndTypes>
Value Interval<Value, EndTypes>::length() const noexcept
{
   // The mathmatical definition of the length of an interval is the absolute
   // difference of its endpoints.
   return end() - start();
}


template <typename Value, typename EndTypes>
bool Interval<Value, EndTypes>::isEmpty() const noexcept
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


template <typename Value, typename EndTypes>
bool Interval<Value, EndTypes>::contains(Value val) const noexcept
{
   return isLeftIncluded(val) && isRightIncluded(val);
}


template <typename Value, typename EndTypes>
IntervalEnd Interval<Value, EndTypes>::leftEnd() const noexcept
{
   if constexpr (std::is_same_v<typename EndTypes::Left, ClosedEndType>)
      return IntervalEnd::Closed;
   else
      return IntervalEnd::Open;
}


template <typename Value, typename EndTypes>
IntervalEnd Interval<Value, EndTypes>::rightEnd() const noexcept
{
   if constexpr (std::is_same_v<typename EndTypes::Right, ClosedEndType>)
      return IntervalEnd::Closed;
   else
      return IntervalEnd::Open;
}


template <typename Value, typename EndTypes>
IntervalType Interval<Value, EndTypes>::type() const noexcept
{
   if constexpr (std::is_same_v<EndTypes, Open>)
      return IntervalType::Open;
   else if constexpr (std::is_same_v<EndTypes, LeftOpen>)
      return IntervalType::LeftOpen;
   else if constexpr (std::is_same_v<EndTypes, RightOpen>)
      return IntervalType::RightOpen;
   else
      return IntervalType::Closed;
}


template <typename Value, typename EndTypes>
bool Interval<Value, EndTypes>::isLeftIncluded(Value val) const
{
   if constexpr (std::is_same_v<typename EndTypes::Left, ClosedEndType>)
      return sutil::greaterEqual(val, start());
   else
      return sutil::greater(val, start());
}


template <typename Value, typename EndTypes>
bool Interval<Value, EndTypes>::isRightIncluded(Value val) const
{
   if constexpr (std::is_same_v<typename EndTypes::Right, ClosedEndType>)
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


template <typename Value, typename EndTypes>
bool operator==(const Interval<Value, EndTypes>& a, const Interval<Value, EndTypes>& b)
{
   return a.start() == b.start() && a.end() == b.end();
}


template <typename Value, typename EndTypes>
bool operator!=(const Interval<Value, EndTypes>& a, const Interval<Value, EndTypes>& b)
{
   return !(a == b);
}


///////////////////

template <typename Value, typename EndTypes1st, typename EndTypes2nd>
SomeInterval<Value> intersectOrderedIntervals(const Interval<Value, EndTypes1st>& first,
                                              const Interval<Value, EndTypes2nd>& second)
{
   if (sutil::less(first.end(), second.start()) ||
       // End of the first interval has the same value as the start of the second interval
       // and one of the endpoints is open.
       (sutil::equal(first.end(), second.start()) &&
        (std::is_same_v<typename EndTypes1st::Right, OpenEndType> ||
         std::is_same_v<typename EndTypes2nd::Left, OpenEndType>)))
   {
      // Disjoint intervals.
      return EmptyInterval<Value>;
   }
   else if (sutil::greater(first.end(), second.end()) ||
            (sutil::equal(first.end(), second.end()) &&
             std::is_same_v<typename EndTypes2nd::Right, OpenEndType>))
   {
      // Second interval is fully contained in first.
      return second;
   }

   // Overlapping.
   return Interval<Value,
                   EndTypePair<typename EndTypes2nd::Left, typename EndTypes1st::Right>>(
      second.start(), first.end());
}


template <typename Value, typename EndTypesA, typename EndTypesB>
SomeInterval<Value> intersect(const Interval<Value, EndTypesA>& a,
                              const Interval<Value, EndTypesB>& b)
{
   if (sutil::less(a.start(), b.start()) ||
       (sutil::equal(a.start(), b.start()) && isStricterEndType<EndTypesA, EndTypesB>))
   {
      return intersectOrderedIntervals(a, b);
   }
   return intersectOrderedIntervals(b, a);
}


///////////////////

template <typename Value, typename EndTypes1st, typename EndTypes2nd>
SomeInterval<Value> uniteOrderedIntervals(const Interval<Value, EndTypes1st>& first,
                                          const Interval<Value, EndTypes2nd>& second)
{
   if (sutil::greaterEqual(first.end(), second.end()))
   {
      return Interval<
         Value, EndTypePair<typename EndTypes1st::Left, typename EndTypes1st::Right>>(
         first.start(), first.end());
   }

   return Interval<Value,
                   EndTypePair<typename EndTypes1st::Left, typename EndTypes2nd::Right>>(
      first.start(), second.end());
}


template <typename Value, typename EndTypesA, typename EndTypesB>
SomeInterval<Value> unite(const Interval<Value, EndTypesA>& a,
                          const Interval<Value, EndTypesB>& b)
{
   if (sutil::lessEqual(a.start(), b.start()))
      return uniteOrderedIntervals(a, b);
   return uniteOrderedIntervals(b, a);
}

} // namespace tec
} // namespace geom