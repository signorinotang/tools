//
//  Brush.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "Brush.h"
#include "EditorApp.h"
#include "Renderable.h"

void Brush_MoveCamera::begin(const vn::vector2f &pos) {
    m_last = pos;
}

void Brush_MoveCamera::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    if (paint) {
        EditorApp::instance().camera().move(m_last - pos);
        m_last = pos;
    }
}

///////////////////////////////////////////////////////


BlockBrush::BlockBrush()
: m_size(1, 1) {
    
}

void BlockBrush::init() {
    m_brush = vnnew SceneNode();
    _resetBrushNode();
}

void BlockBrush::fini() {
    m_brush.release();
}

void BlockBrush::switchIn() {
    EditorApp::instance().layer_brush()->addChild(m_brush.ptr());
}

void BlockBrush::switchOut() {
    EditorApp::instance().layer_brush()->removeChild(m_brush.ptr());
}

void BlockBrush::_resetBrushNode() {
    vn::f32 left, top, right, bottom;
    left = m_size.x / 2 * -40;
    top = m_size.y / 2 * -40;
    right = (m_size.x + 1) / 2 * 40;
    bottom = (m_size.y + 1) / 2 * 40;
    m_brushBox.min_corner.set(left, top);
    m_brushBox.max_corner.set(right, bottom);
    
    m_brush->clearRenderables();
    
    Quad *p = vnnew Quad();
    *p << vn::aabox2f(left - 2, top - 2, right, top) << vn::color_t::kBlack;
    m_brush->addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(right, top - 2, right + 2, bottom) << vn::color_t::kBlack;
    m_brush->addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(left, bottom, right + 2, bottom + 2) << vn::color_t::kBlack;
    m_brush->addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(left - 2, top, left, bottom + 2) << vn::color_t::kBlack;
    m_brush->addRenderable(p, false);
}

void BlockBrush::_updateBrush(const vn::vector2f &pos) {
    vn::vector2f off;
    off.x = (m_size.x + 1) % 2 * 20.f;
    off.y = (m_size.y + 1) % 2 * 20.f;
    vn::vector2i idx = (EditorApp::instance().camera().convertFromView(pos) + off) / 40;
    m_brush->setPosition(idx * 40.f);
    m_index = idx - m_size / 2;
}

void BlockBrush::_updateBrushByIndex() {
    m_brush->setPosition((m_index + m_size / 2) * 40.f);
}

///////////////////////////////////////////////////////

