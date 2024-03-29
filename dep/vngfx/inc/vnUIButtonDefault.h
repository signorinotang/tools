//
//  vnUIButtonDefault.h
//  vngfx
//
//  Created by Wayne on 13-12-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIButtonDefault_h
#define vngfx_vnUIButtonDefault_h

#include "vnUIButton.h"
#include "vnUIRenderText.h"

_vn_begin

class _vn_gfx_api UIButtonDefault : public UIButton {
    VN_DECL_UI_ELEMENT()
public:
    virtual UIButtonDefault * clone() const;
    
     virtual void init(const TreeDataObject *object);
    
    void setText(const str8 &text);
    
protected:
    
    virtual void _onUpdate(f32 deltaTime);
    virtual void _onRender(UIRenderer *renderer);
    
    
    UIRenderText m_renderText;
    vector2f m_textPosition;
};

_vn_end

#endif
