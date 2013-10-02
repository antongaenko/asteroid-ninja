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

#ifndef SPACE_H
#define SPACE_H

// TODO Try without SpaceObject
#include "SpaceObject.h"
#include "Math2D.h"
#include "Drawable.h"
#include <list>
#include <memory>
#include <iostream>

// Forward declarations

class Shader;

class Ship;

class Plasmoid;

class Asteroid;

enum class SpaceEvent {
  SHIP_CRASH,
  ASTEROID_CRACK,
  ASTEROID_DESTROYED
};


/**
 * Main container and printer for all game objects.
 * It rules who, when and how.
 */
class Space : public Drawable {

public:
  Space();
  ~Space();
  
  // external instructions to move or to attack
  void moveShip(float dx, float dy, float curAngle);
  void shipAttack();

  virtual void draw() override;
  virtual void update(float msSinceLastUpdate) override;
  virtual void setSize(int width, int height) override;
  
  // set external listener for space events
  void setListener(std::function<void(SpaceEvent)> l);

protected:
  Matrix prepareViewMatrix(const int resolutionWidth, const int resolutionHeight);
  // prepare shader for drawing
  SpaceObjectShaderConf prepareShader();
  void compileShader();
  SpaceObjectShaderConf useShader();

private:
  std::unique_ptr<Ship> _ship;
  std::list<std::unique_ptr<Plasmoid>> _plasmoids;
  std::list<std::unique_ptr<Asteroid>> _asteroids;
  std::unique_ptr<Shader> _shader;
  SpaceObjectShaderConf _shaderConf;
  // for shader
  unsigned int _viewMatrixLocation;
  Rectangle _bounds;
  // model view transformation ()
  Matrix _viewMatrix;
  // listener on internal space event such as asteroid collision, space collision and etc
  std::function<void(SpaceEvent)> onSpaceEvent;
};

#endif /* SPACE_H */


