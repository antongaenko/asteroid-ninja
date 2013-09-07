//
//  AppDelegate.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 30.08.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import "AppDelegate.h"
#import "MainView2D.h"
#import "MainViewController.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    /*if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[MainView2D alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
    } else {
        self.viewController = [[MainView2D alloc] initWithNibName:@"ViewController_iPad" bundle:nil];
    }*/
//    self.viewController = [[MainView2D alloc] initWithNibName:@"MainWindow" bundle:nil];
    //self.window.rootViewController = self.;
    //[self.window makeKeyAndVisible];
    
    //[self.glView setDepthBufferNeeded:FALSE];
    
    //we create our lesson which contains the OpenGL code
    //(allocated with new -> has to be cleaned up with delete!)
//    lesson = new Lesson02();
    
    //we tell our OpenGL view which lesson we want to use for rendering.
  //  [glView setLesson:lesson];

    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    [[UIApplication sharedApplication] setStatusBarHidden:YES];

    self.window.backgroundColor = [UIColor blackColor];

    MainViewController *_glViewController = [[MainViewController alloc] init];
    [self.window setRootViewController:_glViewController];

    [self.window makeKeyAndVisible];

//    self.glView = [[[GLView alloc] initWithFrame:screenBounds] autorelease];
//    [self.glView setContentScaleFactor:[[UIScreen mainScreen] scale]];
//    [self.window addSubview:_glView];

    return YES;
    
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}




@end
