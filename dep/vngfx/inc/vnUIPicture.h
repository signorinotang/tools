//
//  vnUIPicture.h
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIPicture_h
#define vngfx_vnUIPicture_h

#include "vnUIElement.h"
#include "vnColor.h"
#include "vnUIRenderPicture.h"

_vn_begin

class _vn_gfx_api UIPicture : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    // ctor & dtor
    UIPicture();
	UIPicture(const UIPicture &other);
    virtual UIPicture * clone() const;
    
    
    // properties
    const color_t & getColor() const;
    void setColor(const color_t &color);
    
    const UIRenderPicturePtr & getPicture() const;
    void setPicture(const UIRenderPicturePtr &picture);
    
    // methods
    virtual void init(const TreeDataObject *object);
    
protected:
	virtual void _onBoundingBoxUpdated();
	virtual void _onUpdate(f32 deltaTime);
    virtual void _onRender(UIRenderer *renderer);
    
	color_t m_color;
	UIRenderPicturePtr m_picture;
};

_vn_end


#endif
