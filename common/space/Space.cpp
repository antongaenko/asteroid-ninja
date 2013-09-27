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

void Space::compileShader(Shader *shader) {
  bool compilationResult = shader->compileAndLink();
  if (!compilationResult) {
    error("Shader complilation error.");
    exit(1);
  }
}

SpaceObjectShaderConf Space::useShader(Shader *shader) {
  glUseProgram(shader->getProgram());
  SpaceObjectShaderConf conf;

  //get the attachment points for the attributes position and color
  int resultPosAllocation = glGetAttribLocation(shader->getProgram(), "position");
  int resultColorAllocation = glGetAttribLocation(shader->getProgram(), "color");
  int resultViewMatrixAllocation = glGetUniformLocation(shader->getProgram(), "viewMatrix");

  //check that the locations are valid, negative value means invalid
  if (resultPosAllocation < 0 || resultColorAllocation < 0 || resultViewMatrixAllocation < 0) {
    error("Could not query attribute locations");
  } else {
    conf.setPositionLoc(static_cast<unsigned int>(resultPosAllocation));
    conf.setColorLoc(static_cast<unsigned int>(resultColorAllocation));
    // it's for internal use
    _viewMatrixLocation = static_cast<unsigned int>(resultViewMatrixAllocation);
    glEnableVertexAttribArray(conf.getPositionLoc());
    glEnableVertexAttribArray(conf.getColorLoc());
  }

  return conf;
}

SpaceObjectShaderConf Space::prepareShader() {
  _shader = new Shader("shader.vsh", "shader.fsh");
  compileShader(_shader);
  return useShader(_shader);
}


/**
* Draw the scene and all space objects
*/
void Space::draw(float msSinceLastUpdate) {
  if (!_shader) {
    _shaderConf = prepareShader();
  }

  // TODO Add tick() for update and colision detection

  glUniformMatrix3fv(_viewMatrixLocation, 1, 0, _viewMatrix.flat().getArrayC());
  _ship->update();
  _ship->draw(_shaderConf);

  for(std::vector<Plasmoid>::iterator l = _plasmoids.begin(); l != _plasmoids.end(); ++l) {
    l->update();
    l->draw(_shaderConf);
  }
}

Space::~Space() {
  delete _ship;
  delete _shader;
}


Matrix Space::prepareViewMatrix(const int resolutionWidth, const int resolutionHeight) {
  if (resolutionWidth > 0 && resolutionHeight > 0) {
    // x and y factors to normalize object' geometries
    float xScale = 1.0 / SpaceArchitect::GRAPHIC_RESOLUTION_WIDTH;
    float yScale = 1.0 / SpaceArchitect::GRAPHIC_RESOLUTION_HEIGHT;
    // scale factor to screen resolution from design resolution. See Space Architect for more detail
    // TODO Add parameter enum Scale SaveProportion, StretchWidth and etc
    float commonScale = static_cast<float>(resolutionHeight) / SpaceArchitect::GRAPHIC_RESOLUTION_HEIGHT;

    return ScaleMatrix(xScale, yScale, commonScale);
  }
  return Matrix(true);
}

// TODO Make Enable method which on/off glUseProgram
Space::Space(const int resolutionWidth, const int resolutionHeight) {
  _viewMatrix = prepareViewMatrix(resolutionWidth, resolutionHeight);
  _ship = new Ship(SpaceArchitect::SHIP, kRED, Vector(10, 10));
}

void Space::moveShip(float dx, float dy, float curAngle) {
  _ship->setVelocity(Vector(dx, dy, 0));
  _ship->setAngleInRadians(curAngle);
}


void Space::setSize(int width, int height) {
  _viewMatrix = prepareViewMatrix(width, height);
  // TODO maybe place this outside
  glViewport(0, 0, width, height);
}

void Space::shipAttack() {
  _plasmoids.push_back(_ship->piffPaff());
}
