//
//  GfxAppDelegate.cpp
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "GfxAppDelegate.h"
#include "vnUIRoot.h"
#include "vnUILocatorBorder.h"
#include "vnFontManager.h"
#include "vnUIFrame.h"

GfxAppDelegate::GfxAppDelegate()
: m_device(0)
, m_uiRootElement(0) {
    
}

GfxAppDelegate::~GfxAppDelegate() {
    VN_SAFE_DROP(m_device);
}

bool GfxAppDelegate::initialize(void *window, vn::u32 width, vn::u32 height) {
    m_device = vn::Render2DDevice::create(window, width, height);
    if (!m_device) {
        return false;
    }
    vn::vector2f size((vn::f32)width, (vn::f32)height);
    vn::UIRoot::instance().setViewSize(size);
    m_uiRootElement = _createRootElement();
    vnnew vn::UILocatorBorder(m_uiRootElement);
    _init(size);
    vn::UIRoot::instance().setRootElement(m_uiRootElement, false);
    return true;
}

void GfxAppDelegate::update(vn::f32 delta_time) {
    _update(delta_time);
    vn::UIRoot::instance().update(delta_time);
}

void GfxAppDelegate::draw() {
    if (m_device->begin()) {
        m_device->clear();
        _render();
        vn::UIRoot::instance().render();
        vn::FontManager::instance().discard();
        m_device->end();
        m_device->present();
    }
}

void GfxAppDelegate::finalize() {
    _fini();
    vn::UIRoot::instance().setRootElement(0);
    VN_SAFE_DROP(m_device);
}

void GfxAppDelegate::resize(vn::u32 width, vn::u32 height) {
    m_device->resize(width, height);
    vn::vector2f size(width, height);
    vn::UIRoot::instance().setViewSize(size);
    _resize(size);
}

void GfxAppDelegate::mouse_button_down(vn::s32 index, const vn::vector2f &pos) {
    switch (index) {
        case 0:
            vn::UIRoot::instance().dispath_mouseLeftDown(pos);
            break;
            
        case 1:
            vn::UIRoot::instance().dispath_mouseRightDown(pos);
            break;
    }
}

void GfxAppDelegate::mouse_button_up(vn::s32 index, const vn::vector2f &pos) {
    switch (index) {
        case 0:
            vn::UIRoot::instance().dispath_mouseLeftUp(pos);
            break;
            
        case 1:
            vn::UIRoot::instance().dispath_mouseRightUp(pos);
            break;
    }
}

void GfxAppDelegate::mouse_wheel(const vn::vector2f &pos, const vn::vector2f &delta) {
    vn::UIRoot::instance().dispath_mouseWheel(pos, delta);
}
void GfxAppDelegate::key_down(vn::c32 chr, bool repeat) {
    vn::UIRoot::instance().dispath_keyDown(chr, repeat);
}
void GfxAppDelegate::key_up(vn::c32 chr) {
    vn::UIRoot::instance().dispath_keyUp(chr);
}

void GfxAppDelegate::text_insert(const vn::str32 &text) {
    vn::UIRoot::instance().dispath_textInsert(text);
}

void GfxAppDelegate::text_mark(const vn::str32 &text, vn::u32 caret_pos) {
    vn::UIRoot::instance().dispath_textMark(text, caret_pos);
}

void GfxAppDelegate::text_unmark() {
    vn::UIRoot::instance().dispath_textUnmark();
}

void GfxAppDelegate::text_new_line() {
    vn::UIRoot::instance().dispath_textNewLine();
}

void GfxAppDelegate::text_delete_backward() {
    vn::UIRoot::instance().dispath_textDeleteBackward();
}

void GfxAppDelegate::text_delete_forward() {
    vn::UIRoot::instance().dispath_textDeleteForward();
}

void GfxAppDelegate::text_move_up() {
    vn::UIRoot::instance().dispath_textMoveUp();
}

void GfxAppDelegate::text_move_down() {
    vn::UIRoot::instance().dispath_textMoveDown();
}

void GfxAppDelegate::text_move_left() {
    vn::UIRoot::instance().dispath_textMoveLeft();
}

void GfxAppDelegate::text_move_right() {
    vn::UIRoot::instance().dispath_textMoveRight();
}

vn::UIElement * GfxAppDelegate::_createRootElement() {
    return vnnew vn::UIFrame();
}

void GfxAppDelegate::_init(const vn::vector2f &size) {
    
}

void GfxAppDelegate::_fini() {
    
}

void GfxAppDelegate::_update(vn::f32 deltaTime) {
    
}

void GfxAppDelegate::_render() {
    
}

void GfxAppDelegate::_resize(const vn::vector2f &size) {
    
}