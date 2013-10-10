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

#include "LAlgebra.h"

using namespace lalgebra;

// Y - ray intersects the edge
// N - ray DOESN'T intersect the edge
// VERTEX - ray casting intersects edge in vertex
// MEMBER - tested point belongs an edge
enum class Intersection { Y, N, VERTEX, MEMBER};

// Ray-casting algo. It works for convex and non-convex polygons, also with holes.
// If tested point lies on polygon's border then we think about collision.
class Collider {
public:
  // check collision between two geometries
  static bool isCollision(const Geometry &what, const Geometry &with);

  // check collision between point and geometry
  static bool isCollision(const Vector point, const Geometry &with);

  // cast ray from point to positive X axis and check it's intersection with an edge
  static Intersection getIntersectionRayCastingWithEdge(const Vector point, const Vector begin, const Vector end);


};


#endif //__Collider_H_
