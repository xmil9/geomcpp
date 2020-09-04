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

      Random<Fp> rand{1111};
      PoissonDiscSampling<Fp> sampler{
         domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
      std::vector<Point2<Fp>> samples = sampler.generate();

      VERIFY(samples.size() == 1, caseLabel);
      if (!samples.empty())
         VERIFY(domain.isPointInRect(samples[0]), caseLabel);
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

      const std::size_t numRuns = 10;
      for (std::size_t i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{1111};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate();

         VERIFY(!samples.empty(), caseLabel);
         VERIFY(verifyMinDistance(samples, minDist), caseLabel);
      }
   }
   {
      const std::string caseLabel =
         "Poisson disc sampling for random initial sample (case 2)";

      using Fp = float;

      const Rect<Fp> domain{-30.0f, -50.0f, 100.0f, 200.0f};
      const Fp minDist = 11.5f;

      const std::size_t numRuns = 10;
      for (std::size_t i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{2222};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate();

         VERIFY(!samples.empty(), caseLabel);
         VERIFY(verifyMinDistance(samples, minDist), caseLabel);
      }
   }
}


void testGenerateForGivenInitialSample()
{
   {
      const std::string caseLabel =
         "Poisson disc sampling for given initial sample (case 1)";

      using Fp = float;

      const Rect<Fp> domain{0.0f, 0.0f, 20.0f, 20.0f};
      const Fp minDist = 3.0f;

      const std::size_t numRuns = 10;
      for (std::size_t i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{3333};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate({3.0f, 6.0f});

         VERIFY(!samples.empty(), caseLabel);
         VERIFY(verifyMinDistance(samples, minDist), caseLabel);
      }
   }
   {
      const std::string caseLabel =
         "Poisson disc sampling for given initial sample (case 2)";

      using Fp = double;

      const Rect<Fp> domain{-30.0, -50.0, 100.0, 200.0};
      const Fp minDist = 11.5;

      const std::size_t numRuns = 10;
      for (std::size_t i = 0; i < numRuns; ++i)
      {
         Random<Fp> rand{4444};
         PoissonDiscSampling<Fp> sampler{
            domain, minDist, PoissonDiscSampling<Fp>::NumCandidatesDefault, rand};
         std::vector<Point2<Fp>> samples = sampler.generate({37.0, -3.0});

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
   testGenerateForGivenInitialSample();
}
