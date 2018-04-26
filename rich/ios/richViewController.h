//
//  richViewController.h
//  rich
//
//  Created by sunjun on 12-9-3.
//  Copyright 2012 sunstdio. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface richViewController : UIViewController
{   
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void)startAnimation;
- (void)stopAnimation;

@end
