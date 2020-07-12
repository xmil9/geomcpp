//
// geomcpp
// Triangles
//
// Jul-2020, Michael Lindner
// MIT license
//
#include "line_seg2_ct.h"
#include "point2.h"
#include <array>


namespace geom
{
///////////////////

template <typename T>
class Triangle
{
public:
   using value_type = T;
   using Fp = sutil::FpType<T>;
   
   Triangle();
   constexpr Triangle(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c);
   template <typename U>
   constexpr Triangle(const Point2<U>& a, const Point2<U>& b, const Point2<U>& c);

   const Point2<T>& vertex(std::size_t idx) const;
   const Point2<T>& operator[](std::size_t idx) const;
   LineSeg2<T> edge(std::size_t idx) const;

   // Checks if a given point is a vertex of the triangle.
   template <typename U>
   bool hasVertex(const Point2<U>& pt) const;
   const Point2<T>* vertexArray() const;
   // Checks if the triangle degenerates into a point.
   bool isPoint() const;
   bool isLine() const;
   bool isDegenerate() const;

   Fp area() const;
   //Circle calcCircumcircle() const;
   Point2<T> calcCircumcenter() const;

private:
   std::array<Point2<T>, 3> m_vertices;
};

} // namespace geom
