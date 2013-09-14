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

#ifndef MATH2D_H
#define MATH2D_H

// Point in 2D space
// TODO make fields const
typedef struct point {
    float x;
    float y;
} Point2D;

// RGB color
// TODO make fields const
typedef struct colorRGB {
    float r;
    float g;
    float b;
} ColorRGB;

// TODO should it be const?
static const struct colorRGB kBLUE = {0.0, 0.0, 1.0};
static const struct colorRGB kRED = {1.0, 0.0, 0.0};

// Viewport POD configuration for internal game objects and graphics recalculation
typedef struct config {
    const int width;
    const int height;
    // Developer can choose any resolution as default. And all game objects and graphics were calculated for that by designer.
    // We use scale factor for game object's re-calculation to save designed proportion.
    // fe game object with size 10х10px for default resolution (fe 800x480) should be
    // 13х19px for resolution 1024x768 without kept proportions
    // 13х13px (or 15х15 or 19х19 as chose) for resolution 1024x768 with kept proportions
    const float scale;
    const bool isKeptProportion;

} ViewportConfig;

/**
* Matrix 2x2
*/
class Matrix2 {
public:
    Matrix2();
    // make this matrix identity
    void setIdentity();
    // return 4 elements in one-dimension array ([0][0],[0][1],[1][0],[1][1])
    void flat(float *arrayToFill);

    Matrix2 operator *(Matrix2 &on);
    // allow to make [][] direct access
    float* operator [](int index);

private:
    // matrix[row][column]
    float _m[2][2];
};

// Rectangle
class Rectangle {
private:
    Point2D _topleft;
    Point2D _bottomright;
public:
    Rectangle(Point2D &topleft, Point2D &bottomright);

    bool isIntersected(Rectangle &r);

    bool isContained(Point2D &p);

    Point2D &getTopLeft();

    Point2D &getBottomRight();
};

#endif /* MATH2D_H */


