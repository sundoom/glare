//
//  Graph.mm
//  rich
//
//  Created by sunjun on 12-8-9.
//  Copyright 2012 sunstdio. All rights reserved.
//

#import "richAppDelegate.h"
#import "richViewController.h"

@implementation richAppDelegate

@synthesize window;
@synthesize viewController;

- (void) applicationDidFinishLaunching: (UIApplication*) application
{	
	// Set the view controller as the window's root view controller and display.
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
}

- (void) applicationWillTerminate: (UIApplication*) application
{
	
}

- (void)dealloc 
{
    [viewController release];
    [window release];
    [super dealloc];
}

@end
