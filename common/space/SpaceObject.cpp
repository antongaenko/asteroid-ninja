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

#include "SpaceObject.h"

SpaceObject::SpaceObject(const Geometry &geometry, const ColorRGB &color, const Vector &initPos):
_initialGeometry(geometry),
_transformedGeometry(geometry),
_color(color),
_position(initPos),
_angle(0),
_velocity(0, 0, 0),
_isBumped(false) {};

void SpaceObject::update() {
  _position += _velocity;
  _transformedGeometry = _initialGeometry.
      rotate(_angle, Radians).
      translate(_position.getX(), _position.getY());
}

int SpaceObject::getVertexCount() const {
  return _initialGeometry.getSize();
}

void SpaceObject::setVelocity(const Vector &value) {
  _velocity = value;
}


Vector SpaceObject::getPosition() const {
  return _position;
}


ColorRGB SpaceObject::getColor() const {
  return _color;
}


void SpaceObject::setPosition(const Vector& newPos) {
  _position = newPos;
}


const Geometry& SpaceObject::getCurrentGeometry() const {
  return _transformedGeometry;
};

void SpaceObject::setBumped() {
  _isBumped = true;
}


bool SpaceObject::isBumped() const {
  return _isBumped;
}

void SpaceObject::setAngleInRadians(float angle) {
      _angle = angle;
}


