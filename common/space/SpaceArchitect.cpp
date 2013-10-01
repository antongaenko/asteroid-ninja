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



#import <stdlib.h>
#include "SpaceArchitect.h"

Vector SpaceArchitect::PLASMOID_TURRET_POS = Vector(0, 20);

Vector SpaceArchitect::LASER[1] = {PLASMOID_TURRET_POS};

Vector SpaceArchitect::SHIP[3] = {PLASMOID_TURRET_POS, Vector(-20,-20),Vector(20,-20)};

// TODO It should depend on Game FPS
Vector SpaceArchitect::PLASMOID_VELOCITY = Vector(0, 15, 0);

Geometry<float, SpaceArchitect::ASTEROID_VERTEX_COUNT> SpaceArchitect::generateAsteroid(const int maxRadius) {
  Geometry<float, ASTEROID_VERTEX_COUNT> newGeom;
  // random initial radius
  int initialRadius = rand() % (int)(maxRadius * .5) + (int)(maxRadius * .5);
  float angleDiff = 2 * PI / ASTEROID_VERTEX_COUNT;
  debug("initial radius %d", initialRadius);
  debug("angle diff %f", angleDiff);
  // set first vertex
  newGeom[0] = Vector(0, initialRadius, 1);
  newGeom[1] = newGeom[0] * RotateMatrix(90, Degree);
  newGeom[2] = newGeom[0] * RotateMatrix(270, Degree);
    
  // then rotate it to produce all vertexes
  /*for (int i = 1; i < ASTEROID_VERTEX_COUNT; i++) {
    newGeom[i] = newGeom[i-1] * RotateMatrix(angleDiff);
  }*/

  return newGeom;
}
