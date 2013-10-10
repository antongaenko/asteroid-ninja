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

#ifndef __Collider_CPP_
#define __Collider_CPP_

#include "Collider.h"

bool Collider::isCollision(const Geometry &what, const Geometry &with) {
  // for all vertices of FIRST polygon
  for (int i = 0; i < what.getSize(); i++) {
    if (isCollision(what[i], with)) return true;
  }
  return false;
}

// get the small shift for ray depending on vertex ray intersected before and orientation of begin and end
static float getShiftInsideForPointAndEdge(const float pointY, const Vector begin, const Vector end) {
  if (isEqual(pointY, begin.getY())) {
    // we shift ray to with[e]
    return (end.getY() > begin.getY()) ? FLOAT_COMPARISON_PRECISION * 2 : -FLOAT_COMPARISON_PRECISION * 2;
  } else {
    // it means equality with[e], shift to with[b]
    return (begin.getY() > end.getY()) ? FLOAT_COMPARISON_PRECISION * 2 : -FLOAT_COMPARISON_PRECISION * 2;
  }
}

bool Collider::isCollision(const Vector point, const Geometry &with) {
  int intersectionCounter = 0;
  float shift = 0;

  int size = with.getSize();
  // for ALL edges of polygon
  for (int b = 0; b < size; b++) {
    int e = (b +1) % size;
    
    Intersection r = getIntersectionRayCastingWithEdge(point + Vector(0, shift), with[b], with[e]);
    switch (r) {
      case Intersection::MEMBER:
        // point belong the edge. INSIDE
        return true;
      case Intersection::Y:
        intersectionCounter++;
        break;
      case Intersection::VERTEX:
        // special case when ray intersects vertex of polygon. Count and shift ray to prevent intersection with this vertex again.
        intersectionCounter++;
        // we should shift the ray inside the polygon NOT outside
        shift += getShiftInsideForPointAndEdge(point.getY() + shift, with[b], with[e]);
        break;
      case Intersection::N:
        // nothing, go to the next edge
        break;
    }
  }

  // point is INSIDE if ray from point has intersected the polygon odd times
  return intersectionCounter % 2 == 1;
}

// ray casting from point to edge
Intersection Collider::getIntersectionRayCastingWithEdge(const Vector point, const Vector begin, const Vector end) {
  if (isEqual(point, begin) || isEqual(point, end)) {
    // if point is equal to vertices of this edge then MEMBER
    return Intersection::MEMBER;
  }

  // cast a ray from tested point to positive X axis
  float rayY = point.getY();

  // if tested point is inside edge Y-projection
  if (isInRange(rayY, begin.getY(), end.getY())) {
    if (point.getX() > lalgebra::max(begin.getX(), end.getX())) {
      // if tested point is at the right of the edge then NO INTERSECTION
      return Intersection::N;
    } else if (isEqual(rayY, begin.getY(), FLOAT_COMPARISON_PRECISION) ||
        isEqual(rayY, end.getY(), FLOAT_COMPARISON_PRECISION)) {
      return Intersection::VERTEX;
    } else if (point.getX() < lalgebra::min(begin.getX(), end.getX())) {
      // if tested point is at the left of the edge then INTERSECTION
      return Intersection::Y;
    } else {
      int r = getPointToVectorOrientation(begin, end, point);

      if (r == 0) return Intersection::MEMBER;
      // if the Begin is BELOW the End then point should lie at the LEFT for intersection
      else if (r>0 && (begin.getY() < end.getY())) return Intersection::Y;
      // otherwise at the RIGHT
      else if (r<0 && (begin.getY() > end.getY())) return Intersection::Y;
    }
  }
  return Intersection::N;
}

#endif //__Collider_CPP_