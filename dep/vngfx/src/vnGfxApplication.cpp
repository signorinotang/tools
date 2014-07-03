//
//  vnGfxApplication.cpp
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnGfxApplication.h"

#if VN_PLATFORM == VN_PLATFORM_WIN
#   include "win\vnGfxApplicationImpl.h"
#elif VN_PLATFORM == VN_PLATFORM_MAC
#   include "osx/vnGfxApplicationImpl.h"
#elif VN_PLATFORM == VN_PLATFORM_IOS
#   include "ios/vnGfxApplicationImpl.h"
#else

#endif

_vn_begin

namespace internal {
    class DefaultGfxAppDelegate : public GfxApplication::Delegate {
    public:
        virtual bool initialize(void *window, u32 width, u32 height) { return false; }
    };
    static DefaultGfxAppDelegate s_default_delegate;
}

GfxApplication::GfxApplication()
: m_impl(0) {
    m_impl = vnnew internal::GfxApplicationImpl(&internal::s_default_delegate);
}

GfxApplication::~GfxApplication() {
    delete m_impl;
}

void GfxApplication::setDelegate(Delegate *delegate) {
    m_impl->setDelegate(delegate);
}

void * GfxApplication::createWindow(u32 width, u32 height, const char *title) {
    return m_impl->createWindow(width, height, title);
}

int GfxApplication::run() {
    return m_impl->run();
}

void GfxApplication::quit(int code) {
    m_impl->quit(code);
}

void GfxApplication::thread_init() {
    m_impl->thread_init();
}

void GfxApplication::thread_lock() {
    m_impl->thread_lock();
}

void GfxApplication::thread_unlock() {
    m_impl->thread_unlock();
}

void GfxApplication::setTextInputEnabled(bool enabled) {
    m_impl->setTextInputEnabled(enabled);
}

bool GfxApplication::isTextInputEnabled() {
    return m_impl->isTextInputEnabled();
}

void GfxApplication::setTextInputPosition(const vector2f &pos) {
    m_impl->setTextInputPosition(pos.x, pos.y);
}

vector2f GfxApplication::getMousePosition() {
    return m_impl->getMousePosition();
}

void GfxApplication::performUndo() {
	m_impl->performUndo();
}

void GfxApplication::performRedo() {
	m_impl->performRedo();
}

void GfxApplication::performCut() {
	m_impl->performCut();
}

void GfxApplication::performCopy() {
	m_impl->performCopy();
}

void GfxApplication::performPaste() {
	m_impl->performPaste();
}


_vn_end