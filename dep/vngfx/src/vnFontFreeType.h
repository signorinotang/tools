//
//  vnFontFreeType.h
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __vngfx__vnFontFreeType__
#define __vngfx__vnFontFreeType__

#include "vnFont.h"

#include <map>
#include <list>

#include <ft2build.h>
#include FT_FREETYPE_H

_vn_begin

class FontFreeType : public Font {
public:
    FontFreeType();
    virtual ~FontFreeType();
    
    bool init(const FilePath &file, u32 size);
    
    virtual const Glyph * build(c32 chr);
    virtual f32 height();
    virtual void discard();
    
    static bool ftInit();
    static void ftDone();
    static unsigned long ftRead(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count);
	static void ftClose(FT_Stream stream);
private:
    enum { kTexSize = 1024 };
    static FT_Library s_ftLibrary;
    
    struct GlyphEx;
    typedef std::map<c32, GlyphEx *> GlyphMap;
	typedef std::list<GlyphEx *> GlyphList;
    
    struct GlyphEx : Glyph, AllocatedObject {
        vector2i pos; // in texture.
        u32 pass;
		GlyphList::iterator lstIt;
		GlyphMap::iterator mapIt;
    };
    
    FT_Face m_ftFace;
	FT_StreamRec_ m_ftStream;
    
	s32 m_height;
	s32 m_baseLine;
	Texture2DPtr m_texture;
	u32 m_currentPass;
    
    
	GlyphMap m_glyphMap;
	GlyphList m_glyphList;
    
    void _build(c32 chr, const GlyphMap::iterator &mapIt);
};

_vn_end

#endif /* defined(__vngfx__vnFontFreeType__) */
