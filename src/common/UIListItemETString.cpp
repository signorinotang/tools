//
//  UIListItemETString.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemETString.h"
#include "vnUnicode.h"

VN_IMPL_UI_LOGIC(UIListItemETString, "ListItemETString")


UIListItemETString::UIListItemETString()
: m_editbox(0)
, m_target(0)
, m_editable(true) {
	
}

UIListItemETString::UIListItemETString(const UIListItemETString &other)
: UIListItemText(other)
, m_target(0)
, m_editbox(0)
, m_editable(other.m_editable) {
	
}

UIListItemETString * UIListItemETString::clone() const {
	return vnnew UIListItemETString(*this);
}

UIListItemETString::~UIListItemETString() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void UIListItemETString::ev_destroyed(vn::EditValue *target) {
	if (target == m_target) {
		m_target = 0;
	}
}

void UIListItemETString::ev_changed(vn::EditValue *target) {
	m_editbox->setText(m_target->getValue());
}

void UIListItemETString::setTarget(vn::EditValueString *value) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	if ((m_target = value)) {
		m_target->addObserver(this);
		m_editbox->setEnabled(m_editable);
		m_editbox->setText(value->getValue());
	} else {
		m_editbox->setText(vn::str32());
		m_editbox->setEnabled(false);
	}
}

void UIListItemETString::setEditable(bool editable) {
	m_editable = editable;
	if (m_target) {
		m_editbox->setEnabled(editable);
	}
}

bool UIListItemETString::_startup(vn::UIFactory::ElementMap &namedElements) {
	if (!UIListItemText::_startup(namedElements)) {
		return false;
	}
	m_editbox = dynamic_cast<vn::UITextField *>(namedElements["editbox"]);
	if (!m_editbox) {
		return false;
	}
	m_editbox->setEnabled(false);
	m_editbox->bindAction_TextCompleted(vn::make_memfn(this, &UIListItemETString::_onEditBoxString));
	m_editbox->bindAction_FocusIn(vn::make_memfn(this, &UIListItemETString::_onEditBoxGotFocus));
	m_editbox->bindAction_FocusOut(vn::make_memfn(this, &UIListItemETString::_onEditBoxLoseFocus));
	return true;
}

void UIListItemETString::_onEditBoxGotFocus(vn::UIElement *) {
	m_editbox->selectAll();
}

void UIListItemETString::_onEditBoxLoseFocus(vn::UIElement *) {
	m_editbox->selectNone();
	if (m_target) {
		vn::str8 value;
		vn::Unicode::convert(m_editbox->getText().c_str(), m_editbox->getText().length(), value);
		m_target->setValue(value, true);
	}
}

void UIListItemETString::_onEditBoxString(vn::UIElement *, const vn::str32 &) {
	vn::UIRoot::instance().releaseFocus(m_editbox);
}