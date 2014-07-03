//
//  UIListItemETFloat.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemETFloat.h"
#include "vnUnicode.h"

VN_IMPL_UI_LOGIC(UIListItemETFloat, "ListItemETFloat")


UIListItemETFloat::UIListItemETFloat()
: m_editbox(0)
, m_target(0)
, m_editable(true) {
	
}

UIListItemETFloat::UIListItemETFloat(const UIListItemETFloat &other)
: UIListItemText(other)
, m_target(0)
, m_editbox(0)
, m_editable(other.m_editable) {
	
}

UIListItemETFloat * UIListItemETFloat::clone() const {
	return vnnew UIListItemETFloat(*this);
}

UIListItemETFloat::~UIListItemETFloat() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void UIListItemETFloat::ev_destroyed(vn::EditValue *target) {
	if (target == m_target) {
		m_target = 0;
	}
}

void UIListItemETFloat::ev_changed(vn::EditValue *target) {
	vn::c8 buf[16];
	vn::str8 txt;
	txt.assign(buf, sprintf(buf, "%g", m_target->getValue()));
	m_editbox->setText(txt);
}

void UIListItemETFloat::setTarget(vn::EditValueFloat *value) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	if ((m_target = value)) {
		m_target->addObserver(this);
		m_editbox->setEnabled(m_editable);
		ev_changed(m_target);
	} else {
		m_editbox->setEnabled(false);
	}
}

void UIListItemETFloat::setEditable(bool editable) {
	m_editable = editable;
	if (m_target) {
		m_editbox->setEnabled(editable);
	}
}

bool UIListItemETFloat::_startup(vn::UIFactory::ElementMap &namedElements) {
	if (!UIListItemText::_startup(namedElements)) {
		return false;
	}
	m_editbox = dynamic_cast<vn::UITextField *>(namedElements["editbox"]);
	if (!m_editbox) {
		return false;
	}
	m_editbox->setEnabled(false);
	m_editbox->bindAction_TextCompleted(vn::make_memfn(this, &UIListItemETFloat::_onEditBoxString));
	m_editbox->bindAction_FocusIn(vn::make_memfn(this, &UIListItemETFloat::_onEditBoxGotFocus));
	m_editbox->bindAction_FocusOut(vn::make_memfn(this, &UIListItemETFloat::_onEditBoxLoseFocus));
	return true;
}

void UIListItemETFloat::_onEditBoxGotFocus(vn::UIElement *) {
	m_editbox->selectAll();
}

void UIListItemETFloat::_onEditBoxLoseFocus(vn::UIElement *) {
	m_editbox->selectNone();
	if (m_target) {
		vn::str8 txt;
		vn::Unicode::convert(m_editbox->getText().c_str(), m_editbox->getText().length(), txt);
		vn::f32 value = m_target->getValue();
		if (sscanf(txt.c_str(), "%g", &value)) {
			_setValue(value);
		} else {
			ev_changed(m_target);
		}
	}
}

void UIListItemETFloat::_onEditBoxString(vn::UIElement *, const vn::str32 &) {
	vn::UIRoot::instance().releaseFocus(m_editbox);
}

void UIListItemETFloat::_setValue(vn::f32 value) {
	if (value != m_target->getValue()) {
		m_target->setValue(value, true);
	} else {
		ev_changed(m_target);
	}
}