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
#include "Asteroid.h"
#include "Collider.h"
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

void Space::useShader() {
  glUseProgram(_shader->getProgram());

  //get the attachment points for the attributes position and color
  int resultPosAllocation = glGetAttribLocation(_shader->getProgram(), "position");
  int resultColorAllocation = glGetAttribLocation(_shader->getProgram(), "color");
  int resultViewMatrixAllocation = glGetUniformLocation(_shader->getProgram(), "viewMatrix");

  //check that the locations are valid, negative value means invalid
  if (resultPosAllocation < 0 || resultColorAllocation < 0 || resultViewMatrixAllocation < 0) {
    error("Could not query attribute locations");
  } else {
    _shaderAttributePosition = static_cast<unsigned int>(resultPosAllocation);
    _shaderAttributeColor = static_cast<unsigned int>(resultColorAllocation);
    _viewMatrixLocation = static_cast<unsigned int>(resultViewMatrixAllocation);
  }
}

void Space::prepareShader() {
  _shader = std::unique_ptr<Shader>(new Shader("shader.vsh", "shader.fsh"));
  compileShader();
  useShader();
}

// if object escapes the Space it will be moved to another edge
Vector teleport(const Vector &pos, const Rectangle &bounds) {
  float newX = pos.getX(), newY = pos.getY();
  if (pos.getX() < bounds.getTopLeft().getX()) {
    newX = bounds.getTopRight().getX();
  } else if (pos.getX() > bounds.getTopRight().getX()) {
    newX = bounds.getTopLeft().getX();
  }

  if (pos.getY() < bounds.getBottomLeft().getY()) {
    newY = bounds.getTopLeft().getY();
  } else if (pos.getY() > bounds.getTopRight().getY()) {
    newY = bounds.getBottomRight().getY();
  }

  return Vector(newX, newY);
}


/**
 * Update scene, check lifecycle and check space collisions
 * TODO Add frameRate, startTime and currentFrame and use msSinseLastUpdate for correct animation
 * - update all positions, apply all effects and ...
 * - ... check out of bounds of the space for each object
 * - ... if (out of bounds) remove plasmoids and teleport other objects to counter-side edge (180 degree rotation)
 * - check the collisions with using externally set collider
 * - ... if (collision) we use collider callback to tell about that
 */
void Space::update(float msSinceLastUpdate) {
  // update ship
  _ship->update();
  // teleport it if it's out of space
  if (_bounds.isOutside(_ship->getPosition())) {
    _ship->setPosition(teleport(_ship->getPosition(), _bounds));
  }
  // update asteroids and check collision between space ship and asteroids
  for (auto &a : _asteroids) {
    a->update();
    // teleport each if it's out of space
    if (_bounds.isOutside(a->getPosition())) {
      a->setPosition(teleport(a->getPosition(), _bounds));
    }
    // check collision with space
    if (Collider::isCollision(_ship->getCurrentGeometry(), a->getCurrentGeometry())) {
      debug("crash!");
      _ship->setBumped();
      a->setBumped();
      onSpaceEvent(SpaceEvent::SHIP_CRASH);
      break;
    }
  }

  // TODO check collisions between asteroids too

  // TODO split the space on smaller areas and check collisions in them
  // update plasmoids and check collision between plasmoids and asteroids
  for (auto &p : _plasmoids) {
    p->update();
    for (auto &a : _asteroids) {
      if (Collider::isCollision(p->getCurrentGeometry(), a->getCurrentGeometry())) {
        debug("bang!");
        p->setBumped();
        a->setBumped();
        onSpaceEvent(SpaceEvent::ASTEROID_CRACK);
        // plasmoid can kill only one asteroid
        break;
      }
    }
  }

  // remove out of space OR bumped plasmoids
  _plasmoids.erase(std::remove_if(_plasmoids.begin(), _plasmoids.end(),
      [this](std::unique_ptr<Plasmoid> &p) {
        return p->isBumped() || _bounds.isOutside(p->getPosition());
      }), _plasmoids.end());

  // remove bumped asteroids
  _asteroids.erase(std::remove_if(_asteroids.begin(), _asteroids.end(),
      [](std::unique_ptr<Asteroid> &a) {
        return a->isBumped();
      }), _asteroids.end());
}

// collect colors for ALL vertexes for ALL space objects
void Space::prepareColorVBO(const unsigned int& shaderAttributeColor) {
  int vertexCount = Ship::SIZE + _plasmoids.size() * Plasmoid::SIZE + _asteroids.size() * Asteroid::SIZE;
  float colors[vertexCount * 3];
  // we increment offset on each step so we set -1 to be 0 on first iteration
  int offset = -1;
  // get ship color for all vertices (because we should set color for ALL vertices in VBO for ship)
  for (int i = 0; i < Ship::SIZE; i++) {
    colors[++offset] = _ship->getColor().r;
    colors[++offset] = _ship->getColor().g;
    colors[++offset] = _ship->getColor().b;
  }
  
  // get plasmoid color
  for (auto &p : _plasmoids) {
    // we should set color for all vertices in VBO
    for (int i = 0; i < Plasmoid::SIZE; i++) {
      colors[++offset] = p->getColor().r;
      colors[++offset] = p->getColor().g;
      colors[++offset] = p->getColor().b;
    }
  }
  
  // get asteroid color
  for (auto &a : _asteroids) {
    // we should set color for all vertices in VBO
    for (int i = 0; i < Asteroid::SIZE; i++) {
      colors[++offset] = a->getColor().r;
      colors[++offset] = a->getColor().g;
      colors[++offset] = a->getColor().b;
    }
  }
  
  if (!_colorVboID) glGenBuffers(1, &_colorVboID);
  glBindBuffer(GL_ARRAY_BUFFER, _colorVboID);
  // TODO try to SubBufferData
  
  // transfer new data to VBO
  // size = RGB * vertex count * sizeof(float)
  glBufferData(GL_ARRAY_BUFFER, vertexCount * ColorRGB::SIZE * sizeof(float), colors, GL_DYNAMIC_DRAW);
  // point the shader's "color" attribute to this buffer
  glVertexAttribPointer(shaderAttributeColor, ColorRGB::SIZE, GL_FLOAT, GL_FALSE, 0, NULL);
}

// Get all space objects coordinates and transfer them to Geometry VBO then bind shader "position" parameter to this buffer
void Space::prepareGeomVBO(const unsigned int& shaderAttributeGeom) {
  int vertexCount = getAllVertexCount();
  
  float rawGeometry[vertexCount * Vector::Length];

  int offset = 0;
  auto shipG = _ship->getCurrentGeometry().flat();
  // for all ship points coordinates
  for (int i = 0; i < shipG.getSize(); i++) {
    rawGeometry[offset++] = shipG[i];
  }
  
  // for all plasmoids
  for (auto& p : _plasmoids) {
    auto plasmoidG = p->getCurrentGeometry().flat();
    // for it's all points coordinates
    for (int i = 0; i < plasmoidG.getSize(); i++) {
      rawGeometry[offset++] = plasmoidG[i];
    }
  }

  // for all asteroids
  for (auto& a : _asteroids) {
    auto asteroidG = a->getCurrentGeometry().flat();
    // for it's all points coordinates
    for (int i = 0; i < asteroidG.getSize(); i++) {
      rawGeometry[offset++] = asteroidG[i];
    }
  }
  
  if (!_geomVboID) glGenBuffers(1, &_geomVboID);
  glBindBuffer(GL_ARRAY_BUFFER, _geomVboID);
  // transfer new data to VBO
  glBufferData(GL_ARRAY_BUFFER, vertexCount * Vector::Length * sizeof(float), rawGeometry, GL_DYNAMIC_DRAW);

  // point the shader's "position" attribute to this buffer
  glVertexAttribPointer(shaderAttributeGeom, Vector::Length, GL_FLOAT, GL_FALSE, 0, NULL);
}


/**
* Collect ALL space objects colors and geometries, transfer them to VBO.
* And draw all scene.
* FYI we use only two VBO (color and geometry) for all objects.
*/
void Space::draw() {
  if (!_shader) {
    prepareShader();
    glEnableVertexAttribArray(_shaderAttributeColor);
    glEnableVertexAttribArray(_shaderAttributePosition);
  }
  
  // set view matrix one time for all objects
  glUniformMatrix3fv(_viewMatrixLocation, 1, 0, _viewMatrix.flat().getArrayC());
  
  prepareColorVBO(_shaderAttributeColor);
  prepareGeomVBO(_shaderAttributePosition);
  
  int offset = 0;
  // draw the ship
  glDrawArrays(GL_TRIANGLES, offset, Ship::SIZE);
  offset += Ship::SIZE;

  // draw plasmoids
  glDrawArrays(GL_POINTS, offset, _plasmoids.size() * Plasmoid::SIZE);
  offset += _plasmoids.size() * Plasmoid::SIZE;

  // draw asteroids
  for (int i = 0; i < _asteroids.size(); i++) {
    glDrawArrays(GL_LINE_LOOP, offset, Asteroid::SIZE);
    offset += Asteroid::SIZE;
  }
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
  // add random asteroids
  // TODO generate them with time interval
  for (int i = 0; i < 2; i++) {
    // rand position
    int xRand = rand() % 520;
    int yRand = rand() % 320;

    // rand velocity
    int xVelocityRand = rand() % 2;
    int yVelocityRand = rand() % 2;
    Asteroid *a = new Asteroid(2,
        SpaceArchitect::generateAsteroid(SpaceArchitect::ASTEROID_BIG_RADIUS),
        ColorRGB(.5, .5, .5),
        Vector(xRand, yRand));
    a->setVelocity(Vector(xVelocityRand, yVelocityRand, 0));
    a->setAngularFrequencyRadians(rand() % 2 * DegreesToRadians);

    _asteroids.push_front(std::unique_ptr<Asteroid>(a));
  }
}

// return count of ALL space objects
int Space::getObjCount() const {
  // 1 for ship
  return 1 + _plasmoids.size() + _asteroids.size();
}

int Space::getAllVertexCount() const {
  return Ship::SIZE + _plasmoids.size() * Plasmoid::SIZE + _asteroids.size() * Asteroid::SIZE;
}

// move ship by player
void Space::moveShip(float dx, float dy, float curAngle) {
  _ship->setVelocity(Vector(dx, dy, 0));
  _ship->setAngleInRadians(curAngle);
}

// set space canvas size
void Space::setSize(int width, int height) {
  _viewMatrix = prepareViewMatrix(width, height);
  // set space bounds
  //  _bounds = Rectangle(Vector(-width/2.0, height/2.0), Vector(width/2.0, -height/2.0));
  _bounds = Rectangle(Vector(-width, height), Vector(width, -height));
  // TODO maybe place this outside
  glViewport(0, 0, width, height);
}

// a trigger on an attack command from player
void Space::shipAttack() {
  _plasmoids.push_front(std::move(_ship->piffPaff()));
}

void Space::setListener(std::function<void (SpaceEvent)> l) {
  onSpaceEvent = l;
}
