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

template<int SizeFirst, int SizeSecond>
bool Collider::isCollision(const Geometry<float, SizeFirst> &what, const Geometry<float, SizeSecond> &with) {
  // for all vertices of FIRST polygon
  for (int i = 0; i < SizeFirst; i++) {
    if (isCollision(what[i], with)) return true;
  }
  return false;
}

template<int Size>
bool Collider::isCollision(const Vector point, const Geometry<float, Size> &with) {
  int intersectionCounter = 0;
  float shift = 0;
  // for ALL edges of polygon
  for (int b = 0; b < Size; b++) {
    int e = (b +1) % Size;
    
    int r = isIntersectionRayCastingWithEdge(point + Vector(0, shift), with[b], with[e]);
    // point belong the edge. INSIDE
    if (r == 0) {
      return true;
    } else if (r == 1) {
      intersectionCounter++;
    } else if (r == 2) {
      // special case when ray intersects vertex of polygon. Count and shift ray to prevent intersection with this vertex again.
      intersectionCounter++;
      // we should shift the ray inside the polygon NOT outside
      if (isEqual(point.getY() + shift, with[b].getY())) {
        // we shift ray to with[e]
        if (with[e].getY() > with[b].getY()) shift += FLOAT_COMPARISON_PRECISION * 2;
        else shift -= FLOAT_COMPARISON_PRECISION * 2;
      } else {
        // it means equality with[e], shift to with[b]
        if (with[b].getY() > with[e].getY()) shift += FLOAT_COMPARISON_PRECISION * 2;
        else shift -= FLOAT_COMPARISON_PRECISION * 2;
      }
    }
    // if r < 0 NOTHING
  }
  if (intersectionCounter % 2 == 1) return true;
  else return false;
}

bool Collider::isPointAtLeftOfEdge(Vector point, Vector begin, Vector end) {
  // Here we know that point X is in range of two vertices of the edge.
  // Now we should know if point is at the left OR at the right of edge.
  // It means: do we have INTERSECTION of the ray with the edge or NOT.

  // To know that we can calculate the angle (k) between the edge B-E (where By > Ey) and X axis ...
  // ... and the angle between P-E and X axis (where P is a tested point).

  // calculate B-E-X angle coef
  float edgeAngleCoef = 0;
  if (! isEqual(begin.getX(), end.getX(), FLOAT_COMPARISON_PRECISION)) {
    edgeAngleCoef = (begin.getY() - end.getY()) / (begin.getX() - end.getX());
  } else {
    edgeAngleCoef = MAXFLOAT;
  }

  // calculate P-E-X angle coef
  float pointAngleCoef = 0;
  if (! isEqual(point.getX(), end.getX(), FLOAT_COMPARISON_PRECISION)) {
    pointAngleCoef = (point.getY() - end.getY()) / (point.getX() - end.getX());
  } else {
    pointAngleCoef = MAXFLOAT;
  }

  // If angle (NOT angle coef) P-E-X == B-E-X - point is on the edge and it means INSIDE
  // P-E-X > B-E-X - we have INTERSECTION,
  // if P-E-X < B-E-X - NO,
  if (isEqual(pointAngleCoef, edgeAngleCoef, 1e-2)) {
    return 0;
  } else {
    // compare angle coefficients
    return (edgeAngleCoef >0 && pointAngleCoef < 0) ||
        (edgeAngleCoef >0 && pointAngleCoef > edgeAngleCoef) ||
        (edgeAngleCoef <0 && pointAngleCoef < 0 && pointAngleCoef > edgeAngleCoef);
  }
}

// ray casting from point to edge
int Collider::isIntersectionRayCastingWithEdge(const Vector point, Vector begin, Vector end) {
  if (isEqual(point, begin) || isEqual(point, end)) {
    // if point is equal to vertices of this edge then INSIDE
    return 0;
  }

  // B should be ABOVE E for this algorithm, By > Ey
  if (begin.getY() < end.getY()) {
    Vector t = end;
    end = begin;
    begin = t;
  }

  // cast a ray from tested point to positive X axis
  float rayY = point.getY();

  // if tested point is inside edge Y-projection
  if (isInRange(rayY, begin.getY(), end.getY())) {
    if (point.getX() > math2d::max(begin.getX(), end.getX())) {
      // if tested point is at the right of the edge then NO INTERSECTION
      return -1;
    } else if (isEqual(rayY, begin.getY(), FLOAT_COMPARISON_PRECISION) ||
        isEqual(rayY, end.getY(), FLOAT_COMPARISON_PRECISION)) {
      return 2;
    } else if (point.getX() < math2d::min(begin.getX(), end.getX())) {
      // if tested point is at the left of the edge then INTERSECTION
      return 1;
    } else {
      bool r = isPointAtLeftOfEdge(point, begin, end);
      return r ? 1 : -1;
    }
  }
  return -1;
}

#endif //__Collider_CPP_