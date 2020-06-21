//
// geomcpp
// Types for mathematical intervals.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cassert>
#include <utility>


namespace geom
{

enum class IntervalSide
{
   Left,
   Right,
};


enum class IntervalEnd
{
   Open,
   Closed,
};


enum class IntervalType
{
   Open,
   LeftOpen,
   RightOpen,
   Closed,
};


inline constexpr IntervalType combine(IntervalEnd left, IntervalEnd right)
{
   if (left == IntervalEnd::Open)
      return (right == IntervalEnd::Open) ? IntervalType::Open : IntervalType::LeftOpen;
   return (right == IntervalEnd::Open) ? IntervalType::RightOpen : IntervalType::Closed;
}


inline constexpr IntervalEnd extractLeft(IntervalType type)
{
   switch (type)
   {
      case IntervalType::Open:
      case IntervalType::LeftOpen:
         return IntervalEnd::Open;

      case IntervalType::Closed:
      case IntervalType::RightOpen:
         return IntervalEnd::Closed;
      
      default:
         assert(false && "Unknown interval type.");
         return IntervalEnd::Open;
   }
}


inline constexpr IntervalEnd extractRight(IntervalType type)
{
   switch (type)
   {
      case IntervalType::Open:
      case IntervalType::RightOpen:
         return IntervalEnd::Open;

      case IntervalType::Closed:
      case IntervalType::LeftOpen:
         return IntervalEnd::Closed;
      
      default:
         assert(false && "Unknown interval type.");
         return IntervalEnd::Open;
   }
}

} // namespace geom
