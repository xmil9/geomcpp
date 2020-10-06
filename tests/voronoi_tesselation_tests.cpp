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
#include "essentutils/fputil.h"

using namespace geom;
using namespace sutil;


namespace
{
///////////////////

void testNoPoint()
{
   const std::string caseLabel = "VoronoiTesselation for no points";

   using Fp = float;

   VoronoiTesselation<Fp> vt({});
	const std::vector<VoronoiTile<Fp>> tiles = vt.tesselate();
	VERIFY(tiles.empty(), caseLabel);
}

} // namespace


void testVoronoiTesselation()
{
   testNoPoint();
}
