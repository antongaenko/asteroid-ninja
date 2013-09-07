//
//  AppDelegate.h
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainView2D;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) IBOutlet MainView2D *glView;
@property (strong, nonatomic) IBOutlet UIWindow *window;

@end
