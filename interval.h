//
// geomcpp
// Interval of numbers.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include <algorithm>
#include <type_traits>
#include "essentutils/fputil.h"


namespace geom
{

///////////////////

// Represents a (closed) interval between two values.
template <typename T> class Interval
{
 public:
   using value_type = T;

   Interval();
   constexpr Interval(T start, T end);
   Interval(const Interval&) = default;
   Interval(Interval&&) = default;

   Interval& operator=(const Interval&) = default;
   Interval& operator=(Interval&&) = default;
   explicit operator bool() const;
   bool operator!() const;

   T start() const noexcept;
   T end() const noexcept;
   T length() const noexcept;
   bool isEmpty() const noexcept;
   bool contains(T val) const noexcept;

 private:
   T m_start;
   T m_end;
};


template <typename T> Interval<T>::Interval() : m_start{T{0}}, m_end{T{0}}
{
}


template <typename T>
constexpr Interval<T>::Interval(T start, T end)
: m_start{std::min(start, end)}, m_end{std::max(start, end)}
{
}

template <typename T>
Interval<T>::operator bool() const
{
   return !isEmpty();
}


template <typename T>
bool Interval<T>::operator!() const
{
   return !operator bool();
}


template <typename T> T Interval<T>::start() const noexcept
{
   return m_start;
}


template <typename T> T Interval<T>::end() const noexcept
{
   return m_end;
}


template <typename T> T Interval<T>::length() const noexcept
{
   return m_end - m_start;
}


template <typename T> bool Interval<T>::isEmpty() const noexcept
{
   if constexpr (std::is_floating_point_v<T>)
      return sutil::fpEqual(length(), T{0.0});
   else
      return length() == T{0};
}


template <typename T> bool Interval<T>::contains(T val) const noexcept
{
   if constexpr (std::is_floating_point_v<T>)
      return sutil::fpGreaterEqual(val, m_start) && sutil::fpLessEqual(val, m_end);
   else
      return m_start <= val && val <= m_end;
}


///////////////////

template <typename T> Interval<T> intersect(const Interval<T>& a, const Interval<T>& b)
{
   Interval<T>& first = (a.start() <= b.start()) ? a : b;
   Interval<T>& second = (a.start() <= b.start()) ? b : a;

   if (first.end() < second.start())
   {
      // Disjoint.
      return Interval<T>{};
   }
   else if (first.end() >= second.end())
   {
      // Fully contained.
      return second;
   }
   // Overlapping.
   return new Interval(second.start(), first.end());
}


template <typename T> Interval<T> unite(const Interval<T>& a, const Interval<T>& b)
{
	return Interval(std::min(a.start(), b.start()), std::max(a.end(), b.end()));
}


template <typename T> bool operator==(const Interval<T>& a, const Interval<T>& b)
{
   return a.start() == b.start() && a.end() == b.end();
}


template <typename T> bool operator!=(const Interval<T>& a, const Interval<T>& b)
{
   return !(a == b);
}

} // namespace geom
