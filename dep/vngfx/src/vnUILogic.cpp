//
//  vnUILogic.c
//  vngfx
//
//  Created by Wayne on 13-12-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILogic.h"
#include "vnUIElement.h"
#include "vnLog.h"

_vn_begin

UILogic::UILogic()
: m_root(0) {
    
}

UILogic::UILogic(const UILogic &other)
: m_root(other.m_root ? other.m_root->clone() : 0) {
    
}

UILogic * UILogic::clone() const {
    return vnnew UILogic(*this);
}

UILogic::~UILogic() {
    VN_SAFE_DROP(m_root);
}

void UILogic::init(vn::UIElement *root, bool grab) {
    vnassert(!m_root && root);
    if (grab) {
        root->grab();
    }
    m_root = root;
}

void UILogic::init(const TreeDataObject *object) {
    vnassert(!m_root);
    const TreeDataNode * tdnode = object->queryAttributeNode("root");
    if (tdnode) {
        m_root = UIFactory::instance().createElement(tdnode);
    }
}

UIElement * UILogic::root() const {
    return m_root;
}

bool UILogic::startup() {
    if (!m_root) {
        VN_LOG_ERROR("UILogic: not initialized.");
        return false;
    }
    UIFactory::ElementMap namedElements;
    m_root->buildElementMap(namedElements);
    return _startup(namedElements);
}

void UILogic::update(f32 deltaTime) {
    
}

bool UILogic::_startup(UIFactory::ElementMap &namedElements) {
    return true;
}

_vn_end