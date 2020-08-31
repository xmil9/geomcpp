//
// geomcpp
// Generation of evenly distributed points.
//
// Aug-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "rect.h"
#include "ring.h"
#include "essentutils/rand_util.h"
#include <cmath>
#include <optional>
#include <vector>


namespace geom
{

namespace internals
{
///////////////////

// Grid that divides the domain into cells each containing either the index of a
// sample that lies within the cell or an empty marker. Allows to quickly lookup
// if another sample is nearby.
template <typename T> class BackgroundGrid
{
 public:
   BackgroundGrid(const Rect<T>& domain, T minDist);

   // Inserts the given index of a given sample into the grid.
   void insert(const Point2<T>& sample, int sampleIdx);
   // Checks whether another sample is within the minimal distance of a given
   // test point.
   bool haveSampleWithinMinDistance(const Point2<T>& test) const;

 private:
   static int calcGridRows(const Rect<T>& domain, T cellSize);
   static int calcGridColumns(const Rect<T>& domain, T cellSize);
   int calcRow(T y) const;
   int calcCol(T x) const;
   bool isCellOccupied(int r, int c) const;

 private:
   using Grid = std::vector<std::vector<int>>;

   static constexpr T SqrtOfTwo = static_cast<T>(1.414213562373);
   static constexpr int EmptyCell = -1;

   const Rect<T> m_domain;
   const T m_minDist;
   T m_cellSize;
   Grid m_grid;
};


template <typename T>
BackgroundGrid<T>::BackgroundGrid(const Rect<T>& domain, T minDist)
: m_domain{domain}, m_minDist{minDist},
  // Pick cell size so that when checking whether another point is within the
  // min distance of a given point only a small number of cells has to be
  // checked.
  // Using minDist/sqrt(2) means that the diagonal of a cell is minDist long:
  //   len(diagonal) = sqrt(cellSize^2 + cellSize^2)
  //            	   = sqrt((minDist/sqrt(2))^2 + (minDist/sqrt(2))^2)
  // 	               = sqrt(minDist^2 / 2 + minDist^2 / 2)
  //	               = sqrt(minDist^2)
  //	               = minDist
  // Therefore, for a given point only the following cells need to be checked:
  //   - cell of point itself
  //   - 2 cells in each direction (because cellSize < minDist < 2*cellSize)
  //   - 1 cell in each diagonal (because len(diagonal) == minDist)
  m_cellSize{minDist / SqrtOfTwo},
  m_grid(calcGridRows(m_domain, m_cellSize),
         std::vector<int>(calcGridColumns(m_domain, m_cellSize), EmptyCell))
{
}


template <typename T>
void BackgroundGrid<T>::insert(const Point2<T>& sample, int sampleIdx)
{
   const int r = calcRow(sample.y());
   const int c = calcCol(sample.x());
   m_grid[r][c] = sampleIdx;
}


template <typename T>
bool BackgroundGrid<T>::haveSampleWithinMinDistance(const Point2<T>& test) const
{
   const int testRow = calcRow(test.y);
   const int testCol = calcCol(test.x);

   const int topMostRow = calcRow(test.y - m_minDist);
   const int bottomMostRow = calcRow(test.y + m_minDist);
   const int leftMostCol = calcCol(test.x - m_minDist);
   const int rightMostCol = calcCol(test.x + m_minDist);

   // Depending on where within its cell the test point is located we have to
   // check one or two cells into each direction, e.g. if the test point is
   // located in the top, left quadrant of its cell we have to check two cells
   // to the left and top of the test cell but only one cell to the right and
   // bottom. We have to do this for a strip of cells three cells thick
   // horizontally and vertically.

   // Proceed from the top most row to the bottom most.

   // If necessary, row of cells two cells above the test cell.
   if (topMostRow < testRow - 1)
   {
      for (int c = testCol - 1; c <= testCol + 1; ++c)
         if (isCellOccupied(topMostRow, c))
            return true;
   }
   // Center block of rows.
   for (int r = testRow - 1; r <= testRow + 1; ++r)
   {
      for (int c = leftMostCol; c <= rightMostCol; ++c)
         if (isCellOccupied(r, c))
            return true;
   }
   // If necessary, row of cells two cells below the test cell.
   if (bottomMostRow > testRow + 1)
   {
      for (int c = testCol - 1; c <= testCol + 1; ++c)
         if (isCellOccupied(bottomMostRow, c))
            return true;
   }

   return false;
}


template <typename T>
int BackgroundGrid<T>::calcGridRows(const Rect<T>& domain, T cellSize)
{
   return static_cast<int>(std::ceil(m_domain.height() / m_cellSize));
}


template <typename T>
int BackgroundGrid<T>::calcGridColumns(const Rect<T>& domain, T cellSize)
{
   return static_cast<int>(std::ceil(m_domain.width() / m_cellSize));
}


template <typename T> int BackgroundGrid<T>::calcRow(T y) const
{
   return static_cast<int>(std::floor((y - m_domain.top()) / m_cellSize));
}


template <typename T> int BackgroundGrid<T>::calcCol(T x) const
{
   return static_cast<int>(std::floor((x - m_domain.left()) / m_cellSize));
}


// Checks if a cell at given coordinates is occupied.
template <typename T> bool BackgroundGrid<T>::isCellOccupied(int r, int c) const
{
   if (r < 0 || r >= m_grid.size())
      return false;
   if (c < 0 || c >= m_grid[r].size())
      return false;
   return m_grid[r][c] != EmptyCell;
}


///////////////////

// Represents the ring-shaped area around a given point that candidate samples
// are taken from.
template <typename T> class Annulus
{
 public:
   Annulus(const Point2<T>& center, T innerRadius, T outerRadius, const Rect<T>& domain,
           sutil::Random<T>& rand);

   Point2<T> generatePointInRing();

 private:
   Point2<T> generatePointInBounds();

 private:
   Ring<T> m_ring;
   // Overlap of ring bounds and domain area.
   Rect<T> m_bounds;
   sutil::Random<T>& m_rand;
};


template <typename T>
Annulus<T>::Annulus(const Point2<T>& center, T innerRadius, T outerRadius,
const Rect<T>& domain, sutil::Random<T>& rand)
: m_ring{center, innerRadius, outerRadius}, m_bounds{intersect(m_ring.bounds(), domain)}, m_rand{rand}
{
}


template <typename T> Point2<T> Annulus<T>::generatePointInRing()
{
   Point2<T> pt = generatePointInBounds();
   while (!isPointInRing(m_ring, pt))
      pt = generatePointInBounds();
   return pt;
}


template <typename T> Point2<T> Annulus<T>::generatePointInBounds()
{
   const T x = m_bounds.left() + m_rand.next() * m_bounds.width();
   const T y = m_bounds.top() + m_rand.next() * m_bounds.height();
   return {x, y};
}

} // namespace internals


///////////////////

// Algorithm for generating evenly distributed points.
// Implements Bridson's Algorithm:
// - Time: O(n)
// https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
template <typename T> class PoissonDiscSampling
{
 public:
   // Number of candidates that are generated when trying to find a new sample.
   static constexpr int NumCandidatesDefault = 30;

   PoissonDiscSampling(const Rect<T>& domain, T minDist, int numCandidatePoints,
                       sutil::Random<T>& rand);

   // Generates samples by picking a random initial samples.
   std::vector<Point2<T>> generate();
   // Generates samples with given initial sample.
   std::vector<Point2<T>> generate(const Point2<T>& initialSample);

private:
   // Generates random sample.
   Point2<T> generateSample();
	// Abstracts the process of choosing the next seed sample to generate
	// candidates for. Returns index into sample array.
   int chooseSeed() const;
   // Stores a given sample in the internal data structures.
   void storeSample(const Point2<T>& sample);
	// Marks a given sample as not active anymore
	void deactivateSample(int sampleIdx);
	// Finds a new sample for a given seed sample.
	std::optional<Point2<T>> findNewSample(const Point2<T>& seedSample) const;

 private:
   Rect<T> m_domain;
   // Min distance that samples are allowed to be from each other.
   T m_minDist;
   int m_numCandidates;
   // Max distance from seed sample that candidate samples are looked for.
   T m_maxCandidateDist;
   sutil::Random<T>& m_rand;
   std::vector<Point2<T>> m_samples;
   std::vector<int> m_active;
   internals::BackgroundGrid<T> m_grid;
};


template <typename T>
PoissonDiscSampling<T>::PoissonDiscSampling(const Rect<T>& domain, T minDist,
                                            int numCandidatePoints,
                                            sutil::Random<T>& rand)
: m_domain{domain}, m_minDist{minDist}, m_numCandidates{numCandidatePoints},
  m_maxCandidateDist{2 * minDist}, m_rand{rand}, m_grid{domain, minDist}
{
}


template <typename T>
std::vector<Point2<T>> PoissonDiscSampling<T>::generate()
{
   return generate(generateSample());
}


template <typename T>
std::vector<Point2<T>> PoissonDiscSampling<T>::generate(const Point2<T>& initialSample)
{
	storeSample(initialSample);

	while (!m_active.isEmpty())
   {
		int seedIdx = chooseSeed();
		const Point2<T> seedSample = m_samples.get(seedIdx);
		const auto newSample = findNewSample(seedSample);
		if (!newSample)
			deactivateSample(seedIdx);
		else
			storeSample(*newSample);
	}
		
	return m_samples;
}


template <typename T>
Point2<T> PoissonDiscSampling<T>::generateSample()
{
	const T x = m_domain.left() + m_rand.next() * m_domain.width();
	const T y = m_domain.top() + m_rand.next() * m_domain.height();
   return {x, y};
}


template <typename T>
int PoissonDiscSampling<T>::chooseSeed() const
{
   return m_active[0];
}


template <typename T>
void PoissonDiscSampling<T>::storeSample(const Point2<T>& sample)
{
	m_samples.push_back(sample);
	const int sampleIdx = m_samples.size() - 1;
	m_active.push_back(sampleIdx);
	m_grid.insert(sample, sampleIdx);
}


template <typename T>
void PoissonDiscSampling<T>::deactivateSample(int sampleIdx)
{
   m_active.erase(m_active.begin() + sampleIdx);
}


template <typename T>
std::optional<Point2<T>> PoissonDiscSampling<T>::findNewSample(const Point2<T>& seedSample) const
{
   internals::Annulus annulus{seedSample, m_minDist, m_maxCandidateDist, m_domain, m_rand};
		
	for (std::size_t i = 0; i < m_numCandidates; ++i)
   {
		const Point2<T> candidate = annulus.generatePointInRing();
		if (!m_grid.haveSampleWithinMinDistance(candidate))
			return candidate;
	}

	return std::nullopt;
}

} // namespace geom
