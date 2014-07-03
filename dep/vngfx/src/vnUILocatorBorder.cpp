//
//  vnUILocatorBorder.cpp
//  vngfx
//
//  Created by Wayne on 13-12-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILocatorBorder.h"
#include "vnUIElement.h"
#include "vnUIRoot.h"

_vn_begin

UILocatorBorder::UILocatorBorder(UIElement *owner)
: UILocator(owner) {
	
}

UILocatorBorder::UILocatorBorder(const UILocatorBorder &other, UIElement *owner)
: UILocator(other, owner)
, m_nearDistance(other.m_nearDistance)
, m_farDistance(other.m_farDistance) {
	
}

UILocatorBorder * UILocatorBorder::clone(UIElement *owner) const {
	return vnnew UILocatorBorder(*this, owner);
}

void UILocatorBorder::setBorder(f32 left, f32 top, f32 right, f32 bottom) {
	m_nearDistance.set(left, top);
	m_farDistance.set(right, bottom);
	m_dirty = true;
}

void UILocatorBorder::init(const TreeDataObject *object) {
	m_dirty = true;
	m_nearDistance.x = object->queryAttributeFloat("left", 0);
    m_nearDistance.y = object->queryAttributeFloat("top", 0);
	m_farDistance.x = object->queryAttributeFloat("right");
	m_farDistance.y = object->queryAttributeFloat("bottom");
}

void UILocatorBorder::_set(const aabox2f &location) {
	UIElement *parent = m_owner->parent();
	aabox2f parentRect;
	if (parent) {
		parentRect = parent->boundingBox();
	} else {
		parentRect.max_corner = UIRoot::instance().getViewSize();
	}
	m_nearDistance = location.min_corner - parentRect.min_corner;
	m_farDistance = parentRect.max_corner - location.max_corner;
	m_dirty = false;
}

void UILocatorBorder::_updateBoundingBox() {
	UIElement *parent = m_owner->parent();
	aabox2f parentRect;
	if (parent) {
		parentRect = parent->boundingBox();
	} else {
		parentRect.max_corner = UIRoot::instance().getViewSize();
	}
	aabox2f rect;
	rect.min_corner = parentRect.min_corner + m_nearDistance;
	rect.max_corner = parentRect.max_corner - m_farDistance;
	_setOwnerBoundingBox(rect);
}



_vn_end