//
// geomcpp tests
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "circle_tests.h"
#include "delauney_triangle_tests.h"
#include "geom_util_tests.h"
#include "interval_dec_tests.h"
#include "interval_tec_tests.h"
#include "line_inf2_ct_tests.h"
#include "line_inf2_rt_tests.h"
#include "line_intersection2_ct_tests.h"
#include "line_intersection2_rt_tests.h"
#include "line_ray2_ct_tests.h"
#include "line_ray2_rt_tests.h"
#include "line_seg2_ct_tests.h"
#include "line_seg2_rt_tests.h"
#include "point2_tests.h"
#include "poisson_disc_sampling_tests.h"
#include "poly2_tests.h"
#include "poly_intersection2_tests.h"
#include "poly_line_cut2_tests.h"
#include "rect_tests.h"
#include "ring_tests.h"
#include "triangle_tests.h"
#include "vec2_tests.h"
#include <iostream>


int main()
{
   testCircle();
   testCtLineInf2();
   testCtLineIntersection2();
   testCtLineRay2();
   testCtLineSeg2();
   testDecInterval();
   testDelauneyTriangle();
   testGeometryUtilities();
   testPoint2D();
   testPoissonDiscSampling();
   testPoly2();
   testPolygonIntersection2();
   testPolygonLineCutting2();
   testRect();
   testRing();
   testRtLineInf2();
   testRtLineIntersection2();
   testRtLineRay2();
   testRtLineSeg2();
   testTecInterval();
   testTriangle();
   testVector2D();

   std::cout << "geomcpp tests finished.\n";
   return EXIT_SUCCESS;
}
