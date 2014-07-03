//
//  vnFontFreeType.cpp
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFontFreeType.h"
#include "vnFileManager.h"
#include "vnRender2DInterface.h"

_vn_begin

FT_Library FontFreeType::s_ftLibrary = 0;

bool FontFreeType::ftInit() {
    if (FT_Init_FreeType(&s_ftLibrary)) {
		return false;
	}
	return true;
}

void FontFreeType::ftDone() {
    if (s_ftLibrary) {
        FT_Done_FreeType(s_ftLibrary);
        s_ftLibrary = 0;
    }
}

unsigned long FontFreeType::ftRead(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count) {
    FileStream *fs = (FileStream *)stream->descriptor.pointer;
    fs->seek(offset);
    return fs->read(buffer, count);
}

void FontFreeType::ftClose(FT_Stream stream) {
    FileStream *fs = (FileStream *)stream->descriptor.pointer;
    fs->close();
    stream->descriptor.pointer = 0;
}

////////////////////////////////////////////

FontFreeType::FontFreeType()
: m_ftFace(0)
, m_currentPass(1)
, m_ftStream({}) {
    m_ftStream.read = ftRead;
    m_ftStream.close = ftClose;
}

FontFreeType::~FontFreeType() {
    if (m_ftFace && s_ftLibrary) {
        FT_Done_Face(m_ftFace);
    }
    
    if (m_ftStream.descriptor.pointer) {
        static_cast<FileStream *>(m_ftStream.descriptor.pointer)->close();
    }
    
    for (GlyphList::iterator it = m_glyphList.begin(); it != m_glyphList.end(); ++it) {
		delete *it;
	}
}

bool FontFreeType::init(const vn::FilePath &file, u32 size) {
    FileStream *fs = FileManager::instance().open(file);
    if (!fs) {
        return false;
    }
    m_ftStream.descriptor.pointer = fs;
    m_ftStream.size = (unsigned long)fs->size();
    FT_Open_Args args = { FT_OPEN_STREAM };
	args.stream = &m_ftStream;
    
	if (FT_Open_Face(s_ftLibrary, &args, 0, &m_ftFace )) {
		return false;
	}
	
	s32 height = (s32)((f32)(size << 6) * m_ftFace->units_per_EM / m_ftFace->height / 64.f);
	
    
    FT_Select_Charmap(m_ftFace, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(m_ftFace, height, 0);
    
    m_height = (s32)(m_ftFace->size->metrics.height >> 6);
    m_baseLine = (s32)(m_ftFace->size->metrics.ascender >> 6);
    
    m_texture = Render2DInterface::instance().createTexture(vector2i(kTexSize, kTexSize));
    
    s32 cell = height + 2;
    s32 cnt = kTexSize / cell;
    for (s32 y = 0; y < cnt; ++y) {
        for (s32 x = 0; x < cnt; ++x) {
            GlyphEx *p = vnnew GlyphEx();
            p->valid = false;
            p->size.y = (real)m_height;
            p->texture = m_texture;
            p->pos.set(x * cell + 1, y * cell + 1);
            p->texcoord.min_corner = vector2f(p->pos) / kTexSize;
            p->pass = 0;
            m_glyphList.push_front(p);
            p->lstIt = m_glyphList.begin();
            p->mapIt = m_glyphMap.end();
        }
    }
    
    return true;
}

const Font::Glyph * FontFreeType::build(c32 chr) {
    GlyphMap::iterator it = m_glyphMap.insert(std::make_pair(chr, (GlyphEx *)0)).first;
	if (it->second) {
		it->second->pass = m_currentPass;
	} else {
		_build(chr, it);
	}
	return it->second;
}

f32 FontFreeType::height() {
    return (f32)m_height;
}

void FontFreeType::discard() {
    ++m_currentPass;
}

void FontFreeType::_build(c32 chr, const GlyphMap::iterator &mapIt) {
    GlyphEx *glyph = m_glyphList.back();
	if (glyph->pass == m_currentPass) {
		return ;
	}
	if (glyph->mapIt != m_glyphMap.end()) {
		m_glyphMap.erase(glyph->mapIt);
	}
    FT_UInt idx = FT_Get_Char_Index(m_ftFace, chr);
    glyph->valid = (idx != 0);
	FT_Load_Glyph(m_ftFace, idx, FT_LOAD_RENDER);
	FT_GlyphSlot slot = m_ftFace->glyph;
    
    vector2i size(slot->bitmap.width, slot->bitmap.rows);
    
    glyph->size.x = (real)(slot->advance.x >> 6);
    glyph->bounds.min_corner.set((real)slot->bitmap_left, m_baseLine - (real)slot->bitmap_top);
	glyph->bounds.max_corner = glyph->bounds.min_corner + size;
    glyph->texcoord.max_corner = vector2f(glyph->pos + size) / kTexSize;
    glyph->pass = m_currentPass;
    
    m_glyphList.erase( glyph->lstIt );
	m_glyphList.push_front( glyph );
	glyph->lstIt = m_glyphList.begin();
	glyph->mapIt = mapIt;
	mapIt->second = glyph;
    
	if (!size.x || !size.y) {
		return ;
	}
    
    u32 *pixels = vnmalloc(u32, size.area());
    u32 *dest = pixels;
    
    vnassert(slot->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);
    
    u8 *src = slot->bitmap.buffer;
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            *dest++ = (0x00FFFFFF | (src[x] << 24));
        }
        src += slot->bitmap.pitch;
    }
    
    m_texture->updatePixels(glyph->pos, size, pixels);
    
    vnfree(pixels);
}


_vn_end