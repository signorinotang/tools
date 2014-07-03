//
//  vnGfxApplication.h
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnGfxApplication_h
#define _vnGfxApplication_h

#include "vnGfxDef.h"
#include "vnSingleton.h"
#include "vnVector2.h"

_vn_begin

namespace internal {
    class GfxApplicationImpl;
}

class _vn_gfx_api GfxApplication : public Singleton<GfxApplication> {
public:
    class Delegate {
    public:
        virtual bool initialize(void *window, u32 width, u32 height) = 0;
        virtual void update(f32 delta_time) {}
        virtual void draw() {}
        virtual void finalize() {}
        virtual void resize(u32 width, u32 height) {}
        virtual void mouse_button_down(s32 index, const vector2f &pos) {}
        virtual void mouse_button_up(s32 index, const vector2f &pos) {}
        virtual void mouse_wheel(const vector2f &pos, const vector2f &delta) {}
        virtual void key_down(c32 chr, bool repeat) {}
        virtual void key_up(c32 chr) {}
        virtual void text_insert(const str32 &text) {}
        virtual void text_mark(const str32 &text, u32 caret_pos) {}
        virtual void text_unmark() {}
        virtual void text_new_line() {}
        virtual void text_delete_backward() {}
        virtual void text_delete_forward() {}
		virtual void text_move_up() {}
		virtual void text_move_down() {}
		virtual void text_move_left() {}
		virtual void text_move_right() {}
		virtual void do_undo() {}
		virtual void do_redo() {}
		virtual bool do_cut(str32 &text) { return false; }
		virtual bool do_copy(str32 &text) { return false; }
		virtual void do_paste(const str32 &text) {}
    };
    
    GfxApplication();
    ~GfxApplication();
    
    void setDelegate(Delegate *delegate);
    
    void * createWindow(u32 width, u32 height, const char *title);
    
    int run();
    void quit(int code);
    
    void thread_init();
    void thread_lock();
    void thread_unlock();
    
    void setTextInputEnabled(bool enabled);
    bool isTextInputEnabled();
    void setTextInputPosition(const vector2f &pos);
    
    vector2f getMousePosition();
	
	void performUndo();
	void performRedo();
	void performCut();
	void performCopy();
	void performPaste();
private:
    internal::GfxApplicationImpl *m_impl;
};


_vn_end

#endif