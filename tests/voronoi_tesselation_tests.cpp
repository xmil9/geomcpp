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


// Verifies that a tesselation contains given expected tiles.
template <typename T>
bool hasTiles(const std::vector<VoronoiTile<T>>& tess,
              const std::vector<std::vector<Point2<T>>>& expectedTiles)
{
   if (tess.size() != expectedTiles.size())
      return false;

   for (const auto& expectedTile : expectedTiles)
      if (!hasTileWithVertices(tess, expectedTile))
         return false;
   return true;
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

      const std::vector<std::vector<Point2<Fp>>> expected{
         {border.leftTop(), border.rightTop(), border.rightBottom(),
          border.leftBottom()}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for one point with bounding border";

      using Fp = double;

      constexpr Point2<Fp> pt{1.0, 2.0};
      VoronoiTesselation<Fp> vt({pt});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected{{pt}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for one point with offset border";

      using Fp = double;

      constexpr Point2<Fp> pt{1.0, 2.0};
      constexpr Fp dist{2.0};
      VoronoiTesselation<Fp> vt({pt}, dist);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected{
         {pt.offset(-dist, -dist), pt.offset(-dist, dist), pt.offset(dist, -dist),
          pt.offset(dist, dist)}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
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

      const std::vector<std::vector<Point2<Fp>>> expected = {
         {{0.0, -1.0}, {0.0, 3.75}, {3.0, 2.25}, {3.0, -1.0}},
         {{0.0, 3.75}, {0.0, 6.0}, {3.0, 6.0}, {3.0, 2.25}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for two points without border";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         {{1.0, 2.0}, {1.0, 3.25}, {2.0, 2.75}, {2.0, 2.0}},
         {{1.0, 3.25}, {1.0, 4.0}, {2.0, 4.0}, {2.0, 2.75}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for two points with offset bounding border";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}}, 2.0);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         {{-1.0, 0.0}, {-1.0, 4.25}, {4.0, 1.75}, {4.0, 0.0}},
         {{-1.0, 4.25}, {-1.0, 6.0}, {4.0, 6.0}, {4.0, 1.75}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
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

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-0.95999999, 4.0},
          {-0.49999999, 4.0},
          {1.999999999, 2.75},
          {2.0, 1.4},
          {-0.43999999, 1.3999999}},
         // Tile 2
         {{-0.5, 4.0}, {2.0, 4.0}, {2.0, 2.75}},
         // Tile 3
         {{-0.5, 4.0}, {2.0, 4.0}, {2.0, 2.75}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for three points with border";

      using Fp = double;

      const Rect<Fp> border{-3.0, -1.0, 4.0, 5.0};
      VoronoiTesselation<Fp> vt({{1.0, 2.0}, {2.0, 4.0}, {-2.0, 1.4}}, border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{4.0, 1.75}, {4.0, -1.0}, {0.04, -1.0}, {-1.01111111, 4.25555555}},
         // Tile 2
         {{-1.49499999, 5.0}, {4.0, 5.0}, {3.99999999, 1.75}, {-1.01111111, 4.255555555}},
         // Tile 3
         {{-1.494999999, 5.0},
          {-1.011111111, 4.25555555},
          {0.04, -1.0},
          {-3.0, -1.0},
          {-3.0, 5.0}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
}


void testForFourPoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for four points";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{-1.0, -2.0}, {0.0, 3.0}, {4.0, 1.0}, {3.0, -1.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-1.0, 0.6}, {0.5526315789, 0.2894736842}, {1.125, -2.0}, {-1.0, -2.0}},
         // Tile 2
         {{-1.0, 0.6}, {-1.0, 3.0}, {2.5, 3.0}, {1.5, 1.0}, {0.5526315789, 0.2894736842}},
         // Tile 3
         {{4.0, -0.25},
          {4.0, -2.0},
          {1.125, -2.0},
          {0.5526315789, 0.2894736842},
          {1.5, 1.0}},
         // Tile 4
         {{2.5, 3.0}, {4.0, 3.0}, {4.0, -0.25}, {1.5, 1.0}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for four points with three colinear";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{1.0, -2.0}, {1.0, 1.0}, {1.0, 4.0}, {4.0, 0.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{1.0, -0.5}, {2.166666666, -0.5}, {3.166666666, -2.0}, {1.0, -2.0}},
         // Tile 2
         {{1.0, 2.5}, {3.166666666, 2.5}, {2.166666666, -0.5}, {1.0, -0.5}},
         // Tile 3
         {{1.0, 2.5}, {1.0, 4.0}, {4.0, 4.0}, {4.0, 3.125}, {3.166666666, 2.5}},
         // Tile 4
         {{4.0, 3.125},
          {4.0, -2.0},
          {3.166666666, -2.0},
          {2.166666666, -0.5},
          {3.166666666, 2.5}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for for situation where tile with distant endpoints "
         "intersects before the endpoints";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{10.0, 10.0}, {44.0, 23.0}, {15.0, 44.0}, {22.0, 93.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{44.0, 53.523809523},
          {44.0, 10.0},
          {29.485294117, 10.0},
          {23.60999083, 25.366177818}},
         // Tile 2
         {{10.0, 69.714285714}, {10.0, 93.0}, {44.0, 93.0}, {44.0, 64.857142857}},
         // Tile 3
         {{10.0, 27.367647058},
          {23.609990834, 25.3661778185},
          {29.485294117, 10.0},
          {10.0, 10.0}},
         // Tile 4
         {{10.0, 69.7142857142},
          {44.0, 64.857142857},
          {44.0, 53.523809523},
          {23.60999083, 25.3661778185},
          {10.0, 27.367647058}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
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

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-2.0, -0.25},
          {0.07142857142, 0.7857142857},
          {0.6666666666, -0.9999999999},
          {-2.0, -1.0}},
         // Tile 2
         {{-2.0, 3.0}, {0.1, 0.9}, {0.0714285714, 0.785714285}, {-2.0, -0.25}},
         // Tile 3
         {{1.25, 3.0}, {2.0, 3.0}, {2.0, 1.0}, {0.25, 1.0}},
         // Tile 4
         {{2.0, 1.0},
          {2.0, -1.0},
          {0.66666666, -1.0},
          {0.0714285714, 0.7857142857},
          {0.1, 0.9},
          {0.25, 1.0}},
         // Tile 5
         {{-2.0, 3.0}, {1.25, 3.0}, {0.25, 1.0}, {0.1, 0.9}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
}


void testForRect()
{
   {
      const std::string caseLabel = "VoronoiTesselation for rect";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{-1.0, -2.0}, {-1.0, 3.0}, {5.0, 3.0}, {5.0, -2.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-1.0, 0.5}, {2.0, 0.5}, {2.0, -2.0}, {-1.0, -2.0}},
         // Tile 2
         {{2.0, 3.0}, {5.0, 3.0}, {5.0, 0.5}, {2.0, 0.5}},
         // Tile 3
         {{2.0, 3.0}, {2.0, 0.5}, {-1.0, 0.5}, {-1.0, 3.0}},
         // Tile 4
         {{5.0, 0.5}, {5.0, -2.0}, {2.0, -2.0}, {2.0, 0.5}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for rect with border";

      using Fp = double;

      const Rect<Fp> border{-10.0, -10.0, 10.0, 10.0};
      VoronoiTesselation<Fp> vt({{-1.0, -2.0}, {-1.0, 3.0}, {5.0, 3.0}, {5.0, -2.0}},
                                border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-10.0, 0.5}, {2.0, 0.5}, {2.0, -10.0}, {-10.0, -10.0}},
         // Tile 2
         {{2.0, 10.0}, {10.0, 10.0}, {10.0, 0.5}, {2.0, 0.5}},
         // Tile 3
         {{2.0, 10.0}, {2.0, 0.5}, {-10.0, 0.5}, {-10.0, 10.0}},
         // Tile 4
         {{10.0, 0.5}, {10.0, -10.0}, {2.0, -10.0}, {2.0, 0.5}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel = "VoronoiTesselation for rect with with center point";

      using Fp = double;

      VoronoiTesselation<Fp> vt(
         {{-1.0, -2.0}, {-1.0, 3.0}, {5.0, 3.0}, {5.0, -2.0}, {2.0, 0.5}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-1.0, 0.5}, {-0.541666666, 0.5}, {1.5416666666, -2.0}, {-1.0, -2.0}},
         // Tile 2
         {{1.5416666666, 3.0},
          {2.4583333333, 3.0},
          {4.541666666, 0.5},
          {2.458333333, -2.0},
          {1.5416666666, -2.0},
          {-0.541666666, 0.5}},
         // Tile 3
         {{2.4583333333, 3.0}, {5.0, 3.0}, {5.0, 0.5}, {4.5416666666, 0.5}},
         // Tile 4
         {{1.5416666666, 3.0}, {-0.5416666666, 0.5}, {-1.0, 0.5}, {-1.0, 3.0}},
         // Tile 5
         {{5.0, 0.5}, {5.0, -2.0}, {2.4583333333, -2.0}, {4.5416666666, 0.5}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
   {
      const std::string caseLabel =
         "VoronoiTesselation for rect with with center point and border";

      using Fp = double;

      const Rect<Fp> border{-10.0, -10.0, 10.0, 10.0};
      VoronoiTesselation<Fp> vt(
         {{-1.0, -2.0}, {-1.0, 3.0}, {5.0, 3.0}, {5.0, -2.0}, {2.0, 0.5}}, border);
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{-10.0, 0.5}, {-0.541666666, 0.5}, {2.0, -2.55}, {2.0, -10.0}, {-10.0, -10.0}},
         // Tile 2
         {{-0.5416666666, 0.5}, {2.0, 3.55}, {4.541666666, 0.5}, {2.0, -2.55}},
         // Tile 3
         {{2.0, 10.0}, {10.0, 10.0}, {10.0, 0.5}, {4.5416666666, 0.5}, {2.0, 3.55}},
         // Tile 4
         {{2.0, 10.0}, {2.0, 3.55}, {-0.5416666666, 0.5}, {-10, 0.5}, {-10.0, 10.0}},
         // Tile 5
         {{10.0, 0.5}, {10.0, -10.0}, {2.0, -10.0}, {2.0, -2.55}, {4.5416666666, 0.5}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
   }
}


void testForMorePoints()
{
   {
      const std::string caseLabel = "VoronoiTesselation for more points";

      using Fp = double;

      VoronoiTesselation<Fp> vt({{10.0, 10.0},
                                 {100.0, 100.0},
                                 {27.0, 67.0},
                                 {44.0, 23.0},
                                 {78.0, 51.0},
                                 {15.0, 44.0},
                                 {78.0, 89.0},
                                 {75.0, 56.0},
                                 {22.0, 93.0},
                                 {53.0, 40.0},
                                 {49.0, 71.0}});
      const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();

      const std::vector<std::vector<Point2<Fp>>> expected = {
         // Tile 1
         {{83.235294117, 10.0},
          {29.4852941176, 10.0},
          {23.60999083, 25.3661778185},
          {33.718475073, 39.325513196},
          {78.693251533, 15.515337423}},
         // Tile 2
         {{100.0, 67.6},
          {65.01265822, 46.607594936},
          {57.898123324, 56.390080428},
          {67.65614617, 73.30398671},
          {100.0, 70.363636363}},
         // Tile 3
         {{50.166666666, 100.0},
          {35.6114864864, 82.136824324},
          {10.0, 77.211538461},
          {10.0, 100.0}},
         // Tile 4
         {{10.0, 27.3676470588},
          {23.609990834, 25.3661778185},
          {29.485294117, 10.0},
          {10.0, 10.0}},
         // Tile 5
         {{86.25, 100.0}, {100.0, 100.0}, {100.0, 72.5}},
         // Tile 6
         {{51.086206896, 100.0},
          {86.25, 100.0},
          {100.0, 72.5},
          {100.0, 70.363636363},
          {67.656146179, 73.30398671}},
         // Tile 7
         {{10.0, 61.239130434},
          {10.0, 77.211538461},
          {35.611486486486, 82.13682432432},
          {40.69627507163, 54.17048710601},
          {34.670281995661, 48.36767895878}},
         // Tile 8
         {{100.0, 67.6},
          {100.0, 10.0},
          {83.23529411765, 10.0},
          {78.69325153374, 15.515337423312},
          {65.0126582278, 46.607594936708}},
         // Tile 9
         {{78.69325153374, 15.51533742331},
          {33.71847507331, 39.32551319648},
          {34.67028199566, 48.36767895878},
          {40.69627507163, 54.17048710601},
          {57.89812332439, 56.39008042895},
          {65.01265822784, 46.60759493670}},
         // Tile 10
         {{50.1666666666, 100.0},
          {51.0862068965, 100.0},
          {67.6561461794, 73.3039867109},
          {57.8981233243, 56.3900804289},
          {40.6962750716, 54.1704871060},
          {35.6114864864, 82.1368243243}},
         // Tile 11
         {{10.0, 61.2391304347},
          {34.67028199566, 48.36767895878},
          {33.71847507331, 39.32551319648},
          {23.60999083409, 25.36617781851},
          {10.0, 27.367647058823}}};
      VERIFY(hasTiles(tiles, expected), caseLabel);
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
   testForMorePoints();
}
