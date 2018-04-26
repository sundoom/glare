//
//  richViewController.m
//  rich
//
//  Created by sunjun on 12-9-3.
//  Copyright 2012 sunstdio. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#import "richViewController.h"
#import "EAGLView.h"

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)

#include "../game/Game.h"
NAMESPACE_USE(Rich)

//////////////////////////////////////////////////////////////////////////

@interface richViewController ()
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation richViewController

@synthesize animating, displayLink;

//////////////////////////////////////////////////////////////////////////

/*
====================
GLoad
====================
*/
static Str s_app_dir;
const GData* GLoad(const CHAR* path)
{
	Str name = s_app_dir + path;
	GFile file;
	file.Open(name.c_str());
	U32 pos = file.Seek(0,FS_CUR);
	U32 size = file.Seek(0,FS_END);	
	file.Seek(pos,FS_SET);
	GData* data = GNEW(GData(size+1,0)); CHECK(data);
	CHECK(file.Read(data->Ptr(),data->Size())==size);
	file.Close();
	return data;
}

//////////////////////////////////////////////////////////////////////////

- (void)awakeFromNib
{
	// set the app dir
	s_app_dir = getenv("GPATH");
	s_app_dir += "/data/";
	
	// make the current gl context
	[(EAGLView *)self.view makeCurrent];
	
	// init the game
	Game::instance().init();
	
	// init the animation info
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
}

- (void)dealloc
{
	// exit the game
	Game::instance().exit();
	
    [super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];   
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(runFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)runFrame
{
	// get the frame time
	static F64 system_time = GSeconds();
	F64 current_time = GSeconds();
	F64 frame_time = current_time - system_time;
	if(frame_time == 0) return;
	system_time = current_time;
	
	// make the current gl context
	[(EAGLView*)self.view makeCurrent];  
	
	// reset the render
	unsigned int fbo = [(EAGLView*)self.view getFBO];
	RIReset(fbo);
	
	// reset the window size
	unsigned int width = [(EAGLView*)self.view getWidth];
	unsigned int height = [(EAGLView*)self.view getHeight];
	Game::instance().window(width, height);
	
	// run the game
	Game::instance().run(frame_time);
    
    // swap gl frame buffers
    [(EAGLView*)self.view swapBuffers];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

@end
