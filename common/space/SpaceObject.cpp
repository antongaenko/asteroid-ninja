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

#import <unistd.h>
#include "SpaceObject.h"
#include "Logger.h"


SpaceObject::SpaceObject(unsigned int geomVboID, Point2D geometry[], int countPoints):
_geomVboID(geomVboID),
_countPoints(countPoints),
_color(kBLUE) {
    _geometry = new Point2D[countPoints];
    for (int i=0; i<countPoints; i++) _geometry[i] = geometry[i];
    Point2D initPos = {0, 0};
    _position = initPos;
    debug("init space object with ID(%d)", geomVboID);
}

void SpaceObject::setPosition(Point2D &pos) {
    _position = Point2D(pos);
}

void SpaceObject::setColor(unsigned int colorBufferID, const ColorRGB &color) {
    _colorVboID = colorBufferID;
    _color = ColorRGB(color);
}

// all points in RGB
int SpaceObject::getFlatColorSize() {
    return _countPoints * 3;
}

// each point set by (x,y), so return array is two times longer
int SpaceObject::getFlatGeomSize() {
    return _countPoints * 2;
}

// TODO I think that it's not good idea
Point2D *SpaceObject::getGeometry() {
    return _geometry;
}

unsigned int SpaceObject::getColorBufferID() {
    return _colorVboID;
}

void SpaceObject::getFlatColorArray(float *arrToFill) {
    // go through all points
    for (int i = 0; i < _countPoints; i++) {
        arrToFill[i*3] = _color.r;
        arrToFill[i*3+1] = _color.g;
        arrToFill[i*3+2] = _color.b;
    }
}

SpaceObject::~SpaceObject() {
    delete _geometry;
}

unsigned int SpaceObject::getGeomBufferID() {
    return _geomVboID;
}

void SpaceObject::getFlatGeometryXY(float *arrToFill) {
    getFlatGeometryWithScale(arrToFill, 1, 1);
}

void SpaceObject::getFlatGeometryWithScale(float *arrToFill, float xScale, float yScale) {
    // go through all points
    for (int i = 0; i < _countPoints; i++) {
        // apply scale
        arrToFill[i * 2] = _geometry[i].x * xScale;
        arrToFill[i * 2 + 1] = _geometry[i].y * yScale;
    }
}
