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
#import "Logger.h"

@interface GameViewController ()

@end

@implementation GameViewController

- (id)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  if (self) {
    _game = new Game();
    scoreField.text = [NSString stringWithFormat:@"%d", _game->getCurrentScore()];
    lifeIndicator.text = [NSString stringWithFormat:@"%d", _game->getCurrentLives()];
    // set listener on events
    _game->setListener([self](GameEvent e) {
      switch (e) {
        case GameEvent::SCORE_CHANGES:
          scoreField.text = [NSString stringWithFormat:@"%d", _game->getCurrentScore()];
          break;

        case GameEvent::SHIP_CRASH:
          [self playForKey:@"crash" andPath:[[NSBundle mainBundle] pathForResource:@"crash" ofType:@"m4a"]];
          // if lives are over then END
          if (_game->getCurrentLives() < 1) {
            [self gameOver];
          } else {
            lifeIndicator.text = [NSString stringWithFormat:@"%d", _game->getCurrentLives()];
          }
          break;

        case GameEvent::ASTEROID_BANG:
          [self playForKey:@"asteroid_bang" andPath:[[NSBundle mainBundle] pathForResource:@"asteroid_bang" ofType:@"m4a"]];
          break;

        default:
          error("bad game event %d", e);
          break;
      }
    });

    _sounds = [[NSCache alloc] init];
  }

  return self;
}

- (void)play {
  isPause = false;
  if (!_displayLink) {
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    // TODO Set here frameInterval based on game FPS. Currently 30 fps
    _displayLink.frameInterval = 2;
    lastTimestamp = CACurrentMediaTime();
    [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
  }
}

- (void)pause {
  isPause = true;
  if (_displayLink) {
    // remove display link from app run loop
    [_displayLink invalidate];
    _displayLink = nil;
  }
}

- (void)viewDidAppear:(BOOL)animated {
  [super viewDidAppear:animated];
  [self play];
  // TODO It should be in AppDelegate
  NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
  // also listen for application resign and foreground to free resources
  [nc addObserver:self selector:@selector(cleanUp) name:UIApplicationWillResignActiveNotification object:NULL];
  [nc addObserver:self selector:@selector(applicationEnterForeground) name:UIApplicationWillEnterForegroundNotification object:NULL];
}

- (void)applicationEnterForeground {
  // TODO It should be in AppDelegate
  [self play];
  NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
  // also listen for application resign and foreground to free resources
  [nc addObserver:self selector:@selector(cleanUp) name:UIApplicationWillResignActiveNotification object:NULL];
  [nc addObserver:self selector:@selector(applicationEnterForeground) name:UIApplicationWillEnterForegroundNotification object:NULL];
}

- (void) gameOver {
  [self cleanUp];
  // return to menu
  [self dismissViewControllerAnimated:YES completion:^{
    // clean game to free resources
    delete _game;
    _game = nil;
  }];
}

- (void)cleanUp {
  if (!isPause)[self pause];
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)update:(CADisplayLink *)displayLink {
  double currentTime = [displayLink timestamp];
  double renderTime = currentTime - lastTimestamp;
  lastTimestamp = currentTime;

  if (_game) {
    _game->getCanvas()->update(renderTime);
    [glView renderWithCanvas:_game->getCanvas()];
  }
}

- (void)viewDidDisappear:(BOOL)animated {
  [super viewDidDisappear:animated];
  [self pause];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  [glView setupDrawable:_game->getCanvas()];
  // also prepare audio first time
  [self getForPath:[[NSBundle mainBundle] pathForResource:@"plasmoid" ofType:@"m4a"]];
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
    
    // max player speed = max joystick distance (field radius)
    float xGameShift = xShift / radiusNotExceed * _game->getPlayerSpeedMax();
    float yGameShift = yShiftGL / radiusNotExceed * _game->getPlayerSpeedMax();

    _game->movePlayer(xGameShift, yGameShift, angle);
  }
}

// return AVAudioPlayer for specific path
- (AVAudioPlayer *)getForPath:(NSString *)path {
  NSURL *urlPath = [NSURL fileURLWithPath:path];
  AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithContentsOfURL:urlPath error:nil];
  [player prepareToPlay];
  return player;
}

// get player from cache and use or create new player
- (void)playForKey:(id)key andPath:(NSString *)path {
  if ([_sounds objectForKey:key]) {
    AVAudioPlayer *player = [_sounds objectForKey:key];
    if (!player.playing) {
      [player play];
    }
  } else {
    AVAudioPlayer *player = [self getForPath:path];
    [player play];
    [_sounds setObject:player forKey:key];
  }
}

- (IBAction)onPlayPause:(id)sender {
  UIButton* b = (UIButton*) sender;
  isPause = !isPause;
  if (isPause) {
    [self pause];
    [b setImage:[UIImage imageNamed:@"play_button.png"] forState:UIControlStateNormal];
  } else {
    [self play];
    [b setImage:[UIImage imageNamed:@"pause_button.png"] forState:UIControlStateNormal];
  }
}

- (IBAction)onFire:(id)sender {
  if (!isPause) {
    _game->playerAttack();
    [self playForKey:@"fire" andPath:[[NSBundle mainBundle] pathForResource:@"plasmoid" ofType:@"m4a"]];
  }
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (void)dealloc {
  [self cleanUp];
  if (_game) delete _game;
}


@end
