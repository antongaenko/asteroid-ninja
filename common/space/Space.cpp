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
#include "Logger.h"

#include <algorithm>
#include <iostream>
#include <cctype>
#include <functional>

// generate asteroid, prepare it position, velocity and anglular frequency
// we should avoid ship bounds to prevent early game over
Asteroid* prepareAsteroid(int hits, const Rectangle shipBounds, const Rectangle spaceBounds, const Asteroid* from = NULL) {
  int xPos, yPos;

  if (from) {
    xPos = from->getPosition().getX();
    yPos = from->getPosition().getY();
  } else {
    // random position
    xPos = spaceBounds.getWidth() - (rand() % (spaceBounds.getWidth() * 2));
    yPos = spaceBounds.getHeight() - (rand() % (spaceBounds.getHeight() * 2));
  }

  // if it intersects ship area we will teleport it to space border
  if (shipBounds.isInside({xPos, yPos})) {
    yPos = spaceBounds.getBottomRight().getY();
  }

  Asteroid *a = new Asteroid(hits,
      SpaceArchitect::generateAsteroid(from ? SpaceArchitect::ASTEROID_SMALL_RADIUS : SpaceArchitect::ASTEROID_BIG_RADIUS),
      ColorRGB(.3, .3, .3),
      Vector(xPos, yPos));

  // random velocity
  int velocityX = 1 + rand() % (SpaceArchitect::ASTEROID_VELOCITY_MAX - 1);
  if (flipCoin()) velocityX = -velocityX;
  int velocityY = 1 + rand() % (SpaceArchitect::ASTEROID_VELOCITY_MAX - 1);
  if (flipCoin()) velocityY = -velocityY;

  // random anglular frequency
  float af = (rand() % SpaceArchitect::ASTEROID_ANGULAR_FREQUENCY_DEGREE_MAX + 0.5) * DegreesToRadians;
  if (flipCoin()) af = -af;

  a->setVelocity({velocityX, velocityY});
  a->setAngularFrequencyRadians(af);

  return a;
}

// generate asteroid on space edge
Asteroid* prepareAsteroidOnEdge(int hits, const Rectangle spaceBounds, const Asteroid* from = NULL) {
  return prepareAsteroid(hits, spaceBounds, spaceBounds, from);
}

// TODO Make Enable method which on/off glUseProgram
Space::Space() {
  _ship = std::unique_ptr<Ship>(new Ship({SpaceArchitect::SHIP[0], SpaceArchitect::SHIP[1], SpaceArchitect::SHIP[2]} , kRED, Vector(0, 0)));
}

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
  // for survival mode we change destroyed asteroid with new ones, good luck
  std::vector<Asteroid*> newAsteroids;
  newAsteroids.reserve(5);
  
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
    // check collision with space ship
    if (Collider::isCollision(_ship->getCurrentGeometry(), a->getCurrentGeometry())) {
      _ship->setBumped();
      a->setBumped();
      // place new asteroid on scene for survival mode
      newAsteroids.push_back(prepareAsteroidOnEdge(SpaceArchitect::ASTEROID_BIG_HITS, _bounds));
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
        p->setBumped();
        
        if (a->getHits() <= 1) {
          a->setBumped();
          onSpaceEvent(SpaceEvent::ASTEROID_DESTROYED);
          // make new
          if (flipCoin()) newAsteroids.push_back(prepareAsteroidOnEdge(SpaceArchitect::ASTEROID_BIG_HITS, _bounds));
        } else {
          a->setBumped();
          onSpaceEvent(SpaceEvent::ASTEROID_CRACK);
          // split old for two asteroids
          newAsteroids.push_back(prepareAsteroid(SpaceArchitect::ASTEROID_SMALL_HITS, _shipExtraBounds.translate(_ship->getPosition()), _bounds, a.get()));
          newAsteroids.push_back(prepareAsteroid(SpaceArchitect::ASTEROID_SMALL_HITS, _shipExtraBounds.translate(_ship->getPosition()), _bounds, a.get()));
        }

        // plasmoid can kill only one asteroid
        break;
      }
    }
  }

  // update new asteroids and push them to all friends for drawing
  for (auto a : newAsteroids) {
    a->update();
    _asteroids.push_back(std::unique_ptr<Asteroid>(a));
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
  
  if (_ship->isBumped()) {
    _ship->disableBumped();
    onSpaceEvent(SpaceEvent::SHIP_CRASH);
  }
}

// set raw color by space object colors
void fillByColor(const SpaceObject* obj, float* arr, int& offset) {
  // we should set color for all vertices in VBO
  for (int i = 0; i < obj->getVertexCount(); i++) {
    arr[++offset] = obj->getColor().r;
    arr[++offset] = obj->getColor().g;
    arr[++offset] = obj->getColor().b;
  }
}

// set raw geometry by space object geometry
void fillByGeom(const SpaceObject* obj, float* arr, int& offset) {
  auto soGeom = obj->getCurrentGeometry().flat();
  for (auto& c : soGeom) arr[offset++] = c;
}

// collect colors for ALL vertexes for ALL space objects
void Space::prepareColorVBO(const unsigned int& shaderAttributeColor) {
  int vertexCount = getAllVertexCount();
  float colors[vertexCount * 3];
  // we increment offset on each step so we set -1 to be 0 on first iteration
  int offset = -1;
  // get ship color for all vertices
  fillByColor(_ship.get(), colors, offset);
  
  // get plasmoid color for ALL plasmoids
  for (auto &p : _plasmoids) fillByColor(p.get(), colors, offset);
  
  // get asteroid color for ALL asteroids
  for (auto &a : _asteroids) fillByColor(a.get(), colors, offset);


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
  // get ship geometry
  fillByGeom(_ship.get(), rawGeometry, offset);
  
  // for all plasmoids
  for (auto& p : _plasmoids) fillByGeom(p.get(), rawGeometry, offset);

  // for all asteroids
  for (auto& a : _asteroids) fillByGeom(a.get(), rawGeometry, offset);

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

  std::vector<float> flatM = _viewMatrix.flat();
  // set view matrix one time for all objects
  glUniformMatrix3fv(_viewMatrixLocation, 1, 0, &flatM[0]);
  
  if (!_colorVboID || !_geomVboID) {
    unsigned int _vboIDs[2];
    glGenBuffers(2, _vboIDs);
    _geomVboID = _vboIDs[0];
    _colorVboID = _vboIDs[1];
  }

  prepareColorVBO(_shaderAttributeColor);
  prepareGeomVBO(_shaderAttributePosition);
  
  int offset = 0;
  // draw the ship
  glDrawArrays(GL_TRIANGLES, offset, _ship->getVertexCount());
  offset += _ship->getVertexCount();
  
  // draw plasmoids
  for (auto& p : _plasmoids) {
    glDrawArrays(GL_POINTS, offset, p->getVertexCount());
    offset += p->getVertexCount();
  }

  // draw asteroids
  for (auto& a : _asteroids) {
    glDrawArrays(GL_TRIANGLE_FAN, offset, a->getVertexCount());
    offset += a->getVertexCount();
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

int Space::getAllVertexCount() const {
  int count = _ship->getVertexCount();
  for (auto& p : _plasmoids) count += p->getVertexCount();
  for (auto& a : _asteroids) count += a->getVertexCount();
  return count;
}

// move ship by player
void Space::moveShip(float dx, float dy, float curAngle) {
  _ship->setVelocity(Vector(dx, dy));
  _ship->setAngleInRadians(curAngle);
}

// set space canvas size
void Space::setSize(int width, int height) {
  _viewMatrix = prepareViewMatrix(width, height);
  // set space bounds
  _bounds = Rectangle(Vector(-width, height), Vector(width, -height));
  // ship extra bounds is a quarter of space bounds
  _shipExtraBounds = Rectangle(_bounds.getTopLeft() * ScaleMatrix(.25,.25), _bounds.getBottomRight() * ScaleMatrix(.25,.25));
  // TODO maybe place this outside
  glViewport(0, 0, width, height);

  // also add random asteroids
  for (int i = 0; i < 10; i++) {
    _asteroids.push_front(std::unique_ptr<Asteroid>(prepareAsteroid(SpaceArchitect::ASTEROID_BIG_HITS, _shipExtraBounds, _bounds)));
  }
}

// a trigger on an attack command from player
void Space::shipAttack() {
  _plasmoids.push_front(std::move(_ship->piffPaff()));
}

void Space::setListener(std::function<void (SpaceEvent)> l) {
  onSpaceEvent = l;
}
