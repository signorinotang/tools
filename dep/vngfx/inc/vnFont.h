//
//  vnFont.h
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnFont_h
#define vngfx_vnFont_h

#include "vnTexture2D.h"
#include "vnAABox2.h"
#include "vnFilePath.h"

_vn_begin

class _vn_gfx_api Font : public RefCounted {
public:
    struct Glyph {
        bool valid;
        aabox2f bounds;
        vector2f size;
        aabox2f texcoord;
        Texture2DPtr texture;
    };
    
    virtual const Glyph * build(c32 chr) = 0;
    virtual f32 height() = 0;
    virtual void discard() = 0;
};

typedef RefCountedPtr<Font> FontPtr;

_vn_end

#endif
