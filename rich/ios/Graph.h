//
//  Graph.h
//  rich
//
//  Created by sunjun on 12-8-9.
//  Copyright 2012 sunstdio. All rights reserved.
//

#import <UIKit/UIKit.h>

@class richViewController;

@interface richAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    richViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet richViewController *viewController;

@end

