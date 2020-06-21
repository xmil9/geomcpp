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
// (d)ata-(e)n(c)oded
namespace dec
{

///////////////////

template <typename Value> struct Endpoint
{
   Value val = Value(0);
   IntervalSide side = IntervalSide::Left;
   IntervalEnd type = IntervalEnd::Open;
};


// Implements a helper concept for comparing endpoints.
// When endpoint values are equal the ordering of the endpoints is determined
// by extending the endpoint's value towards the outside of its orientation.
// If the endpoint is closed the value is extended by one, if the endpoint is
// open the value is not extended.
template <typename Value> Value extendedValue(Endpoint<Value> ep)
{
   if (ep.type == IntervalEnd::Closed)
   {
      const Value offset = (ep.side == IntervalSide::Left) ? Value(-1) : Value(1);
      return ep.val + offset;
   }
   return ep.val;
}


// Define ordering for endpoints that have the same side.
// The ordering has the following principles:
// - First compare the values.
// - If the values are unequal use their order.
// - If the values are equal, then the end type is examined:
//    - Closed end types are thought of as to extend the value range by 1 to the
//      outside side of the end point.
//    - Open end types are thought of as to not extend the value range.
//    - The extended values are compared.
template <typename Value> bool operator==(Endpoint<Value> a, Endpoint<Value> b)
{
   assert(a.side == b.side && "Cannot compare endpoints with different sides");
   return sutil::equal(a.val, b.val) && a.type == b.type;
}

template <typename Value> bool operator!=(Endpoint<Value> a, Endpoint<Value> b)
{
   return !(a == b);
}

template <typename Value> bool operator<(Endpoint<Value> a, Endpoint<Value> b)
{
   assert(a.side == b.side && "Cannot compare endpoints with different sides");
   return sutil::less(a.val, b.val) ||
          (sutil::equal(a.val, b.val) && sutil::less(extendedValue(a), extendedValue(b)));
}

template <typename Value> bool operator<=(Endpoint<Value> a, Endpoint<Value> b)
{
   return a < b || a == b;
}

template <typename Value> bool operator>(Endpoint<Value> a, Endpoint<Value> b)
{
   return !(a <= b);
}

template <typename Value> bool operator>=(Endpoint<Value> a, Endpoint<Value> b)
{
   return !(a < b);
}


// Checks if two endpoints have overlapping value ranges.
template <typename Value> bool overlapping(Endpoint<Value> a, Endpoint<Value> b)
{
   if (a.side == b.side)
   {
      // Endpoints with the same side always overlap.
      return true;
   }
   else
   {
      const Value left = (a.side == IntervalSide::Left) ? a.val : b.val;
      const Value right = (a.side == IntervalSide::Left) ? b.val : a.val;

      // To overlap the left endpoint needs to be less than the right endpoint
      // or, if their values are equal, they both need to be closed.
      return sutil::less(left, right) ||
             (sutil::equal(left, right) && a.type == IntervalEnd::Closed &&
              b.type == IntervalEnd::Closed);
   }
}


///////////////////

// Interval with data-encoded end types.
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
   explicit operator bool() const { return !isEmpty(); }
   bool operator!() const { return !operator bool(); }

   Value start() const noexcept { return m_left.val; }
   Value end() const noexcept { return m_right.val; }
   Endpoint<Value> leftEndpoint() const noexcept { return m_left; }
   Endpoint<Value> rightEndpoint() const noexcept { return m_right; }
   IntervalEnd leftEndType() const noexcept { return m_left.type; }
   IntervalEnd rightEndType() const noexcept { return m_right.type; }

   IntervalType type() const noexcept;
   Value length() const noexcept;
   bool isEmpty() const noexcept;
   bool contains(Value val) const noexcept;

 private:
   bool containsLeft(Value val) const noexcept;
   bool containsRight(Value val) const noexcept;

 private:
   Endpoint<Value> m_left;
   Endpoint<Value> m_right;
};


template <typename Value>
constexpr Interval<Value>::Interval(Value start, Value end, IntervalType type)
: m_left{std::min(start, end), IntervalSide::Left, extractLeft(type)},
  m_right{std::max(start, end), IntervalSide::Right, extractRight(type)}
{
}


template <typename Value>
constexpr Interval<Value>::Interval(Value start, Value end, IntervalEnd left,
                                    IntervalEnd right)
: m_left{std::min(start, end), IntervalSide::Left, left}, m_right{std::max(start, end),
                                                                  IntervalSide::Right,
                                                                  right}
{
}


template <typename Value> IntervalType Interval<Value>::type() const noexcept
{
   return combine(leftEndType(), rightEndType());
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
   return containsLeft(val) && containsRight(val);
}


template <typename Value> bool Interval<Value>::containsLeft(Value val) const noexcept
{
   if (leftEndType() == IntervalEnd::Open)
      return sutil::greater(val, start());
   return sutil::greaterEqual(val, start());
}


template <typename Value> bool Interval<Value>::containsRight(Value val) const noexcept
{
   if (rightEndType() == IntervalEnd::Open)
      return sutil::less(val, end());
   return sutil::lessEqual(val, end());
}


///////////////////

// Variable template for an empty interval.
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
   const bool keepOrder = a.leftEndpoint() <= b.leftEndpoint();
   const Interval<Value>& first = keepOrder ? a : b;
   const Interval<Value>& second = keepOrder ? b : a;

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

   // Overlapping intervals.
   return Interval<Value>{second.start(), first.end(), second.leftEndType(),
                          first.rightEndType()};
}


template <typename Value>
Interval<Value> unite(const Interval<Value>& a, const Interval<Value>& b)
{
   const Interval<Value>& left = (a.leftEndpoint() <= b.leftEndpoint()) ? a : b;
   const Interval<Value>& right = (a.rightEndpoint() >= b.rightEndpoint()) ? a : b;
   return Interval<Value>{left.start(), right.end(), left.leftEndType(),
                          right.rightEndType()};
}

} // namespace dec
} // namespace geom
