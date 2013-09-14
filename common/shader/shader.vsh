//
//  Shader.vsh
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

// model view transformation
uniform highp mat2 modelView;

//the incoming vertex' position
attribute vec2 position;

//and its color
attribute vec3 color;

//the varying statement tells the shader pipeline that this variable
//has to be passed on to the next stage (so the fragment shader)
varying lowp vec3 colorVarying;


//the shader entry point is the main method
void main()
{
    //gl_PointSize = 5.0;
    colorVarying = color; //save the color for the fragment shader
    gl_Position = vec4(modelView * position, 1.0, 1.0);
    //gl_Position = vec4(position, 1.0, 1.0); //copy the position
}
