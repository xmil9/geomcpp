//
// geomcpp tests
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "circle_tests.h"
#include "interval_dec_tests.h"
#include "interval_tec_tests.h"
#include "line_inf2_ct_tests.h"
#include "line_intersection2_rt_tests.h"
#include "line_ray2_rt_tests.h"
#include "line_seg2_ct_tests.h"
#include "line_seg2_rt_tests.h"
#include "line2_rt_tests.h"
#include "point2_tests.h"
#include "rect_tests.h"
#include "vec2_tests.h"
#include <iostream>


int main()
{
   testCircle();
   testDecInterval();
   testPoint2D();
   testTecInterval();
   testRect();
   testCtLineInf2();
   testCtLineSeg2();
   testRtLine2();
   testRtLineIntersection2();
   testRtLineRay2();
   testRtLineSeg2();
   testVector2D();

   std::cout << "geomcpp tests finished.\n";
   return EXIT_SUCCESS;
}
