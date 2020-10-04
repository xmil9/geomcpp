//
// geomcpp
// Voronoi tiles.
//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "poly2.h"


namespace geom
{
///////////////////

template <typename T> class VoronoiTile
{
 public:
   VoronoiTile(Point2<T> seed, Poly2<T> outline);
   ~VoronoiTile() = default;
   VoronoiTile(const VoronoiTile&) = default;
   VoronoiTile(VoronoiTile&&) = default;
   
   VoronoiTile& operator=(const VoronoiTile&) = default;
   VoronoiTile& operator=(VoronoiTile&&) = default;

   const Point2<T>& seed() const { return m_seed; }
   const Point2<T>& outline() const { return m_outline; }
   std::size_t countVertices() const { return m_outline.size(); }
   bool containsVertex(const Point2<T>& pt) const { return m_outline.contains(pt); }

 private:
   Point2<T> m_seed;
   Poly2<T> m_outline;
};


template <typename T>
VoronoiTile<T>::VoronoiTile(Point2<T> seed, Poly2<T> outline)
: m_seed{std::move(seed)}, m_outline{std::move(outline)}
{
}

} // namespace geom
