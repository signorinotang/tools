//
//  vnUILabel.h
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILabel_h
#define vngfx_vnUILabel_h

#include "vnUIElement.h"
#include "vnUIRenderText.h"

_vn_begin

class _vn_gfx_api UILabel : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    
    // ctor & dtor
    UILabel();
    virtual UILabel * clone() const;
    
    
    // properties.
    
    u32 getTextAlignment() const;
    void setTextAlignment(u32 alignment);
    
    void setFont(const FontPtr &font);
    const FontPtr & getFont() const;
    
    void setText(const str8 &text);
    void setText(const str32 &text);
    const str8 & getText() const;
    
    void setTextColor(const color_t &color);
    const color_t & getTextColor() const;
    
    // methods
    virtual void init(const TreeDataObject *object);
    
    const UIRenderText & _text() const;
    
protected:
    virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
    
    str8 m_text;
    UIRenderText m_renderText;
    u32 m_textAlignment;
    color_t m_textColor;
    vector2f m_textPosition;
    
};

_vn_end

#endif
