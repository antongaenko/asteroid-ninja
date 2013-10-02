//
//  GameViewController.h
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 21.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GameView2D.h"

class Game;

@interface GameViewController : UIViewController {

@public

  Game* _game;

  CADisplayLink* _displayLink;
  
  NSCache *_sounds;

  IBOutlet GameView2D * glView;
  __weak IBOutlet UILabel *scoreField;
    __weak IBOutlet UIImageView *joystickField;
    __weak IBOutlet UIView *joystickBody;
    __weak IBOutlet UIButton *fireButton;
}

@end
