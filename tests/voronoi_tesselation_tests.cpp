//
// geomcpp tests
// Tests for Delauney triangulation.
//
// Sept-2020, Michael Lindner
// MIT license
//
#include "voronoi_tesselation_tests.h"
#include "test_util.h"
#include "voronoi_tesselation.h"
#include "voronoi_tile.h"
#include "essentutils/fputil.h"
#include <algorithm>

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

// Verifies that a given tile contains given points.
// Avoids depending on the order of points in the tile.
template <typename T>
bool hasVertices(const VoronoiTile<T>& t, const std::vector<Point2<T>>& pts)
{
   if (t.size() != pts.size())
      return false;

   for (const auto& pt : pts)
      if (!t.contains(pt))
         return false;

   return true;
}

// Verifies that given tiles contains one tiles with given points.
// Avoids depending on the order of tiles and the order of points in
// a tile.
template <typename T>
bool hasTileWithVertices(const std::vector<VoronoiTile<T>>& tess,
                         const std::vector<Point2<T>>& pts)
{
   for (const auto& tile : tess)
      if (hasVertices(tile, pts))
         return true;
   return false;
}


///////////////////

void testNoPoint()
{
   const std::string caseLabel = "VoronoiTesselation for no points";

   using Fp = float;

   VoronoiTesselation<Fp> vt({});
   const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();
   VERIFY(tiles.empty(), caseLabel);
}


void testOnePoint()
{
   {
      const std::string caseLabel = "VoronoiTesselation for one point with border";

      using Fp = float;

      const Rect<Fp> border{0.0f, -1.0f, 3.0f, 6.0f};
      VoronoiTesselation<Fp> vt({{1.0f, 2.0f}}, border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 1, caseLabel);
      const std::vector<Point2<Fp>> expectedPts{
         border.leftTop(), border.rightTop(), border.rightBottom(), border.leftBottom()};
      VERIFY(hasTileWithVertices(tiles, expectedPts), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for one point with bounding border";

      using Fp = double;

      constexpr Point2<Fp> pt{1.0, 2.0};
      VoronoiTesselation<Fp> vt({pt});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 1, caseLabel);
      const std::vector<Point2<Fp>> expectedPts{pt};
      VERIFY(hasTileWithVertices(tiles, expectedPts), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for one point with offset border";

      using Fp = double;

      constexpr Point2<Fp> pt{1.0, 2.0};
      constexpr Fp dist{2.0};
      VoronoiTesselation<Fp> vt({pt}, dist);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 1, caseLabel);
      const std::vector<Point2<Fp>> expectedPts{
         pt.offset(-dist, -dist), pt.offset(-dist, dist), pt.offset(dist, -dist),
         pt.offset(dist, dist)};
      VERIFY(hasTileWithVertices(tiles, expectedPts), caseLabel);
   }
}

} // namespace


void testVoronoiTesselation()
{
   testNoPoint();
   testOnePoint();
}
