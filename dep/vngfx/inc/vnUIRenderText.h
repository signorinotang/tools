//
//  vnUIRenderText.h
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIRenderText_h
#define vngfx_vnUIRenderText_h

#include "vnFont.h"
#include "vnColor.h"

#include <vector>

_vn_begin

class UIRenderer;

class _vn_gfx_api UIRenderText : public AllocatedObject {
public:
    // ctor & dtor
    UIRenderText();
    virtual ~UIRenderText();
    
    // methods
    void setFont(const FontPtr &font);
    const FontPtr & getFont() const;
    
    void setText(const str32 &text);
    void setText(const str8 &text);
    void clearText();
    
    const vector2f & size() const;
    size_t length() const;
    
    f32 offset(size_t index) const;
	f32 width(size_t index) const;
	size_t index_round(f32 offset) const;
	size_t index(f32 offset) const;
    
    void update();
    
    void render(UIRenderer *renderer, const vector2f &position, const color_t &color);
    void render(UIRenderer *renderer, const vector2f &position, const color_t &color, size_t first, size_t last);
    
protected:
    struct CharInfo {
        c32 chr;
        const Font::Glyph *glyph;
        f32 offsetX;
        
        CharInfo() : chr(0), glyph(0), offsetX(0) {}
    };
    typedef std::vector<CharInfo> CharInfoArray;
    
    FontPtr m_font;
    CharInfoArray m_chars;
    vector2f m_size;
};

_vn_end

#endif
