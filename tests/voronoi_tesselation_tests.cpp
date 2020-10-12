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

void testForNoPoints()
{
   const std::string caseLabel = "VoronoiTesselation for no points";

   using Fp = float;

   VoronoiTesselation<Fp> vt({});
   const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();
   VERIFY(tiles.empty(), caseLabel);
}


void testForOnePoint()
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


void testForTwoPoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for two points with border";

      using Fp = double;

      const Rect<Fp> border{0.0, -1.0, 3.0, 6.0};
      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}}, border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 2, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {0.0, -1.0}, {0.0, 3.75}, {3.0, 2.25}, {3.0, -1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {0.0, 3.75}, {0.0, 6.0}, {3.0, 6.0}, {3.0, 2.25}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for two points without border";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 2, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {1.0, 2.0}, {1.0, 3.25}, {2.0, 2.75}, {2.0, 2.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {1.0, 3.25}, {1.0, 4.0}, {2.0, 4.0}, {2.0, 2.75}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for two points with offset bounding border";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}}, 2.0);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 2, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {-1.0, 0.0}, {-1.0, 4.25}, {4.0, 1.75}, {4.0, 0.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {-1.0, 4.25}, {-1.0, 6.0}, {4.0, 6.0}, {4.0, 1.75}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
   }
}


void testForThreePoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for three points";

      using Fp = double;

      // Very special case! The circum-center of the single Delauney
      // triangle lies outside the border bounds. This causes some
      // special situations in the code that find the Voronoi edges.
      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}, {-2.0, 1.4}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 3, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{{-0.95999999, 4.0},
                                                  {-0.49999999, 4.0},
                                                  {1.999999999, 2.75},
                                                  {2.0, 1.4},
                                                  {-0.43999999, 1.3999999}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{{-0.5, 4.0}, {2.0, 4.0}, {2.0, 2.75}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
      const std::vector<Point2<Fp>> expectedTileC{{-0.5, 4.0}, {2.0, 4.0}, {2.0, 2.75}};
      VERIFY(hasTileWithVertices(tiles, expectedTileC), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for three points with border";

      using Fp = double;

      const Rect<Fp> border{-3.0, -1.0, 4.0, 5.0};
      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}, {-2.0, 1.4}}, border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 3, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {4.0, 1.75}, {4.0, -1.0}, {0.04, -1.0}, {-1.01111111, 4.25555555}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {-1.49499999, 5.0}, {4.0, 5.0}, {3.99999999, 1.75}, {-1.01111111, 4.255555555}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
      const std::vector<Point2<Fp>> expectedTileC{{-1.494999999, 5.0},
                                                  {-1.011111111, 4.25555555},
                                                  {0.04, -1.0},
                                                  {-3.0, -1.0},
                                                  {-3.0, 5.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileC), caseLabel);
   }
}


void testForFourPoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for four points";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{-1.0, -2.0}, {0.0, 3.0}, {4.0, 1.0}, {3.0, -1.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 4, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {-1.0, 0.6}, {0.5526315789, 0.2894736842}, {1.125, -2.0}, {-1.0, -2.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {-1.0, 0.6}, {-1.0, 3.0}, {2.5, 3.0}, {1.5, 1.0}, {0.5526315789, 0.2894736842}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
      const std::vector<Point2<Fp>> expectedTileC{{4.0, -0.25},
                                                  {4.0, -2.0},
                                                  {1.125, -2.0},
                                                  {0.5526315789, 0.2894736842},
                                                  {1.5, 1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileC), caseLabel);
      const std::vector<Point2<Fp>> expectedTileD{{2.5, 3.0},
                                                  {4.0, 3.0},
                                                  {4.0, -0.25},
                                                  {1.5, 1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileD), caseLabel);
   }
}


void testForFivePoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for five points";

      using Fp = double;

      VoronoiTesselation<Fp> vt(
         {{-1.0, -1.0}, {-2.0, 1.0}, {0.0, 3.0}, {2.0, 2.0}, {2.0, 0.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 5, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{{-2.0, -0.25},
                                                  {0.07142857142, 0.7857142857},
                                                  {0.6666666666, -0.9999999999},
                                                  {-2.0, -1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {-2.0, 3.0}, {0.1, 0.9}, {0.0714285714, 0.785714285}, {-2.0, -0.25}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
      const std::vector<Point2<Fp>> expectedTileC{
         {1.25, 3.0}, {2.0, 3.0}, {2.0, 1.0}, {0.25, 1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileC), caseLabel);
      const std::vector<Point2<Fp>> expectedTileD{
         {2.0, 1.0}, {2.0, -1.0}, {0.66666666, -1.0}, {0.0714285714, 0.7857142857},
         {0.1, 0.9}, {0.25, 1.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileD), caseLabel);
      const std::vector<Point2<Fp>> expectedTileE{
         {-2.0, 3.0}, {1.25, 3.0}, {0.25, 1.0}, {0.1, 0.9}};
      VERIFY(hasTileWithVertices(tiles, expectedTileE), caseLabel);
   }
}


void testForRect()
{
   {
      const std::string caseLabel = "VoronoiTesselation for rect";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{-1.0, -2.0}, {-1.0, 3.0}, {5.0, 3.0}, {5.0, -2.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      VERIFY(tiles.size() == 4, caseLabel);
      const std::vector<Point2<Fp>> expectedTileA{
         {-1.0, 0.5}, {2.0, 0.5}, {2.0, -2.0}, {-1.0, -2.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileA), caseLabel);
      const std::vector<Point2<Fp>> expectedTileB{
         {2.0, 3.0}, {5.0, 3.0}, {5.0, 0.5}, {2.0, 0.5}};
      VERIFY(hasTileWithVertices(tiles, expectedTileB), caseLabel);
      const std::vector<Point2<Fp>> expectedTileC{
         {2.0, 3.0}, {2.0, 0.5}, {-1.0, 0.5}, {-1.0, 3.0}};
      VERIFY(hasTileWithVertices(tiles, expectedTileC), caseLabel);
      const std::vector<Point2<Fp>> expectedTileD{
         {5.0, 0.5}, {5.0, -2.0}, {2.0, -2.0}, {2.0, 0.5}};
      VERIFY(hasTileWithVertices(tiles, expectedTileD), caseLabel);
   }
}

} // namespace


void testVoronoiTesselation()
{
   testForNoPoints();
   testForOnePoint();
   testForTwoPoints();
   testForThreePoints();
   testForFourPoints();
   testForFivePoints();
   testForRect();
}
