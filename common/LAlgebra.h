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



#ifndef LALGEBRA_H
#define LALGEBRA_H

#include "Geometry.h"

namespace lalgebra {
  static const float FLOAT_COMPARISON_PRECISION = 1e-4;

  typedef Matrix33<float> Matrix;
  typedef RotateMatrix33<float> RotateMatrix;
  typedef TranslateMatrix33<float> TranslateMatrix;
  typedef ScaleMatrix33<float> ScaleMatrix;
  typedef Vector3<float> Vector;
  typedef GeometryT<float> Geometry;

  float min(float f, float s);
  float max(float f, float s);

  // check two floats with precision
  bool isEqual(const float one, const float two, const float precision);

  // check if value is in range (include confines)
  // it also check reverse intervals [-1; -3] correctly
  bool isInRange(const float what, const float from, const float to);

  // compare two vectors on equality
  bool isEqual(const Vector &v, const Vector &v2);

  // calculate orientation of point relatively vector (begin - end) in XY dimension (W isn't used)
  // 1 - it's at the left of vector
  // 0 - it belongs vector
  // -1 - it's at the right of vector
  int getPointToVectorOrientation(const Vector &begin, const Vector &end, const Vector &point);

  // RGB color
  typedef struct colorRGB {
    static const int SIZE = 3;

    colorRGB() {};

    colorRGB(float r, float g, float b): r(r), g(g), b(b) {};
    float r;
    float g;
    float b;
  } ColorRGB;

  static const struct colorRGB kBLUE = {0.0, 0.0, 1.0};
  static const struct colorRGB kRED = {1.0, 0.0, 0.0};
  static const struct colorRGB kGREEN = {0.0, 1.0, 0.0};


  // Rectangle
  class Rectangle : public Geometry {
  public:
    Rectangle(Vector topleft = Vector(0, 0), Vector bottomright = Vector(0, 0));

    bool isIntersected(Rectangle &r);

    const Vector getTopLeft() const;
    const Vector getTopRight() const;
    const Vector getBottomLeft() const;
    const Vector getBottomRight() const;
    int getWidth() const;
    int getHeight() const;

    bool isInside(Vector p);
    bool isOutside(Vector p);

  };

}

#endif /* LALGEBRA_H */


