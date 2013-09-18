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
#include "math.h"

namespace math2d {

/**
* Vector3 implementation
*/

//Vector3::Vector3(Vector3& another):_x(another.getX()), _y(another.getY()), _z(another.getZ()) {}

bool isEqual(const Vector &v, const Vector &v2) {
  return fabs(v.getX() - v2.getX()) < FLOAT_COMPARISON_PRECISION &&
    fabs(v.getY() - v2.getY()) < FLOAT_COMPARISON_PRECISION &&
    fabs(v.getZ() - v2.getZ()) < FLOAT_COMPARISON_PRECISION;

}


/**
* Matrix implementation
*/
// TODO don't like to work with array. Maybe allow to access directly flat array behind.
  Matrix2::Matrix2() {
    // Nil matrix
    _m[0][0] = 0;
    _m[0][1] = 0;
    _m[1][0] = 0;
    _m[1][1] = 0;
  }

  void Matrix2::setIdentity() {
    _m[0][0] = 1;
    _m[0][1] = 0;
    _m[1][0] = 0;
    _m[1][1] = 1;
  }

  float *Matrix2::operator [](int index) {
    return _m[index];
  }

  void Matrix2::flat(float *arrayToFill) {
    arrayToFill[0] = _m[0][0];
    arrayToFill[1] = _m[0][1];
    arrayToFill[2] = _m[1][0];
    arrayToFill[3] = _m[1][1];
  }

  Matrix2 Matrix2::operator *(Matrix2& on) {
    Matrix2 result;
    result[0][0] = _m[0][0] * on[0][0] + _m[0][1] * on[1][0];
    result[0][1] = _m[0][0] * on[0][1] + _m[0][1] * on[1][1];
    result[1][0] = _m[1][0] * on[0][0] + _m[1][1] * on[1][0];
    result[1][1] = _m[1][0] * on[0][1] + _m[1][1] * on[1][1];
    return result;
  }

/**
* Rotate matrix implementation
*/
  RotateMatrix2::RotateMatrix2(float angle):Matrix2() {
    _m[0][0] = cosf(angle);
    _m[0][1] = sinf(angle);
    _m[1][0] = -sinf(angle);
    _m[1][1] = cosf(angle);
  }

/**
* Translate matrix implementation
*/
  TranslateMatrix2::TranslateMatrix2(float dx, float dy):Matrix2() {
    _m[0][0] = 1;
    //_m[0][1] = dx;
    //_m[1][0] = dy;
    _m[1][1] = -1;
  }

/**
* Rectangle implementation
*/
  bool Rectangle::isIntersected(Rectangle &r) {
    return isContained(r.getTopLeft()) || isContained(r.getBottomRight());
  }

  bool Rectangle::isContained(Point2D &p) {
    return !(p.x < _topleft.x) && (p.x < _bottomright.x) &&
        !(p.y < _topleft.y) && (p.y < _bottomright.y);
  }

  Point2D &Rectangle::getBottomRight() {
    return _bottomright;
  }

  Rectangle::Rectangle(Point2D& topleft, Point2D& bottomright):
  _topleft(topleft),
  _bottomright(bottomright) {
  }

  Point2D &Rectangle::getTopLeft() {
    return _topleft;
  }

}


