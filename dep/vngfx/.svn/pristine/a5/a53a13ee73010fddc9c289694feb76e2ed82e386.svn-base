//
//  vnUIRenderer.cpp
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIRenderer.h"
#include "vnRender2DInterface.h"

_vn_begin

void UIRenderer::pushClippingBox(const aabox2f &clipingBox) {
	if (m_clippingBoxes.empty()) {
		m_clippingBoxes.push(clipingBox);
	} else {
		m_clippingBoxes.push(m_clippingBoxes.top().intersection(clipingBox));
	}
}

void UIRenderer::popClippingBox() {
	vnassert(!m_clippingBoxes.empty());
	m_clippingBoxes.pop();
}

void UIRenderer::drawRect(const aabox2f &rect, const color_t &color) {
    const aabox2f *pcb;
    if (!_getClippingBox(pcb)) {
		// not clip
		_draw(rect, color);
	} else if (pcb) {
		// clip
		aabox2f _rect = rect;
		if (_clipRect(*pcb, _rect)) {
			_draw(_rect, color);
		}
	}
}

void UIRenderer::drawRect(const aabox2f &rect, const color_t &color, Texture2D *texture, const vector2f *texcoords) {
    vnassert(texture && texcoords);
    const aabox2f *pcb;
    if (!_getClippingBox(pcb)) {
		// not clip
		_draw(rect, color, texture, texcoords);
	} else if (pcb) {
		// clip
		aabox2f _rect = rect;
        vector2f _texcoords[4];
		if (_clipRect(*pcb, _rect, texcoords, _texcoords)) {
			_draw(_rect, color, texture, _texcoords);
		}
	}
}

void UIRenderer::drawRect(const aabox2f &rect, const color_t &color, Picture *picture) {
    if (!picture || picture->texture.null()) {
        return drawRect(rect, color);
    }
    drawRect(rect, color, picture->texture.ptr(), picture->texcoords);
}

void UIRenderer::_draw(const rect_type &rect, vn::color_t color) {
    Render2DInterface &r = Render2DInterface::instance();
    vector2f positions[4];
    vector2f texcoords[4];
    color_t colors[4] = { color, color, color, color };
    
    rect.build_corners(positions);
    
    r.setTexture(0);
    r.drawQuad(positions, colors, texcoords);
}

void UIRenderer::_draw(const rect_type &rect, color_t color, Texture2D *texture, const vector2f *texcoords) {
    Render2DInterface &r = Render2DInterface::instance();
    vector2f positions[4];
    color_t colors[4] = { color, color, color, color };
    
    rect.build_corners(positions);
    
    r.setTexture(texture);
    r.drawQuad(positions, colors, texcoords);
}

bool UIRenderer::_getClippingBox(const aabox2f *&clippingBox) {
	if (m_clippingBoxes.empty()) {
		return false;
	}
	const aabox2f &cb = m_clippingBoxes.top();
	if (cb.min_corner < cb.max_corner) {
		clippingBox = &cb;
	} else {
		clippingBox = 0;
	}
	return true;
}

bool UIRenderer::_clipRect(const aabox2f &clippingBox, aabox2f &rect) {
	if ( rect.min_corner.x > clippingBox.max_corner.x ) {
		return false;
	}
	if ( rect.min_corner.y > clippingBox.max_corner.y ) {
		return false;
	}
	if ( rect.max_corner.x < clippingBox.min_corner.x ) {
		return false;
	}
	if ( rect.max_corner.y < clippingBox.min_corner.y ) {
		return false;
	}
	if ( rect.min_corner.x < clippingBox.min_corner.x ) {
		rect.min_corner.x = clippingBox.min_corner.x;
	}
	if ( rect.min_corner.y < clippingBox.min_corner.y ) {
		rect.min_corner.y = clippingBox.min_corner.y;
	}
	if ( rect.max_corner.x > clippingBox.max_corner.x ) {
		rect.max_corner.x = clippingBox.max_corner.x;
	}
	if ( rect.max_corner.y > clippingBox.max_corner.y ) {
		rect.max_corner.y = clippingBox.max_corner.y;
	}
	return true;
}

bool UIRenderer::_clipRect(const aabox2f &clippingBox, aabox2f &rect, const vector2f src_texcoords[4], vector2f dest_texcoords[4]) {
	vector2f size = rect.size();
	vector2f fmin = (clippingBox.min_corner - rect.min_corner) / size ;
	if ( fmin.x >= 1 || fmin.y >= 1 ) {
		return false;
	}
	vector2f fmax = (clippingBox.max_corner - rect.min_corner) / size;
	if ( fmax.x <= 0 || fmax.y <= 0 ) {
		return false;
	}
    
	if ( fmin.x > 0 ) {
		rect.min_corner.x = clippingBox.min_corner.x;
		f32 inv = 1.f - fmin.x;
		dest_texcoords[0].x = src_texcoords[0].x * inv + src_texcoords[1].x * fmin.x;
		dest_texcoords[3].x = src_texcoords[3].x * inv + src_texcoords[2].x * fmin.x;
	} else {
        dest_texcoords[0].x = src_texcoords[0].x;
        dest_texcoords[3].x = src_texcoords[3].x;
    }
	if ( fmin.y > 0 ) {
		rect.min_corner.y = clippingBox.min_corner.y;
		f32 inv = 1.f - fmin.y;
		dest_texcoords[0].y = src_texcoords[0].y * inv + src_texcoords[3].y * fmin.y;
		dest_texcoords[1].y = src_texcoords[1].y * inv + src_texcoords[2].y * fmin.y;
	} else {
        dest_texcoords[0].y = src_texcoords[0].y;
        dest_texcoords[1].y = src_texcoords[1].y;
    }
	if ( fmax.x < 1 ) {
		rect.max_corner.x = clippingBox.max_corner.x;
		f32 inv = 1.f - fmax.x;
		dest_texcoords[1].x = src_texcoords[0].x * inv + src_texcoords[1].x * fmax.x;
		dest_texcoords[2].x = src_texcoords[3].x * inv + src_texcoords[2].x * fmax.x;
	} else {
        dest_texcoords[1].x = src_texcoords[1].x;
        dest_texcoords[2].x = src_texcoords[2].x;
    }
	if ( fmax.y < 1 ) {
		rect.max_corner.y = clippingBox.max_corner.y;
		f32 inv = 1.f - fmax.y;
		dest_texcoords[3].y = src_texcoords[0].y * inv + src_texcoords[3].y * fmax.y;
		dest_texcoords[2].y = src_texcoords[1].y * inv + src_texcoords[2].y * fmax.y;
	} else {
        dest_texcoords[3].y = src_texcoords[3].y;
        dest_texcoords[2].y = src_texcoords[2].y;
    }
	return true;
}

_vn_end