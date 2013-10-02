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

#import "GameViewController.h"
#import "Game.h"
#import "Drawable.h"
#import <AVFoundation/AVFoundation.h>

@interface GameViewController ()

@end

@implementation GameViewController

- (id)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  if (self) {
    _game = new Game();
    _game->setListener([self](GameEvent e) {
      switch (e) {
        case GameEvent::SCORE_CHANGES:
          scoreField.text = [NSString stringWithFormat:@"%d", _game->getCurrentScore()];
          break;
          
        case GameEvent::SHIP_CRASH:
          [self playForKey:@"crash" andPath:[[NSBundle mainBundle] pathForResource:@"crash" ofType:@"m4a"]];
          break;
        
        case GameEvent::ASTEROID_BANG:
          [self playForKey:@"asteroid_bang" andPath:[[NSBundle mainBundle] pathForResource:@"asteroid_bang" ofType:@"m4a"]];
          break;
          
        default:
          break;
      }
      
       });

    _sounds = [[NSCache alloc] init];
  }

  return self;
}

- (void)viewDidAppear:(BOOL)animated {
  [super viewDidAppear:animated];
  if (!_displayLink) {
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
  }
}

- (void)update:(CADisplayLink *)displayLink {
  // TODO Count here time sinse last update
  _game->getCanvas()->update(0);
  [glView renderWithCanvas:_game->getCanvas()];
}

- (void)viewDidDisappear:(BOOL)animated {
  [super viewDidDisappear:animated];

  if (_displayLink != nil) {
    // stop the render loop
    [_displayLink invalidate];
    _displayLink = nil;
  }
}

- (void)viewDidLoad {
  [super viewDidLoad];
  [glView setupDrawable:_game->getCanvas()];
}

// Simple joystick behaviour
- (IBAction)onJoystickMove:(id)sender withEvent:(UIEvent *)event {
  CGPoint point = [[[event allTouches] anyObject] locationInView:self.view];
  float xShift = point.x - joystickField.center.x;

  float yShift = point.y - joystickField.center.y;
  float distance = sqrtf(xShift * xShift + yShift * yShift);
  float radiusNotExceed = joystickField.bounds.size.width / 2;
  // joystick should stay inside field
  if (distance < radiusNotExceed) {
    UIControl *control = sender;
    control.center = point;
    // in OpenGL Y positive acess watches at the top, in Cocoa at the bottom
    float yShiftGL = -yShift;
    // we need the angle opposite xShift. In OpenGL positive angle is counter-wise
    float angle = -atan2f(xShift, yShiftGL);

    // TODO check this formula
    // max player speed = max joystick distance (field radius)
    float gameFactor = distance / radiusNotExceed * Game::MAX_PLAYER_SPEED_PX;
    // also we need to divide on FPS
    gameFactor = gameFactor / Game::FPS;

    _game->movePlayer(xShift * gameFactor, yShiftGL * gameFactor, angle);
  }


}

// return AVAudioPlayer for specific path
-(AVAudioPlayer*) getForPath:(NSString*) path {
  NSURL *urlPath= [NSURL fileURLWithPath:path];
  AVAudioPlayer* player = [[AVAudioPlayer alloc] initWithContentsOfURL:urlPath error:nil];
  // TODO get system volume
  player.volume = 0.5;
  [player prepareToPlay];
  return player;
}

// get player from cache and use or create new player
-(void)playForKey:(id)key andPath:(NSString*) path {
  if ([_sounds objectForKey:key]) {
    AVAudioPlayer* player = [_sounds objectForKey:key];
    [player play];
  } else {
    AVAudioPlayer* player = [self getForPath:path];
    [player play];
    [_sounds setObject:player forKey:key];
  }
}

// AVAudioPlayer delegate method
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag {
  // set the player ready to play for the next time
  if (flag) [player prepareToPlay];
}

- (IBAction)onFire:(id)sender {
  _game->playerAttack();
  [self playForKey:@"fire" andPath:[[NSBundle mainBundle] pathForResource:@"plasmoid" ofType:@"m4a"]];
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (void)dealloc {
  delete _game;
}


@end
