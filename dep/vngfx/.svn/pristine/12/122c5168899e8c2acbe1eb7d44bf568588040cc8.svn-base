//
//  UIRenderPicture.cpp
//  vngfx
//
//  Created by Wayne on 13-12-22.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIRenderPicture.h"
#include "vnUIRenderer.h"
#include "vnPictureManager.h"

_vn_begin

void UIRenderPicture::update(f32 deltaTime) {
	
}

void UIRenderPicture::render(UIRenderer *renderer, const color_t &color) {
	
}

void UIRenderPicture::setLocation(const aabox2f &rect) {
	
}

void UIRenderPicture::init(const TreeDataObject *object) {
	
}

vector2f UIRenderPicture::calculateMinimumSize() const {
	return vector2f::kZero;
}

UIRenderPictureEmpty * UIRenderPictureEmpty::clone() const {
	return vnnew UIRenderPictureEmpty();
}

UIRenderPicture0 * UIRenderPicture0::clone() const {
	return vnnew UIRenderPicture0(*this);
}

void UIRenderPicture0::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect, color);
}

void UIRenderPicture0::setLocation(const aabox2f &rect) {
	m_rect = rect;
}

UIRenderPicture1 * UIRenderPicture1::clone() const {
	return vnnew UIRenderPicture1(*this);
}

void UIRenderPicture1::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect, color, m_picture.ptr());
}

void UIRenderPicture1::init(const vn::TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image");
    if (tdstr) {
        m_picture = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
}

void UIRenderPicture1::setPicture(const PicturePtr &picture) {
	m_picture = picture;
}

const PicturePtr & UIRenderPicture1::getPicture() const {
	return m_picture;
}

UIRenderPicture3V::UIRenderPicture3V()
: m_heightTop(0)
, m_heightBottom(0) {
	
}

UIRenderPicture3V * UIRenderPicture3V::clone() const {
	return vnnew UIRenderPicture3V(*this);
}

void UIRenderPicture3V::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
}

void UIRenderPicture3V::setLocation(const aabox2f &rect) {
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner.set(rect.max_corner.x, rect.min_corner.y + m_heightTop);
	m_rect[2].min_corner.set(rect.min_corner.x, rect.max_corner.y - m_heightBottom);
	m_rect[2].max_corner = rect.max_corner;
	m_rect[1].min_corner.set(rect.min_corner.x, m_rect[0].max_corner.y);
	m_rect[1].max_corner.set(rect.max_corner.x, m_rect[2].min_corner.y);
}

void UIRenderPicture3V::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image1");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image2");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image3");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_heightTop = object->queryAttributeFloat("height_top");
	m_heightBottom = object->queryAttributeFloat("height_bottom");
}

vector2f UIRenderPicture3V::calculateMinimumSize() const {
	return vector2f(0, m_heightTop + m_heightBottom);
}

void UIRenderPicture3V::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 3);
	m_picture[index] = picture;
}

const PicturePtr & UIRenderPicture3V::getPicture(size_t index) const {
	vnassert(index < 3);
	return m_picture[index];
}

void UIRenderPicture3V::setHeightTop(f32 height) {
	m_heightTop = height;
}

f32 UIRenderPicture3V::getHeightTop() const {
	return m_heightTop;
}

void UIRenderPicture3V::setHeightBottom(f32 height) {
	m_heightBottom = height;
}

f32 UIRenderPicture3V::getHeightBottom() const {
	return m_heightBottom;
}

UIRenderPicture3H::UIRenderPicture3H()
: m_widthLeft(0)
, m_widthRight(0) {
	
}

UIRenderPicture3H * UIRenderPicture3H::clone() const {
	return vnnew UIRenderPicture3H(*this);
}

void UIRenderPicture3H::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
}

void UIRenderPicture3H::setLocation(const aabox2f &rect) {
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner.set(rect.min_corner.x + m_widthLeft, rect.max_corner.y);
	m_rect[2].min_corner.set(rect.max_corner.x - m_widthRight, rect.min_corner.y);
	m_rect[2].max_corner = rect.max_corner;
	m_rect[1].min_corner.set(m_rect[0].max_corner.x, rect.min_corner.y);
	m_rect[1].max_corner.set(m_rect[2].min_corner.x, rect.max_corner.y);
}

void UIRenderPicture3H::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image1");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image2");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image3");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_widthLeft = object->queryAttributeFloat("width_left");
	m_widthRight = object->queryAttributeFloat("width_right");
}

vector2f UIRenderPicture3H::calculateMinimumSize() const {
	return vector2f(m_widthLeft + m_widthRight, 0);
}

void UIRenderPicture3H::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 3);
	m_picture[index] = picture;
}

const PicturePtr & UIRenderPicture3H::getPicture(size_t index) const {
	vnassert(index < 3);
	return m_picture[index];
}

void UIRenderPicture3H::setWidthLeft(f32 width) {
	m_widthLeft = width;
}

f32 UIRenderPicture3H::getWidthLeft() const {
	return m_widthRight;
}

void UIRenderPicture3H::setWidthRight(f32 width) {
	m_widthRight = width;
}

f32 UIRenderPicture3H::getWidthRight() const {
	return m_widthRight;
}

UIRenderPicture9::UIRenderPicture9()
: m_widthLeft(0)
, m_widthRight(0)
, m_heightTop(0)
, m_heightBottom(0) {
	
}

UIRenderPicture9 * UIRenderPicture9::clone() const {
	return vnnew UIRenderPicture9(*this);
}

void UIRenderPicture9::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
	renderer->drawRect(m_rect[3], color, m_picture[3].ptr());
	renderer->drawRect(m_rect[4], color, m_picture[4].ptr());
	renderer->drawRect(m_rect[5], color, m_picture[5].ptr());
	renderer->drawRect(m_rect[6], color, m_picture[6].ptr());
	renderer->drawRect(m_rect[7], color, m_picture[7].ptr());
	renderer->drawRect(m_rect[8], color, m_picture[8].ptr());
}

void UIRenderPicture9::setLocation(const aabox2f &rect) {
	m_rect[4].min_corner = rect.min_corner + vector2f(m_widthLeft, m_heightTop);
	m_rect[4].max_corner = rect.max_corner - vector2f(m_widthRight, m_heightBottom);
	
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner = m_rect[4].min_corner;
	
	m_rect[1].min_corner.set(m_rect[4].min_corner.x, rect.min_corner.y);
	m_rect[1].max_corner.set(m_rect[4].max_corner.x, m_rect[4].min_corner.y);
	
	m_rect[2].min_corner.set(m_rect[4].max_corner.x, rect.min_corner.y);
	m_rect[2].max_corner.set(rect.max_corner.x, m_rect[4].min_corner.y);
	
	m_rect[3].min_corner.set(rect.min_corner.x, m_rect[4].min_corner.y);
	m_rect[3].max_corner.set(m_rect[4].min_corner.x, m_rect[4].max_corner.y);
	
	m_rect[5].min_corner.set(m_rect[4].max_corner.x, m_rect[4].min_corner.y);
	m_rect[5].max_corner.set(rect.max_corner.x, m_rect[4].max_corner.y);
	
	m_rect[6].min_corner.set(rect.min_corner.x, m_rect[4].max_corner.y);
	m_rect[6].max_corner.set(m_rect[4].min_corner.x, rect.max_corner.y);
	
	m_rect[7].min_corner.set(m_rect[4].min_corner.x, m_rect[4].max_corner.y);
	m_rect[7].max_corner.set(m_rect[4].max_corner.x, rect.max_corner.y);
	
	m_rect[8].min_corner = m_rect[4].max_corner;
	m_rect[8].max_corner = rect.max_corner;
}

void UIRenderPicture9::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image11");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image12");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image13");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image21");
    if (tdstr) {
        m_picture[3] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image22");
    if (tdstr) {
        m_picture[4] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image23");
    if (tdstr) {
        m_picture[5] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image31");
    if (tdstr) {
        m_picture[6] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image32");
    if (tdstr) {
        m_picture[7] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image33");
    if (tdstr) {
        m_picture[8] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_widthLeft = object->queryAttributeFloat("width_left");
	m_widthRight = object->queryAttributeFloat("width_right");
	m_heightTop = object->queryAttributeFloat("height_top");
	m_heightBottom = object->queryAttributeFloat("height_bottom");
}

vector2f UIRenderPicture9::calculateMinimumSize() const {
	return vector2f(m_widthLeft + m_widthRight, m_heightTop + m_heightBottom);
}

void UIRenderPicture9::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 9);
	m_picture[index] = picture;
}

const PicturePtr & UIRenderPicture9::getPicture(size_t index) const {
	vnassert(index < 9);
	return m_picture[index];
}

void UIRenderPicture9::setWidthLeft(f32 width) {
	m_widthLeft = width;
}

f32 UIRenderPicture9::getWidthLeft() const {
	return m_widthRight;
}

void UIRenderPicture9::setWidthRight(f32 width) {
	m_widthRight = width;
}

f32 UIRenderPicture9::getWidthRight() const {
	return m_widthRight;
}

void UIRenderPicture9::setHeightTop(f32 height) {
	m_heightTop = height;
}

f32 UIRenderPicture9::getHeightTop() const {
	return m_heightTop;
}

void UIRenderPicture9::setHeightBottom(f32 height) {
	m_heightBottom = height;
}

f32 UIRenderPicture9::getHeightBottom() const {
	return m_heightBottom;
}

_vn_end