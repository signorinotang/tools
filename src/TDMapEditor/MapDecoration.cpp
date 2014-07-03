//
//  MapDecoration.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/1/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapDecoration.h"

MapDecoration::MapDecoration()
: m_node(vnnew SceneNode())
, m_bkg(vnnew SceneNode())
, m_operator(vnnew SceneNode())
, m_frame(vnnew WireFrame())
, m_operatorFrame(vnnew WireFrame())
, m_flipX(false), m_flipY(false)
, m_lightMode(false)
, m_color(vn::color_t::kWhite)
, m_extent(1, 1) {
    m_bkg->setLocalBoundingBox(vn::aabox2f(-0.5f, -0.5f, 0.5f, 0.5f));
    m_bkg->addChild(m_operator, false);
    m_operator->setInheritTransformM(false);
    m_operator->setLocalBoundingBox(vn::aabox2f(0, 0, 16, 16));
    
    *m_operatorFrame << m_operator->getLocalBoundingBox() << 4.f << vn::color_t(0xFFE0E0E0);
    m_operator->addRenderable(m_operatorFrame, false);
    
    *m_frame << vn::color_t(0xFF, 0xFF, 0xFF, 0x40) << vn::Render2DInterface::kAlphaAdditive << m_bkg->getLocalBoundingBox() << -2.f;
    m_bkg->addRenderable(m_frame, false);
    addChild(m_bkg, false);
    addChild(m_node, false);
    
    m_bkg->setVisible(false);
}

void MapDecoration::setDesc(const MapDecorationDescPtr &desc, bool update) {
    m_desc = desc;
    if (update && !desc.null()) {
        setPosition(desc->position);
        setFlipX(desc->flipX);
        setFlipY(desc->flipY);
        setLightMode(desc->lightMode);
        setColor(desc->color);
        setExtent(desc->extent);
    }
}

const MapDecorationDescPtr & MapDecoration::getDesc() const {
    return m_desc;
}

void MapDecoration::setPosition(const vn::vector2f &position) {
    SceneNode::setPosition(position);
    if (!m_desc.null()) {
        m_desc->position = position;
    }
}

bool MapDecoration::isFlipX() const {
    return m_flipX;
}

bool MapDecoration::isFlipY() const {
    return m_flipY;
}

bool MapDecoration::isLightMode() const {
    return m_lightMode;
}

bool MapDecoration::isSelected() const {
    return m_bkg->isVisible();
}

const vn::vector2f & MapDecoration::getExtent() const {
    return m_extent;
}

const vn::color_t & MapDecoration::getColor() const {
    return m_color;
}

void MapDecoration::setFlipX(bool flipX) {
    m_flipX = flipX;
    m_node->setScale(vn::vector2f((m_flipX ? -1 : 1), (m_flipY ? -1 : 1)));
    if (!m_desc.null()) {
        m_desc->flipX = m_flipX;
    }
}

void MapDecoration::setFlipY(bool flipY) {
    m_flipY = flipY;
    m_node->setScale(vn::vector2f((m_flipX ? -1 : 1), (m_flipY ? -1 : 1)));
    if (!m_desc.null()) {
        m_desc->flipY = m_flipY;
    }
}

void MapDecoration::setLightMode(bool lightMode) {
    m_lightMode = lightMode;
    if (!m_desc.null()) {
        m_desc->lightMode = m_lightMode;
    }
    _onLightModeUpdate();
}

void MapDecoration::setSelected(bool selected) {
    m_bkg->setVisible(selected);
}

void MapDecoration::setExtent(const vn::vector2f &extent) {
    if (m_extent != extent) {
        m_extent = extent;
        vn::vector2f half = extent * 0.5f;
        vn::aabox2f rect(-half, half);
        m_bkg->setLocalBoundingBox(rect);
        m_node->setLocalBoundingBox(rect);
        m_operator->setPosition(half);
        
        *m_frame << m_bkg->getLocalBoundingBox();
        _onExtentUpdate();
        
        if (!m_desc.null()) {
            m_desc->extent = m_extent;
        }
    }
}

void MapDecoration::setColor(const vn::color_t &color) {
    m_color = color;
    if (!m_desc.null()) {
        m_desc->color = color;
    }
    _onColorUpdate();
}

bool MapDecoration::contains(const vn::vector2f &point) {
    return m_bkg->worldBoundingBox().contains(point);
}

bool MapDecoration::canOperate(const vn::vector2f &point) {
    return m_operator->worldBoundingBox().contains(point);
}

void MapDecoration::setOperating(bool operating) {
    *m_operatorFrame << vn::color_t(operating ? 0xFF808080 : 0xFFE0E0E0);
}

void MapDecoration::_onExtentUpdate() {
    
}

void MapDecoration::_onLightModeUpdate() {
    
}

void MapDecoration::_onColorUpdate() {
    
}