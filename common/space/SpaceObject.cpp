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
#include "Logger.h"

/**
* SpaceObjectShaderConf definition
*/
SpaceObjectShaderConf::SpaceObjectShaderConf(const int positionLocation, const int colorLocation):
_positionLocation(positionLocation),
_colorLocation(colorLocation) {}

unsigned int SpaceObjectShaderConf::getPositionLoc() const {
  if (_positionLocation<0) {
    error("position location hasn't been set yet");
    return 0;
  } else {
    return static_cast<unsigned int>(_positionLocation);
  }
}

void SpaceObjectShaderConf::setPositionLoc(const unsigned int value) {
  _positionLocation = value;
}

unsigned int SpaceObjectShaderConf::getColorLoc() const {
  if (_colorLocation<0) {
    error("color location hasn't been set yet");
    return 0;
  } else {
    return static_cast<unsigned int>(_colorLocation);
  }
}

void SpaceObjectShaderConf::setColorLoc(const unsigned int value) {
  _colorLocation = value;
}



