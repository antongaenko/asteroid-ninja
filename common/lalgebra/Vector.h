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

#ifndef __Vector_H_
#define __Vector_H_

#include "Matrix.h"
#include <initializer_list>

/**
 * Vector-row in homogeneous coordinates
*/
template <class T>
class Vector3 {
public:
  static const int Length = 3;
  Vector3(T x = 0, T y = 0, T w = 1);
  Vector3(std::initializer_list<T> l);

  T getX() const;
  T getY() const;
  T getW() const;

  // it divide X and Y on scale factor W
  Vector3<T>& applyScale();

  // apply scale and sum both
  Vector3<T> operator +(Vector3<T> v) const;

  // apply scale and sum both
  void operator += (Vector3<T> v);

  // multiply on matrix and return NEW transformed vector
  Vector3<T> operator *(const SquareMatrix<T, 3> &m);

private:
  T _x;
  T _y;
  T _w;
};


// for template we should have implementation in included file (this header)
#include "Vector.cpp"


#endif //__Vector_H_
