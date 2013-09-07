//
//  AppDelegate.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "MainView2D.h"
#import "Game.h"
#import "Logger.h"

//@interface MainView2D (PrivateMethods)
//- (void)createFramebuffer;
//
//- (void)deleteFramebuffer;
//@end


//start the actual implementation of our view here
@implementation MainView2D

//@synthesize context;

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    _frameBufferWidth = (GLint) frame.size.width;
    _frameBufferHeight = (GLint) frame.size.height;

    glDisable(GL_DEPTH_TEST);

    if (self) {
        [self setupLayer];
        [self setupContext];
        [self setupRenderBuffer];
        [self setupFrameBuffer];

        _game = new Game();
        _game->setRenderbufferSize(_frameBufferWidth, _frameBufferHeight);
        _game->init();

        //scene = new StartScene();
    }
    return self;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    _glLayer = (CAEAGLLayer *) self.layer;
    // TODO try it with RGB565
    /*_glLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
            [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
            kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
            nil];*/
    // turn off transparency
    _glLayer.opaque = YES;
}

- (void)setupContext {
    // init with OpenGL ES 2

    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }

    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }

}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_glLayer];
}

- (void)setupFrameBuffer {
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);

    //get the storage from iOS so it can be displayed in the view
    //[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *) self.layer];
    //get the frame's width and height
    //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_framebufferWidth);
    //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_framebufferHeight);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Framebuffer status: %x with error %x", (int)status, glGetError());
    }
}

// render with display update rate
- (void)startRenderLoop {
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

//we have to be able to stop the render loop
- (void)stopRenderLoop {
    if (_displayLink != nil) {
        //if the display link is present, we invalidate it (so the loop stops)
        [_displayLink invalidate];
        _displayLink = nil;
        NSLog(@"Stopping Render Loop");
    }
}

//deleting the framebuffer and all the buffers it contains
- (void)deleteFramebuffer {
    NSLog(@"delete frame buffer");
    //we need a valid and current context to access any OpenGL methods
    if (_context) {
        [EAGLContext setCurrentContext:_context];

        //if the default framebuffer has been set, delete it.
        if (_frameBuffer) {
            glDeleteFramebuffers(1, &_frameBuffer);
            _frameBuffer = 0;
        }

        //same for the renderbuffers, if they are set, delete them
        if (_colorRenderBuffer) {
            glDeleteRenderbuffers(1, &_colorRenderBuffer);
            _colorRenderBuffer = 0;
        }
    }
}


//this is where all the magic happens!
- (void)render:(CADisplayLink*)displayLink {
    glClearColor(0.5, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    _game->draw();
    //scene->render();
    //scene->update(displayLink.duration);
    [_context presentRenderbuffer:GL_RENDERBUFFER];


    //we need a context for rendering
    /*if (_context != nil) {
        // TODO place here init method
        if (!_frameBuffer) [self setupFrameBuffer];

        [_context presentRenderbuffer:GL_RENDERBUFFER];
    } else {
        NSLog(@"Context not set!");
    } */
}

// TODO place here clean code
- (void)layoutSubviews {
    NSLog(@"layout");
    //[self deleteFramebuffer];
    // TODO should we stop render loop here?
}

// TODO place here clean code
- (void)dealloc {
    [self deleteFramebuffer];
    // TODO should we stop render loop here?
}


@end
