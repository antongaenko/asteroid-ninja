/* Copyright (C) 2013 by Anton Gaenko
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

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "Math2D.h"
#include "OpenGL.h"
#include "Logger.h"

using namespace math2d;

template <int Size>
class SpaceObject {
public:
  static const int SIZE = Size;
  
  // TODO It's better to use template for countPoints param if asteroid point count will be static
  SpaceObject(const Geometry<float, Size> &geometry, const ColorRGB &color, const Vector &initPos = Vector(0, 0, 1));

  // apply to geometry all transformations
  virtual void update();


  void setAngleInRadians(float angle);
  void setVelocity(const Vector &value);
  Vector getPosition() const;
  ColorRGB& getColor();
  void setPosition(const Vector& newPos);

  // instruct that this object is bumped by another
  void setBumped();
  // check if object is bumped
  bool isBumped() const;

  // methods to work with inside geometry
  const Geometry<float, Size>& getCurrentGeometry() const;

  ~SpaceObject() {};

protected:
  void bindBuffers();

  // initial 2D vertices of object
  Geometry<float, Size> _initialGeometry;
  // geometry with applying all rotations and translations
  Geometry<float, Size> _transformedGeometry;
  // color for all points
  ColorRGB _color;
  // center of object. It changes each frame.
  Vector _position;
  // angle in Radians
  float _angle;
  // movement velocity in the Space
  Vector _velocity;

  // flag to instruct that this object is bumped by another object
  bool _isBumped;

};


template <int Size>
SpaceObject<Size>::SpaceObject(const Geometry<float, Size> &geometry, const ColorRGB &color, const Vector &initPos):
_initialGeometry(geometry),
_color(color),
_position(initPos),
_angle(0),
_velocity(0, 0, 0),
_isBumped(false) {};

template <int Size>
void SpaceObject<Size>::bindBuffers() {}

template <int Size>
void SpaceObject<Size>::setAngleInRadians(float angle) {
  _angle = angle;
}

template <int Size>
void SpaceObject<Size>::setVelocity(const Vector &value) {
  _velocity = value;
}

template <int Size>
Vector SpaceObject<Size>::getPosition() const {
  return _position;
}

template <int Size>
ColorRGB& SpaceObject<Size>::getColor() {
  return _color;
}

template <int Size>
void SpaceObject<Size>::setPosition(const Vector& newPos) {
  _position = newPos;
}

template <int Size>
const Geometry<float, Size>& SpaceObject<Size>::getCurrentGeometry() const {
  return _transformedGeometry;
};

template <int Size>
void SpaceObject<Size>::update() {
  _position += _velocity;
  _transformedGeometry = _initialGeometry.
      rotate(_angle, Radians).
      translate(_position.getX(), _position.getY()); 
}

template <int Size>
void SpaceObject<Size>::setBumped() {
  _isBumped = true;
}

template <int Size>
bool SpaceObject<Size>::isBumped() const {
  return _isBumped;
}

#endif /* SPACEOBJECT_H */