//
//  vnUIFrame.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIFrame.h"

_vn_begin

UIFrame * UIFrame::clone() const {
    return vnnew UIFrame(*this);
}

bool UIFrame::contains(const vector2f &point) const {
    return false;
}

_vn_end