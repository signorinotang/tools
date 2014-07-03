//
//  MapDecorationAnimation.cpp
//  TDMapEditor
//
//  Created by Wayne on 5/21/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapDecorationAnimation.h"


MapDecorationAnimation::MapDecorationAnimation()
: m_controller(vnnew AnimationController(this))
, m_animNode(vnnew SceneNode())
, m_quad(vnnew Quad()) {
    m_controller->setLoop(true);
    m_node->addChild(m_animNode.ptr());
    m_animNode->addRenderable(m_quad, false);
}

MapDecoration::Type MapDecorationAnimation::type() const {
    return kAnimation;
}

void MapDecorationAnimation::setDesc(const MapDecorationDescPtr &desc, bool update) {
    if (update && !desc.null()) {
        setAnimationPath(desc->file.getFullPath());
    }
    MapDecoration::setDesc(desc, update);
}

void MapDecorationAnimation::setAnimationPath(const vn::str8 &animationPath) {
    vn::FilePath path(animationPath.c_str());
    vn::RefCountedPtr<const AnimationGroup> group = AnimationManager::instance().loadFromANS(path);
    if (!group.null() && group->size()) {
        const Animation *anim = group->at(0);
        vn::aabox2f box(-8, -8, 8, 8);
        if (!anim->frames.empty()) {
            box.reset(anim->frames[0].rect.min_corner);
            box.add_point(anim->frames[0].rect.max_corner);
            for (size_t i = 1; i < anim->frames.size(); ++i) {
                box.add_box(anim->frames[i].rect);
            }
        }
        m_animBoundingBox.x = vn::math::max_(box.max_corner.x, vn::math::abs(box.min_corner.x));
        m_animBoundingBox.y = vn::math::max_(box.max_corner.y, vn::math::abs(box.min_corner.y));
        m_animBoundingBox *= 2.f;
        m_controller->setAnimation(anim);
    }
    if (!m_desc.null()) {
        m_desc->file = path;
    }
}

void MapDecorationAnimation::update(vn::f32 deltaTime) {
    MapDecoration::update(deltaTime);
    m_controller->advance(deltaTime);
}

bool MapDecorationAnimation::canOperate(const vn::vector2f &point) {
    return MapDecoration::canOperate(point);
}

void MapDecorationAnimation::anim_frame(const aabox2f &rect, const PicturePtr &image) {
    *m_quad << rect << image.ptr();
}

void MapDecorationAnimation::_onExtentUpdate() {
    if (m_animBoundingBox.x && m_animBoundingBox.y) {
        m_animNode->setScale(m_extent / m_animBoundingBox);
    }
}

void MapDecorationAnimation::_onColorUpdate() {
    *m_quad << m_color;
}

void MapDecorationAnimation::_onLightModeUpdate() {
    *m_quad << (m_lightMode ? vn::Render2DInterface::kAlphaAdditive : vn::Render2DInterface::kAlphaBlend);
}


