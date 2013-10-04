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




#ifndef __Collider_H_
#define __Collider_H_

#include "Math2D.h"

using namespace math2d;

class Collider {
public:
  // Check the orientation of each point of first polygon relatively each side of second polygon.
  // If we find any point which have equal orientation with the center of second polygon for ALL sides
  // than we have a collision.
  // Also there is an exceptional case when a point can lay on a side of second polygon.
  // We consider it.
  template<int SizeFirst, int SizeSecond>
  static bool isCollision(const Geometry<float, SizeFirst> &what, const Geometry<float, SizeSecond> &with) {
    Vector center = getCentroid(with);
    
    int centerCP, pointCP;
    bool isInside = true;
    // for all vertices of FIRST polygon
    for (int i = 0; i < SizeFirst; i++) {
      isInside = true;
      // for all vertices of SECOND polygon
      for (int k = 0; k < SizeSecond; k++) {
        centerCP = crossProduct2D(with[k % SizeSecond], with[(k+1) % SizeSecond], center);
        pointCP = crossProduct2D(with[k % SizeSecond], with[(k+1) % SizeSecond], what[i]);
        // if for ANY side we have different signs (orientation) than this point is outside of polygon
        if (centerCP * pointCP < 0) {
          isInside = false;
          break;
        }
      }
      // if any point is inside polygon then there is a collision
      if (isInside) {
        break;
      }
    }
    
    return isInside;
  }
};


#endif //__Collider_H_
