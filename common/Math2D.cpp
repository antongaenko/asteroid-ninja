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

  // compare two floats
  bool isEqual(const float one, const float two) {
    return fabs(one - two) < FLOAT_COMPARISON_PRECISION &&
        fabs(one - two) < FLOAT_COMPARISON_PRECISION &&
        fabs(one - two) < FLOAT_COMPARISON_PRECISION;
  }

  // check if value is in range (include confines)
  bool isInRange(const float what, const float low, const float high) {
    bool isEqualLow = isEqual(what, low);
    bool isEqualHigh = isEqual(what, high);
    return (what > low || isEqualLow || isEqual(what, low)) && (what < high || isEqualHigh || isEqual(what, high));
  }

  // check two float vectors on equality with the simplest float comparision method
  bool isEqual(const Vector &v, const Vector &v2) {
    return isEqual(v.getX(), v2.getX()) &&
        isEqual(v.getY(), v2.getY()) &&
        isEqual(v.getZ(), v2.getZ());
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
    return getTopRight().getX() - getTopLeft().getX();
  };
  
  const int Rectangle::getHeight() const {
    return getTopRight().getY() - getBottomRight().getY();
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
}


