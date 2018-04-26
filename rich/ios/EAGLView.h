//
//  EAGLView.h
//  rich
//
//  Created by sunjun on 12-9-2.
//  Copyright 2012 sunstdio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
@private
    EAGLContext *context;
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
		GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;  
}

- (void)makeCurrent;
- (void)swapBuffers;
- (unsigned int)getFBO;
- (unsigned int)getWidth;
- (unsigned int)getHeight;

@end
