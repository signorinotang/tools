//
//  vnUIPicture.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIPicture.h"
#include "vnUIRenderer.h"
#include "vnPictureManager.h"

_vn_begin

UIPicture::UIPicture()
: m_color(color_t::kWhite)
, m_picture(vnnew UIRenderPicture0()){
    
}

UIPicture::UIPicture(const UIPicture &other)
: UIElement(other)
, m_color(other.m_color)
, m_picture(other.m_picture->clone()) {
	
}

UIPicture * UIPicture::clone() const {
    return vnnew UIPicture(*this);
}

const color_t & UIPicture::getColor() const {
    return m_color;
}

void UIPicture::setColor(const color_t &color) {
    m_color = color;
}

const UIRenderPicturePtr & UIPicture::getPicture() const {
    return m_picture;
}

void UIPicture::setPicture(const UIRenderPicturePtr &picture) {
	vnassert(!picture.null());
    m_picture = picture;
    m_picture->setLocation(m_boundingBox);
}

void UIPicture::init(const TreeDataObject *object) {
    UIElement::init(object);
    m_color.convertFromBGRA(object->queryAttributeInteger("color", 0xFFFFFFFF));
	const TreeDataObject *tdobj = object->queryAttributeObject("image");
	UIRenderPicture *pic = 0;
	if (tdobj) {
		pic = UIFactory::instance().createRenderPicture(tdobj);
	}
	if (pic) {
		m_picture = pic;
	}
}

void UIPicture::_onBoundingBoxUpdated() {
	UIElement::_onBoundingBoxUpdated();
	m_picture->setLocation(m_boundingBox);
}

void UIPicture::_onUpdate(f32 deltaTime) {
	m_picture->update(deltaTime);
}

void UIPicture::_onRender(UIRenderer *renderer) {
    m_picture->render(renderer, m_color);
}

_vn_end