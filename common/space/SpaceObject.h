/* Copyright (C) 2013 by Anton Gaenko
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

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "Math2D.h"

class SpaceObject {
public:
    // TODO It's better to use template for countPoints param if asteroid point count will be static
    SpaceObject(unsigned int geomVboID, Point2D geometry[], int countPoints);
    Point2D* getGeometry();
    unsigned int getGeomBufferID();
    unsigned int getColorBufferID();
    void setColor(unsigned int colorBufferID, const ColorRGB &color);
    // set object to position in 2D space
    void setPosition(Point2D &pos);
    // return flat array in model space (each point set in XY space)
    void getFlatGeometryXY(float *arrToFill);
    // convert points to other view space not model space
    void getFlatGeometryWithScale(float *arrToFill, float xScale, float yScale);
    // size = countPoints * 3 (because RGB)
    void getFlatColorArray(float *arrToFill);
    // return size of flat geometry array, which can hold geometry
    int getFlatGeomSize();
    // return size of flat color array, which can hold geometry
    int getFlatColorSize();
    ~SpaceObject();


private:
    // 2D vertices of object
    Point2D *_geometry; /* Pointer to array of Point2D */
    // count of points in geometry
    int _countPoints;
    // specify that our object geometry is attached to graphic buffer
    unsigned int _geomVboID;
    // specify that our object color is attached to graphic buffer
    unsigned int _colorVboID;
    // rectangle which contains this object geometry
    Rectangle *_bounds;
    // color for all points
    // TODO make sure I use value OR reference for PODs in ALL places (like position and color and Rectangle)
    ColorRGB _color;
    // center of object. It changes each frame.
    Point2D _position;
    // rotation
    float _angle;
    // movement velocity in the SpacePainter
    float _velocity;

};

#endif /* SPACEOBJECT_H */