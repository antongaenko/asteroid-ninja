/*
 Copyright (C) 2013 by Anton Gaenko 
 Mail anton.gaenko@yahoo.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "Math2D.h"

namespace math2d {
  
  float min(float f, float s) {
    return f < s ? f : s;
  }
  
  float max(float f, float s) {
    return f > s ? f : s;
  }

  bool isEqual(const float one, const float two, const float precision = FLOAT_COMPARISON_PRECISION) {
    return fabs(one - two) < precision &&
        fabs(one - two) < precision &&
        fabs(one - two) < precision;
  }

  bool isInRange(const float what, const float from, const float to) {
    // we use XOR for correct checking reverse intervals [-1; -3] (from right to left)
    return ((what > from) ^ (what > to)) || isEqual(what, from) || isEqual(what, to);
  }

  // check two float vectors on equality with the simplest float comparision method
  bool isEqual(const Vector &v, const Vector &v2) {
    return isEqual(v.getX(), v2.getX()) &&
        isEqual(v.getY(), v2.getY()) &&
        isEqual(v.getW(), v2.getW());
  }

  /**
  * Rectangle implementation
  */
  Rectangle::Rectangle(Vector topleft, Vector bottomright):Geometry<float,4>() {
    // calculate all points
    a[0] = topleft;
    a[1] = Vector(bottomright.getX(), topleft.getY());
    a[2] = bottomright;
    a[3] = Vector(topleft.getX(), bottomright.getY());

    // check right rectangle initialization and orientation
    if (topleft.getX() > bottomright.getX() || topleft.getY() < bottomright.getY()) {
      error("Incorrect rectangle. Check Top-Left and Bottom-Right parameters.");
    }
  }

  const Vector &Rectangle::getTopLeft() const {
    return a[0];
  }

  const Vector &Rectangle::getTopRight() const {
    return a[1];
  }

  const Vector &Rectangle::getBottomRight() const {
    return a[2];
  }

  const Vector &Rectangle::getBottomLeft() const {
    return a[3];
  }
  
  const int Rectangle::getWidth() const {
    return static_cast<int>(getTopRight().getX() - getTopLeft().getX());
  };
  
  const int Rectangle::getHeight() const {
    return static_cast<int>(getTopRight().getY() - getBottomRight().getY());
  };


  // check all vertexes on inersection with other rectangle
  bool Rectangle::isIntersected(Rectangle &r) {
    return isInside(r.getTopLeft()) || isInside(r.getBottomRight()) ||
        isInside(r.getTopRight()) || isInside(r.getBottomLeft());
  }

  // include the edges
  bool Rectangle::isInside(Vector p) {
    return isInRange(p.getX(), getTopLeft().getX(), getBottomRight().getX()) &&
        isInRange(p.getY(), getBottomRight().getY(), getTopLeft().getY());
  }

  bool Rectangle::isOutside(Vector p) {
    return !isInside(p);
  }

  // return cross-product of two vectors in XY dimension (W isn't used)
  // in geometrical sense positive value means that tested point lies to the left of the vector,
  // negative - to the right of the vector
  // and 0 that tested point lays on vector's straight
  int crossProduct2D(const Vector&center, const Vector&end1, const Vector& end2) {
    // (x2 - x1)(y3 - y1) - (y2 - y1)(x3 - x1)
    return static_cast<int>((end1.getX() - center.getX()) * (end2.getY() - center.getY()) -
    (end1.getY() - center.getY()) * (end2.getX() - center.getX()));
  }
  
  template <class T>
  T Vector3<T>::getX() const {
    return _x;
  }
  
  template <class T>
  Vector3<T>& Vector3<T>::operator=(const Vector3& another) {
    //debug("assignment");
    _x = another._x;
    _y = another._y;
    _w = another._w;
    return *this;
  }
  
}


