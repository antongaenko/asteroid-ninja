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

#include "Shader.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

//constructor
//initializes our member variables with default values and stores the shader file names
Shader::Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename):
    m_fragmentShader(0), m_vertexShader(0), m_shaderProgram(0),
    m_fragmentShaderFilename(fragmentShaderFilename),
    m_vertexShaderFilename(vertexShaderFilename)
{
    
}

//destructor just calls cleanup
Shader::~Shader()
{
    cleanup();
}

//frees the OpenGL IDs for the shaders if they are present
void Shader::cleanup()
{
    if (m_vertexShader) {
		glDeleteShader(m_vertexShader);
		m_vertexShader = 0;
	}
	if (m_fragmentShader) {
		glDeleteShader(m_fragmentShader);
		m_fragmentShader = 0;
	}
	if (m_shaderProgram) {
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}
}

//returns the shader program id
unsigned int Shader::getProgram()
{
    return m_shaderProgram;
}

//this methods invokes loading the files from disk, compiles the two shaders 
//and combines them to one shader program.
//returns false if errors occurred
bool Shader::compileAndLink()
{
    //create the shader program
    m_shaderProgram = glCreateProgram();
    
    //load and compile the vertex and fragment shaders:
    m_vertexShader = loadAndCompileShaderFile(GL_VERTEX_SHADER, m_vertexShaderFilename);
    
    m_fragmentShader = loadAndCompileShaderFile(GL_FRAGMENT_SHADER, m_fragmentShaderFilename);
    
    //check for errors
    if(m_vertexShader == 0 || m_fragmentShader == 0)
    {
        NSLog(@"Error while compiling shaders");
        cleanup(); // destroy everything that we allocated as it is not valid
        return false; //if one of the shaders didn't load or compile, we return false
    }
    
    //attach the vertex and fragment shader to our shader program
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    
    //link the program to make it ready for use
    if(!linkProgram())
    {
        //if something went wrong, cleanup and return false
        cleanup();
        return false;
    }
    
    //if we reach this point, then the program was successfully created
    return true;
}

unsigned int Shader::loadAndCompileShaderFile(GLenum type, const std::string& filename) {
    //separate extension and filename:
    std::string baseFileName = filename.substr(0, filename.find_last_of('.'));
    std::string extension = filename.substr(filename.find_last_of('.') + 1, filename.length()-1);
    
	NSString *fn = [[NSString alloc] initWithCString:baseFileName.c_str() encoding:NSUTF8StringEncoding];
    NSString *ext = [[NSString alloc] initWithCString:extension.c_str() encoding:NSUTF8StringEncoding];

    NSString *path = [[NSBundle mainBundle] pathForResource:fn ofType:ext];
    
    //load 
    const GLchar *sources = (GLchar *)[[NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    //make sure that we were able to load the source code
	if (!sources)
	{
		NSLog(@"Failed to load shader: %s", filename.c_str());
		return false;
	}
	
    //create shader
    GLuint shader = glCreateShader(type);
    
    //specify the loaded source code for this shader
    glShaderSource(shader, 1, &sources, NULL);
    
    //compile the shader
    glCompileShader(shader);
	
    //now we check for any errors
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) //we had errors
	{
		NSLog(@"Failed to compile shader: %s", filename.c_str());
        
        //OpenGL provides us with an info log, which contains the compile errors
        //we first have to query its length and then allocate memory for the log message
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0)
        {
            //allocate memory for the log
            unsigned char *log = new unsigned char[logLength];
            
            //retrieve the log
            glGetShaderInfoLog(shader, logLength, &logLength, (GLchar*)&log[0]);
            
            //print it to the console
            NSLog(@"\n%s", log);
            
            //free the allocated memory
            delete log;
        }
        
        //return an invalid shader id to show that something went wrong
        return 0;
	}
	
    //no errors -> return the shader
	return shader;
}

// link the shader program (binding the vertex and fragment shader together)
// so we can use it for rendering
bool Shader::linkProgram() {
	//link the program, which makes sure a vertex and fragment shader are present
    glLinkProgram(m_shaderProgram);
    
    //check for errors
    GLint status;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
		NSLog(@"Failed to link program");
	
    //return the status which is true if everyhing went well
	return status;
}