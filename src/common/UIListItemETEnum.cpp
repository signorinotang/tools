//
//  UIListItemETEnum.cpp
//  test
//
//  Created by Wayne on 14-1-5.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemETEnum.h"
#include "vnUILabel.h"

VN_IMPL_UI_LOGIC(UIListItemETEnum, "ListItemETEnum")


UIListItemETEnum::UIListItemETEnum()
: m_list(0)
, m_button(0)
, m_value(0)
, m_target(0)
, m_editable(true) {
	
}

UIListItemETEnum::UIListItemETEnum(const UIListItemETEnum &other)
: UIListItemText(other)
, m_target(0)
, m_list(0)
, m_button(0)
, m_value(0)
, m_editable(other.m_editable) {
	
}

UIListItemETEnum * UIListItemETEnum::clone() const {
	return vnnew UIListItemETEnum(*this);
}

UIListItemETEnum::~UIListItemETEnum() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void UIListItemETEnum::ev_destroyed(vn::EditValue *target) {
	if (target == m_target) {
		m_target = 0;
	}
}

void UIListItemETEnum::ev_changed(vn::EditValue *target) {
	size_t index = m_list->getItemIndexByData((void *)(long)m_target->getValue());
	m_list->setSelectedItemIndex(index);
}

void UIListItemETEnum::setTarget(vn::EditValueInt *value) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	const vn::EditTypeEnumeration *t_enum = dynamic_cast<const vn::EditTypeEnumeration *>(value->type());
	if (!t_enum) {
		value = 0;
	}
	m_list->clearItems();
	if ((m_target = value)) {
		m_target->addObserver(this);
		m_button->setEnabled(m_editable);
		const vn::EditTypeEnumeration::Items &items = t_enum->items();
		for (vn::EditTypeEnumeration::Items::const_iterator it = items.begin(); it != items.end(); ++it) {
			size_t index = m_list->addItem(it->first, (void *)(long)it->second);
			if (value->getValue() == it->second) {
				m_list->setSelectedItemIndex(index);
			}
		}
	} else {
		m_button->setEnabled(false);
		m_list->setSelectedItemIndex(-1);
	}
}

void UIListItemETEnum::setEditable(bool editable) {
	m_editable = editable;
	if (m_target) {
		m_button->setEnabled(editable);
	}
}

bool UIListItemETEnum::_startup(vn::UIFactory::ElementMap &namedElements) {
	if (!UIListItemText::_startup(namedElements)) {
		return false;
	}
	m_button = dynamic_cast<vn::UIButton *>(namedElements["button"]);
	m_value = dynamic_cast<vn::UILabel *>(namedElements["value"]);
	m_list = dynamic_cast<UIPopList *>(namedElements["list"]);
	if (!m_button || !m_value || !m_list) {
		return false;
	}
	m_button->setEnabled(false);
	m_button->bindAction_Clicked(vn::make_memfn(this, &UIListItemETEnum::_onClicked));
	m_list->bindAction_SelectedChanged(vn::make_memfn(this, &UIListItemETEnum::_onSelectedChanged));
	return true;
}

void UIListItemETEnum::_onClicked(vn::UIElement *) {
	m_list->pop();
}

void UIListItemETEnum::_onSelectedChanged(vn::UIElement *, size_t index) {
	if (index == -1) {
		m_value->setText(vn::str8());
		return ;
	}
	vn::s32 value = (vn::u32)(long)m_list->getItemData(index);
	m_value->setText(m_list->getItemText(index));
	if (m_target) {
		m_target->setValue(value, true);
	}
}