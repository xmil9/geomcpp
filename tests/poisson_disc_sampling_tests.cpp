//
// geomcpp tests
// Tests for Poisson disc sampling algorithm.
//
// Aug-2020, Michael Lindner
// MIT license
//
#include "poisson_disc_sampling_tests.h"
#include "point2.h"
#include "poisson_disc_sampling.h"
#include "test_util.h"
#include "essentutils/fputil.h"
#include <vector>

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

template <typename T>
bool verifyMinDistance(const std::vector<Point2<T>>& samples, T minDist)
{
   const T distSq = minDist * minDist;

   for (const auto& sample : samples)
      for (const auto& other : samples)
         if (sample != other && distSquared(sample, other) < distSq)
            return false;

   return true;
}


///////////////////

void testGenerateForMinDistanceLargerThanDomainBounds()
{
   {
      const std::string caseLabel =
         "Poisson disc sampling for min distance larger than domain bounds";

      using Fp = double;

      const Rect<Fp> domain{0.0, 0.0, 2.0, 2.0};
      const Fp minDist = 3.0;

      const int numRuns = 10;
      for (int i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{1111};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate();

         VERIFY(samples.size() == 1, caseLabel);
         if (!samples.empty())
            VERIFY(domain.isPointInRect(samples[0]), caseLabel);
      }
   }
}


void testGenerateForRandomInitialSample()
{
   {
      const std::string caseLabel =
         "Poisson disc sampling for random initial sample (case 1)";

      using Fp = double;

      const Rect<Fp> domain{0.0, 0.0, 20.0, 20.0};
      const Fp minDist = 3.0;

      const int numRuns = 10;
      for (int i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{1111};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate();

         VERIFY(!samples.empty(), caseLabel);
         VERIFY(verifyMinDistance(samples, minDist), caseLabel);
      }
   }
}

} // namespace


///////////////////

void testPoissonDiscSampling()
{
   testGenerateForMinDistanceLargerThanDomainBounds();
   testGenerateForRandomInitialSample();
}
