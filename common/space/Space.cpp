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

#include "OpenGL.h"
#include "Space.h"
#include "SpaceArchitect.h"
#include "Ship.h"
#include "Shader.h"
#include "Plasmoid.h"
#include <algorithm>
#include <iostream>
#include <cctype>
#include <functional>

void Space::compileShader() {
  bool compilationResult = _shader->compileAndLink();
  if (!compilationResult) {
    error("Shader complilation error.");
    exit(1);
  }
}

SpaceObjectShaderConf Space::useShader() {
  glUseProgram(_shader->getProgram());
  SpaceObjectShaderConf conf;

  //get the attachment points for the attributes position and color
  int resultPosAllocation = glGetAttribLocation(_shader->getProgram(), "position");
  int resultColorAllocation = glGetAttribLocation(_shader->getProgram(), "color");
  int resultViewMatrixAllocation = glGetUniformLocation(_shader->getProgram(), "viewMatrix");

  //check that the locations are valid, negative value means invalid
  if (resultPosAllocation < 0 || resultColorAllocation < 0 || resultViewMatrixAllocation < 0) {
    error("Could not query attribute locations");
  } else {
    conf.setPositionLoc(static_cast<unsigned int>(resultPosAllocation));
    conf.setColorLoc(static_cast<unsigned int>(resultColorAllocation));
    // it's for internal use
    _viewMatrixLocation = static_cast<unsigned int>(resultViewMatrixAllocation);
    // it allows us to bind VBO to shaders attributes
    glEnableVertexAttribArray(conf.getPositionLoc());
    glEnableVertexAttribArray(conf.getColorLoc());
  }

  return conf;
}

SpaceObjectShaderConf Space::prepareShader() {
  _shader = std::unique_ptr<Shader>(new Shader("shader.vsh", "shader.fsh"));
  compileShader();
  return useShader();
}

class OutOfBoundsChecker : public std::unary_function<Plasmoid, bool> {
public:
  OutOfBoundsChecker(Rectangle &bounds):_bounds(bounds) {}

  bool operator ()(const Plasmoid p) {
      bool oob = _bounds.isOutside(p.getPosition());
      debug("out of bounds %b",oob);
    return _bounds.isOutside(p.getPosition());
  }
private:
  Rectangle &_bounds;
};

bool op(Plasmoid p) {return true;}


/**
* Update and draw the scene and all space objects
* TODO Add frameRate, startTime and currentFrame and use msSinseLastUpdate for correct animation
* - update all positions, apply all effects and ...
* - ... check out of bounds of the space for each object
* - ... if (out of bounds) remove plasmoids and teleport other objects to counter-side edge (180 degree rotation)
* - check the collisions with using externally set collider
* - ... if (collision) we use collider callback to tell about that
*
* - set universal viewMatrix (ScaleMatrix at this case)
* - draw all of remaining objects
*/
void Space::draw(float msSinceLastUpdate) {
  if (!_shader) {
    _shaderConf = prepareShader();
  }

  // TODO Add tick() for update and collision detection
  _ship->update();
  Vector shipPos = _ship->getPosition();
  if (_bounds.isOutside(shipPos)) {
    // TODO subtract width or height
    Vector newPos = shipPos * RotateMatrix(180, Degree);
    _ship->setPosition(newPos);
  }

  // update plasmoids
  for (auto &p : _plasmoids) p->update();

  // set remove list from plasmoids out of space
  auto removed = std::remove_if(_plasmoids.begin(), _plasmoids.end(),
      [this](std::unique_ptr<Plasmoid>& p) {
        return this->_bounds.isOutside(p->getPosition());
      });
  // remove out of space plasmoids
  _plasmoids.erase(removed, _plasmoids.end());

  // set view matrix one time for all objects
  glUniformMatrix3fv(_viewMatrixLocation, 1, 0, _viewMatrix.flat().getArrayC());

  // draw all stuff
  _ship->draw(_shaderConf);
  for (auto &p : _plasmoids) p->draw(_shaderConf);
}

Space::~Space() {
  debug("Space is disappearing...");
}


Matrix Space::prepareViewMatrix(const int resolutionWidth, const int resolutionHeight) {
  if (resolutionWidth > 0 && resolutionHeight > 0) {
    // x and y factors to normalize object' geometries
    float xScale = 1.0 / resolutionWidth; //SpaceArchitect::GRAPHIC_RESOLUTION_WIDTH;
    float yScale = 1.0 / resolutionHeight; //SpaceArchitect::GRAPHIC_RESOLUTION_HEIGHT;
    // scale factor to screen resolution from design resolution. See Space Architect for more detail
    // TODO Add parameter enum Scale SaveProportion, StretchWidth and etc
    float commonScale = 1; //static_cast<float>(resolutionHeight) / SpaceArchitect::GRAPHIC_RESOLUTION_HEIGHT;

    return ScaleMatrix(xScale, yScale, commonScale);
  }
  return Matrix(true);
}

// TODO Make Enable method which on/off glUseProgram
Space::Space() {
  _ship = std::unique_ptr<Ship>(new Ship(SpaceArchitect::SHIP, kRED, Vector(10, 10)));
}

void Space::moveShip(float dx, float dy, float curAngle) {
  _ship->setVelocity(Vector(dx, dy, 0));
  _ship->setAngleInRadians(curAngle);
}


void Space::setSize(int width, int height) {
  _viewMatrix = prepareViewMatrix(width, height);
  // set space bounds
  //  _bounds = Rectangle(Vector(-width/2.0, height/2.0), Vector(width/2.0, -height/2.0));
  _bounds = Rectangle(Vector(-width, height), Vector(width, -height));
  // TODO maybe place this outside
  glViewport(0, 0, width, height);
}

void Space::shipAttack() {
  _plasmoids.push_back(std::move(_ship->piffPaff()));
}
