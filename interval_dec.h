//
// geomcpp
// Mathematical interval encoding the open/closed state as data.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "interval_types.h"
#include "essentutils/fputil.h"
#include <algorithm>
#include <type_traits>
#include <utility>


namespace geom
{
// Interval with data-encoded end types.
namespace dec
{

///////////////////

template <typename Value> class Interval
{
 public:
   using value_type = Value;

   constexpr Interval(Value start, Value end, IntervalType type);
   constexpr Interval(Value start, Value end, IntervalEnd left, IntervalEnd right);
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
   std::pair<IntervalEnd, IntervalEnd> m_endTypes;
};


template <typename Value>
constexpr Interval<Value>::Interval(Value start, Value end, IntervalType type)
: m_endPoints{std::min(start, end), std::max(start, end)}, m_endTypes{split(type)}
{
}


template <typename Value>
constexpr Interval<Value>::Interval(Value start, Value end, IntervalEnd left,
                                    IntervalEnd right)
: m_endPoints{std::min(start, end), std::max(start, end)}, m_endTypes{left, right}
{
}


template <typename Value> Interval<Value>::operator bool() const
{
   return !isEmpty();
}


template <typename Value> bool Interval<Value>::operator!() const
{
   return !operator bool();
}


template <typename Value> Value Interval<Value>::start() const noexcept
{
   return m_endPoints.first;
}


template <typename Value> Value Interval<Value>::end() const noexcept
{
   return m_endPoints.second;
}


template <typename Value> Value Interval<Value>::length() const noexcept
{
   // The mathmatical definition of the length of an interval is the absolute
   // difference of its endpoints.
   return end() - start();
}


template <typename Value> bool Interval<Value>::isEmpty() const noexcept
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


template <typename Value> bool Interval<Value>::contains(Value val) const noexcept
{
   return isLeftIncluded(val) && isRightIncluded(val);
}


template <typename Value> IntervalEnd Interval<Value>::leftEnd() const noexcept
{
   return m_endTypes.first;
}


template <typename Value> IntervalEnd Interval<Value>::rightEnd() const noexcept
{
   return m_endTypes.second;
}


template <typename Value> IntervalType Interval<Value>::type() const noexcept
{
   return combine(leftEnd(), rightEnd());
}


template <typename Value> bool Interval<Value>::isLeftIncluded(Value val) const
{
   if (leftEnd() == IntervalEnd::Open)
      return sutil::greater(val, start());
   return sutil::greaterEqual(val, start());
}


template <typename Value> bool Interval<Value>::isRightIncluded(Value val) const
{
   if (rightEnd() == IntervalEnd::Open)
      return sutil::less(val, end());
   return sutil::lessEqual(val, end());
}


///////////////////

template <typename Value>
constexpr Interval<Value> EmptyInterval{Value{0}, Value{0}, IntervalType::Open};


template <typename Value>
bool operator==(const Interval<Value>& a, const Interval<Value>& b)
{
   return a.start() == b.start() && a.end() == b.end() && a.type() == b.type();
}


template <typename Value>
bool operator!=(const Interval<Value>& a, const Interval<Value>& b)
{
   return !(a == b);
}


///////////////////

template <typename Value>
Interval<Value> intersect(const Interval<Value>& a, const Interval<Value>& b)
{
   const bool keepOrder = sutil::lessEqual(a.start(), b.start());
   const Interval<Value>& first = keepOrder ? a : b;
   const Interval<Value>& second = keepOrder ? b : a;

   if (sutil::less(first.end(), second.start()))
   {
      // Disjoint intervals.
      return EmptyInterval<Value>;
   }
   else if (sutil::greaterEqual(first.end(), second.end()))
   {
      // Second interval is fully contained in first.
      return second;
   }

   // Overlapping intervals.
   return Interval<Value>{first.start(), second.end(), first.leftEnd(),
                          second.rightEnd()};
}


template <typename Value>
Interval<Value> unite(const Interval<Value>& a, const Interval<Value>& b)
{
   const Interval<Value>& left = sutil::lessEqual(a.start(), b.start()) ? a : b;
   const Interval<Value>& right = sutil::greaterEqual(a.end(), b.end()) ? a : b;
   return Interval<Value>{left.start(), right.end(), left.leftEnd(), right.rightEnd()};
}

} // namespace dec
} // namespace geom
