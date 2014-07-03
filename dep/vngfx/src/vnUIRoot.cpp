//
//  vnUIRoot.cpp
//  vngfx
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIRoot.h"
#include "vnUIElement.h"
#include "vnGfxApplication.h"
#include "vnUILocator.h"


_vn_begin

UIRoot::UIRoot()
: m_root(0)
, m_focus(0)
, m_hover(0)
, m_capture(0) {
    
}

UIRoot::~UIRoot() {
    VN_SAFE_DROP(m_root);
}

void UIRoot::update(f32 deltaTime) {
    _updateMouse();
    if (m_root) {
        m_root->update(deltaTime);
    }
}

void UIRoot::render() {
    if (m_root) {
        m_root->render(&m_renderer);
    }
}

void UIRoot::setRootElement(vn::UIElement *root, bool grab) {
    if (m_root) {
        reset();
        m_root->onFocusOut();
		m_root->drop();
	}
	if((m_root = root)) {
        if (grab) {
            root->grab();
        }
        UILocator *loc = m_root->locator();
        if (loc) {
            loc->markDirty();
        }
        m_root->onFocusIn();
	}
}

UIElement * UIRoot::getRootElement() {
	return m_root;
}

const vector2f & UIRoot::getViewSize() {
    return m_viewSize;
}

void UIRoot::setViewSize(const vector2f &size) {
    m_viewSize = size;
    if (m_root) {
        UILocator *loc = m_root->locator();
        if (loc) {
            loc->markDirty();
        }
    }
}

void UIRoot::hookMouseEvent(MouseEventHook *hook) {
	unhookMouseEvent(hook);
	m_mouseEventHooks.push_back(hook);
}

void UIRoot::unhookMouseEvent(MouseEventHook *hook) {
	for (MouseEventHooks::iterator it = m_mouseEventHooks.begin(); it != m_mouseEventHooks.end(); ++it) {
		if (*it == hook) {
			m_mouseEventHooks.erase(it);
			break;
		}
	}
}

void UIRoot::reset() {
    if (m_hover) {
        m_hover->onMouseLeave();
        m_hover = 0;
    }
    if (m_capture) {
        m_capture->onMouseCaptureCancelled();
        m_capture = 0;
    }
    if (m_focus) {
        m_focus->onFocusOut();
        m_focus = 0;
        m_root->onFocusIn();
    }
}

bool UIRoot::dispath_mouseLeftDown(const vector2f &pos) {
    if (m_capture) {
		_sendMouseLeftDown(m_capture, pos);
    } else if (m_hover) {
        _sendMouseLeftDown(m_hover, pos);
    } else {
		_sendMouseLeftDown(0, pos);
        return false;
    }
    return true;
}

bool UIRoot::dispath_mouseLeftUp(const vector2f &pos) {
    if (m_capture) {
        _sendMouseLeftUp(m_capture, pos);
    } else if (m_hover) {
        _sendMouseLeftUp(m_hover, pos);
    } else {
		_sendMouseLeftUp(0, pos);
        return false;
    }
    return true;
}

bool UIRoot::dispath_mouseRightDown(const vector2f &pos) {
    if (m_capture) {
        _sendMouseRightDown(m_capture, pos);
    } else if (m_hover) {
        _sendMouseRightDown(m_hover, pos);
    } else {
		_sendMouseRightDown(0, pos);
        return false;
    }
    return true;
}

bool UIRoot::dispath_mouseRightUp(const vector2f &pos) {
    if (m_capture) {
        _sendMouseRightUp(m_capture, pos);
    } else if (m_hover) {
        _sendMouseRightUp(m_hover, pos);
    } else {
		_sendMouseRightUp(0, pos);
        return false;
    }
    return true;
}

bool UIRoot::dispath_mouseWheel(const vector2f &pos, const vector2f &delta) {
    if (m_capture) {
        _sendMouseWheel(m_capture, pos, delta);
    } else if (m_hover) {
        _sendMouseWheel(m_hover, pos, delta);
    } else {
		_sendMouseWheel(0, pos, delta);
        return false;
    }
    return true;
}

void UIRoot::dispath_keyDown(c32 chr, bool repeat) {
    if (m_focus) {
        m_focus->onKeyDown(chr, repeat);
    } else if (m_root) {
        m_root->onKeyDown(chr, repeat);
    };
}

void UIRoot::dispath_keyUp(c32 chr) {
    if (m_focus) {
        m_focus->onKeyUp(chr);
    } else if (m_root) {
        m_root->onKeyUp(chr);
    }
}

void UIRoot::dispath_textInsert(const str32 &text) {
    if (m_focus) {
        m_focus->onTextInsert(text);
    }
}

void UIRoot::dispath_textMark(const str32 &text, u32 caretPos) {
    if (m_focus) {
        m_focus->onTextMark(text, caretPos);
    }
}

void UIRoot::dispath_textUnmark() {
    if (m_focus) {
        m_focus->onTextUnmark();
    }
}

void UIRoot::dispath_textNewLine() {
    if (m_focus) {
        m_focus->onTextNewLine();
    }
}

void UIRoot::dispath_textDeleteBackward() {
    if (m_focus) {
        m_focus->onTextDeleteBackward();
    }
}

void UIRoot::dispath_textDeleteForward() {
    if (m_focus) {
        m_focus->onTextDeleteForward();
    }
}

void UIRoot::dispath_textMoveUp() {
	if (m_focus) {
        m_focus->onTextMoveUp();
    }
}

void UIRoot::dispath_textMoveDown() {
	if (m_focus) {
        m_focus->onTextMoveDown();
    }
}

void UIRoot::dispath_textMoveLeft() {
	if (m_focus) {
        m_focus->onTextMoveLeft();
    }
}

void UIRoot::dispath_textMoveRight() {
	if (m_focus) {
        m_focus->onTextMoveRight();
    }
}

void UIRoot::captureMouse(UIElement *element) {
    vnassert(element);
    if (m_capture != element) {
        if (m_capture) {
            m_capture->onMouseCaptureCancelled();
        }
        m_capture = element;
    }
}

void UIRoot::releaseMouse(UIElement *element) {
    vnassert(element);
    if (m_capture == element) {
        m_capture = 0;
    }
}

void UIRoot::requireFocus(UIElement *element) {
    vnassert(element);
    if (element == m_focus) {
        return ;
    }
    if (m_focus) {
        m_focus->onFocusOut();
    } else {
        m_root->onFocusOut();
    }
    m_focus = element;
    m_focus->onFocusIn();
}

void UIRoot::releaseFocus(UIElement *element) {
    vnassert(element);
    if (m_focus != element) {
        return ;
    }
    m_focus->onFocusOut();
	m_focus = 0;
    m_root->onFocusIn();
}

void UIRoot::_notify_disappeared(vn::UIElement *element) {
	if (m_hover == element) {
		m_hover = 0;
	}
	if (m_capture == element) {
		m_capture = 0;
	}
	if (m_focus == element) {
		m_focus = 0;
	}
}

void UIRoot::_updateMouse() {
    vector2f mpos = GfxApplication::instance().getMousePosition();
    UIElement *p = 0;
    if (mpos >= vector2f::kZero && mpos < m_viewSize) {
        if (m_root && m_root->isEnabled() && m_root->isVisible()) {
            p = m_root->pick(mpos);
        }
    }
    if (m_capture && p != m_capture) {
        p = 0;
    }
    if (m_hover != p) {
        if (m_hover) {
            m_hover->onMouseLeave();
        }
        m_hover = p;
        if (m_hover) {
            m_hover->onMouseEnter();
        }
    }
}

void UIRoot::_sendMouseLeftDown(UIElement *target, const vector2f &pos) {
	MouseEventHooks hooks = m_mouseEventHooks;
	for (MouseEventHooks::iterator it = hooks.begin(); it != hooks.end(); ++it) {
		(*it)->hookMouseLeftDown(target, pos);
	}
	if (target) {
		target->onMouseLeftDown(pos);
	}
}
void UIRoot::_sendMouseLeftUp(UIElement *target, const vector2f &pos) {
	MouseEventHooks hooks = m_mouseEventHooks;
	for (MouseEventHooks::iterator it = hooks.begin(); it != hooks.end(); ++it) {
		(*it)->hookMouseLeftUp(target, pos);
	}
	if (target) {
		target->onMouseLeftUp(pos);
	}
}

void UIRoot::_sendMouseRightDown(UIElement *target, const vector2f &pos) {
	MouseEventHooks hooks = m_mouseEventHooks;
	for (MouseEventHooks::iterator it = hooks.begin(); it != hooks.end(); ++it) {
		(*it)->hookMouseRightDown(target, pos);
	}
	if (target) {
		target->onMouseRightDown(pos);
	}
}

void UIRoot::_sendMouseRightUp(UIElement *target, const vector2f &pos) {
	MouseEventHooks hooks = m_mouseEventHooks;
	for (MouseEventHooks::iterator it = hooks.begin(); it != hooks.end(); ++it) {
		(*it)->hookMouseRightUp(target, pos);
	}
	if (target) {
		target->onMouseRightUp(pos);
	}
}

void UIRoot::_sendMouseWheel(UIElement *target, const vector2f &pos, const vector2f &delta) {
	MouseEventHooks hooks = m_mouseEventHooks;
	for (MouseEventHooks::iterator it = hooks.begin(); it != hooks.end(); ++it) {
		(*it)->hookMouseWheel(target, pos, delta);
	}
	if (target) {
		target->onMouseWheel(pos, delta);
	}
}




_vn_end