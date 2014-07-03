//
//  vnTexture2D.cpp
//  vngfx
//
//  Created by Wayne on 13-12-9.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnTexture2D.h"

#if VN_PLATFORM == VN_PLATFORM_MAC
#	include <OpenGL/gl3.h>
#elif VN_PLATFORM == VN_PLATFORM_WIN

#endif

_vn_begin

Texture::Type Texture2D::type() const {
    return kTT_Texture2D;
}

_vn_end
