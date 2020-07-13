//
// geomcpp
// Triangles
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include <vector>


namespace geom
{
///////////////////


template <typename T> class Poly2
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;
   using iterator = typename std::vector<Point2<T>>::iterator;
   using const_iterator = typename std::vector<Point2<T>>::const_iterator;

   Poly2() = default;
   constexpr Poly2(Point2<T> pt);
   Poly2(Point2<T> pt);
   template <typename Iter>
   Poly2(Iter first, Iter last);
   Poly2(std::initializer_list ilist);
   Poly2(const Poly2&) = default;
   Poly2(Poly2&&) = default;
   
   Poly2& operator=(const Poly2&) = default;
   Poly2& operator=(Poly2&&) = default;

   std::size_t countVertices() const;
   Point2<T>& operator[](std::size_t idx);
   const Point2<T>& operator[](std::size_t idx) const;
   iterator contains(const Point2<T>& pt);
   const_iterator contains(const Point2<T>& pt) const;
   iterator add(Point2<T> pt);
   iterator insert(Point2<T> pt, std::size_t pos);

private:
   std::vector<Point2<T>> m_vertices;
};

} // namespace geom
