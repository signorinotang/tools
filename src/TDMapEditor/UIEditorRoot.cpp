//
//  UIEditorRoot.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "UIEditorRoot.h"
#include "EditorApp.h"


void UIEditorRoot::onMouseLeftDown(const vn::vector2f &position) {
    EditorApp::instance().onRootMouseLeftDown(position);
}

void UIEditorRoot::onMouseLeftUp(const vn::vector2f &position) {
    EditorApp::instance().onRootMouseLeftUp(position);
}

void UIEditorRoot::onMouseRightDown(const vn::vector2f &position) {
    EditorApp::instance().onRootMouseRightDown(position);
}

void UIEditorRoot::onMouseRightUp(const vn::vector2f &position) {
    EditorApp::instance().onRootMouseRightUp(position);
}

void UIEditorRoot::onMouseCaptureCancelled() {
    EditorApp::instance().onRootMouseCaptureCancelled();
}

void UIEditorRoot::onKeyDown(vn::c32 chr, bool repeat) {
    EditorApp::instance().onRootKeyDown(chr, repeat);
}

void UIEditorRoot::onKeyUp(vn::c32 chr) {
    EditorApp::instance().onRootKeyUp(chr);
}

void UIEditorRoot::onFocusIn() {
    EditorApp::instance().onRootFocusIn();
}

void UIEditorRoot::onFocusOut() {
    EditorApp::instance().onRootFocusOut();
}