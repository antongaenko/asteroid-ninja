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


#ifndef __Matrix_CPP_
#define __Matrix_CPP_

#include "Matrix.h"

template <class T, int Size>
SquareMatrix<T,Size>::SquareMatrix(bool isIdentity) {
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      _m[i][j] = isIdentity && (i == j) ? 1 : 0;
    }
  }
}


// return all elements in one-dimension array row by row
template <class T, int Size>
std::vector<T> SquareMatrix<T,Size>::flat() {
  std::vector<float> flat;
  flat.reserve(Size * Size);

  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      flat.push_back(_m[i][j]);
    }
  }
  return flat;
}

template <class T, int Size>
T SquareMatrix<T,Size>::operator ()(const int row, const int col) const { return _m[row][col]; }

/**
 * Multiply to other square matrix with the same size using the simplest algorithm with O(n^3).
 * Return new matrix.
 */
template <class T, int Size>
SquareMatrix<T, Size> SquareMatrix<T,Size>::operator *(const SquareMatrix<T, Size> &on) {
  SquareMatrix r;

  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      r._m[i][j] = 0;
      for (int k = 0; k < Size; k++) {
        r._m[i][j] = r._m[i][j] + _m[i][k] * on._m[k][j];
      }
    }
  }

  return r;
}

template <class T, int Size>
int SquareMatrix<T,Size>::getSize() {return Size;}

/*
 Rotate matrix
 */
template <class T>
RotateMatrix33<T>::RotateMatrix33(float angle, AngleMeasure in):Matrix33<T>(true) {
  float angleInRadians = in == Radians ? angle : angle * DegreesToRadians;
  this->_m[0][0] = cosf(angleInRadians);
  this->_m[0][1] = sinf(angleInRadians);
  this->_m[1][0] = -sinf(angleInRadians);
  this->_m[1][1] = cosf(angleInRadians);
}

/*
 Translate matrix
 */
template <class T>
TranslateMatrix33<T>::TranslateMatrix33(float dx, float dy):Matrix33<T>(true) {
  this->_m[2][0] = dx;
  this->_m[2][1] = dy;
}

/*
 Scale matrix
 */
template <class T>
ScaleMatrix33<T>::ScaleMatrix33(const float xScale, const float yScale, const float commonScale):Matrix33<T>(true) {
  this->_m[0][0] = xScale;
  this->_m[1][1] = yScale;
  // In homogeneous coordinates X and Y is dividing by this factor X/Z.
  // It's why, here we use reverse value.
  this->_m[2][2] = 1.0 / commonScale;
}

template <class T>
ScaleMatrix33<T>::ScaleMatrix33(const float commonScale):Matrix33<T>(true) {
  // In homogeneous coordinates X and Y is dividing by this factor X/Z.
  // It's why, here we use reverse value.
  this->_m[2][2] = 1.0 / commonScale;
}


#endif //__Matrix_CPP_