/*
 Copyright (C) 2011 by Carsten Haubold
 
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

#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>

//our shader class encapsulates all methods needed for loading and compiling shaders from a file
class Shader
{
public:
    
    //create a shader with these fragment and vertex shaders
    Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

    //destructor just calls cleanup()
    ~Shader();
    
    //this methods invokes loading the files from disk, compiles the two shaders 
    //and combines them to one shader program.
    //returns false if errors occurred
    bool compileAndLink();
    
    //returns the ID of the shader program which is needed to bind it etc
    unsigned int getProgram();
    
private:
    
    //cleans up the shaders, deletes them in the OpenGL context
    void cleanup();
    
    //loads a file and compiles it to either a GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
    unsigned int loadAndCompileShaderFile(GLenum type, const std::string& filename);
    
    //links the program
    bool linkProgram();
    
    //---------------------------------------------------------
    //ids for the vertex and fragment shaders, and the final shader program
    unsigned int m_fragmentShader;
    unsigned int m_vertexShader;
    unsigned int m_shaderProgram;
    
    //store the filenames for the shaders until we actually load them
    std::string m_fragmentShaderFilename;
    std::string m_vertexShaderFilename;
};

#endif
