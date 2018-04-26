//
//  richAppDelegate.h
//  rich
//
//  Created by sunjun on 12-9-3.
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

