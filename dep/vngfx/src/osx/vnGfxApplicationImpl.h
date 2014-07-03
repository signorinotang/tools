//
//  vnGfxApplicationImpl.h
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __vngfx__vnGfxApplicationImpl__
#define __vngfx__vnGfxApplicationImpl__

#include "vnGfxApplication.h"
#include "vnAllocatedObject.h"

_vn_begin

namespace internal {
    
    class GfxApplicationImpl : public AllocatedObject {
    public:
        GfxApplicationImpl(GfxApplication::Delegate *delegate);
        ~GfxApplicationImpl();
        
        void setDelegate(GfxApplication::Delegate *delegate);
        
        void * createWindow(u32 width, u32 height, const char *title);
        
        int run();
        void quit(int code);
        
        vector2f getMousePosition();
        
        void setTextInputEnabled(bool enabled);
        bool isTextInputEnabled();
        void setTextInputPosition(float x, float y);
        
        
        void thread_init();
        void thread_lock();
        void thread_unlock();
		
		void performUndo();
		void performRedo();
		void performCut();
		void performCopy();
		void performPaste();
        
        void _view_init(void *view);
        void _view_update();
        void _view_draw();
        void _view_resize(u32 width, u32 height);
        void _view_close();
        
        void _mouse_button_down(s32 index, f32 x, f32 y);
        void _mouse_button_up(s32 index, f32 x, f32 y);
        void _mouse_wheel(f32 x, f32 y, f32 dx, f32 dy);
        
        void _key_down(c32 chr, bool repeat);
        void _key_up(c32 chr);
        
        void _text_mark(const c32 *text, u32 caret_pos);
        void _text_unmark();
        void _text_insert(const c32 *text);
        void _text_newLine();
        void _text_deleteBackward();
        void _text_deleteForward();
		
		void _text_move_up();
		void _text_move_down();
		void _text_move_left();
		void _text_move_right();
		
        
        static GfxApplicationImpl *_instance;
        
    private:
        GfxApplication::Delegate *m_delegate;
        void *m_window;
        void *m_view;
        timeval m_last_time;
        u32 m_width, m_height;
    };
    
}

_vn_end


#endif /* defined(__vngfx__vnGfxApplicationImpl__) */
