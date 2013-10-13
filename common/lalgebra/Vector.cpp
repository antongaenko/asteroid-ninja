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

#ifndef __Vector_CPP_
#define __Vector_CPP_

#include "Vector.h"

template <class T>
Vector3<T>::Vector3(T x, T y, T w):_x(x), _y(y), _w(w) {}

template <class T>
Vector3<T>::Vector3(std::initializer_list<T> l) {
  _w = 1;
  auto it = l.begin();
  if (it != l.end()) _x = *it;
  if (++it != l.end()) _y = *it;
  if (++it != l.end()) _w = *it;
}

template <class T>
T Vector3<T>::getX() const {
  return _x;
}

template <class T>
T Vector3<T>::getY() const {
  return _y;
}

template <class T>
T Vector3<T>::getW() const {
  return _w;
}

// it divide X and Y on scale factor W
template <class T>
Vector3<T>& Vector3<T>::applyScale() {
  _x = _w == 0 ? 0 : _x / _w;
  _y = _w == 0 ? 0 : _y / _w;
  _w = 1;
  return *this;
}

template <class T>
// it doesn't sum W. It applies it as scale factor for second Vector firstly.
Vector3<T> Vector3<T>::operator +(Vector3<T> v) const {
  auto withScale = v.applyScale();
  if (_w == 0) return Vector3<T>(withScale._x, withScale._y, 1);
  else return Vector3<T>(_x / _w + withScale._x, _y / _w + withScale._y, 1);
}

template <class T>
// it doesn't sum W. It applies it as scale factor for second Vector firstly.
void Vector3<T>::operator += (Vector3<T> v) {
  auto withScale = v.applyScale();
  applyScale();

  _x += withScale._x;
  _y += withScale._y;
}

template <class T>
// multiply on matrix and return NEW transformed vector
Vector3<T> Vector3<T>::operator *(const SquareMatrix<T, 3> &m) {
  return Vector3<T>(
      getX() * m(0, 0) + getY() * m(1, 0) + getW() * m(2, 0),
      getX() * m(0, 1) + getY() * m(1, 1) + getW() * m(2, 1),
      getX() * m(0, 2) + getY() * m(1, 2) + getW() * m(2, 2)
  );
};


#endif //__Vector_CPP_