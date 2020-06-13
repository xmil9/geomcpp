//
// essentutils
// Floating point utilities.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cassert>
#include <cmath>
#include <type_traits>


namespace sutil
{

////////////////////

template <typename FP> struct FpTraits
{
};


template <> struct FpTraits<double>
{
   // Threshold below which two fp values are considered equal.
   static constexpr double epsilon = 0.0000001;
};


template <> struct FpTraits<float>
{
   // Threshold below which two fp values are considered equal.
   static constexpr float epsilon = 0.0000001f;
};


template <> struct FpTraits<long double>
{
   // Threshold below which two fp values are considered equal.
   static constexpr long double epsilon = 0.0000001L;
};


///////////////////

template <typename FP> bool fpEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   return std::abs(a - b) <= eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpEqual(FP a, FP b)
{
   return fpEqual(a, b, Traits::epsilon);
}


template <typename FP> bool fpLess(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
	// Check that a is smaller than b by at least the epsilon value
	// because within that threshold they would still be considered
	// equal.
   return a - b < -eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpLess(FP a, FP b)
{
   return fpLess(a, b, Traits::epsilon);
}


template <typename FP> bool fpLessEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
	// Check that b is larger than a by at least the epsilon value
	// because within that threshold they would still be considered
	// equal.
   return a - b <= eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpLessEqual(FP a, FP b)
{
   return fpLessEqual(a, b, Traits::epsilon);
}


template <typename FP> bool fpGreater(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
	// Check that a is larger than b by at least the epsilon value
	// because within that threshold they would still be considered
	// equal.
   return a - b > eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpGreater(FP a, FP b)
{
   return fpGreater(a, b, Traits::epsilon);
}


template <typename FP> bool fpGreaterEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
	// Check that b is smaller than a by at least the epsilon value
	// because within that threshold they would still be considered
	// equal.
   return a - b >= -eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpGreaterEqual(FP a, FP b)
{
   return fpGreaterEqual(a, b, Traits::epsilon);
}

} // namespace sutil
