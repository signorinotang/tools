//
//  vnUILocator.cpp
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILocator.h"
#include "vnUIElement.h"

_vn_begin

UILocator::UILocator(UIElement *owner)
: m_owner(owner) {
	vnassert(owner != 0);
	if (m_owner->m_locator) {
		delete m_owner->m_locator;
	}
	m_owner->m_locator = this;
}

UILocator::UILocator(const UILocator &other, UIElement *owner)
: m_owner(owner)
, m_dirty(other.m_dirty) {
	vnassert(owner != 0);
	if (m_owner->m_locator) {
		delete m_owner->m_locator;
	}
	m_owner->m_locator = this;
}

UILocator::~UILocator() {
	m_owner->m_locator = 0;
}

void UILocator::markDirty() {
	m_dirty = true;
}

void UILocator::cleanDirty() {
	if (m_dirty) {
		m_dirty = false;
		_updateBoundingBox();
	}
}

bool UILocator::isDirty() const {
	return m_dirty;
}

void UILocator::update(f32 deltaTime) {
	_onUpdate(deltaTime);
	cleanDirty();
}

void UILocator::init(const vn::TreeDataObject *object) {

}

void UILocator::_onUpdate(f32 deltaTime) {
	
}

void UILocator::_setOwnerBoundingBox(const aabox2f &boundingBox) {
	m_owner->m_boundingBox = boundingBox;
	m_owner->_onBoundingBoxUpdated();
}

_vn_end