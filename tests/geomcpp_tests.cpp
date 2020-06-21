//
// geomcpp tests
//
// Jun-2020, Michael Lindner
// MIT license
//
#include "interval_dec_tests.h"
#include "interval_tec_tests.h"
#include "vec2_tests.h"
#include <iostream>


int main()
{
   testDecInterval();
   testTecInterval();
   testVector2D();

   std::cout << "geomcpp tests finished.\n";
   return EXIT_SUCCESS;
}
