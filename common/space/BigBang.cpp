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

#include "BigBang.h"
#include "Plasmoid.h"
#include "SpaceArchitect.h"

BigBang::BigBang(const Geometry& geometry, const ColorRGB& color, const Vector & initPos):
SpaceObject(geometry, color, initPos) {}

void BigBang::setAngularFrequencyRadians(const float angularFrequency) {
  _angularFrequency = angularFrequency;
};


void BigBang::update(float portion) {
  _angle += _angularFrequency * portion;
  SpaceObject::update(portion);
}


std::vector<std::unique_ptr<Plasmoid>> BigBang::boom() {
  static int const PLASMOID_COUNT = 10;
  std::vector<std::unique_ptr<Plasmoid>> v;
  v.reserve(PLASMOID_COUNT);
  float angleDiff = 2 * PI / PLASMOID_COUNT;
  
  float angle = _angle;
  for (int i = 0; i < PLASMOID_COUNT; i++) {
    auto p = std::unique_ptr<Plasmoid>(new Plasmoid({SpaceArchitect::LASER}, _color, _position));
    // then apply rotation to velocity vector
    p->setVelocity(SpaceArchitect::PLASMOID_VELOCITY * RotateMatrix(angle, Radians));
    p->setAngleInRadians(angle);
    angle += angleDiff;
    p->update();
    v.push_back(std::move(p));
  }
  
  return v;
}