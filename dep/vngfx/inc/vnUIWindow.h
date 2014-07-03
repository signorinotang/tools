//
//  vnUIWindow.h
//  vngfx
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIWindow_h
#define vngfx_vnUIWindow_h

#include "vnUIElement.h"

_vn_begin

class _vn_gfx_api UIWindow : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    
    enum BorderIndex {
        kBorderLeft = 1,
        kBorderTop = 2,
        kBorderRight = 4,
        kBorderBottom = 8,
    };
    
    UIWindow();
    virtual UIWindow * clone() const;
    
    void setBorderSize(BorderIndex index, f32 size);
    f32 getBorderSize(BorderIndex index) const;
    
    void setMinSize(const vector2f &size);
    const vector2f & getMinSize() const;
    
    void setSizeable(bool sizeable);
    bool isSizeable() const;
    
    virtual void init(const TreeDataObject *object);
    
    virtual void onMouseLeftDown(const vector2f &position);
    virtual void onMouseLeftUp(const vector2f &position);
    virtual void onMouseCaptureCancelled();
    
protected:
    
    virtual void _onUpdate(f32 deltaTime);
    
    f32 m_borderLeft;
    f32 m_borderTop;
    f32 m_borderRight;
    f32 m_borderBottom;
    
    u32 m_mouseDown;
    vector2f m_lastMousePos;
    bool m_sizeable;
    vector2f m_minSize;
};

_vn_end

#endif