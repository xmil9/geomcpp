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
#include "vec2.h"
#include <array>
#include <optional>


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
   std::optional<Circle<T>> calcCircumcircle() const;
   // Calculates the circumcenter of the triangle. The circumcenter is the center
   // of the triangle's circumcircle.
   std::optional<Point2<T>> calcCircumcenter() const;

 private:
   // Vertices are arranged in ccw order.
   std::array<Point2<T>, 3> m_vertices;
};


template <typename T>
constexpr Triangle<T>::Triangle(const Point2<T>& a, const Point2<T>& b,
                                const Point2<T>& c)
{
   const bool isCcw = ccw(Vec2<T>{a, b}, Vec2<T>{b, c});
   m_vertices[0] = a;
   m_vertices[1] = isCcw ? b : c;
   m_vertices[2] = isCcw ? c : b;
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
   return ct::LineSeg2<T>(m_vertices[idx], m_vertices[(idx + 1) % 3]);
}


template <typename T> bool Triangle<T>::isPoint() const
{
   return m_vertices[0] == m_vertices[1] && m_vertices[0] == m_vertices[2];
}


template <typename T> bool Triangle<T>::isLine() const
{
   if (isPoint())
      return false;
   const ct::LineSeg2<T> side01(m_vertices[0], m_vertices[1]);
   return m_vertices[0] == m_vertices[1] || side01.isPointOnInfiniteLine(m_vertices[2]);
}


template <typename T> bool Triangle<T>::isDegenerate() const
{
   return isPoint() || isLine();
}


template <typename T> typename Triangle<T>::Fp Triangle<T>::area() const
{
   if (isDegenerate())
      return 0.0;
   // The perp dot product gives the area of the parallelogram spanned by two
   // vectors. Half of that is the area of the triangle that is formed by the
   // two vectors and a line connecting their endpoints.
   const Vec2<T> v(m_vertices[0], m_vertices[1]);
   const Vec2<T> w(m_vertices[0], m_vertices[2]);
   return std::abs(perpDot(v, w)) / 2.0;
}


template <typename T> std::optional<Circle<T>> Triangle<T>::calcCircumcircle() const
{
   if (isPoint())
      return std::make_optional<Circle<T>>(m_vertices[0], T(0));
   if (isLine())
      return std::nullopt;

   const auto center = calcCircumcenter();
   if (!center)
      return std::nullopt;

   const Fp radius = Vec2<T>(*center, m_vertices[0]).length();
   const Circle<T> ccircle{*center, static_cast<T>(radius)};
   return ccircle;
}


template <typename T> std::optional<Point2<T>> Triangle<T>::calcCircumcenter() const
{
   // Source:
   // https://www.geeksforgeeks.org/program-find-circumcenter-triangle-2
   // The circumcenter of the triangle is point where all the perpendicular
   // bisectors of the sides of the triangle intersect.
   // It is enough to intersect two of the three bisectors.

   const ct::LineSeg2<T> side01(m_vertices[0], m_vertices[1]);
   const ct::LineInf2<T> bisector01(side01.midPoint(), side01.direction().ccwNormal());
   const ct::LineSeg2<T> side12(m_vertices[1], m_vertices[2]);
   const ct::LineInf2<T> bisector12(side12.midPoint(), side12.direction().ccwNormal());

   const auto x = ct::intersect(bisector01, bisector12);
   if (x && std::holds_alternative<Point2<T>>(*x))
         return std::get<Point2<T>>(*x);
   return std::nullopt;
}

} // namespace geom
