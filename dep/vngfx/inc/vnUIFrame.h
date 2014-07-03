//
//  vnUIFrame.h
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIFrame_h
#define vngfx_vnUIFrame_h

#include "vnUIElement.h"

_vn_begin

class _vn_gfx_api UIFrame : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    virtual UIFrame * clone() const;
    virtual bool contains(const vector2f &point) const;
};

_vn_end

#endif
