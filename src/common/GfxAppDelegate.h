//
//  GfxAppDelegate.h
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __test__GfxAppDelegate__
#define __test__GfxAppDelegate__

#include "vnGfxApplication.h"
#include "vnRender2DDevice.h"
#include "vnUIElement.h"

class GfxAppDelegate : public vn::GfxApplication::Delegate {
public:
    GfxAppDelegate();
    ~GfxAppDelegate();
    
    bool initialize(void *window, vn::u32 width, vn::u32 height);
    void update(vn::f32 delta_time);
    void draw();
    void finalize();
    void resize(vn::u32 width, vn::u32 height);
    void mouse_button_down(vn::s32 index, const vn::vector2f &pos);
    void mouse_button_up(vn::s32 index, const vn::vector2f &pos);
    void mouse_wheel(const vn::vector2f &pos, const vn::vector2f &delta);
    void key_down(vn::c32 chr, bool repeat);
    void key_up(vn::c32 chr);
    void text_insert(const vn::str32 &text);
    void text_mark(const vn::str32 &text, vn::u32 caret_pos);
    void text_unmark();
    void text_new_line();
    void text_delete_backward();
    void text_delete_forward();
    void text_move_up();
    void text_move_down();
    void text_move_left();
    void text_move_right();
    
protected:
    virtual vn::UIElement * _createRootElement();
    virtual void _init(const vn::vector2f &size);
    virtual void _fini();
    virtual void _update(vn::f32 deltaTime);
    virtual void _render();
    virtual void _resize(const vn::vector2f &size);
    
    vn::Render2DDevice *m_device;
    vn::UIElement *m_uiRootElement;
};

#endif /* defined(__test__GfxAppDelegate__) */
