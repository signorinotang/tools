//
//  UIListItemDecorationAnimation.cpp
//  TDMapEditor
//
//  Created by Wayne on 5/21/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIListItemDecorationAnimation.h"

VN_IMPL_UI_LOGIC(UIListItemDecorationAnimation, "DecorationAnimation")

UIListItemDecorationAnimation * UIListItemDecorationAnimation::clone() const {
    return vnnew UIListItemDecorationAnimation(*this);
}

void UIListItemDecorationAnimation::set(const vn::TreeDataObject *object) {
    UIListItemText::set(object);
    object->queryAttributeString("anim", m_animationPath);
}

const vn::str8 & UIListItemDecorationAnimation::getAnimationPath() const {
    return m_animationPath;
}

