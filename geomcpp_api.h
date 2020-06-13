//
// geomcpp
// Import/export directives.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once

// Projects that use the DLL target build of essentuils have
// to define the GEOMCPP_DLL macro to make GEOMCPP_API
// resolve to '__declspec(dllimport)'.
#ifdef GEOMCPP_DLL
#  ifdef EXPORT_API
#    ifdef _MSC_VER
#      define GEOMCPP_API __declspec(dllexport)
#    endif
#  else
#    ifdef _MSC_VER
#      define GEOMCPP_API __declspec(dllimport)
#    endif
#  endif
#endif

// If not defined yet, define is as empty/nothing.
#ifndef GEOMCPP_API
#  define GEOMCPP_API
#endif
