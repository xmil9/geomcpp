//
// geomcpp
// Voronoi tesselation algorithm.
//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "delauney_triangle.h"
#include "delauney_triangulation.h"
#include "geom_util.h"
#include "line_intersection2_ct.h"
#include "line_inf2_ct.h"
#include "line_ray2_ct.h"
#include "line_seg2_ct.h"
#include "poly_intersection2.h"
#include "poly_line_cut2.h"
#include "poly2.h"
#include "rect.h"
#include "vec2.h"
#include "voronoi_tile.h"
#include <algorithm>
#include <optional>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>


namespace geom
{
namespace internals
{
///////////////////

// Creates a polygon from a given rectangle.
// Overload for a collection of edges is further down.
template <typename T> Poly2<T> makePolygon(const Rect<T>& r)
{
   Poly2<T> poly;
   poly.add(r.leftTop());
   poly.add(r.leftBottom());
   poly.add(r.rightBottom());
   poly.add(r.rightTop());
   return poly;
}


///////////////////

// An edge of the Voronoi tesselation can either be a line segment or a line ray.
template <typename T> using VoronoiEdge = std::variant<ct::LineSeg2<T>, ct::LineRay2<T>>;

template <typename T> Vec2<T> direction(const VoronoiEdge<T>& e)
{
   return std::visit([&](const auto& line) { return line.direction(); }, e);
}

template <typename T> bool hasEndPoint(const VoronoiEdge<T>& e)
{
   return std::visit([&](const auto& line) { return line.endPoint().has_value(); }, e);
}

// Voronoi edges (either a line segment or ray) always have a start point.
template <typename T> Point2<T> startPoint(const VoronoiEdge<T>& e)
{
   return std::visit(
      [&](const auto& line) {
         assert(line.startPoint());
         return *line.startPoint();
      },
      e);
}

template <typename T> std::optional<Point2<T>> endPoint(const VoronoiEdge<T>& e)
{
   return std::visit([&](const auto& line) { return line.endPoint(); }, e);
}


///////////////////

// An edge of the Delauney triangulation and the one or two triangles that
// it is part of.
template <typename T> class DelauneyEdge
{
 public:
   DelauneyEdge(ct::LineSeg2<T> edge, DelauneyTriangle<T> t);
   ~DelauneyEdge() = default;
   DelauneyEdge(const DelauneyEdge&) = default;
   DelauneyEdge(DelauneyEdge&&) = default;

   DelauneyEdge& operator=(const DelauneyEdge&) = default;
   DelauneyEdge& operator=(DelauneyEdge&&) = default;

   // Add a given triangle to the triangles that the edge is part of.
   void addTriangle(const DelauneyTriangle<T>& t) { m_triangles.second = t; }
   // Checks if this edge is the same as a given edge. The check is direction
   // insensitive.
   bool isEdge(const ct::LineSeg2<T>& e) const;
   // Creates a Voronoi edge for this edge.
   std::optional<VoronoiEdge<T>> makeVoronoiEdge() const;

 private:
   // Creates a Voronoi edge between two given Delauney triangles.
   static std::optional<VoronoiEdge<T>>
   makeVoronoiEdgeBetweenTriangles(const DelauneyTriangle<T>& a,
                                   const DelauneyTriangle<T>& b);
   // Creates a Voronoi edge for a given Delauney edge that only has one
   // adjacent Delauney triangle.
   static std::optional<VoronoiEdge<T>>
   makeInfiniteVoronoiEdge(const ct::LineSeg2<T>& delauneyEdge,
                           const DelauneyTriangle<T>& triangle);

 private:
   ct::LineSeg2<T> m_edge;
   std::pair<DelauneyTriangle<T>, std::optional<DelauneyTriangle<T>>> m_triangles;
};


template <typename T>
DelauneyEdge<T>::DelauneyEdge(ct::LineSeg2<T> edge, DelauneyTriangle<T> t)
: m_edge{std::move(edge)}, m_triangles{std::move(t), std::nullopt}
{
}

template <typename T> bool DelauneyEdge<T>::isEdge(const ct::LineSeg2<T>& e) const
{
   // It's ok to dereference start and end points because line segments are guaranteed
   // to have them.
   assert(m_edge.startPoint().has_value() && m_edge.endPoint().has_value());
   assert(e.startPoint().has_value() && e.endPoint().has_value());
   const Point2<T>& sa = *m_edge.startPoint();
   const Point2<T>& ea = *m_edge.endPoint();
   const Point2<T>& sb = *e.startPoint();
   const Point2<T>& eb = *e.endPoint();
   return (sa == sb && ea == eb) || (sa == eb && ea == sb);
}


template <typename T>
std::optional<VoronoiEdge<T>> DelauneyEdge<T>::makeVoronoiEdge() const
{
   if (!m_triangles.second)
      return makeInfiniteVoronoiEdge(m_edge, m_triangles.first);
   return makeVoronoiEdgeBetweenTriangles(m_triangles.first, *m_triangles.second);
}


template <typename T>
std::optional<VoronoiEdge<T>>
DelauneyEdge<T>::makeVoronoiEdgeBetweenTriangles(const DelauneyTriangle<T>& a,
                                                 const DelauneyTriangle<T>& b)
{
   const Point2<T> ca = a.circumcenter();
   const Point2<T> cb = b.circumcenter();
   if (ca != cb)
      return std::make_optional<VoronoiEdge<T>>(ct::LineSeg2<T>(ca, cb));
   // Degenerate edge.
   return std::nullopt;
}


template <typename T>
std::optional<VoronoiEdge<T>>
DelauneyEdge<T>::makeInfiniteVoronoiEdge(const ct::LineSeg2<T>& delauneyEdge,
                                         const DelauneyTriangle<T>& triangle)
{
   // The cirumcenter of the triangle is the start point of the Voronoi
   // edge and the edge travels in the direction away from the triangle.
   // Since the triangle is oriented ccw, a cw normal to any of its edges
   // points away from it.
   const VoronoiEdge<T> ve =
      ct::LineRay2<T>{triangle.circumcenter(), delauneyEdge.direction().cwNormal()};
   // Optional will always be filled here. The return type is only optional to match
   // the signature of the other 'make Voronoi edge' functions.
   return std::make_optional(ve);
}


///////////////////

// Collection of Delauney edges involved in constructing of a Voronoi tile.
template <typename T> class DelauneyEdgeCollection
{
 public:
   // Adds an edge to the collection.
   void addEdge(const ct::LineSeg2<T>& edge, const DelauneyTriangle<T>& t);
   // Generates Voronoi edges for the collection of Delauney edges.
   std::vector<VoronoiEdge<T>> makeVoronoiEdges() const;

 private:
   // Returns the index of a given edge it it is in the collection or null.
   typename std::vector<DelauneyEdge<T>>::iterator findEdge(const ct::LineSeg2<T>& edge);

 private:
   std::vector<DelauneyEdge<T>> m_edges;
};


template <typename T>
void DelauneyEdgeCollection<T>::addEdge(const ct::LineSeg2<T>& edge,
                                        const DelauneyTriangle<T>& t)
{
   auto pos = findEdge(edge);
   if (pos != m_edges.end())
      pos->addTriangle(t);
   else
      m_edges.push_back(DelauneyEdge<T>{edge, t});
}


template <typename T>
std::vector<VoronoiEdge<T>> DelauneyEdgeCollection<T>::makeVoronoiEdges() const
{
   std::vector<VoronoiEdge<T>> voronoiEdges;
   for (const auto& de : m_edges)
   {
      auto ve = de.makeVoronoiEdge();
      if (ve)
         voronoiEdges.push_back(*ve);
   }
   return voronoiEdges;
}


template <typename T>
typename std::vector<DelauneyEdge<T>>::iterator
DelauneyEdgeCollection<T>::findEdge(const ct::LineSeg2<T>& edge)
{
   return std::find_if(m_edges.begin(), m_edges.end(),
                       [&](const DelauneyEdge<T>& de) { return de.isEdge(edge); });
}


///////////////////

// Builds a polygon from an unordered list of edges. Clips polygon at given bounds.
template <typename T> class PolygonBuilder
{
 public:
   PolygonBuilder(std::vector<VoronoiEdge<T>> edges, const Rect<T>& clipBounds);
   ~PolygonBuilder() = default;
   PolygonBuilder(const PolygonBuilder&) = default;
   PolygonBuilder(PolygonBuilder&&) = default;

   PolygonBuilder& operator=(const PolygonBuilder&) = default;
   PolygonBuilder& operator=(PolygonBuilder&&) = default;

   // Builds the polygon.
   Poly2<T> build();

 private:
   // Returns an order list of vertices that results from connecting the
   // available edges.
   std::vector<Point2<T>> createVertexSequence();
   // Returns those edges from a given list that do not connect to an other
   // edge at one of their endpoints.
   std::vector<VoronoiEdge<T>> findEndEdges();
   // Orders the available edges. Uses given edges at start and end.
   std::vector<Point2<T>> orderEdges(const std::vector<VoronoiEdge<T>>& endEdges);
   // Returns a point far past the end point of a given line.
   static Point2<T> calcDistantPoint(const VoronoiEdge<T>& edge);
   // Finds the edge that connects to a given previous edge.
   std::optional<VoronoiEdge<T>> findNextEdge(const std::optional<Point2<T>>& connector);
   // Checks if a given point exists as endpoint of an edge excluding
   // a specific edge at a given index. Use -1 to include all edges.
   int findEndpoint(const std::optional<Point2<T>>& pt, int excludedIdx) const;
   // Make sures end edges do not intersect before they end at their distant
   // end points. The polygon would not be convex in that case, creating
   // problems when intersecting it with the border.
   static void fixIntersectingEndEdges(std::vector<Point2<T>> vertices);

 private:
   std::vector<VoronoiEdge<T>> m_edges;
   Poly2<T> m_clip;
};


template <typename T>
PolygonBuilder<T>::PolygonBuilder(std::vector<VoronoiEdge<T>> edges,
                                  const Rect<T>& clipBounds)
: m_edges{std::move(edges)}, m_clip{makePolygon(clipBounds)}
{
}


template <typename T> Poly2<T> PolygonBuilder<T>::build()
{
   const std::vector<Point2<T>> vertexSeq{createVertexSequence()};
   const Poly2<T> unclipped{vertexSeq.cbegin(), vertexSeq.cend()};
   return intersectConvexPolygons(unclipped, m_clip);
}

template <typename T> std::vector<Point2<T>> PolygonBuilder<T>::createVertexSequence()
{
   if (m_edges.empty())
      return {};

   std::vector<VoronoiEdge<T>> endEdges = findEndEdges();
   std::vector<Point2<T>> vertices = orderEdges(endEdges);

   return vertices;
}


template <typename T> std::vector<VoronoiEdge<T>> PolygonBuilder<T>::findEndEdges()
{
   std::vector<VoronoiEdge<T>> result;

   // Keep track of found edges so that we can delete them later.
   using EdgePos = typename std::vector<VoronoiEdge<T>>::const_iterator;
   std::vector<EdgePos> foundEdges;

   const auto endPos = m_edges.end();
   for (auto pos = m_edges.begin(); pos != endPos; ++pos)
   {
      const VoronoiEdge<T>& e = *pos;
      if (!hasEndPoint(e))
      {
         result.push_back(e);
         foundEdges.push_back(pos);
      }

      // We can stop after we found two.
      if (foundEdges.size() > 1)
         break;
   }

   // Remove the found end edges from the available edges.
   for (std::size_t i = foundEdges.size() - 1; i >= 0; --i)
      m_edges.erase(foundEdges[i]);

   return result;
}


template <typename T>
std::vector<Point2<T>>
PolygonBuilder<T>::orderEdges(const std::vector<VoronoiEdge<T>>& endEdges)
{
   // There should be either zero or two end edges.
   const bool isOpenPath = endEdges.size() == 2;

   std::vector<Point2<T>> vertices;

   std::optional<VoronoiEdge<T>> nextEdge;
   if (isOpenPath)
   {
      // Process the given start edge first.
      VoronoiEdge<T> startEdge = endEdges[0];
      vertices.push_back(calcDistantPoint(startEdge));
      nextEdge = findNextEdge(startPoint(startEdge));
   }
   else
   {
      // We can start with any edges. Use the first one.
      nextEdge = m_edges[0];
      m_edges.erase(m_edges.begin());
   }

   // Concatenate the edges and store each start point.
   while (nextEdge)
   {
      vertices.push_back(startPoint(*nextEdge));
      nextEdge = findNextEdge(endPoint(*nextEdge));
   }

   // Append the end points of the given end edge.
   if (isOpenPath)
   {
      vertices.push_back(startPoint(endEdges[1]));
      vertices.push_back(calcDistantPoint(endEdges[1]));

      fixIntersectingEndEdges(vertices);
   }

   return vertices;
}


template <typename T>
Point2<T> PolygonBuilder<T>::calcDistantPoint(const VoronoiEdge<T>& edge)
{
   static constexpr T FarDist = 100000;
   const Vec2<T> normedDir = direction(edge).normalize();
   const Vec2<T> far = normedDir.scale(FarDist);
   return startPoint(edge).offset(far.x(), far.y());
}


template <typename T>
std::optional<VoronoiEdge<T>>
PolygonBuilder<T>::findNextEdge(const std::optional<Point2<T>>& connector)
{
   if (!connector)
      return std::nullopt;

   const int edgeIdx = findEndpoint(connector, -1);
   if (edgeIdx == -1)
      return std::nullopt;

   const VoronoiEdge<T> e = m_edges[edgeIdx];
   m_edges.erase(m_edges.begin() + edgeIdx);

   // Make sure the end points of the edge are in the correct order.
   if (*connector == startPoint(e))
      return e;
   // Flip the found edge.
   return ct::LineSeg2<T>(*endPoint(e), startPoint(e));
}


template <typename T>
int PolygonBuilder<T>::findEndpoint(const std::optional<Point2<T>>& pt,
                                    int excludedIdx) const
{
   for (int i = 0; i < m_edges.size(); ++i)
   {
      if (i != excludedIdx &&
          (startPoint(m_edges[i]) == pt || endPoint(m_edges[i]) == pt))
      {
         return i;
      }
   }
   return -1;
}


template <typename T>
void PolygonBuilder<T>::fixIntersectingEndEdges(std::vector<Point2<T>> vertices)
{
   const auto x = intersect(
      ct::LineSeg2{vertices[1], vertices[0]},
      ct::LineSeg2{vertices[vertices.size() - 2], vertices[vertices.size() - 1]});
   if (x && std::holds_alternative<Point2<T>>(*x))
   {
      const Point2<T> xPt = std::get<Point2<T>>(*x);

      // It's ok if the intersection is at the start points.
      if (xPt != vertices[1] && xPt != vertices[vertices.size() - 2])
      {
         vertices[0] == xPt;
         vertices[vertices.size() - 1] = xPt;
      }
   }
}


///////////////////

// Creates a polygon from given unordered edges.
// Overload for a simple rectangle is further up.
template <typename T>
Poly2<T> makePolygon(const std::vector<VoronoiEdge<T>>& edges,
                     const Rect<T>& borderBounds)
{
   PolygonBuilder<T> builder{edges, borderBounds};
   return builder.build();
}


// Checks if a given point and polygon are on the same side of a given line.
template <typename T>
bool areOnSameSideOf(const Point2<T>& pt, const Poly2<T>& poly,
                     const ct::LineInf2<T>& line)
{
   const Vec2<T> lineDir = line.direction();
   const bool isLeft = ccw(lineDir, Vec2<T>{line.anchor(), pt});

   for (const auto& polyPt : poly)
   {
      const bool isPolyPtOnLine = line.isPointOnLine(polyPt).has_value();
      const bool isPolyPtLeft = ccw(lineDir, Vec2<T>{line.anchor(), polyPt});
      if (!isPolyPtOnLine && isPolyPtLeft != isLeft)
         return false;
   }

   return true;
}

} // namespace internals


///////////////////

template <typename T> class VoronoiTesselation
{
 public:
   explicit VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples);
   VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples, T borderOffset);
   VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples, const Rect<T>& border);
   ~VoronoiTesselation() = default;
   VoronoiTesselation(const VoronoiTesselation&) = default;
   VoronoiTesselation(VoronoiTesselation&&) = default;

   VoronoiTesselation& operator=(const VoronoiTesselation&) = default;
   VoronoiTesselation& operator=(VoronoiTesselation&&) = default;

   // Starts the Voronoi tesselation.
   std::vector<VoronoiTile<T>> tesselate();
   // Returns the Delauney triangulation that was used to perform the tesselation.
   const std::vector<Triangle<T>>& getTriangulation() const { return m_triangulation; }

 private:
   using EdgeMap = std::unordered_map<Point2<T>, internals::DelauneyEdgeCollection<T>,
                                      std::hash<Point2<T>>, pointLess<T>>;

   // Degenerate tesselation into a single tile.
   std::vector<VoronoiTile<T>> tesselateIntoSingleTile();
   // Degenerate tesselation into two tiles.
   std::vector<VoronoiTile<T>> tesselateIntoTwoTiles();

   // Calculates bounding box at a given offset around a given list of points.
   static Rect<T> calcBorder(const std::vector<Point2<T>>& points, T offset);
   // Performs a Delauney triangulation for the configured sample points.
   std::vector<DelauneyTriangle<T>> delauneyTriangulation();
   // Collects all edges of Delauney triangles that share a given sample
   // point.
   EdgeMap
   collectDelauneyEdges(const std::vector<DelauneyTriangle<T>>& delauneyTriangles) const;

 private:
   // List of points to generate the Voronoi tesselation for.
   std::vector<Point2<T>> m_samples;
   // Border around the sample points. Used to terminate Voronoi edges that
   // would extend to infinity.
   Rect<T> m_border;
   // List of tiles generated by the the tesselation.
   std::vector<VoronoiTile<T>> m_tiles;
   // Triangles of the Delauney triangulation. A by-product of the tesselation
   // that can be useful, e.g. for debugging.
   std::vector<Triangle<T>> m_triangulation;
};


template <typename T>
VoronoiTesselation<T>::VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples)
: VoronoiTesselation{uniqueSamples, T(0)}
{
}


template <typename T>
VoronoiTesselation<T>::VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples,
                                          T borderOffset)
: VoronoiTesselation{uniqueSamples, calcBorder(uniqueSamples, borderOffset)}
{
}


template <typename T>
VoronoiTesselation<T>::VoronoiTesselation(const std::vector<Point2<T>>& uniqueSamples,
                                          const Rect<T>& border)
: m_samples{uniqueSamples}, m_border{border}
{
}


template <typename T> std::vector<VoronoiTile<T>> VoronoiTesselation<T>::tesselate()
{
   // Handle some degenerate cases.
   if (m_samples.size() == 0)
      return m_tiles;
   if (m_samples.size() == 1)
      return tesselateIntoSingleTile();
   if (m_samples.size() == 2)
      return tesselateIntoTwoTiles();

   // General case for more than three sample points.
   // - Each sample point is the seed of a Voronoi tile.
   // - Perform Delauney triangulation.
   // - For each sample point collect all Delauney edges that share it.
   // - From each Delauney edge build the corresponding Voronoi edge as
   //   follows:
   // -   If two Delauney triangles share the edge, then the Voronoi edge
   //     is the line connecting the triangles' circumcenters.
   // -   If only one Delauney triangle contains the edge, then the Voronoi
   //     edge is clipped by the given border.
   // - Combine the collected Voronoi edges into a polygon that forms the
   //   outline of the Voronoi tile for the processed sample point.

   // Run triangulation.
   std::vector<DelauneyTriangle<T>> delauneyTriangles = delauneyTriangulation();

   // Associates each vertex of all Delauney triangles with the edges
   // that connect to it.
   EdgeMap edgeMap = collectDelauneyEdges(delauneyTriangles);

   // Calculate the Voronoi tile for each sample point. (The Delauney
   // vertices might actually be slightly off the original sample points
   // because of floating point calculation inaccuracies).
   for (auto& [sample, delauneyEdges] : edgeMap)
   {
      const std::vector<internals::VoronoiEdge<T>> voronoiEdges =
         delauneyEdges.makeVoronoiEdges();

      const Poly2<T> voronoiPoly = internals::makePolygon(voronoiEdges, m_border);
      if (voronoiPoly.size() > 0)
         m_tiles.emplace_back(sample, voronoiPoly);
   }

   return m_tiles;
}


template <typename T>
std::vector<VoronoiTile<T>> VoronoiTesselation<T>::tesselateIntoSingleTile()
{
   assert(m_samples.size() == 1);
   const Point2<T>& sample = m_samples[0];

   Poly2<T> outline;
   if (m_border.isDegenerate())
   {
      // Tile degenerates into single point.
      outline.add(sample);
   }
   else
   {
      // Tile covers the entire area.
      outline = internals::makePolygon(m_border);
   }

   m_tiles.emplace_back(sample, outline);
   return m_tiles;
}


template <typename T>
std::vector<VoronoiTile<T>> VoronoiTesselation<T>::tesselateIntoTwoTiles()
{
   assert(m_samples.size() == 2);
   const Point2<T>& pa = m_samples[0];
   const Point2<T>& pb = m_samples[1];

   // Split area into two tiles along the bisection of
   // the edge between the two sample points.
   const ct::LineSeg2<T> sampleEdge{pa, pb};
   const Vec2<T> normal = sampleEdge.direction().ccwNormal();
   const ct::LineInf2<T> bisection{sampleEdge.midPoint(), normal};

   const std::vector<Poly2<T>> tilePolys =
      cutConvexPolygon(internals::makePolygon(m_border), bisection);
   if (tilePolys.size() == 2)
   {
      // Figure out which polygon belongs to which sample point.
      const bool isFirstPolyForA =
         internals::areOnSameSideOf(pa, tilePolys[0], bisection);
      m_tiles.emplace_back(pa, tilePolys[isFirstPolyForA ? 0 : 1]);
      m_tiles.emplace_back(pb, tilePolys[isFirstPolyForA ? 1 : 0]);
   }
   else
   {
      // Unexpected case. Abandon tesselation.
      m_tiles.clear();
   }

   return m_tiles;
}


template <typename T>
Rect<T> VoronoiTesselation<T>::calcBorder(const std::vector<Point2<T>>& points, T offset)
{
   auto border = calcPathBounds<T>(points.begin(), points.end());
   if (!border)
      return {};

   border->inflate(offset);
   return *border;
}


template <typename T>
std::vector<DelauneyTriangle<T>> VoronoiTesselation<T>::delauneyTriangulation()
{
   DelauneyTriangulation delauney{m_samples};
   m_triangulation = delauney.triangulate();
   return delauney.delauneyTriangles();
}


template <typename T>
typename VoronoiTesselation<T>::EdgeMap VoronoiTesselation<T>::collectDelauneyEdges(
   const std::vector<DelauneyTriangle<T>>& delauneyTriangles) const
{
   EdgeMap edgeMap;

   for (const auto& dt : delauneyTriangles)
   {
      for (std::size_t i = 0; i < 3; ++i)
      {
         const Point2<T> v = dt[i];
         auto pos = edgeMap.find(v);
         if (pos == edgeMap.end())
         {
            bool inserted = false;
            std::tie(pos, inserted) = edgeMap.insert(
               EdgeMap::value_type(v, internals::DelauneyEdgeCollection<T>{}));
            assert(inserted);
         }

         // Keep edges ordered.
         internals::DelauneyEdgeCollection<T>& edges = pos->second;
         edges.addEdge(ct::LineSeg2<T>{dt[i == 0 ? 2 : i - 1], v}, dt);
         edges.addEdge(ct::LineSeg2<T>{v, dt[i == 2 ? 0 : i + 1]}, dt);
      }
   }

   return edgeMap;
}

} // namespace geom
