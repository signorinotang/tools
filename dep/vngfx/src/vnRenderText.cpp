//
//  vnRenderText.cpp
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnRenderText.h"
#include "vnUnicode.h"
#include "vnRender2DInterface.h"

_vn_begin

RenderText::RenderText()
: m_color(color_t::kWhite)
, m_alignment(0) {
    
}

RenderText::~RenderText() {
    
}

void RenderText::setFont(const FontPtr &font) {
    m_font = font;
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        it->glyph = 0;
    }
    m_size = vector2f::kZero;
}

const FontPtr & RenderText::getFont() const {
    return m_font;
}

void RenderText::setText(const str8 &text) {
    str32 _txt;
    Unicode::convert(text.c_str(), text.length(), _txt);
    setText(_txt);
}

void RenderText::setText(const str32 &text) {
    m_chars.resize(text.length());
    if (m_chars.empty()) {
        return ;
    }
    CharInfo *dest = &m_chars.front();
    const c32 *src = text.c_str();
    while (*src) {
        dest++->chr = *src++;
    }
    m_size = vector2f::kZero;
}

void RenderText::clearText() {
    m_chars.clear();
    m_size = vector2f::kZero;
}

void RenderText::setLocation(const aabox2f &rect) {
    m_rect = rect;
}

const aabox2f & RenderText::getLocation() const {
    return m_rect;
}

void RenderText::setAlignment(u32 alignment) {
    m_alignment = alignment;
}

u32 RenderText::getAlignment() const {
    return m_alignment;
}

void RenderText::setColor(const color_t &color) {
    m_color = color;
}

void RenderText::update() {
    if (m_font.null()) {
		return ;
	}
    m_size = vector2f::kZero;
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        CharInfo &ci = *it;
        ci.glyph = m_font->build(ci.chr);
        if (!ci.glyph) {
            continue;
        }
        ci.color[0] = ci.color[1] = ci.color[2] = ci.color[3] = m_color;
        ci.glyph->texcoord.build_corners(ci.texcoord);
        if (ci.glyph->size.y > m_size.y) {
            m_size.y = ci.glyph->size.y;
        }
        m_size.x += ci.glyph->size.x;
    }
    
    vector2f pen;
    if (m_alignment) {
        switch (m_alignment & kHorizontalCenter) {
            case kHorizontalCenter:
                pen.x = (m_rect.max_corner.x + m_rect.min_corner.x - m_size.x) * 0.5f;
                break;
                
            case kRight:
                pen.x = m_rect.max_corner.x - m_size.x;
                break;
                
            default:
                pen.x = m_rect.min_corner.x;
                break;
        }
        switch (m_alignment & kVerticalCenter) {
            case kVerticalCenter:
                pen.y = (m_rect.max_corner.y + m_rect.min_corner.y - m_size.y) * 0.5f;
                break;
                
            case kBottom:
                pen.y = m_rect.max_corner.y - m_size.y;
                break;
                
            default:
                pen.y = m_rect.min_corner.y;
                break;
        }
    } else {
        pen = m_rect.min_corner;
    }
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        CharInfo &ci = *it;
        if (!ci.glyph) {
            continue;
        }
        aabox2f rect = ci.glyph->bounds;
        rect += pen;
        rect.build_corners(ci.pos);
        pen.x += ci.glyph->size.x;
    }
}

void RenderText::updateAndTransform(const matrix2 &m, const vector2f &t) {
	if (m_font.null()) {
		return ;
	}
    m_size = vector2f::kZero;
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        CharInfo &ci = *it;
        ci.glyph = m_font->build(ci.chr);
        if (!ci.glyph) {
            continue;
        }
        ci.color[0] = ci.color[1] = ci.color[2] = ci.color[3] = m_color;
        ci.glyph->texcoord.build_corners(ci.texcoord);
        if (ci.glyph->size.y > m_size.y) {
            m_size.y = ci.glyph->size.y;
        }
        m_size.x += ci.glyph->size.x;
    }
    
    vector2f pen;
    if (m_alignment) {
        switch (m_alignment & kHorizontalCenter) {
            case kHorizontalCenter:
                pen.x = (m_rect.max_corner.x + m_rect.min_corner.x - m_size.x) * 0.5f;
                break;
                
            case kRight:
                pen.x = m_rect.max_corner.x - m_size.x;
                break;
                
            default:
                pen.x = m_rect.min_corner.x;
                break;
        }
        switch (m_alignment & kVerticalCenter) {
            case kVerticalCenter:
                pen.y = (m_rect.max_corner.y + m_rect.min_corner.y - m_size.y) * 0.5f;
                break;
                
            case kBottom:
                pen.y = m_rect.max_corner.y - m_size.y;
                break;
                
            default:
                pen.y = m_rect.min_corner.y;
                break;
        }
    } else {
        pen = m_rect.min_corner;
    }
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        CharInfo &ci = *it;
        if (!ci.glyph) {
            continue;
        }
        aabox2f rect = ci.glyph->bounds;
        rect += pen;
        ci.pos[0] = rect.corner_0() * m + t;
		ci.pos[1] = rect.corner_1() * m + t;
		ci.pos[2] = rect.corner_2() * m + t;
		ci.pos[3] = rect.corner_3() * m + t;
		
		
        pen.x += ci.glyph->size.x;
    }
}

void RenderText::render() const {
    if (m_font.null()) {
		return ;
	}
    Render2DInterface &r = Render2DInterface::instance();
    for (CharInfoArray::const_iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        const CharInfo &ci = *it;
        if (ci.glyph) {
            r.setTexture(ci.glyph->texture.ptr());
            r.drawQuad(ci.pos, ci.color, ci.texcoord);
        }
    }
}

_vn_end
