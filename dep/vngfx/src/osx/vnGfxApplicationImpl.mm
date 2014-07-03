//
//  vnGfxApplicationImpl.cpp
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnGfxApplicationImpl.h"
#import <AppKit/NSCursor.h>
#import <AppKit/NSWindow.h>
#include <sys/time.h>

#import "vnGfxView.h"

_vn_begin

namespace internal {
    
    GfxApplicationImpl * GfxApplicationImpl::_instance = 0;
    
    GfxApplicationImpl::GfxApplicationImpl(GfxApplication::Delegate *delegate)
    : m_delegate(delegate)
    , m_window(0), m_view(0)
    , m_width(0), m_height(0) {
        _instance = this;
    }
    
    GfxApplicationImpl::~GfxApplicationImpl() {
        _instance = 0;
    }
    
    void GfxApplicationImpl::setDelegate(GfxApplication::Delegate *delegate) {
        m_delegate = delegate;
    }
    
    void * GfxApplicationImpl::createWindow(u32 width, u32 height, const char *title) {
        NSUInteger styleMask = NSTitledWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask | NSClosableWindowMask;
        NSWindow *wnd = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height) styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
        if (!wnd) {
            return 0;
        }
        wnd.collectionBehavior |= NSWindowCollectionBehaviorFullScreenPrimary;
        [wnd setTitle:[NSString stringWithUTF8String:title]];
        m_width = width;
        m_height = height;
        m_window = (void *)CFBridgingRetain(wnd);
        [wnd center];
        
        return m_window;
    }
    
    int GfxApplicationImpl::run() {
        if (!m_delegate->initialize(m_window, m_width, m_height)) {
            return -1;
        }
        
		gettimeofday(&m_last_time, 0);
		
        NSWindow * wnd = (__bridge NSWindow *)m_window;
        [wnd makeKeyAndOrderFront:nil];
        [wnd makeMainWindow];
        
        [(__bridge id<GfxView>)m_view start];
        
        return 0;
    }
    
    void GfxApplicationImpl::quit(int code) {
        [NSApp terminate:nil];
    }
    
    void GfxApplicationImpl::thread_init() {
        [(__bridge id<GfxView>)m_view thread_init];
    }
    
    void GfxApplicationImpl::thread_lock() {
        [(__bridge id<GfxView>)m_view thread_lock];
    }
    
    void GfxApplicationImpl::thread_unlock() {
        [(__bridge id<GfxView>)m_view thread_unlock];
    }
	
	void GfxApplicationImpl::performUndo() {
		[(__bridge id<GfxView>)m_view thread_lock];
		m_delegate->do_undo();
		[(__bridge id<GfxView>)m_view thread_unlock];
	}
	
	void GfxApplicationImpl::performRedo() {
		[(__bridge id<GfxView>)m_view thread_lock];
		m_delegate->do_redo();
		[(__bridge id<GfxView>)m_view thread_unlock];
	}
	
	void GfxApplicationImpl::performCut() {
		
	}
	
	void GfxApplicationImpl::performCopy() {
		
	}
	
	void GfxApplicationImpl::performPaste() {
		
	}
    
    vector2f GfxApplicationImpl::getMousePosition() {
        NSPoint pt = [(__bridge NSWindow *)m_window convertScreenToBase:[NSEvent mouseLocation]];
        pt.y = m_height - pt.y;
        return vector2f(pt.x, pt.y);
    }
    
    void GfxApplicationImpl::setTextInputEnabled(bool enabled) {
        [(__bridge id<GfxView>)m_view setTextInputEnabled:enabled];
    }
    
    bool GfxApplicationImpl::isTextInputEnabled() {
        return [(__bridge id<GfxView>)m_view isTextInputEnabled];
    }
    
    void GfxApplicationImpl::setTextInputPosition(float x, float y) {
        [(__bridge id<GfxView>)m_view setTextInputPositionX:x andY:m_height - y];
    }
    
    void GfxApplicationImpl::_view_init(void *view) {
        m_view = view;
    }
    
    void GfxApplicationImpl::_view_draw() {
        m_delegate->draw();
    }
    
    void GfxApplicationImpl::_view_update() {
        timeval tv;
		gettimeofday(&tv, 0);
		f32 dt = (tv.tv_sec - m_last_time.tv_sec) + (tv.tv_usec - m_last_time.tv_usec) * 1e-6f;
		m_last_time = tv;
		if (dt < 0) { dt = 0.f; }
        m_delegate->update(dt);
    }
    
    void GfxApplicationImpl::_view_resize(u32 width, u32 height) {
        if (width != m_width || height != m_height) {
            m_width = width;
            m_height = height;
            m_delegate->resize(width, height);
        }
    }
    
    void GfxApplicationImpl::_view_close() {
        [(__bridge id<GfxView>)m_view stop];
        m_delegate->finalize();
    }
    
    void GfxApplicationImpl::_mouse_button_down(s32 index, f32 x, f32 y) {
        m_delegate->mouse_button_down(index, vector2f(x, m_height - y));
    }
    
    void GfxApplicationImpl::_mouse_button_up(s32 index, f32 x, f32 y) {
        m_delegate->mouse_button_up(index, vector2f(x, m_height - y));
    }
    
    void GfxApplicationImpl::_mouse_wheel(f32 x, f32 y, f32 dx, f32 dy) {
        m_delegate->mouse_wheel(vector2f(x, m_height - y), vector2f(dx, dy));
    }
    
    void GfxApplicationImpl::_key_down(c32 chr, bool repeat) {
        m_delegate->key_down(chr, repeat);
    }
    
    void GfxApplicationImpl::_key_up(c32 chr) {
        m_delegate->key_up(chr);
    }
    
    void GfxApplicationImpl::_text_mark(const c32 *text, u32 caret_pos) {
        m_delegate->text_mark(text, caret_pos);
    }
    
    void GfxApplicationImpl::_text_unmark() {
        m_delegate->text_unmark();
    }
    
    void GfxApplicationImpl::_text_insert(const c32 *text) {
        m_delegate->text_insert(text);
    }
    
    void GfxApplicationImpl::_text_newLine() {
        m_delegate->text_new_line();
    }
    
    void GfxApplicationImpl::_text_deleteBackward() {
        m_delegate->text_delete_backward();
    }
    
    void GfxApplicationImpl::_text_deleteForward() {
        m_delegate->text_delete_forward();
    }
	
	void GfxApplicationImpl::_text_move_up() {
		m_delegate->text_move_up();
	}
	
	void GfxApplicationImpl::_text_move_down() {
		m_delegate->text_move_down();
	}
	
	void GfxApplicationImpl::_text_move_left() {
		m_delegate->text_move_left();
	}
	
	void GfxApplicationImpl::_text_move_right() {
		m_delegate->text_move_right();
	}
}

_vn_end