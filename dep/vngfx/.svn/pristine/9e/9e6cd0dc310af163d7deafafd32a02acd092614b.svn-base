//
//  vnFontManager.h
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnFontManager_h
#define vngfx_vnFontManager_h

#include "vnFont.h"
#include "vnSingleton.h"

#include <map>

_vn_begin

class _vn_gfx_api FontManager : public Singleton<FontManager> {
public:
    FontManager();
    ~FontManager();
    
    const FontPtr & createFontFT(const str8 &name, const FilePath &file, u32 size);
	const FontPtr & findFont(const str8 &name);
    const FontPtr & defaultFont();
    
	void discard();
	
	void freeFont(const str8 &name);
	void freeAllFonts();
	void freeUnusedFonts();
private:
    
    typedef std::map<str8, FontPtr> FontMap;
    FontMap m_fonts;
    FontPtr m_defaultFont;
};

_vn_end

#endif
