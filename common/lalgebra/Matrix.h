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


#ifndef __Matrix_H_
#define __Matrix_H_

#include <vector>
#include <math.h>

static const float PI = 3.141592;
static const float DegreesToRadians = PI / 180;
enum AngleMeasure {Degree, Radians};

/**
  * Square matrix [N][N].
  * Is can be only Identity or Nil.
  * We do all transformations by multiplication.
  */
template <class T, int Size>
class SquareMatrix {
public:
  SquareMatrix(bool isIdentity = false);

  // return all elements in one-dimension array row by row
  std::vector<T> flat();

  // return element at specific row and column
  T operator ()(const int row, const int col) const;

  /**
   * Multiply to other square matrix with the same size using the simplest algorithm with O(n^3).
   * Return new matrix.
   */
  virtual SquareMatrix<T, Size> operator *(const SquareMatrix<T, Size> &on);

  // return count of row (for square matrix it means for column too)
  int getSize();

protected:
  // matrix[row][column]
  T _m[Size][Size];

};



/**
* Matrix 3x3
 */
template<class T>
using Matrix33 = SquareMatrix<T, 3>;

/**
* Rotate matrix 3x3.
* User should set angle for transformations.
* Positive angle specify counter-clockwise rotation.
*/
template <class T>
class RotateMatrix33 : public Matrix33<T> {
public:
  RotateMatrix33(float angle, AngleMeasure in = Radians);
};

/**
* Translate matrix 3x3.
* It add dx to X and dy to Y coordinates.
* (1,1,1) * TranslateMatrix33(1,1) = (2,2,1)
*/
template <class T>
class TranslateMatrix33 : public Matrix33<T> {
public:
  TranslateMatrix33(float dx, float dy);
};

/**
* Scale matrix 3x3.
* (1,1,1) * ScaleMatrix33(2) = (1,1,1/2)
*/
template <class T>
class ScaleMatrix33 : public Matrix33<T> {
public:
  ScaleMatrix33(const float xScale, const float yScale, const float commonScale = 1.0);

  ScaleMatrix33(const float commonScale);
};

// for template we should have implementation in included file (this header)
#include "Matrix.cpp"


#endif //__Matrix_H_
