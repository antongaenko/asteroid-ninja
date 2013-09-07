//
//  AppDelegate.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//


#import <UIKit/UIKit.h>

#import <OpenGLES/ES2/gl.h>

//forward declarations again
@class EAGLContext;
@class CAEAGLLayer;
class Game;

// This class combines our OpenGL context (which is our access to all drawing functionality)
// with a UIView that can be displayed on the iOS device. It handles the creation and presentation
// of our drawing surface, as well as handling the render loop which allows for seamless animations.
@interface MainView2D : UIView {
@private
    // The pixel dimensions of the OpenGL layer.
    GLint _frameBufferWidth;
    GLint _frameBufferHeight;
    
    CAEAGLLayer *_glLayer;
    EAGLContext *_context;
    
    GLuint _frameBuffer, _colorRenderBuffer;
    
    // The display link is used to create a render loop
    CADisplayLink* _displayLink;
    
    // The pointer to the lesson which we're rendering
    Game* _game;

}

// TODO should it be public?
//@property (nonatomic, retain) EAGLContext *context;

// TODO should it be public?
- (void) startRenderLoop;
- (void) stopRenderLoop;
@end
