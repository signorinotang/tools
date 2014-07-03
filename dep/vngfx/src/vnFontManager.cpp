//
//  vnFontManager.cpp
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFontManager.h"
#include "vnFontFreeType.h"

_vn_begin

FontManager::FontManager() {
    FontFreeType::ftInit();
}

FontManager::~FontManager() {
    m_fonts.clear();
    m_defaultFont.release();
    FontFreeType::ftDone();
}

const FontPtr & FontManager::findFont(const str8 &name) {
	FontMap::iterator it = m_fonts.find(name);
	if (it == m_fonts.end()) {
		return m_defaultFont;
	}
	return it->second;
}

const FontPtr & FontManager::defaultFont() {
    return m_defaultFont;
}

void FontManager::discard() {
	for (FontMap::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it) {
		it->second->discard();
	}
    if (!m_defaultFont.null()) {
        m_defaultFont->discard();
    }
}

void FontManager::freeFont(const str8 &name) {
    if (name.empty()) {
        m_defaultFont.release();
        return ;
    }
	m_fonts.erase(name);
}

void FontManager::freeAllFonts() {
	m_fonts.clear();
    m_defaultFont.release();
}

void FontManager::freeUnusedFonts() {
	for (FontMap::iterator it = m_fonts.begin(); it != m_fonts.end(); ) {
		if (it->second.ref_count() == 1) {
			it = m_fonts.erase(it);
		} else {
			++it;
		}
	}
}

const FontPtr & FontManager::createFontFT(const str8 &name, const FilePath &file, u32 size) {
    if (name.empty()) {
        FontFreeType *font = vnnew FontFreeType();
        if (!font->init(file, size)) {
            delete font;
            return FontPtr::kNull;
        }
        return m_defaultFont = font;
    }
    std::pair<FontMap::iterator, bool> ret = m_fonts.insert(std::make_pair(name, FontPtr::kNull));
    FontFreeType *font = vnnew FontFreeType();
    if (!font->init(file, size)) {
        delete font;
        m_fonts.erase(ret.first);
        return FontPtr::kNull;
    }
    return ret.first->second = font;
}

_vn_end