//
//  vnUITextField.cpp
//  vngfx
//
//  Created by Wayne on 13-12-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUITextField.h"
#include "vnGfxApplication.h"
#include "vnFontManager.h"
#include "vnUnicode.h"
#include "vnLog.h"

_vn_begin

UITextField::UITextField()
: m_caretIndex(0)
, m_insertIndex(0)
, m_selectionIndex(-1)
, m_selectionLength(0)
, m_renderIndex0(0)
, m_renderIndex1(0)
, m_renderIndex2(0)
, m_renderIndex3(0)
, m_textColor(color_t::kWhite)
, m_selectedTextColor(color_t::kBlack)
, m_markedTextColor(color_t::kGreen)
, m_selectionColor(0xA0, 0xA0, 0xA0)
, m_readOnly(false)
, m_hasFocus(false)
, m_mouseDown(false)
, m_caretTimer(0)
, m_textAlignment(0)
, m_textDirty(false)
, m_autoReleaseFocus(true) {
    m_clipping = true;
}

UITextField * UITextField::clone() const {
    return vnnew UITextField(*this);
}

// properties
void UITextField::setFont(const FontPtr &font) {
    m_renderText.setFont(font);
}

const FontPtr & UITextField::getFont() const {
    return m_renderText.getFont();
}

void UITextField::setText(const str8 &text) {
    str32 _txt;
    Unicode::convert(text.c_str(), text.length(), _txt);
    setText(_txt);
}

void UITextField::setText(const str32 &text) {
    m_text = text;
    m_selectionIndex = -1;
    m_selectionLength = 0;
    m_caretIndex = 0;
	m_insertIndex = 0;
    m_textDirty = true;
}

const str32 & UITextField::getText() const {
    return m_text;
}

void UITextField::setTextColor(const color_t &color) {
    m_textColor = color;
}

const color_t & UITextField::getTextColor() const {
    return m_textColor;
}

void UITextField::setSelectedTextColor(const color_t &color) {
    m_selectedTextColor = color;
}

const color_t & UITextField::getSelectedTextColor() const {
    return m_selectedTextColor;
}

void UITextField::setMarkedTextColor(const color_t &color) {
    m_markedTextColor = color;
}

const color_t & UITextField::getMarkedTextColor() const {
    return m_markedTextColor;
}

void UITextField::setSelectionColor(const color_t &color) {
    m_selectionColor = color;
}

const color_t & UITextField::getSelectionColor() const {
    return m_selectionColor;
}

void UITextField::setTextAlignment(u32 alignment) {
    m_textAlignment = alignment;
}

u32 UITextField::getTextAlignment() const {
    return m_textAlignment;
}

void UITextField::setClipping(bool clipping) {
    m_clipping = true;
}

void UITextField::setAutoReleaseFocus(bool autoReleaseFocus) {
	m_autoReleaseFocus = autoReleaseFocus;
	if (!m_hasFocus || m_readOnly) {
		return ;
	}
	if (m_autoReleaseFocus) {
		UIRoot::instance().hookMouseEvent(this);
	} else {
		UIRoot::instance().unhookMouseEvent(this);
	}
}

bool UITextField::isAutoReleaseFocus() const {
	return m_autoReleaseFocus;
}

void UITextField::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
    if (!m_hasFocus) {
        return ;
    }
    GfxApplication::instance().setTextInputEnabled(!m_readOnly);
}

bool UITextField::isReadOnly() const {
    return m_readOnly;
}

// methods

void UITextField::init(const TreeDataObject *object) {
    UIElement::init(object);
    const TreeDataString *tdstr = object->queryAttributeString("font");
    if (tdstr) {
        m_renderText.setFont(FontManager::instance().findFont(tdstr->value()));
    } else {
        m_renderText.setFont(FontManager::instance().defaultFont());
    }
    tdstr = object->queryAttributeString("text");
    if (tdstr) {
        setText(tdstr->value());
    }
    m_textColor.convertFromBGRA(object->queryAttributeInteger("color", m_textColor.convertToBGRA()));
    m_textAlignment = object->queryAttributeInteger("alignment");
	
	setAutoReleaseFocus(object->queryAttributeBoolean("auto_release_focus", m_autoReleaseFocus));
    
    setReadOnly(object->queryAttributeBoolean("read_only", m_readOnly));
}

void UITextField::bindAction_TextCompleted(const function<void(UIElement *, const str32 &)> &func) {
	m_fnTextCompleted = func;
}

void UITextField::bindAction_FocusIn(const function<void(UIElement *)> &func) {
	m_fnFocusIn = func;
}

void UITextField::bindAction_FocusOut(const function<void(UIElement *)> &func) {
	m_fnFocusOut = func;
}

void UITextField::selectAll() {
	if (!m_markedText.empty() || m_text.empty()) {
		return ;
	}
	m_selectionIndex = 0;
	m_selectionLength = m_text.length();
	m_insertIndex = m_text.length();
	m_caretIndex = m_insertIndex;
}

void UITextField::selectNone() {
	if (!m_markedText.empty()) {
		return ;
	}
	m_selectionIndex = -1;
	m_selectionLength = 0;
	m_caretIndex = 0;
	m_insertIndex = 0;
}

// events
void UITextField::onMouseLeftDown(const vector2f &position) {
    if (!m_hasFocus) {
        UIRoot::instance().requireFocus(this);
        if (m_selectionLength) {
			return  ;
		}
    }
	if (!m_markedText.empty()) {
		return ;
	}
    m_mouseDown = true;
    UIRoot::instance().captureMouse(this);
	
	if (!m_textDirty) {
		m_insertIndex = m_renderText.index_round(vn::GfxApplication::instance().getMousePosition().x - m_boundingBox.min_corner.x - m_textPosition.x);
	}
}

void UITextField::onMouseLeftUp(const vector2f &position) {
    if (!m_mouseDown) {
        return ;
    }
    m_mouseDown = false;
    UIRoot::instance().releaseMouse(this);
}

void UITextField::onMouseCaptureCancelled() {
    m_mouseDown = false;
}

void UITextField::hookMouseLeftDown(UIElement *target, const vector2f &pos) {
	if (target != this) {
		UIRoot::instance().releaseFocus(this);
	}
}

void UITextField::hookMouseLeftUp(UIElement *target, const vector2f &pos) {
	if (target != this) {
		UIRoot::instance().releaseFocus(this);
	}
}

void UITextField::hookMouseRightDown(UIElement *target, const vector2f &pos) {
	if (target != this) {
		UIRoot::instance().releaseFocus(this);
	}
}

void UITextField::hookMouseRightUp(UIElement *target, const vector2f &pos) {
	if (target != this) {
		UIRoot::instance().releaseFocus(this);
	}
}

void UITextField::onFocusIn() {
    m_hasFocus = true;
    m_caretTimer = 0;
    if (!m_readOnly) {
        GfxApplication::instance().setTextInputEnabled(true);
    }
	if (m_autoReleaseFocus) {
		UIRoot::instance().hookMouseEvent(this);
	}
	if (!m_fnFocusIn.null()) {
		m_fnFocusIn(this);
	}
}

void UITextField::onFocusOut() {
    m_hasFocus = false;
    if (!m_readOnly) {
        GfxApplication::instance().setTextInputEnabled(false);
    }
	if (m_autoReleaseFocus) {
		UIRoot::instance().unhookMouseEvent(this);
	}
	if (!m_fnFocusOut.null()) {
		m_fnFocusOut(this);
	}
}

void UITextField::onTextInsert(const str32 &text) {
    if (m_selectionLength) {
        str32::iterator it = m_text.begin() + m_selectionIndex;
        m_text.erase(it, it + m_selectionLength);
        m_selectionLength = 0;
		m_insertIndex = m_selectionIndex;
		m_selectionIndex = -1;
    }
    m_text.insert(m_text.begin() + m_insertIndex, text.begin(), text.end());
    m_textDirty = true;
    m_insertIndex += text.length();
    m_caretIndex = m_insertIndex;
	m_caretTimer = 0;
}

void UITextField::onTextMark(const str32 &text, u32 caretPos) {
    if (m_selectionLength) {
        str32::iterator it = m_text.begin() + m_selectionIndex;
        m_text.erase(it, it + m_selectionLength);
        m_selectionLength = 0;
		m_insertIndex = m_selectionIndex;
		m_selectionIndex = -1;
    }
    m_markedText = text;
    m_textDirty = true;
    m_caretIndex = m_insertIndex + caretPos;
	m_caretTimer = 0;
}

void UITextField::onTextUnmark() {
    m_markedText.clear();
    m_textDirty = true;
	m_caretIndex = m_insertIndex;
}

void UITextField::onTextNewLine() {
    if (!m_fnTextCompleted.null()) {
		str32 text = m_text;
		m_fnTextCompleted(this, text);
	}
}

void UITextField::onTextDeleteBackward() {
    if (m_selectionLength) {
        str32::iterator it = m_text.begin() + m_selectionIndex;
        m_text.erase(it, it + m_selectionLength);
        m_selectionLength = 0;
        m_textDirty = true;
		m_insertIndex = m_selectionIndex;
		m_selectionIndex = -1;
        m_caretIndex = m_insertIndex;
    } else if (m_insertIndex) {
        m_text.erase(m_text.begin() + --m_insertIndex);
        m_textDirty = true;
        m_caretIndex = m_insertIndex;
    }
	m_caretTimer = 0;
}

void UITextField::onTextDeleteForward() {
    if (m_selectionLength) {
        str32::iterator it = m_text.begin() + m_selectionIndex;
        m_text.erase(it, it + m_selectionLength);
        m_selectionLength = 0;
        m_textDirty = true;
		m_insertIndex = m_selectionIndex;
		m_selectionIndex = -1;
        m_caretIndex = m_insertIndex;
    } else if (m_insertIndex != m_text.length()) {
        m_text.erase(m_text.begin() + m_insertIndex);
        m_textDirty = true;
        m_caretIndex = m_insertIndex;
    }
	m_caretTimer = 0;
}

void UITextField::onTextMoveLeft() {
	if (m_selectionLength) {
		m_insertIndex = m_selectionIndex;
		m_selectionLength = 0;
		m_selectionIndex = -1;
		m_caretIndex = m_insertIndex;
	} else if (m_insertIndex) {
		m_caretIndex = --m_insertIndex;
	}
	m_caretTimer = 0;
}

void UITextField::onTextMoveRight() {
	if (m_selectionLength) {
		m_insertIndex = m_selectionIndex + m_selectionLength;
		m_selectionLength = 0;
		m_selectionIndex = -1;
		m_caretIndex = m_insertIndex;
	} else if (m_insertIndex != m_text.length()) {
		m_caretIndex = ++m_insertIndex;
	}
	m_caretTimer = 0;
}

void UITextField::_onUpdate(f32 deltaTime) {
    
    
	_updateText();
	
	// update selecting
	if (m_mouseDown) {
		f32 ptx = vn::GfxApplication::instance().getMousePosition().x - m_boundingBox.min_corner.x - m_textPosition.x;
		m_caretIndex = m_renderText.index_round(ptx);
		if (m_caretIndex == m_insertIndex) {
			m_selectionIndex = -1;
			m_selectionLength = 0;
		} else {
			if (m_caretIndex > m_insertIndex) {
				m_selectionIndex = m_insertIndex;
				m_selectionLength = m_caretIndex - m_insertIndex;
			} else {
				m_selectionIndex = m_caretIndex;
				m_selectionLength = m_insertIndex - m_caretIndex;
			}
		}
	}
	
	if (m_selectionLength) {
		m_selectionRect.min_corner.x = m_renderText.offset(m_selectionIndex) + m_textPosition.x;
		m_selectionRect.max_corner.x = m_renderText.offset(m_selectionIndex + m_selectionLength) + m_textPosition.x;
		m_selectionRect.min_corner.y = m_textPosition.y;
		m_selectionRect.max_corner.y = m_textPosition.y + m_renderText.size().y;
		m_selectionRect += m_boundingBox.min_corner;
	}
	
	// update caret
	if (!m_hasFocus) {
		return ;
	}
	
	f32 offset = m_renderText.offset(m_caretIndex);
    m_caretRect.min_corner = m_boundingBox.min_corner + m_textPosition + vector2f(offset, 0);
	m_caretRect.max_corner = m_caretRect.min_corner + vector2f(1, m_renderText.size().y);
	
	if (!m_markedText.empty()) {
		m_caretColor = m_markedTextColor;
	} else {
		m_caretColor = m_textColor;
	}

	m_caretTimer += deltaTime;
	while (m_caretTimer >= 1.f) {
		m_caretTimer -= 1.f;
	}
	
	vector2f inputPos = m_boundingBox.min_corner + m_textPosition;
	inputPos.x += m_renderText.offset(m_insertIndex);
	inputPos.y += m_renderText.size().y;
	
	if (inputPos.x < m_boundingBox.min_corner.x) {
		inputPos.x = m_boundingBox.min_corner.x;
	}
	GfxApplication::instance().setTextInputPosition(inputPos);
	
}

void UITextField::_onRender(UIRenderer *renderer) {
    m_renderText.render(renderer, m_boundingBox.min_corner + m_textPosition, m_textColor, m_renderIndex0, m_renderIndex1);
	if (m_selectionLength) {
		renderer->drawRect(m_selectionRect, m_selectionColor);
		m_renderText.render(renderer, m_boundingBox.min_corner + m_textPosition, m_selectedTextColor, m_renderIndex1, m_renderIndex2);
	} else {
		m_renderText.render(renderer, m_boundingBox.min_corner + m_textPosition, m_markedTextColor, m_renderIndex1, m_renderIndex2);
	}
    
    m_renderText.render(renderer, m_boundingBox.min_corner + m_textPosition, m_textColor, m_renderIndex2, m_renderIndex3);
	
	if (m_hasFocus && m_caretTimer < 0.5f) {
		renderer->drawRect(m_caretRect, m_caretColor);
	}
}

bool UITextField::_bindAction(const c8 *name, RefCounted *func_impl) {
    VN_UI_ELEMENT_BIND_ACTION(UITextField, "string", m_fnTextCompleted, name, func_impl);
	VN_UI_ELEMENT_BIND_ACTION(UITextField, "focus_in", m_fnFocusIn, name, func_impl);
	VN_UI_ELEMENT_BIND_ACTION(UITextField, "focus_out", m_fnFocusOut, name, func_impl);
    return UIElement::_bindAction(name, func_impl);
}

void UITextField::_updateText() {
	if (m_textDirty) {
        m_textDirty = false;
        str32 txt;
        str32::iterator it = m_text.begin() + m_insertIndex;
        txt.append(m_text.begin(), it);
        txt.append(m_markedText);
        txt.append(it, m_text.end());
        m_renderText.setText(txt);
    }
    m_renderText.update();
	
	vector2f size = m_boundingBox.size();
	switch (m_textAlignment & kTA_VerticalCenter) {
		case kTA_VerticalCenter:
			m_textPosition.y = (size.y - m_renderText.size().y) * 0.5f;
			break;
			
		case kTA_Bottom:
			m_textPosition.y = size.y - m_renderText.size().y;
			break;
			
		default:
			m_textPosition.y = 0;
			break;
	}
	
	
	if (size.x >= m_renderText.size().x + 2) {
		switch (m_textAlignment & kTA_HorizontalCenter) {
            case kTA_HorizontalCenter:
                m_textPosition.x = (size.x - m_renderText.size().x) * 0.5f;
                break;
                
            case kTA_Right:
                m_textPosition.x = size.x - m_renderText.size().x;
                break;
                
            default:
                m_textPosition.x = 0;
                break;
        }
		m_renderIndex0 = 0;
		if (m_selectionLength) {
			m_renderIndex1 = m_selectionIndex;
			m_renderIndex2 = m_selectionIndex + m_selectionLength;
		} else {
			m_renderIndex1 = m_insertIndex;
			m_renderIndex2 = m_insertIndex + m_markedText.length();
		}
		m_renderIndex3 = m_renderText.length();
		f32 right_delta = size.x - m_textPosition.x - m_renderText.offset(m_caretIndex) - 2;
		if (right_delta <= 0) {
			m_textPosition.x += right_delta;
		}
	} else {
		f32 caret = m_renderText.offset(m_caretIndex);
		f32 right_delta = size.x - m_textPosition.x - caret - 2;
		f32 left_delta = caret + m_textPosition.x;
		if (m_caretIndex) {
			left_delta -= m_renderText.width(m_caretIndex - 1);
		}
		if (right_delta <= 0) {
			m_textPosition.x += right_delta;
		} else if (left_delta <= 0) {
			m_textPosition.x -= left_delta;
		}
		right_delta = size.x - m_textPosition.x - m_renderText.size().x - 2;
		if (right_delta > 0) {
			m_textPosition.x += right_delta;
		}
		
		f32 left_off = -m_textPosition.x;
		f32 right_off = m_boundingBox.max_corner.x - m_textPosition.x;
		m_renderIndex0 = m_renderText.index(left_off);
		m_renderIndex3 = m_renderText.index(right_off);
		if (m_renderIndex3 != m_renderText.length()) {
			++m_renderIndex3;
		}
		
		if (m_selectionLength) {
			m_renderIndex1 = math::max_(m_selectionIndex, m_renderIndex0);
			m_renderIndex2 = math::min_(m_selectionIndex + m_selectionLength, m_renderIndex3);
		} else {
			m_renderIndex1 = math::max_(m_insertIndex, m_renderIndex0);
			m_renderIndex2 = math::min_(m_insertIndex + m_markedText.length(), m_renderIndex3);
		}
	}
}

_vn_end