//
// geomcpp
// Triangles
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "circle.h"
#include "line_inf2_ct.h"
#include "line_intersection2_ct.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include <array>


namespace geom
{
///////////////////

template <typename T> class Triangle
{
 public:
   using value_type = T;
   using Fp = sutil::FpType<T>;

   Triangle() = default;
   constexpr Triangle(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c);
   template <typename U>
   constexpr Triangle(const Point2<U>& a, const Point2<U>& b, const Point2<U>& c);

   const Point2<T>& operator[](std::size_t idx) const;
   // Checks if a given point is a vertex of the triangle.
   template <typename U> bool hasVertex(const Point2<U>& pt) const;
   const Point2<T>* vertexArray() const;
   ct::LineSeg2<T> edge(std::size_t idx) const;

   // Checks if the triangle degenerates into a point.
   bool isPoint() const;
   bool isLine() const;
   bool isDegenerate() const;

   Fp area() const;
   // Calculates the circumcircle of the triangle. The circumcircle is the circle
   // that goes through all three points of the triangle.
   Circle<T> calcCircumcircle() const;
   // Calculates the circumcenter of the triangle. The circumcenter is the center
   // of the triangle's circumcircle.
   Point2<T> calcCircumcenter() const;

 private:
   std::array<Point2<T>, 3> m_vertices;
};


template <typename T>
constexpr Triangle<T>::Triangle(const Point2<T>& a, const Point2<T>& b,
                                const Point2<T>& c)
: m_vertices{a, b, c}
{
}


template <typename T>
template <typename U>
constexpr Triangle<T>::Triangle(const Point2<U>& a, const Point2<U>& b,
                                const Point2<U>& c)
: m_vertices{a, b, c}
{
}


template <typename T> const Point2<T>& Triangle<T>::operator[](std::size_t idx) const
{
   return m_vertices[idx];
}


template <typename T>
template <typename U>
bool Triangle<T>::hasVertex(const Point2<U>& pt) const
{
   return std::find(m_vertices.begin(), m_vertices.end(), pt) != m_vertices.end();
}


template <typename T> const Point2<T>* Triangle<T>::vertexArray() const
{
   return m_vertices.data();
}


template <typename T> ct::LineSeg2<T> Triangle<T>::edge(std::size_t idx) const
{
   return ct::LineSeg2<T>(m_vertices[idx], m_vertices[idx % 3]);
}


template <typename T> bool Triangle<T>::isPoint() const
{
   return m_vertices[0] == m_vertices[1] && m_vertices[0] == m_vertices[2];
}


template <typename T> bool Triangle<T>::isLine() const
{
   if (isPoint())
      return false;
   const LineSeg2<T> side01(m_vertices[0], m_vertices[1]);
   return m_vertices[0] == m_vertices[1] || side01.isPointOnInfiniteLine(m_vertices[2]);
}


template <typename T> bool Triangle<T>::isDegenerate() const
{
   return isPoint() || isLine();
}


template <typename T> Triangle<T>::Fp Triangle<T>::area() const
{
   if (isDegenerate())
      return 0.0;
   // The perp dot product gives the area of the parallelogram spanned by two
   // vectors. Half of that is the area of the triangle that is formed by the
   // two vectors and a line connecting their endpoints.
   const Vec2<T> v(m_vertices[0], m_vertices[1]);
   const Vec2<T> w(m_vertices[0], m_vertices[2]);
   return std::abs(v.perpDot(w)) / 2.0;
}


template <typename T> Circle<T> Triangle<T>::calcCircumcircle() const
{
   if (isPoint())
      return new Circle2<T>(m_vertices[0], 0.0);
   if (isLine())
      return null;

   const Point2<T> center = calcCircumcenter();
   const Fp radius = Vec2<T>(center, m_vertices[0]).length();
   return Circle2<T>(center, radius);
}


template <typename T> Point2<T> Triangle<T>::calcCircumcenter() const
{
   // Source:
   // https://www.geeksforgeeks.org/program-find-circumcenter-triangle-2
   // The circumcenter of the triangle is point where all the perpendicular
   // bisectors of the sides of the triangle intersect.
   // It is enough to intersect two of the three bisectors.

   const LineSeg2<T> side01(m_vertices[0], m_vertices[1]);
   const LineInf<T> bisector01(side01.midPoint(), side01.direction().ccwNormal());
   const LineSeg2<T> side12(m_vertices[1], m_vertices[2]);
   const LineInf<T> bisector12(side12.midPoint(), side12.direction().ccwNormal());

   const auto x = ct::intersect(bisector01, bisector12);
   if (auto xPt = std::get_if<Point2<T>>(isect))
      return *xPt;

   throw std::runtime_error("Triangle circumcircle - Failed to calculate circumcenter.");
}

} // namespace geom
