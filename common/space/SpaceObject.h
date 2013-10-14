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

#include "LAlgebra.h"
#include "OpenGL.h"

using namespace lalgebra;

class SpaceObject {
public:
  SpaceObject(const Geometry &geometry, const ColorRGB &color, const Vector &initPos = Vector(0, 0, 1));

  // apply to geometry all transformations
  virtual void update(float portion);

  int getVertexCount() const;

  void setAngleInRadians(float angle);
  void setVelocity(const Vector &value);
  Vector getPosition() const;
  ColorRGB getColor() const;
  void setPosition(const Vector& newPos);

  // instruct that this object is bumped by another
  void setBumped();
  void disableBumped();
  // check if object is bumped
  bool isBumped() const;

  // methods to work with inside geometry
  const Geometry& getCurrentGeometry() const;

  ~SpaceObject() {};

protected:
  // initial 2D vertices of object
  Geometry _initialGeometry;
  // geometry with applying all rotations and translations
  Geometry _transformedGeometry;
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

#endif /* SPACEOBJECT_H */