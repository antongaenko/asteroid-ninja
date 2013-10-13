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



#include <stdlib.h>
#include "SpaceArchitect.h"
#include "Logger.h"
#include <ctime>

Vector SpaceArchitect::PLASMOID_TURRET_POS = {0, 30};

Vector SpaceArchitect::LASER = PLASMOID_TURRET_POS;

Vector SpaceArchitect::SHIP[3] = {PLASMOID_TURRET_POS, {-20,-30}, {20,-30}};

// TODO It should depend on Game FPS
Vector SpaceArchitect::PLASMOID_VELOCITY = {0, 30};

const int SpaceArchitect::SHIP_VELOCITY_MAX = 8;

const int SpaceArchitect::DESIGN_FPS = 30;

const int SpaceArchitect::ASTEROID_VERTEX_COUNT_MIN = 10;
const int SpaceArchitect::ASTEROID_VERTEX_COUNT_MAX = 20;
const int SpaceArchitect::ASTEROID_BIG_RADIUS = 80;
const int SpaceArchitect::ASTEROID_SMALL_RADIUS = 40;
const int SpaceArchitect::ASTEROID_VELOCITY_MAX = 10;
const int SpaceArchitect::ASTEROID_ANGULAR_FREQUENCY_DEGREE_MAX = 5;
const int SpaceArchitect::ASTEROID_BIG_HITS = 2;
const int SpaceArchitect::ASTEROID_SMALL_HITS = 1;

// generate new asteroid geometry
// set initial point and rotate it's vector to produce new polygon points
Geometry SpaceArchitect::generateAsteroid(const int maxRadius) {
  static bool isSeeding;
  if (!isSeeding) {
    srand(time(0));
    isSeeding = true;
  }
  
  int vertCount = randInRange(ASTEROID_VERTEX_COUNT_MIN, ASTEROID_VERTEX_COUNT_MAX);

  Geometry newGeom(vertCount);
  // random initial radius
  int initialRadius = maxRadius; //randInRange((int) .8 * maxRadius, maxRadius);
  float angleDiff = 2 * PI / vertCount;
  
  // set first vertex
  newGeom.add({0, initialRadius});
  
  // then rotate it to produce all vertexes
  for (int i = 1; i < vertCount; i++) {
    float randAngleFactor = randInRange(0.7, 1.3);
    float randAngle = angleDiff * randAngleFactor;
    float randRadFactor = randInRange(0.85, 1.15);
    newGeom.add(newGeom[i-1] * RotateMatrix(randAngle) * ScaleMatrix(randRadFactor, randRadFactor));
  }
  
  // we should shift geometry relatively centroid to allow right rotations
  auto c = getCentroid(newGeom);
  newGeom = newGeom.translate(-c.getX(), -c.getY());

  auto v = newGeom.flat();

  return newGeom;
}
