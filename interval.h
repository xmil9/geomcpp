//
// geomcpp
// ClosedInterval of numbers.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "essentutils/fputil.h"
#include <algorithm>
#include <type_traits>
#include <variant>


namespace geom
{

///////////////////

struct OpenInclusion
{
};

struct ClosedInclusion
{
};


template <typename Value, typename Inclusion> struct Endpoint
{
   using Inclusion_t = Inclusion;
   Value val;
};


template <typename Value, typename Inclusion>
bool isLeftIncluded(Value val, Value leftEndpoint)
{
   if constexpr (std::is_floating_point_v<Value>)
   {
      if constexpr (std::is_same_v<Inclusion, ClosedInclusion>)
         return sutil::fpGreaterEqual(val, leftEndpoint);
      else
         return sutil::fpGreater(val, leftEndpoint);
   }
   else
   {
      if constexpr (std::is_same_v<Inclusion, ClosedInclusion>)
         return val >= leftEndpoint;
      else
         return val > leftEndpoint;
   }
}


template <typename Value, typename Inclusion>
bool isRightIncluded(Value val, Value rightEndpoint)
{
   if constexpr (std::is_floating_point_v<Value>)
   {
      if constexpr (std::is_same_v<Inclusion, ClosedInclusion>)
         return sutil::fpLessEqual(val, rightEndpoint);
      else
         return sutil::fpLess(val, rightEndpoint);
   }
   else
   {
      if constexpr (std::is_same_v<Inclusion, ClosedInclusion>)
         return val <= rightEndpoint;
      else
         return val < rightEndpoint;
   }
}


///////////////////

template <typename LeftInclusion, typename RightInclusion> struct IntervalInclusion
{
   using Left = LeftInclusion;
   using Right = RightInclusion;
};

using Closed = IntervalInclusion<ClosedInclusion, ClosedInclusion>;
using Open = IntervalInclusion<OpenInclusion, OpenInclusion>;
using LeftOpen = IntervalInclusion<OpenInclusion, ClosedInclusion>;
using RightOpen = IntervalInclusion<ClosedInclusion, OpenInclusion>;


///////////////////

template <typename Value, typename Inclusion> class Interval
{
 public:
   using value_type = Value;
   using Inclusion_t = Inclusion;

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
   constexpr bool isLeftOpen();
   constexpr bool isRightOpen();

 private:
   Value m_start;
   Value m_end;
};


template <typename Value, typename Inclusion>
constexpr Interval<Value, Inclusion>::Interval(Value start, Value end)
: m_start{std::min(start, end)}, m_end{std::max(start, end)}
{
}

template <typename Value, typename Inclusion>
Interval<Value, Inclusion>::operator bool() const
{
   return !isEmpty();
}


template <typename Value, typename Inclusion>
bool Interval<Value, Inclusion>::operator!() const
{
   return !operator bool();
}


template <typename Value, typename Inclusion>
Value Interval<Value, Inclusion>::start() const noexcept
{
   return m_start;
}


template <typename Value, typename Inclusion>
Value Interval<Value, Inclusion>::end() const noexcept
{
   return m_end;
}


template <typename Value, typename Inclusion>
Value Interval<Value, Inclusion>::length() const noexcept
{
   // The mathmatical definition of the length of an interval is the absolute
   // difference of its endpoints.
   return m_end - m_start;
}


template <typename Value, typename Inclusion>
bool Interval<Value, Inclusion>::isEmpty() const noexcept
{
   // Closed intervals contain at least one point (two unless start and endpoint
   // are the same).
   if constexpr (std::is_same_v<typename Inclusion::Left, ClosedInclusion> &&
                 std::is_same_v<typename Inclusion::Right, ClosedInclusion>)
   {
      return false;
   }
   else
   {
      if constexpr (std::is_floating_point_v<Value>)
         return sutil::fpEqual(m_start, m_end);
      else
         return m_start == m_end;
   }
}


template <typename Value, typename Inclusion>
bool Interval<Value, Inclusion>::contains(Value val) const noexcept
{
   return isLeftIncluded<Value, typename Inclusion::Left>(val, m_start) &&
          isRightIncluded<Value, typename Inclusion::Right>(val, m_end);
}


template <typename Value, typename Inclusion>
constexpr bool Interval<Value, Inclusion>::isLeftOpen()
{
   return std::is_same_v<typename Inclusion::Left, OpenInclusion>;
}


template <typename Value, typename Inclusion>
constexpr bool Interval<Value, Inclusion>::isRightOpen()
{
   return std::is_same_v<typename Inclusion::Right, OpenInclusion>;
}


///////////////////

template <typename Value> using ClosedInterval = Interval<Value, Closed>;
template <typename Value> using OpenInterval = Interval<Value, Open>;
template <typename Value> using LeftOpenInterval = Interval<Value, LeftOpen>;
template <typename Value> using RightOpenInterval = Interval<Value, RightOpen>;

template <typename Value> constexpr OpenInterval<Value> EmptyInterval{Value{0}, Value{0}};

template <typename Value>
using SomeInterval = std::variant<ClosedInterval<Value>, OpenInterval<Value>,
                                  LeftOpenInterval<Value>, RightOpenInterval<Value>>;


///////////////////

template <typename Value, typename Inclusion1st, typename Inclusion2nd>
SomeInterval<Value> intersectOrderedByStart(const Interval<Value, Inclusion1st>& first,
                                            const Interval<Value, Inclusion2nd>& second)
{
   // TODO: abstract comparisions by number type
   if (first.end() < second.start())
   {
      // Disjoint.
      return EmptyInterval<Value>;
   }
   else if (first.end() >= second.end())
   {
      // Fully contained.
      return second;
   }

   // Overlapping.
   return Interval<Value, IntervalInclusion<typename Inclusion2nd::Left,
                                            typename Inclusion1st::Right>>(second.start(),
                                                                           first.end());
}


template <typename Value, typename InclusionA, typename InclusionB>
SomeInterval<Value> intersect(const Interval<Value, InclusionA>& a,
                              const Interval<Value, InclusionB>& b)
{
   // TODO: abstract comparision by number type
   if (a.start() <= b.start())
      return intersectOrderedByStart(a, b);
   return intersectOrderedByStart(b, a);
}


///////////////////

template <typename Value, typename Inclusion1st, typename Inclusion2nd>
SomeInterval<Value> uniteOrderedByStart(const Interval<Value, Inclusion1st>& first,
                                        const Interval<Value, Inclusion2nd>& second)
{
   // TODO: abstract comparision by number type
   if (first.end() >= second.end())
   {
      return Interval<Value, IntervalInclusion<typename Inclusion1st::Left,
                                               typename Inclusion1st::Right>>(
         first.start(), first.end());
   }

   return Interval<Value, IntervalInclusion<typename Inclusion1st::Left,
                                            typename Inclusion2nd::Right>>(first.start(),
                                                                           second.end());
}


template <typename Value, typename InclusionA, typename InclusionB>
SomeInterval<Value> unite(const Interval<Value, InclusionA>& a,
                          const Interval<Value, InclusionB>& b)
{
   // TODO: abstract comparision by number type
   if (a.start() <= b.start())
      return uniteOrderedByStart(a, b);
   return uniteOrderedByStart(b, a);
}


///////////////////

template <typename Value, typename Inclusion>
bool operator==(const Interval<Value, Inclusion>& a, const Interval<Value, Inclusion>& b)
{
   return a.start() == b.start() && a.end() == b.end();
}


template <typename Value, typename Inclusion>
bool operator!=(const Interval<Value, Inclusion>& a, const Interval<Value, Inclusion>& b)
{
   return !(a == b);
}

} // namespace geom
