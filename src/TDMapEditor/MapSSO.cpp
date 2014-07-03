//
//  MapSSO.cpp
//  TDMapEditor
//
//  Created by Wayne on 6/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapSSO.h"


MapSSO::MapSSO(const MapStateScriptObjectDescPtr &desc)
: m_desc(desc) {
    if (m_desc.null()) {
        m_desc = vnnew MapStateScriptObjectDesc();
    }
    m_objectNode->addRenderable(m_quad, false);
    this->addChild(m_objectNode, false);
    
    *m_frame << 4.f;
    m_frameNode->setVisible(false);
    m_frameNode->addRenderable(m_frame, false);
    m_frameNode->setInheritTransformM(false);
    m_frameNode->setInheritTransformT(false);
    this->addChild(m_frameNode, false);
}

bool MapSSO::init(const FilePath &scriptFile, const vn::vector2f &pos) {
    if (!sso_init(scriptFile)) {
        return false;
    }
    setPosition(pos);

    m_desc->scriptFile = scriptFile;
    m_desc->position = pos;
    return true;
}

bool MapSSO::init() {
    if (!sso_init(m_desc->scriptFile)) {
        return false;
    }
    setPosition(m_desc->position);
    return true;
}

void MapSSO::sso_setVisible(bool visible) {
    m_objectNode->setVisible(visible);
}

void MapSSO::sso_setScale(const vn::vector2f &scale) {
    m_objectNode->setScale(scale);
}

void MapSSO::sso_move(const vn::vector2f &offset) {
    m_objectNode->setPosition(m_objectNode->getPosition() + offset * 40.f);
}

void MapSSO::sso_setRotation(const vn::f32 rotation) {
    m_objectNode->setRotation(rotation);
}

void MapSSO::sso_destroy() {
    
}

void MapSSO::anim_frame(const aabox2f &rect, const PicturePtr &image) {
    *m_quad << rect << image.ptr();
    m_objectNode->setLocalBoundingBox(rect);
}

void MapSSO::setPosition(const vector2f &position) {
    SceneNode::setPosition(position);
    m_desc->position = position;
}

void MapSSO::update(vn::f32 deltaTime) {
    sso_update(deltaTime);
    SceneNode::update(deltaTime);

    m_frameNode->setLocalBoundingBox(m_objectNode->worldBoundingBox());
    *m_frame << m_objectNode->worldBoundingBox();
    m_frameNode->cleanDirty();
}

bool MapSSO::contains(const vn::vector2f &point) const {
    return m_objectNode->worldBoundingBox().contains(point);
}

void MapSSO::setFrameVisible(bool visible) {
    m_frameNode->setVisible(visible);
}

