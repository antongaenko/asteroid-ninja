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
#import "Game.h"
#include "SpacePainter.h"
#import "Logger.h"

// TODO move it to cpp and cut all Objective C code

const GLfloat shipGeometry[] = {
        -11.5, -0.5,
        0.5, -0.5,
        0.0, 0.5
};

const GLfloat shipColor[] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
};

//Game constructor, set default values
Game::Game():
    _renderbufferWidth(0),
    _renderbufferHeight(0),
    _geometryBuffer(0),
    _colorBuffer(0),
    _shader(NULL)
{

}

//Game destructor
Game::~Game()
{
    //cleanup here
}

//save the renderbuffer size in the member variables
void Game::setRenderbufferSize(unsigned int width, unsigned int height)
{
    info("set render buffer size");
    _renderbufferWidth = width;
    _renderbufferHeight = height;


    glViewport(0, 0, _renderbufferWidth, _renderbufferHeight);
}

void Game::init()
{

    info("Game init...");

    //set the color we use for clearing our _colorRenderbuffer to black
    //glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_CULL_FACE);

    //---------------------------------------------------
    //create a triangle
    //generate an ID for our geometry buffer in the video memory and make it the active one
    /*glGenBuffers(1, &_geometryBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _geometryBuffer);

    debug("ship geometry size %d", sizeof(shipGeometry));
    debug("glfloat size %d", sizeof(GLfloat));
    //send the data to the video memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipGeometry), shipGeometry, GL_STATIC_DRAW);

    //generate an ID for the color buffer in the video memory and make it the active one
    glGenBuffers(1, &_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);

    //send the data to the video memory
    debug("ship color size %d", sizeof(shipGeometry));
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipColor), shipColor, GL_STATIC_DRAW);*/

    //---------------------------------------------------
    //load our shader


    info("init space");
    ViewportConfig vp = {_renderbufferWidth,_renderbufferHeight,0.2,false};
    _spacePainter = new SpacePainter(vp);

    // TODO code here glError checking
}

//drawing a frame
void Game::draw()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // TODO place here framerate-free animation code
    _spacePainter->draw();






}