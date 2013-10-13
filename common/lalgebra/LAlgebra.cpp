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

#include "LAlgebra.h"
#include "Logger.h"

namespace lalgebra {
  
  float min(float f, float s) {
    return f < s ? f : s;
  }
  
  float max(float f, float s) {
    return f > s ? f : s;
  }

  bool flipCoin() {
    return rand() % 2 < 1;
  }
  
  bool isEqual(const float one, const float two, const float precision = FLOAT_COMPARISON_PRECISION) {
    return fabs(one - two) < precision;
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
  Rectangle::Rectangle(Vector topleft, Vector bottomright):
    Geometry({topleft,
      Vector(bottomright.getX(), topleft.getY()),
      bottomright,
      Vector(topleft.getX(), bottomright.getY())}) {
    // check right rectangle initialization and orientation
    if (topleft.getX() > bottomright.getX() || topleft.getY() < bottomright.getY()) {
      error("Incorrect rectangle. Check Top-Left and Bottom-Right parameters.");
    }
  }
  
  Rectangle::Rectangle(Geometry g):Geometry(4) {
    if (g.getSize() >= 4) {
      (*this)[0] = g[0];
      (*this)[1] = g[1];
      (*this)[2] = g[2];
      (*this)[3] = g[3];
    }
  }

  Vector Rectangle::getTopLeft() const {
    return (*this)[0];
  }

  Vector Rectangle::getTopRight() const {
    return (*this)[1];
  }

  Vector Rectangle::getBottomRight() const {
    return (*this)[2];
  }

  Vector Rectangle::getBottomLeft() const {
    return (*this)[3];
  }
  
  int Rectangle::getWidth() const {
    return static_cast<int>(getTopRight().getX() - getTopLeft().getX());
  };
  
  int Rectangle::getHeight() const {
    return static_cast<int>(getTopRight().getY() - getBottomRight().getY());
  };


  // check all vertexes on intersection with other rectangle
  bool Rectangle::isIntersected(Rectangle &r) const {
    return isInside(r.getTopLeft()) || isInside(r.getBottomRight()) ||
        isInside(r.getTopRight()) || isInside(r.getBottomLeft());
  }

  // include the edges
  bool Rectangle::isInside(Vector p) const {
    return isInRange(p.getX(), getTopLeft().getX(), getBottomRight().getX()) &&
        isInRange(p.getY(), getBottomRight().getY(), getTopLeft().getY());
  }

  bool Rectangle::isOutside(Vector p) const {
    return !isInside(p);
  }

  // calculate orientation of point relatively vector (begin - end) in XY dimension (W isn't used)
  // 1 - it's at the left of vector
  // 0 - it belongs vector
  // -1 - it's at the right of vector
  int getPointToVectorOrientation(const Vector &begin, const Vector &end, const Vector &point) {
    // calculate cross product of two vectors (x2 - x1)(y3 - y1) - (y2 - y1)(x3 - x1)
    float r = ((end.getX() - begin.getX()) * (point.getY() - begin.getY()) -
              (end.getY() - begin.getY()) * (point.getX() - begin.getX()));
    if (isEqual(r, 0)) return 0;
    else if (r>0) return 1;
    else return -1;
  }

  // get centroid for specific geometry
  Vector getCentroid(const Geometry &geom) {
    float xSum = 0, ySum = 0, size = geom.getSize();
    for (int i=0; i < size; i++) {
      xSum += geom[i].getX();
      ySum += geom[i].getY();
    }
    return Vector(xSum / size, ySum / size);
  }
  

  
}


