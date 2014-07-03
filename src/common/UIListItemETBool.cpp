//
//  UIListItemETBool.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemETBool.h"

VN_IMPL_UI_LOGIC(UIListItemETBool, "ListItemETBool")

UIListItemETBool::UIListItemETBool()
: m_checkbox(0)
, m_target(0)
, m_editable(true) {
	
}

UIListItemETBool::UIListItemETBool(const UIListItemETBool &other)
: UIListItemText(other)
, m_target(0)
, m_checkbox(0)
, m_editable(other.m_editable) {
	
}

UIListItemETBool * UIListItemETBool::clone() const {
	return vnnew UIListItemETBool(*this);
}

UIListItemETBool::~UIListItemETBool() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void UIListItemETBool::ev_destroyed(vn::EditValue *target) {
	if (target == m_target) {
		m_target = 0;
	}
}

void UIListItemETBool::ev_changed(vn::EditValue *target) {
	m_checkbox->setPushed(m_target->getValue());
}

void UIListItemETBool::setTarget(vn::EditValueBool *value) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	if ((m_target = value)) {
		m_target->addObserver(this);
		m_checkbox->setPushed(value->getValue());
		m_checkbox->setEnabled(m_editable);
	} else {
		m_checkbox->setEnabled(false);
	}
}

void UIListItemETBool::setEditable(bool editable) {
	m_editable = editable;
	if (m_target) {
		m_checkbox->setEnabled(editable);
	}
}

bool UIListItemETBool::_startup(vn::UIFactory::ElementMap &namedElements) {
	if (!UIListItemText::_startup(namedElements)) {
		return false;
	}
	m_checkbox = dynamic_cast<vn::UIButton *>(namedElements["checkbox"]);
	if (!m_checkbox) {
		return false;
	}
	m_checkbox->bindAction_Clicked(vn::make_memfn(this, &UIListItemETBool::_onClicked));
	m_checkbox->setEnabled(false);
	return true;
}

void UIListItemETBool::_onClicked(vn::UIElement *sender) {
	if (m_target) {
		m_target->setValue(!m_checkbox->isPushed(), true);
	}
}

