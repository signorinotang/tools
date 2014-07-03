//
//  UIListItemDecorationImage.cpp
//  TDMapEditor
//
//  Created by Wayne on 3/31/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemDecorationImage.h"
#include "vnUIPicture.h"
#include "vnPictureManager.h"

VN_IMPL_UI_LOGIC(UIListItemDecorationImage, "DecorationImage")

UIListItemDecorationImage::UIListItemDecorationImage()
: m_image(0) {
    
}

UIListItemDecorationImage::UIListItemDecorationImage(const UIListItemDecorationImage &other)
: UIListItemText(other)
, m_image(0)
, m_imagePath(other.m_imagePath) {
    
}

UIListItemDecorationImage * UIListItemDecorationImage::clone() const {
    return vnnew UIListItemDecorationImage(*this);
}


void UIListItemDecorationImage::set(const vn::TreeDataObject *object) {
    UIListItemText::set(object);
    object->queryAttributeString("image", m_imagePath);
    if (m_image) {
        vn::PicturePtr pic = vn::PictureManager::instance().createPicture(m_imagePath.c_str());
        if (!pic.null()) {
            vn::UIRenderPicture1 *img = vnnew vn::UIRenderPicture1();
            img->setPicture(pic);
            m_image->setPicture(img);
        }
    }
}

const vn::str8 & UIListItemDecorationImage::getImagePath() const {
    return m_imagePath;
}

bool UIListItemDecorationImage::_startup(vn::UIFactory::ElementMap &namedElements) {
    if (!UIListItemText::_startup(namedElements)) {
        return false;
    }
    m_image = dynamic_cast<vn::UIPicture *>(namedElements["image"]);
    if (!m_image) {
        return false;
    }
    return true;
}