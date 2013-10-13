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


#ifndef __Geometry_CPP_
#define __Geometry_CPP_

#include "Geometry.h"

template <class T>
GeometryT<T>::GeometryT(int size):_vectors() {
  _vectors.reserve(size);
}

template <class T>
GeometryT<T>::GeometryT(std::initializer_list <Vector3<T>> vs):_vectors(vs) {}

template <class T>
GeometryT<T>::~GeometryT() {
  _vectors.clear();
}

template <class T>
void GeometryT<T>::add(const Vector3<T> v) {
  _vectors.push_back(v);
}

template <class T>
int GeometryT<T>::getSize() const {return _vectors.size();}

template <class T>
int GeometryT<T>::getFlatSize() const {return getSize() * Vector3<T>::Length;};

template <class T>
Vector3<T> GeometryT<T>::operator [](const int index) const {return _vectors[index];}

// it transforms all vectors in geometry and returns NEW geometry
template <class T>
GeometryT<T> GeometryT<T>::operator *(const Matrix33<T> &m) {
  GeometryT r(getSize());
  for (auto &v : _vectors) r.add(v * m);
  return r;
}

// return flat array of XYW for ALL vectors (vertices)
template <class T>
std::vector<float> GeometryT<T>::flat() const {
  std::vector<float> flat;
  flat.reserve(getFlatSize());

  for (int i = 0; i < getSize(); i++) {
    flat.push_back(_vectors[i].getX());
    flat.push_back(_vectors[i].getY());
    flat.push_back(_vectors[i].getW());
  }

  return flat;
}

// scale ALL vectors
template <class T>
GeometryT<T> GeometryT<T>::scale(float scale) {return (*this) * ScaleMatrix33<T>(scale);}

// rotate ALL vectors
template <class T>
GeometryT<T> GeometryT<T>::rotate(float angle, AngleMeasure unit) {return (*this) * RotateMatrix33<T>(angle, unit);}

// translate ALL vectors
template <class T>
GeometryT<T> GeometryT<T>::translate(float dx, float dy) {return (*this) * TranslateMatrix33<T>(dx, dy);}

template <class T>
GeometryT<T> GeometryT<T>::translate(Vector3<T> v) {
  Vector3<T> withScale = v.applyScale();
  return this->translate(withScale.getX(), withScale.getY());
}

#endif //__Geometry_CPP_


