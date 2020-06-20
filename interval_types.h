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


inline IntervalType combine(IntervalEnd left, IntervalEnd right)
{
   if (left == IntervalEnd::Open)
      return (right == IntervalEnd::Open) ? IntervalType::Open : IntervalType::LeftOpen;
   return (right == IntervalEnd::Open) ? IntervalType::RightOpen : IntervalType::Closed;
}


inline std::pair<IntervalEnd, IntervalEnd> split(IntervalType type)
{
   switch (type)
   {
      case IntervalType::LeftOpen:
         return {IntervalEnd::Open, IntervalEnd::Closed};
      case IntervalType::RightOpen:
         return {IntervalEnd::Closed, IntervalEnd::Open};
      case IntervalType::Closed:
         return {IntervalEnd::Closed, IntervalEnd::Closed};
      default:
         assert(false && "Unknown interval type.");
         [[fallthrough]];
      case IntervalType::Open:
         return {IntervalEnd::Open, IntervalEnd::Open};
   }
}

} // namespace geom
