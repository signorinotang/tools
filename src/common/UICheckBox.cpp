//
//  UICheckBox.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UICheckBox.h"
#include "vnUIRenderer.h"

VN_IMPL_UI_ELEMENT(UICheckBox, "CheckBox")

UICheckBox * UICheckBox::clone() const {
	return vnnew UICheckBox(*this);
}

void UICheckBox::_onRender(vn::UIRenderer *renderer) {
	static vn::color_t c_disabled(0x80, 0x80, 0x80);
	static vn::color_t c_normal(0xE0, 0xE0, 0xE0);
	static vn::color_t c_hover(0xF0, 0xF0, 0xF0);
	static vn::color_t c_pressed(0xE8, 0xE8, 0xE8);
	switch (m_state) {
		case kDisabled:
			_drawFrame(renderer, c_disabled);
			if (m_pushed) {
				_drawBody(renderer, c_disabled);
			}
			break;
			
		case kNormal:
			_drawFrame(renderer, c_normal);
			break;
			
		case kNormalMouseEntered:
			_drawFrame(renderer, c_hover);
			break;
			
		case kNormalPressed:
			_drawFrame(renderer, c_pressed);
			break;
			
		case kPushed:
			_drawFrame(renderer, c_normal);
			_drawBody(renderer, c_normal);
			break;
			
		case kPushedMouseEntered:
			_drawFrame(renderer, c_hover);
			_drawBody(renderer, c_hover);
			break;
			
		case kPushedPressed:
			_drawFrame(renderer, c_pressed);
			_drawBody(renderer, c_pressed);
			break;
	}
}

void UICheckBox::_drawFrame(vn::UIRenderer *renderer, const vn::color_t &color) {
	vn::aabox2f rect;
	rect.min_corner = m_boundingBox.min_corner;
	rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 2);
	renderer->drawRect(rect, color);
	
	rect.max_corner.set(m_boundingBox.min_corner.x + 2, m_boundingBox.max_corner.y);
	renderer->drawRect(rect, color);
	
	rect.max_corner = m_boundingBox.max_corner;
	rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 2);
	renderer->drawRect(rect, color);
	
	rect.min_corner.set(m_boundingBox.max_corner.x - 2, m_boundingBox.min_corner.y);
	renderer->drawRect(rect, color);
}

void UICheckBox::_drawBody(vn::UIRenderer *renderer, const vn::color_t &color) {
	vn::aabox2f rect;
	rect.min_corner = m_boundingBox.min_corner + vn::vector2f(4, 4);
	rect.max_corner = m_boundingBox.max_corner - vn::vector2f(4, 4);
	renderer->drawRect(rect, color);
}