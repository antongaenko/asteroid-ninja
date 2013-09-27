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

#import "GameView2D.h"
#import "Game.h"
#import "Logger.h"
#import "Drawable.h"


@implementation GameView2D

- (id)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  if (self) {
    // save this order: get layer, setup context, render buffer and framebuffer
    [self setupLayer];
    [self setupContext];
    [self setupRenderBuffer];
    // we see all objects from above, so we don't need counter-side faces
    glEnable(GL_CULL_FACE);
    // TODO it's 2D view, we don't need this test
    glDisable(GL_DEPTH_TEST);

    [self setupFrameBuffer];
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
    error("OpenGLES 2.0 context initialization failed.");
    exit(1);
  }

  if (![EAGLContext setCurrentContext:_context]) {
    error("OpenGL context setup failed.");
    exit(1);
  }
}

// init color render buffer
- (void)setupRenderBuffer {
  glGenRenderbuffers(1, &_colorRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
  [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_glLayer];
}

// init frame buffer with render buffers
- (void)setupFrameBuffer {
  glGenFramebuffers(1, &_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);

  // get width and height of frame buffer
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_frameBufferWidth);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_frameBufferHeight);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    error("Frame buffer setup failed: %x with error %x", (int) status, glGetError());
  }
}

// delete frame and render buffers
- (void)deleteFrameBuffer {
  info("delete frame buffer");
  //we need a valid and current context to access any OpenGL methods
  if (_context) {
    [EAGLContext setCurrentContext:_context];

    // if we have any buffers
    if (_frameBuffer) {
      glDeleteFramebuffers(1, &_frameBuffer);
      _frameBuffer = 0;
    }

    if (_colorRenderBuffer) {
      glDeleteRenderbuffers(1, &_colorRenderBuffer);
      _colorRenderBuffer = 0;
    }
  }
}

// allow us to setup canvas
- (void)setupDrawable:(Drawable *)canvas {
  canvas->setSize(_frameBufferWidth, _frameBufferHeight);
}

// render itself and delegate rendering to canvas
- (void)renderWithCanvas:(Drawable *)canvas {
  if (_context) {
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO use time sinse last update for correct animation
    canvas->draw(0);

    [_context presentRenderbuffer:GL_RENDERBUFFER];
  } else {
    error("Set context before rendering.");
  }
}

- (void)dealloc {
  [self deleteFrameBuffer];
}


@end
