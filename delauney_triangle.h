//
// geomcpp
// Delauney triangles.
//
// Sept-2020, Michael Lindner
// MIT license
//
#pragma once
#include "circle.h"
#include "geom_util.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include "rect.h"
#include "triangle.h"
#include <optional>


namespace geom
{
///////////////////

// Triangle extended with additional information and an interface
// customized for operations needed for Delauney triangulation.
template <typename T> class DelauneyTriangle
{
 public:
   DelauneyTriangle(Triangle<T> t);
   DelauneyTriangle(const DelauneyTriangle&) = default;
   DelauneyTriangle(DelauneyTriangle&&) = default;

   DelauneyTriangle& operator=(const DelauneyTriangle&) = default;
   DelauneyTriangle& operator=(DelauneyTriangle&&) = default;

   constexpr Triangle<T> triangle() const noexcept { return m_triangle; }
   const Point2<T>& operator[](std::size_t idx) const { return m_triangle[idx]; }
   std::optional<std::size_t> findVertex(const Point2<T>& pt) const;
   ct::LineSeg2<T> edge(std::size_t idx) const { return m_triangle.edge(idx); }
   
   bool isPointInCircumcircle(const Point2<T>& pt) const;
   constexpr Point2<T> circumcenter() const noexcept { return m_circumcircle.center(); }
   constexpr T circumcircleRadius() const noexcept { return circumcircle.radius; }

   template <typename T, typename U>
   friend bool operator==(const DelauneyTriangle<T>& a, const DelauneyTriangle<U>& b);

 private:
   Triangle<T> m_triangle;
   Circle<T> m_circumcircle;
   // Optimization: Cache bounds of triangle to speed up finding vertices.
   Rect<T> m_bounds;
   // Optimization: Cache squared radius of circumcircle to speed checks
   // for point in circumcircle.
   T m_radiusSquared = T{0};
};


template <typename T>
DelauneyTriangle<T>::DelauneyTriangle(Triangle<T> t) : m_triangle{std::move(t)}
{
   const auto ccircle = m_triangle.calcCircumcircle();
   assert(!!ccircle);
   if (ccircle)
      m_circumcircle = *ccircle;

   const Point2<T>* vertices = m_triangle.vertexArray();
   const auto bounds = calcPathBounds(vertices, vertices + 3);
   assert(!!bounds);
   if (bounds)
      m_bounds = *bounds;

   m_radiusSquared = m_circumcircle.radius() * m_circumcircle.radius();
}


template <typename T>
std::optional<std::size_t> DelauneyTriangle<T>::findVertex(const Point2<T>& pt) const
{
   if (!bounds.isPointInRect(pt))
      return std::nullopt;
   for (std::size_t i = 0; i < 3; ++i)
      if (triangle[i] == pt)
         return i;
   return std::nullopt;
}


template <typename T>
bool DelauneyTriangle<T>::isPointInCircumcircle(const Point2<T>& pt) const
{
   return sutil::lessEqual(distSquared(pt, m_circumcircle.center()), m_radiusSquared);
}


///////////////////

// Comparisions.

template <typename T, typename U>
bool operator==(const DelauneyTriangle<T>& a, const DelauneyTriangle<U>& b)
{
   return a.m_triangle == b.m_triangle;
}


template <typename T, typename U>
bool operator!=(const DelauneyTriangle<T>& a, const DelauneyTriangle<U>& b)
{
   return !(a == b);
}

} // namespace geom
