//
// geomcpp
// 2D rectangle.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include "point2.h"
#include "essentutils/fputil.h"
#include <algorithm>
#include <type_traits>


namespace geom
{
///////////////////

// Rectangle.
// Always normalized: left <= right, top <= bottom.
template <typename T> class Rect
{
 public:
   using value_type = T;

   Rect() = default;
   constexpr Rect(T l, T t, T r, T b);
   template <typename U>
   constexpr Rect(const Point2<U>& leftTop, const Point2<U>& rightBot);
   Rect(const Rect&) = default;
   Rect(Rect&&) = default;

   Rect& operator=(const Rect&) = default;
   Rect& operator=(Rect&&) = default;

   T left() const noexcept { return m_l; }
   T top() const noexcept { return m_t; }
   T right() const noexcept { return m_r; }
   T bottom() const noexcept { return m_b; }
   void setLeft(T l);
   void setTop(T t);
   void setRight(T r);
   void setBottom(T b);

   bool isDegenerate() const;
   T width() const;
   T height() const;
   Point2<T> leftTop() const;
   Point2<T> rightTop() const;
   Point2<T> leftBottom() const;
   Point2<T> rightBottom() const;
   Point2<T> center() const;

   template <typename U> bool isPointInRect(const Point2<U>& pt) const;
   template <typename U> void inflate(U by);

 private:
   void normalize();

 private:
   T m_l = T(0);
   T m_t = T(0);
   T m_r = T(0);
   T m_b = T(0);
};


template <typename T>
constexpr Rect<T>::Rect(T l, T t, T r, T b)
: m_l{l <= r ? l : r}, m_t{t <= b ? t : b}, m_r{r > l ? r : l}, m_b{b > t ? b : t}
{
}


template <typename T>
template <typename U>
constexpr Rect<T>::Rect(const Point2<U>& leftTop, const Point2<U>& rightBot)
: Rect{static_cast<T>(leftTop.x()), static_cast<T>(leftTop.y()),
       static_cast<T>(rightBot.x()), static_cast<T>(rightBot.y())}
{
}


template <typename T> void Rect<T>::setLeft(T l)
{
   m_l = l;
   normalize();
}


template <typename T> void Rect<T>::setTop(T t)
{
   m_t = t;
   normalize();
}


template <typename T> void Rect<T>::setRight(T r)
{
   m_r = r;
   normalize();
}


template <typename T> void Rect<T>::setBottom(T b)
{
   m_b = b;
   normalize();
}


template <typename T> bool Rect<T>::isDegenerate() const
{
   return sutil::equal(left(), right()) || sutil::equal(top(), bottom());
}


template <typename T> T Rect<T>::width() const
{
   return right() - left();
}


template <typename T> T Rect<T>::height() const
{
   return bottom() - top();
}


template <typename T> Point2<T> Rect<T>::leftTop() const
{
   return Point2(left(), top());
}


template <typename T> Point2<T> Rect<T>::rightTop() const
{
   return Point2(right(), top());
}


template <typename T> Point2<T> Rect<T>::leftBottom() const
{
   return Point2(left(), bottom());
}


template <typename T> Point2<T> Rect<T>::rightBottom() const
{
   return Point2(right(), bottom());
}


template <typename T> Point2<T> Rect<T>::center() const
{
   return Point2((left() + right()) / T(2.0), (top() + bottom()) / T(2.0));
}


template <typename T>
template <typename U>
bool Rect<T>::isPointInRect(const Point2<U>& pt) const
{
   return sutil::greaterEqual(static_cast<T>(pt.x()), left()) &&
          sutil::lessEqual(static_cast<T>(pt.x()), right()) &&
          sutil::greaterEqual(static_cast<T>(pt.y()), top()) &&
          sutil::lessEqual(static_cast<T>(pt.y()), bottom());
}


template <typename T> template <typename U> void Rect<T>::inflate(U by)
{
   m_l -= by;
   m_r += by;
   m_t -= by;
   m_b += by;
   normalize();
}


template <typename T> void Rect<T>::normalize()
{
   using std::swap;
   if (left() > right())
      swap(m_l, m_r);
   if (top() > bottom())
      swap(m_t, m_b);
}


///////////////////

// Comparisions.

template <typename T, typename U> bool operator==(const Rect<T>& a, const Rect<U>& b)
{
   return sutil::equal(a.left(), b.left()) && sutil::equal(a.top(), b.top()) &&
          sutil::equal(a.right(), b.right()) && sutil::equal(a.bottom(), b.bottom());
}


template <typename T, typename U> bool operator!=(const Rect<T>& a, const Rect<U>& b)
{
   return !(a == b);
}


///////////////////

// Rectangle intersection.
template <typename T, typename U>
Rect<std::common_type_t<T, U>> intersect(const Rect<T>& a, const Rect<U>& b)
{
   using R = std::common_type_t<T, U>;

   if (sutil::greater(a.left(), b.right()) || sutil::greater(b.left(), a.right()) ||
       sutil::greater(a.top(), b.bottom()) || sutil::greater(b.top(), a.bottom()))
   {
      return Rect<R>{};
   }
   return Rect<R>(std::max(a.left(), b.left()), std::max(a.top(), b.top()),
                  std::min(a.right(), b.right()), std::min(a.bottom(), b.bottom()));
}


// Rectangle union.
template <typename T, typename U>
Rect<std::common_type_t<T, U>> unite(const Rect<T>& a, const Rect<U>& b)
{
   using R = std::common_type_t<T, U>;
   return Rect<R>(std::min(a.left(), b.left()), std::min(a.top(), b.top()),
                  std::max(a.right(), b.right()), std::max(a.bottom(), b.bottom()));
}


} // namespace geom
