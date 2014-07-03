//
//  Renderable.cpp
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "Renderable.h"
#include "SceneNode.h"

Renderable::Renderable()
: m_sceneNode(0) {
	
}

SceneNode * Renderable::sceneNode() const {
	return m_sceneNode;
}


Quad::Quad()
: m_dirty(false)
, m_blend(vn::Render2DInterface::kAlphaBlend) {
    *this << vn::color_t::kWhite;
}

void Quad::update() {
	if (m_dirty) {
		m_dirty = false;
		if (m_sceneNode) {
			m_corners[0] = m_sceneNode->convertToWorld(m_rect.corner_0());
			m_corners[1] = m_sceneNode->convertToWorld(m_rect.corner_1());
			m_corners[2] = m_sceneNode->convertToWorld(m_rect.corner_2());
			m_corners[3] = m_sceneNode->convertToWorld(m_rect.corner_3());
		} else {
			m_rect.build_corners(m_corners);
		}
	}
}

void Quad::render() {
    vn::Render2DInterface &r = vn::Render2DInterface::instance();
    r.setBlendMode(m_blend);
    r.setTexture(m_texture.ptr());
    r.drawQuad(m_corners, m_colors, m_texcoords);
}

Quad & Quad::operator <<(const vn::aabox2f &rect) {
    m_rect = rect;
	m_dirty = true;
	return *this;
}

Quad & Quad::operator <<(const vn::color_t &color) {
    m_colors[0] = m_colors[1] = m_colors[2] = m_colors[3] = color;
	return *this;
}

Quad & Quad::operator <<(const vn::Picture *picture) {
    if (picture) {
        m_texture = picture->texture;
        m_texcoords[0] = picture->texcoords[0];
        m_texcoords[1] = picture->texcoords[1];
        m_texcoords[2] = picture->texcoords[2];
        m_texcoords[3] = picture->texcoords[3];
    } else {
        m_texture.release();
    }
	return *this;
}

Quad & Quad::operator <<(vn::Render2DInterface::BlendMode blend) {
	m_blend = blend;
	return *this;
}

void Quad::onTransformUpdated() {
	m_dirty = true;
}

/////////////////////////////////////////////////////////////////////

WireFrame::WireFrame()
: m_dirty(false)
, m_blend(vn::Render2DInterface::kAlphaBlend)
, m_width(1) {
    *this << vn::color_t::kWhite;
}

void WireFrame::update() {
    if (m_dirty) {
        m_dirty = false;
        vn::vector2f width(m_width, m_width);
        m_rect.build_corners(m_corners);
        vn::aabox2f(m_rect.min_corner + width, m_rect.max_corner - width).build_corners(m_corners + 4);
        if (m_sceneNode) {
            for (size_t i = 0; i < 8; ++i) {
                m_corners[i] = m_sceneNode->convertToWorld(m_corners[i]);
            }
        }
    }
}

void WireFrame::render() {
    vn::Render2DInterface &r = vn::Render2DInterface::instance();
    r.setBlendMode(m_blend);
    r.setTexture(0);
    static vn::u16 s_indices[] = {
        0, 1, 5, 0, 5, 4,
        1, 2, 6, 1, 6, 5,
        7, 6, 2, 7, 2, 3,
        0, 4, 7, 0, 7, 3
    };
    r.drawTriangles(8, m_corners, m_colors, m_texcoords, 8, s_indices);
}

WireFrame & WireFrame::operator <<(const vn::aabox2f &rect) {
    if (m_rect != rect) {
        m_rect = rect;
        m_dirty = true;
    }
    return *this;
}

WireFrame & WireFrame::operator <<(const vn::color_t &color) {
    for (size_t i = 0; i < 8; ++i) {
        m_colors[i] = color;
    }
    return *this;
}

WireFrame & WireFrame::operator <<(vn::Render2DInterface::BlendMode blend) {
    m_blend = blend;
    return *this;
}

WireFrame & WireFrame::operator <<(vn::f32 width) {
    if (m_width != width) {
        m_width = width;
        m_dirty = true;
    }
    return *this;
}

void WireFrame::onTransformUpdated() {
    m_dirty = true;
}

/////////////////////////////////////////////////////////////////////

Text::Text()
: m_blend(vn::Render2DInterface::kAlphaBlend) {
	
}

void Text::update() {
	if (m_sceneNode) {
		m_renderText.updateAndTransform(m_sceneNode->transformM(), m_sceneNode->transformT());
	} else {
		m_renderText.update();
	}
}

void Text::render() {
	vn::Render2DInterface::instance().setBlendMode(m_blend);
	m_renderText.render();
}

Text & Text::operator <<(const vn::aabox2f &rect) {
	m_renderText.setLocation(rect);
	return *this;
}

Text & Text::operator <<(const vn::color_t &color) {
	m_renderText.setColor(color);
	return *this;
}

Text & Text::operator <<(const vn::str8 &text) {
	m_renderText.setText(text);
	return *this;
}

Text & Text::operator <<(const vn::str32 &text) {
	m_renderText.setText(text);
	return *this;
}

Text & Text::operator <<(const vn::FontPtr &font) {
	m_renderText.setFont(font);
	return *this;
}

Text & Text::operator <<(vn::RenderText::Alignment align) {
    m_renderText.setAlignment(align);
    return *this;
}

Text & Text::operator <<(vn::Render2DInterface::BlendMode blend) {
	m_blend = blend;
	return *this;
}