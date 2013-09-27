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

// We need to place it with SpaceObject because they know each other very closely.
// So user of SpaceObject will understand what parameters in shader
class SpaceObjectShaderConf {
public:
  SpaceObjectShaderConf(const int positionLocation = -1, const int colorLocation = -1);
  void setPositionLoc(const unsigned int value);
  unsigned int getPositionLoc() const;
  void setColorLoc(const unsigned int value);
  unsigned int getColorLoc() const;

private:
  int _positionLocation;
  int _colorLocation;
};


template <int Size>
class SpaceObject {
public:
  // TODO It's better to use template for countPoints param if asteroid point count will be static
  SpaceObject(const Geometry<float, Size> &geometry, const ColorRGB &color, const Vector &initPos = Vector(0, 0, 1));

  void draw(const SpaceObjectShaderConf &conf);
  // apply to geometry all transformations
  void update();

  void setAngleInRadians(float angle);
  void setVelocity(const Vector &value);
  // get object transformation matrix
  Matrix getTransformation();

  ~SpaceObject() {};

protected:
  void bindBuffers();

  // initial 2D vertices of object
  Geometry<float, Size> _initialGeometry;
  // geometry with applying all rotations and translations
  Geometry<float, Size> _transformedGeometry;

  // specify that our object geometry is attached to graphic buffer
  unsigned int _geomVboID;

  // specify that our object color is attached to graphic buffer
  unsigned int _colorVboID;
  // color for all points
  ColorRGB _color;
  // center of object. It changes each frame.
  Vector _position;
  // angle in Radians
  float _angle;
  // movement velocity in the Space
  Vector _velocity;

};


template <int Size>
SpaceObject<Size>::SpaceObject(const Geometry<float, Size> &geometry, const ColorRGB &color, const Vector &initPos):
_initialGeometry(geometry),
_color(color),
_position(initPos),
_angle(0),
_velocity(0, 0, 0),
_geomVboID(0),
_colorVboID(0) {};

template <int Size>
void SpaceObject<Size>::bindBuffers() {
  // Geometry buffer we will set with data later
  glGenBuffers(1, &_geomVboID);
  // but colors here
  glGenBuffers(1, &_colorVboID);
  glBindBuffer(GL_ARRAY_BUFFER, _colorVboID);

  float colors[Size * 3];
  for (int i = 0; i < Size * 3; i += 3) {
    colors[i] = _color.r;
    colors[i + 1] = _color.g;
    colors[i + 2] = _color.b;
  }

  // size = RGB * point count * sizeof(float)
  glBufferData(GL_ARRAY_BUFFER, 3 * Size * sizeof(float), colors, GL_STATIC_DRAW);
  if (glGetError()) error("space draw GLerror(%d)", glGetError());
}

template <int Size>
void SpaceObject<Size>::draw(const SpaceObjectShaderConf &conf) {
  if (_geomVboID == 0 && _colorVboID == 0) bindBuffers();

  glBindBuffer(GL_ARRAY_BUFFER, _geomVboID);

  glBufferData(GL_ARRAY_BUFFER, Size * Vector::Length * sizeof(float), _transformedGeometry.flat().getArrayC(), GL_STATIC_DRAW);

  //glBindBuffer(GL_ARRAY_BUFFER, _geomVboID);
  //point the position attribute to this buffer, being tuples of 4 floats for each vertex
  //    glVertexAttribPointer(_positionLocation, 4, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribPointer(conf.getPositionLoc(), Vector::Length, GL_FLOAT, GL_FALSE, 0, NULL);

  //bind the color VBO
  glBindBuffer(GL_ARRAY_BUFFER, _colorVboID);
  //this attribute is only 3 floats per vertex
  //    glVertexAttribPointer(_colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribPointer(conf.getColorLoc(), 3, GL_FLOAT, GL_FALSE, 0, NULL);


  if (glGetError()) error("space draw GLerror(%d)", glGetError());
  //initiate the drawing process, we want a triangle, start at index 0 and draw 3 vertices
  if (Size == 1) {
    glDrawArrays(GL_POINTS, 0, 1);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  // TODO Use indicies and experiment with GL_LINES and LINES_STRIP

};

template <int Size>
void SpaceObject<Size>::setAngleInRadians(float angle) {
  _angle = angle;
}

template <int Size>
void SpaceObject<Size>::setVelocity(const Vector &value) {
  _velocity = value;
}

template <int Size>
Matrix SpaceObject<Size>::getTransformation() {
  return RotateMatrix(_angle) * TranslateMatrix(_position.getX(), _position.getY());
}

template <int Size>
void SpaceObject<Size>::update() {
  _position += _velocity;
  _transformedGeometry = _initialGeometry.
      rotate(_angle, Radians).
      translate(_position.getX(), _position.getY());
}

#endif /* SPACEOBJECT_H */