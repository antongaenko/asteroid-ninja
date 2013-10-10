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


#ifndef __Geometry_H_
#define __Geometry_H_

#include <vector>
#include "Geometry.h"
#include "Vector.h"
#include <initializer_list>


/**
  * Geometry is a set of Vectors (points)
  */
template <class T>
class GeometryT {
public:
  GeometryT(int size);
  GeometryT(std::initializer_list <Vector3<T>> vs);
  ~GeometryT();

  // add Vector to this Geometry
  void add(const Vector3<T> v);

  // get count of Vectors (points)
  int getSize() const;

  // get count of coords of Vectors (X, Y and scale factor too)
  int getFlatSize() const;

  // get Vector by index
  Vector3<T> operator [](const int index) const;

  // transform all vectors in geometry and returns NEW geometry
  GeometryT operator *(const Matrix33<T> &m);

  // return flat array of XYW for ALL vectors (vertices)
  std::vector<float> flat() const;

  // scale ALL vectors
  GeometryT scale(float scale);

  // rotate ALL vectors
  GeometryT rotate(float angle, AngleMeasure unit = Radians);

  // translate ALL vectors
  GeometryT translate(float dx, float dy);

private:
  std::vector <Vector3<T>> _vectors;
};


// for template we should have implementation in included file (fe header)
#include "Geometry.cpp"

#endif //__Geometry_H_
