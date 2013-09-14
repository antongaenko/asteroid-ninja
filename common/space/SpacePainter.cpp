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

#include "OpenGL.h"
#include "SpacePainter.h"
#import "Logger.h"
#import "Shader.h"

/**
* Check graphic errors
*/
void checkGlErrors() {
    unsigned int err = glGetError();
    if (err) error("space draw GLerror(%d)", err);
}

/**
* Draw the scene and all space objects
*/
void SpacePainter::draw() {
    //bind the geometry VBO
    glBindBuffer(GL_ARRAY_BUFFER, _ship->getGeomBufferID());
    //point the position attribute to this buffer, being tuples of 4 floats for each vertex
//    glVertexAttribPointer(_positionLocation, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(_positionLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    //bind the color VBO
    glBindBuffer(GL_ARRAY_BUFFER, _ship->getColorBufferID());
    //this attribute is only 3 floats per vertex
//    glVertexAttribPointer(_colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(_colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    float matrix[4];
    _modelView.flat(matrix);
    glUniformMatrix2fv(_mvLocation, 1, 0, matrix);

    //initiate the drawing process, we want a triangle, start at index 0 and draw 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // TODO Use indicies and experiment with GL_LINES and LINES_STRIP
    checkGlErrors();
}

SpacePainter::~SpacePainter() {
    delete _ship;
}

// TODO code clean method and clean shaders

/**
* Setup vertex and fragment shaders
* TODO Split to different functions
*/
void SpacePainter::setShaderProgram() {
    _shader = new Shader("shader.vsh", "shader.fsh");

    if(!_shader->compileAndLink())
    {
        error("Encountered problems when loading shader, application will crash...");
    }

    //tell OpenGL to use this shader for all coming rendering
    glUseProgram(_shader->getProgram());

    //get the attachment points for the attributes position and color
    int resultPosAllocation = glGetAttribLocation(_shader->getProgram(), "position");
    int resultColorAllocation = glGetAttribLocation(_shader->getProgram(), "color");
    int resultModelViewAllocation = glGetUniformLocation(_shader->getProgram(), "modelView");

    //check that the locations are valid, negative value means invalid
    if(resultPosAllocation < 0 || resultColorAllocation < 0)
    {
        error("Could not query attribute locations");
    } else {
        _positionLocation = (GLuint) resultPosAllocation;
        _colorLocation = (GLuint) resultColorAllocation;
        _mvLocation = (GLuint) resultModelViewAllocation;
    }

    // TODO place it with above code into a transaction, because it's unsafe cast
    glEnableVertexAttribArray(_positionLocation);
    glEnableVertexAttribArray(_colorLocation);
}

// TODO Make Enable method which on/off glUseProgram

SpacePainter::SpacePainter(ViewportConfig& viewportConfig):
    _viewportConfig(viewportConfig) {
    _modelView.setIdentity();

    setShaderProgram();

    // TODO place it in SpaceDesigner
    Point2D shipGeometry[3] = {
            {-20, -20},
            {20, -20},
            {0.0, 20}};

    unsigned int _shipGeometryBufID;
    glGenBuffers(1, &_shipGeometryBufID);
    glBindBuffer(GL_ARRAY_BUFFER, _shipGeometryBufID);
    _ship = new Ship(_shipGeometryBufID,shipGeometry,3);
    // size = XY * 3 points * sizeof(float)

    // TODO Make sure you also do it with resolution scale factor
    float flatShipGeometry[_ship->getFlatGeomSize()];
    _ship->getFlatGeometryWithScale(flatShipGeometry, 1.0 / _viewportConfig.width, 1.0 / _viewportConfig.height);
    debugArray("flat geometry", flatShipGeometry, 6, 2);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), flatShipGeometry, GL_STATIC_DRAW);

    unsigned int _shipColorBufID;
    glGenBuffers(1, &_shipColorBufID);
    glBindBuffer(GL_ARRAY_BUFFER, _shipColorBufID);
    _ship->setColor(_shipColorBufID, kRED);
    float flatColorArr[_ship->getFlatColorSize()];
    _ship->getFlatColorArray(flatColorArr);
    // size = RGB * 3 points * sizeof(float)
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), flatColorArr, GL_STATIC_DRAW);


}