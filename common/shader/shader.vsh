//
//  Shader.vsh
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

// model view transformation
uniform highp mat3 viewMatrix;

//the incoming vertex' position
attribute vec3 position;

//and its color
attribute vec3 color;

//the varying statement tells the shader pipeline that this variable
//has to be passed on to the next stage (so the fragment _shader)
varying lowp vec3 colorVarying;


//the _shader entry point is the main method
void main()
{
    // for ship plasmoids
    gl_PointSize = 2.0;
    colorVarying = color; //save the color for the fragment shader
    // we use Vector(x,y,w) so we need use W here to prevent Z scaling and set output W to 1.0
    vec3 tp = position * viewMatrix;
    gl_Position = vec4(tp.x / tp.z, tp.y / tp.z, 0.0, 1.0);
}
