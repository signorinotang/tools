//
//  vnRenderText.h
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnRenderText_h
#define vngfx_vnRenderText_h

#include "vnFont.h"
#include "vnColor.h"
#include "vnPicture.h"
#include "vnMatrix2.h"

#include <vector>

_vn_begin

class _vn_gfx_api RenderText : public AllocatedObject {
public:
    
    enum Alignment {
        kLeft = 1,
        kRight = 2,
        kHorizontalCenter = 3,
        kTop = 4,
        kBottom = 8,
        kVerticalCenter = 12,
        kCenter = 15
    };
    
    RenderText();
    ~RenderText();
    
    void setFont(const FontPtr &font);
    const FontPtr &getFont() const;
    
    void setText(const str8 &text);
    void setText(const str32 &text);
    void clearText();
    
    void setLocation(const aabox2f &rect);
    const aabox2f & getLocation() const;
    
    void setAlignment(u32 alignment);
    u32 getAlignment() const;
    
    void setColor(const color_t &color);
    
    void update();
	void updateAndTransform(const matrix2 &m, const vector2f &t);
    void render() const;
    
private:
    struct CharInfo {
        c32 chr;
        const Font::Glyph *glyph;
        vector2f pos[4];
        color_t color[4];
        vector2f texcoord[4];
        
        CharInfo() : chr(0), glyph(0) {}
    };
    typedef std::vector<CharInfo> CharInfoArray;
    
    FontPtr m_font;
    color_t m_color;
    aabox2f m_rect;
    u32 m_alignment;
    CharInfoArray m_chars;
    vector2f m_size;
};

_vn_end


#endif
