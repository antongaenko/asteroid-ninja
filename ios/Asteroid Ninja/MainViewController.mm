//
// Created by Anton Gaenko on 05.09.13.
// Copyright (c) 2013 Anton Gaenko. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import "MainViewController.h"
#import "MainView2D.h"


@implementation MainViewController {
}

-(void)loadView {
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    float scale = [[UIScreen mainScreen] scale];

    CGRect myBounds = CGRectMake(screenBounds.origin.x, screenBounds.origin.y,
            screenBounds.size.height*scale, screenBounds.size.width*scale);

    MainView2D *glView = [[MainView2D alloc] initWithFrame:myBounds];
    [glView setMultipleTouchEnabled:YES];
    //[glView setContentScaleFactor:scale];

    // TODO it should be at another place
    [glView startRenderLoop];

    self.view = glView;
}


@end