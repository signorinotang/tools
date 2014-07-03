//
//  vnRender2DView.h
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "vnGfxView.h"
#include "vnMutex.h"
#include "vnRecursiveLock.h"

typedef _vn_ns::RecursiveLock<_vn_ns::Mutex> LockType;

@interface Render2DView : NSOpenGLView<GfxView, NSTextInputClient> {
    CVDisplayLinkRef m_displayLink;
    NSRange m_markedRange;
    NSPoint m_inputPosition;
    bool m_inputEnabled;
    LockType m_lock;
}

- (void)updateAndDraw;
- (void)present;

@end
