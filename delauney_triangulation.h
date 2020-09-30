//
// geomcpp
// Delauney triangulation.
//
// Sept-2020, Michael Lindner
// MIT license
//
#pragma once
#include "delauney_triangle.h"
#include "geom_util.h"
#include "line_seg2_ct.h"
#include "point2.h"
#include "rect.h"
#include "triangle.h"
#include "essentutils/fputil.h"
#include <algorithm>
#include <optional>
#include <set>
#include <vector>


namespace geom
{
namespace internals
{
///////////////////

// Data structure to hold individual edges of triangles.
template <typename T> class EdgeBuffer
{
 public:
   using const_iterator = typename std::vector<ct::LineSeg2<T>>::const_iterator;

 public:
   void addEdges(const DelauneyTriangle<T>& t);
   void clear() { m_edges.clear(); }
   void removeDuplicates();
   bool isDuplicateEdge(const ct::LineSeg2<T>& a, const ct::LineSeg2<T>& b) const;

   std::size_t size() const { return m_edges.size(); }
   const ct::LineSeg2<T>& operator[](std::size_t idx) const { return m_edges[idx]; }
   const_iterator cbegin() const { return m_edges.cbegin(); }
   const_iterator cend() const { return m_edges.cend(); }
   const_iterator begin() const { return cbegin(); }
   const_iterator end() const { return cend(); }

 private:
   std::vector<ct::LineSeg2<T>> m_edges;
};


template <typename T> void EdgeBuffer<T>::addEdges(const DelauneyTriangle<T>& t)
{
   for (int i = 0; i <= 2; ++i)
      m_edges.push_back(t.edge(i));
}


template <typename T> void EdgeBuffer<T>::removeDuplicates()
{
   using IterSet = std::set<const_iterator>;

   IterSet duplicates;
   for (const_iterator i = begin(); i != end(); ++i)
   {
      for (const_iterator j = i + 1; j != end(); ++j)
      {
         if (isDuplicateEdge(*i, *j))
         {
            // Remove both duplicates!
            duplicates.insert(i);
            duplicates.insert(j);
         }
      }
   }

   std::for_each(duplicates.rbegin(), duplicates.rend(),
                 [&](const_iterator iter) { m_edges.erase(iter); });
}


template <typename T>
bool EdgeBuffer<T>::isDuplicateEdge(const ct::LineSeg2<T>& a,
                                    const ct::LineSeg2<T>& b) const
{
   const auto sa = a.startPoint();
   const auto ea = a.endPoint();
   const auto sb = b.startPoint();
   const auto eb = b.endPoint();
   return (sa == sb && ea == eb) || (sa == eb && ea == sb);
}

} // namespace internals


///////////////////

// Implementation of Bowyer-Watson algorithm to perform a Delauney triangulation
// in 2D.
// Triangulates a set of points so that each resulting triangle's cirumcircle
// has an empty interior, i.e. does not contain any of the other points. This
// is called the 'Delauney condition'.
// Source:
// http://paulbourke.net/papers/triangulate/
template <typename T> class DelauneyTriangulation
{
 public:
   // Caller is responsible that sample points don't contain duplicates.
   explicit DelauneyTriangulation(std::vector<Point2<T>> samples);

   // Starts the Delauney triangulation.
   std::vector<Triangle<T>> triangulate();

   // Returns the triangulation extended with information that the algorithm
   // cached for each triangle, e.g. a triangle's circumcircle. Access is given
   // as optimization, so that callers don't need to recalculate data that the
   // algorithm already calculated.
   std::vector<DelauneyTriangle<T>> delauneyTriangles() const;

   // Checks whether a given list of triangles satisfies the Delauney condition,
   // i.e. each triangle's cirumcircle has an empty interior (does not contain
   // any of the other triangles corner points).
   static bool isDelauneyConditionSatisfied(const std::vector<Triangle<T>>& triangles);

 private:
   // Adds the edges of active triangles whose circumcircle contains a given sample
   // point to a given edge buffer and removes the triangles from the given list.
   void findEnclosingPolygonEdges(const Point2<T>& sample,
                                  internals::EdgeBuffer<T>& edges);

   // For each given edge generate a new triangle with a given sample point.
   void generateNewTriangles(const Point2<T>& sample,
                             const internals::EdgeBuffer<T>& edges);

   // Mark all remaining triangles as settles.
   void settleRemainingTriangles();

   // Removes triangles from a given list that share vertices with a given master
   // triangle.
   void removeTrianglesSharingVertices(const Triangle<T>& master);

   // Calculates a triangle that encloses the given points. The points need to
   // be completely inside the triangle, not just on its outline.
   static Triangle<T> calcBoundingTriangle(const std::vector<Point2<T>>& points);

   // Checks if a given  triangle needs to ever be considered again during the
   // triangulation. Requires that the sample points of the triangulation
   // are sorted by ascending x-coordinates.
   static bool hasTriangleSettled(const DelauneyTriangle<T>& t, const Point2<T>& pt);

   // Returns a list of plain triangles from a given list of Delauney triangles.
   static std::vector<Triangle<T>>
   prepareResult(const std::vector<DelauneyTriangle<T>>& delauneyTriangles);

   // Returns a set of all unique vertices of a given list of triangles.
   static std::set<Point2<T>> collectPoints(const std::vector<Triangle<T>>& triangles);

 private:
   // List of points that define the triangulation.
   std::vector<Point2<T>> m_samples;
   // Triangle that bounds all input points.
   Triangle<T> m_boundingTriangle;
   // Current state of the triangulation. Holds active triangles and some data
   // that they are annotated with.
   std::vector<DelauneyTriangle<T>> m_triangulation;
   // List of triangles that don't need to be considered anymore for further
   // triangulation steps.
   std::vector<DelauneyTriangle<T>> m_settledTriangles;
};


template <typename T>
DelauneyTriangulation<T>::DelauneyTriangulation(std::vector<Point2<T>> samples)
: m_samples{std::move(samples)}, m_boundingTriangle{calcBoundingTriangle(m_samples)}
{
   // Add bounding triangle vertices to the end of the vertex list.
   if (!m_boundingTriangle.isDegenerate())
   {
      m_samples.push_back(m_boundingTriangle[0]);
      m_samples.push_back(m_boundingTriangle[1]);
      m_samples.push_back(m_boundingTriangle[2]);
   }

   // Sort all collected sample points by their x-coordinate to enable
   // detecting triangles that cannot affect the triangulation anymore.
   std::sort(m_samples.begin(), m_samples.end(),
             [](const Point2<T>& a, const Point2<T>& b) {
                return sutil::lessEqual(a.x(), b.x());
             });
}


template <typename T> std::vector<Triangle<T>> DelauneyTriangulation<T>::triangulate()
{
   if (m_boundingTriangle.isDegenerate())
      return {};

   m_triangulation.push_back(m_boundingTriangle);
   internals::EdgeBuffer<T> edges;

   for (const Point2<T>& sample : m_samples)
   {
      edges.clear();
      findEnclosingPolygonEdges(sample, edges);
      edges.removeDuplicates();
      generateNewTriangles(sample, edges);
   }

   settleRemainingTriangles();
   removeTrianglesSharingVertices(m_boundingTriangle);

   return prepareResult(m_settledTriangles);
}


template <typename T>
std::vector<DelauneyTriangle<T>> DelauneyTriangulation<T>::delauneyTriangles() const
{
   return m_settledTriangles;
}


template <typename T>
bool DelauneyTriangulation<T>::isDelauneyConditionSatisfied(
   const std::vector<Triangle<T>>& triangles)
{
   std::set<Point2<T>> vertices = collectPoints(triangles);
   for (const auto& t : triangles)
   {
      const Circle<T> ccircle = t.calcCircumcircle();
      for (const auto& pt : vertices)
         if (ccircle.isPointInsideCircle(pt))
            return false;
   }

   return true;
}


template <typename T>
void DelauneyTriangulation<T>::findEnclosingPolygonEdges(const Point2<T>& sample,
                                                         internals::EdgeBuffer<T>& edges)
{
   std::size_t i = 0;
   while (i < m_triangulation.size())
   {
      const DelauneyTriangle<T>& t = m_triangulation[i];
      if (hasTriangleSettled(t, sample))
      {
         m_triangulation.erase(m_triangulation.begin() + i);
         m_settledTriangles.push_back(t);
         continue;
      }

      if (t.isPointInCircumcircle(sample))
      {
         edges.addEdges(t);
         m_triangulation.erase(m_triangulation.begin() + i);
         // Since we removed the triangle we don't need to increase
         // the loop counter.
      }
      else
      {
         ++i;
      }
   }
}


template <typename T>
void DelauneyTriangulation<T>::generateNewTriangles(const Point2<T>& sample,
                                                    const internals::EdgeBuffer<T>& edges)
{
   for (const ct::LineSeg2<T>& e : edges)
   {
      const auto startPt = e.startPoint();
      const auto endPt = e.endPoint();
      if (!startPt || !endPt)
         continue;

      const Triangle<T> t(sample, *startPt, *endPt);
      // Skip triangles that are lines or points.
      if (!t.isDegenerate())
         m_triangulation.push_back(t);
   }
}


template <typename T> void DelauneyTriangulation<T>::settleRemainingTriangles()
{
   std::move(m_triangulation.begin(), m_triangulation.end(),
             std::back_inserter(m_settledTriangles));
   m_triangulation.clear();
}


template <typename T>
void DelauneyTriangulation<T>::removeTrianglesSharingVertices(const Triangle<T>& master)
{
   std::size_t i = 0;
   while (i < m_settledTriangles.size())
   {
      const DelauneyTriangle<T>& t = m_settledTriangles[i];

      bool wasRemoved = false;
      for (std::size_t j = 0; j < 3; ++j)
      {
         if (master.hasVertex(t[j]))
         {
            m_settledTriangles.erase(m_settledTriangles.begin() + i);
            wasRemoved = true;
            break;
         }
      }

      if (!wasRemoved)
         ++i;
   }
}


template <typename T>
Triangle<T>
DelauneyTriangulation<T>::calcBoundingTriangle(const std::vector<Point2<T>>& points)
{
   const auto bounds = calcPathBounds<T>(points.begin(), points.end());
   if (!bounds || bounds->isDegenerate())
      return Triangle<T>{};

   const T dimMax = std::max(bounds->width(), bounds->height());
   const Point2<T> center = bounds->center();
   constexpr T Scale = 20;

   const Point2<T> a{center.x() - Scale * dimMax, center.y() - dimMax};
   const Point2<T> b{center.x(), center.y() + Scale * dimMax};
   const Point2<T> c{center.x() + Scale * dimMax, center.y() - dimMax};
   return Triangle{a, b, c};
}


template <typename T>
bool DelauneyTriangulation<T>::hasTriangleSettled(const DelauneyTriangle<T>& t,
                                                  const Point2<T>& pt)
{
   return sutil::greater(pt.x() - t.circumcenter().x(), t.circumcircleRadius());
}


template <typename T>
std::vector<Triangle<T>> DelauneyTriangulation<T>::prepareResult(
   const std::vector<DelauneyTriangle<T>>& delauneyTriangles)
{
   std::vector<Triangle<T>> result(delauneyTriangles.size());
   std::transform(delauneyTriangles.begin(), delauneyTriangles.end(), result.begin(),
                  [](const auto& dt) { return dt.triangle(); });
   return result;
}


template <typename T>
std::set<Point2<T>>
DelauneyTriangulation<T>::collectPoints(const std::vector<Triangle<T>>& triangles)
{
   std::set<Point2<T>> vertices;
   for (const auto& t : triangles)
      std::copy(t.begin(), t.end(), std::inserter(vertices));
   return vertices;
}

} // namespace geom
