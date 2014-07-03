//
//  vnTexture2D.h
//  vngfx
//
//  Created by Wayne on 13-12-9.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnTexture2D_h
#define vngfx_vnTexture2D_h

#include "vnTexture.h"

_vn_begin

class _vn_gfx_api Texture2D : public Texture {
public:
    virtual Type type() const;
    
    virtual bool updatePixels(const vector2i &offset, const vector2i &size, const void *pixels) = 0;
};

typedef RefCountedPtr<Texture2D> Texture2DPtr;

_vn_end

#endif
