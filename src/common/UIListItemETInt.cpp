//
//  UIListItemETInt.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemETInt.h"
#include "vnUnicode.h"

VN_IMPL_UI_LOGIC(UIListItemETInt, "ListItemETInt")


UIListItemETInt::UIListItemETInt()
: m_editbox(0)
, m_target(0)
, m_editable(true) {
	
}

UIListItemETInt::UIListItemETInt(const UIListItemETInt &other)
: UIListItemText(other)
, m_target(0)
, m_editbox(0)
, m_editable(other.m_editable) {
	
}

UIListItemETInt * UIListItemETInt::clone() const {
	return vnnew UIListItemETInt(*this);
}

UIListItemETInt::~UIListItemETInt() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void UIListItemETInt::ev_destroyed(vn::EditValue *target) {
	if (target == m_target) {
		m_target = 0;
	}
}

void UIListItemETInt::ev_changed(vn::EditValue *target) {
	vn::c8 buf[16];
	vn::str8 txt;
	txt.assign(buf, sprintf(buf, "%d", m_target->getValue()));
	m_editbox->setText(txt);
}

void UIListItemETInt::setTarget(vn::EditValueInt *value) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	if ((m_target = value)) {
		m_target->addObserver(this);
		m_editbox->setEnabled(m_editable);
		vn::c8 buf[16];
		vn::str8 txt;
		txt.assign(buf, sprintf(buf, "%d", m_target->getValue()));
		m_editbox->setText(txt);
	} else {
		m_editbox->setEnabled(false);
	}
}

void UIListItemETInt::setEditable(bool editable) {
	m_editable = editable;
	if (m_target) {
		m_editbox->setEnabled(editable);
	}
}

bool UIListItemETInt::_startup(vn::UIFactory::ElementMap &namedElements) {
	if (!UIListItemText::_startup(namedElements)) {
		return false;
	}
	m_editbox = dynamic_cast<vn::UITextField *>(namedElements["editbox"]);
	if (!m_editbox) {
		return false;
	}
	m_editbox->setEnabled(false);
	m_editbox->bindAction_TextCompleted(vn::make_memfn(this, &UIListItemETInt::_onEditBoxString));
	m_editbox->bindAction_FocusIn(vn::make_memfn(this, &UIListItemETInt::_onEditBoxGotFocus));
	m_editbox->bindAction_FocusOut(vn::make_memfn(this, &UIListItemETInt::_onEditBoxLoseFocus));
	return true;
}

void UIListItemETInt::_onEditBoxGotFocus(vn::UIElement *) {
	m_editbox->selectAll();
}

void UIListItemETInt::_onEditBoxLoseFocus(vn::UIElement *) {
	m_editbox->selectNone();
	if (m_target) {
		vn::str8 txt;
		vn::Unicode::convert(m_editbox->getText().c_str(), m_editbox->getText().length(), txt);
		vn::s32 value = m_target->getValue();
		if (sscanf(txt.c_str(), "%d", &value)) {
			_setValue(value);
		} else {
			ev_changed(m_target);
		}
	}
}

void UIListItemETInt::_onEditBoxString(vn::UIElement *, const vn::str32 &) {
	vn::UIRoot::instance().releaseFocus(m_editbox);
}

void UIListItemETInt::_setValue(vn::s32 value) {
	if (value != m_target->getValue()) {
		m_target->setValue(value, true);
	} else {
		ev_changed(m_target);
	}
}