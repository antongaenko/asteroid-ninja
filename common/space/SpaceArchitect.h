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




#ifndef __SpaceDesigner_H_
#define __SpaceDesigner_H_

#include "LAlgebra.h"

using namespace lalgebra;


// TODO make config
class SpaceArchitect {
  public:
  // This parameters show which resolution we use for geometry.
  // It helps us to scale geometry to device specific resolution.
  //
  // fe game object with size 10х10px for default resolution (fe 800x480) should be
  // 13х19px for resolution 1024x768 without kept proportions
  // 13х13px (or 15х15 or 19х19 for choiсe) for resolution 1024x768 with kept proportions
  static const int GRAPHIC_RESOLUTION_WIDTH = 800;
  static const int GRAPHIC_RESOLUTION_HEIGHT = 480;
  // ship laser tower position
  static Vector PLASMOID_TURRET_POS;
  static Vector LASER;
  static Vector SHIP[3];

  static Vector PLASMOID_VELOCITY;
  static const int SHIP_VELOCITY_MAX;

  static const int DESIGN_FPS;

  static const int ASTEROID_VERTEX_COUNT_MIN;
  static const int ASTEROID_VERTEX_COUNT_MAX;
  static const int ASTEROID_BIG_RADIUS;
  static const int ASTEROID_SMALL_RADIUS;

  static const int ASTEROID_VELOCITY_MAX;
  static const int ASTEROID_ANGULAR_FREQUENCY_DEGREE_MAX;

  static const int ASTEROID_BIG_HITS;
  static const int ASTEROID_SMALL_HITS;

  static Geometry generateAsteroid(const int maxRadius);

};



#endif //__SpaceDesigner_H_
