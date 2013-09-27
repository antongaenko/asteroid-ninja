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

bool isEqual(const Vector &v, const Vector &v2) {
  return fabs(v.getX() - v2.getX()) < FLOAT_COMPARISON_PRECISION &&
    fabs(v.getY() - v2.getY()) < FLOAT_COMPARISON_PRECISION &&
    fabs(v.getZ() - v2.getZ()) < FLOAT_COMPARISON_PRECISION;

}

/**
* Rectangle implementation
*/
  bool Rectangle::isIntersected(Rectangle &r) {
    return isContained(r.getTopLeft()) || isContained(r.getBottomRight());
  }

  bool Rectangle::isContained(Vector &p) {
    return !(p.getX() < _topleft.getX()) && (p.getX() < _bottomright.getX()) &&
        !(p.getY() < _topleft.getY()) && (p.getY() < _bottomright.getY());
  }

  Vector &Rectangle::getBottomRight() {
    return _bottomright;
  }

  Rectangle::Rectangle(Vector& topleft, Vector& bottomright):
  _topleft(topleft),
  _bottomright(bottomright) {
  }

  Vector &Rectangle::getTopLeft() {
    return _topleft;
  }

}


