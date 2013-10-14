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

#include "LAlgebra.h"
#include "Drawable.h"
#include <list>
#include <memory>
#include <iostream>

// Forward declarations

class Shader;

class Ship;

class Plasmoid;

class Asteroid;

class BigBang;

enum class SpaceEvent {
  SHIP_CRASH,
  // when plasmoid split asteroid
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
  void placeBomb();

  // Drawable interface
  virtual void draw() override;
  virtual void update(float msSinceLastUpdate) override;
  virtual void setSize(int width, int height) override;
  
  // return count of ALL space objects vertexes
  int getAllVertexCount() const;
  
  // set external listener for space events
  void setListener(std::function<void(SpaceEvent)> l);

protected:
  lalgebra::Matrix prepareViewMatrix(const int resolutionWidth, const int resolutionHeight);
  // prepare shader for drawing
  void prepareShader();
  void compileShader();
  void useShader();
  // prepare buffers and bind them to shader input
  void prepareColorVBO(const unsigned int& shaderAttributeColor);
  void prepareGeomVBO(const unsigned int& shaderAttributePosition);

private:
  // space objects
  std::unique_ptr<Ship> _ship;
  std::list<std::unique_ptr<Plasmoid>> _plasmoids;
  std::list<std::unique_ptr<Asteroid>> _asteroids;
  std::unique_ptr<BigBang> _bomb;

  // space bounds
  lalgebra::Rectangle _bounds;
  // ship bounds. It's larger than space for some game logic (fe asteroids should be generated outside this field,
  // bonus can be generated in this field)
  lalgebra::Rectangle _shipExtraBounds;
  // model view transformation ()
  lalgebra::Matrix _viewMatrix;
  // listener on internal space event such as asteroid collision, space collision and etc
  std::function<void(SpaceEvent)> onSpaceEvent;
  
  std::unique_ptr<Shader> _shader;
  
  // use this index to pass view matrix to shader
  unsigned int _viewMatrixLocation;
  
  // get them from shader and use for binding VBOs to them
  unsigned int _shaderAttributePosition;
  unsigned int _shaderAttributeColor;
  
  // VBO indexes
  unsigned int _geomVboID;
  unsigned int _colorVboID;
};

#endif /* SPACE_H */


