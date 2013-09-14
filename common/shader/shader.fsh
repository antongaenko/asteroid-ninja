//
//  Shader.fsh
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

//incoming values from the vertex shader stage.
//if the vertices of a primitive have different values, they are interpolated!
varying lowp vec3 colorVarying;

void main()
{
    //create a vec4 from the vec3 by passing a 1.0 for alpha
    //and assign that color to be this fragment's color
    gl_FragColor = vec4(colorVarying, 1.0);
}
