//
//  GameViewController.h
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 21.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GameView2D.h"
#import "Game.h"

class Game;

@interface GameViewController : UIViewController {

@private
  // it contains game logic and it's a bridge to c++ sources
  Game* _game;

  bool isPause;

  CADisplayLink* _displayLink;
  // cache of AVAudioPlayer 
  NSCache* _sounds;
  
  double lastTimestamp;

  __weak IBOutlet UILabel *lifeIndicator;
  IBOutlet GameView2D * glView;
  __weak IBOutlet UILabel *scoreField;
  __weak IBOutlet UIImageView *joystickField;
  __weak IBOutlet UIView *joystickBody;
  __weak IBOutlet UIButton *fireButton;
}

-(void) gameOver;
-(void) cleanUp;

@end
