//
//  vnUIRenderText.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIRenderText.h"
#include "vnUnicode.h"
#include "vnUIRenderer.h"

_vn_begin

UIRenderText::UIRenderText() {
    
}

UIRenderText::~UIRenderText() {
    
}

void UIRenderText::setFont(const FontPtr &font) {
    m_font = font;
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        it->glyph = 0;
    }
    if (m_font.null()) {
        m_size = vector2f::kZero;
    } else {
        m_size.set(0, m_font->height());
    }
}

const FontPtr & UIRenderText::getFont() const {
    return m_font;
}

void UIRenderText::setText(const str8 &text) {
    str32 _txt;
    Unicode::convert(text.c_str(), text.length(), _txt);
    setText(_txt);
}

void UIRenderText::setText(const str32 &text) {
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

void UIRenderText::clearText() {
    m_chars.clear();
    m_size = vector2f::kZero;
}

const vector2f & UIRenderText::size() const {
    return m_size;
}

f32 UIRenderText::offset(size_t index) const {
    if (index < m_chars.size()) {
        return m_chars[index].offsetX;
    }
    return m_size.x;
}

f32 UIRenderText::width(size_t index) const {
	if (index < m_chars.size()) {
		const CharInfo &ci = m_chars[index];
		return ci.glyph ? ci.glyph->size.x : 0;
	}
	return 0;
}

size_t UIRenderText::length() const {
    return m_chars.size();
}

size_t UIRenderText::index_round(f32 offset) const {
	if (offset < 0) {
		return 0;
	} else if (offset >= m_size.x) {
		return m_chars.size();
	}
	long low = 0, high = m_chars.size() - 1;
	while (low <= high) {
		long mid = (low + high) / 2;
		const CharInfo &ci = m_chars[mid];
		f32 left = ci.offsetX;
		f32 right = ci.offsetX + (ci.glyph ? ci.glyph->size.x : 0);
		if (offset < left) {
			high = mid - 1;
		} else if (offset >= right) {
			low = mid + 1;
		} else if (offset < (left + right) / 2.f) {
			return mid;
		} else {
			return mid + 1;
		}
	}
	vnassert(false);
	return 0;
}

size_t UIRenderText::index(f32 offset) const {
	if (offset < 0) {
		return 0;
	} else if (offset >= m_size.x) {
		return m_chars.size();
	}
	long low = 0, high = m_chars.size() - 1;
	while (low <= high) {
		long mid = (low + high) / 2;
		const CharInfo &ci = m_chars[mid];
		f32 left = ci.offsetX;
		f32 right = ci.offsetX + (ci.glyph ? ci.glyph->size.x : 0);
		if (offset < left) {
			high = mid - 1;
		} else if (offset >= right) {
			low = mid + 1;
		} else {
			return mid;
		}
	}
	vnassert(false);
	return 0;
}

void UIRenderText::update() {
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
        if (ci.glyph->size.y > m_size.y) {
            m_size.y = ci.glyph->size.y;
        }
        ci.offsetX = m_size.x;
        m_size.x += ci.glyph->size.x;
    }
	f32 height = m_font->height();
	if (m_size.y < height) {
		m_size.y = height;
	}
}

void UIRenderText::render(UIRenderer *renderer, const vector2f &position, const color_t &color) {
    vector2f texcoords[4];
    for (CharInfoArray::iterator it = m_chars.begin(); it != m_chars.end(); ++it) {
        CharInfo &ci = *it;
        if (!ci.glyph) {
            continue;
        }
        aabox2f rect = ci.glyph->bounds + position + vector2f(ci.offsetX, 0);
        ci.glyph->texcoord.build_corners(texcoords);
        renderer->drawRect(rect, color, ci.glyph->texture.ptr(), texcoords);
    }
}

void UIRenderText::render(UIRenderer *renderer, const vector2f &position, const color_t &color, size_t first, size_t last) {
    vector2f texcoords[4];
    for (size_t i = first; i < last; ++i) {
        CharInfo &ci = m_chars[i];
        if (!ci.glyph) {
            continue;
        }
        aabox2f rect = ci.glyph->bounds + position + vector2f(ci.offsetX, 0);
        ci.glyph->texcoord.build_corners(texcoords);
        renderer->drawRect(rect, color, ci.glyph->texture.ptr(), texcoords);
    }
}


_vn_end
