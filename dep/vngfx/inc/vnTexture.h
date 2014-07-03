//
//  vnTexture.h
//  vngfx
//
//  Created by Wayne on 13-12-9.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnTexture_h
#define vngfx_vnTexture_h

#include "vnGfxDef.h"
#include "vnRefCounted.h"
#include "vnVector2.h"

_vn_begin

class _vn_gfx_api Texture : public RefCounted {
public:
    enum Type {
        kTT_Texture2D
    };
    
    enum PixelFormat {
        kTPF_Unknown = 0,
        kTPF_RGBA
    };
    
    Texture();
    virtual Type type() const = 0;
    
    const vector2i & size() const;
    PixelFormat format() const;
protected:
    //u32 m_object;
    vector2i m_size;
    PixelFormat m_format;
    
    //static u32 _cast_pixel_format(PixelFormat format);
};

typedef RefCountedPtr<Texture> TexturePtr;



_vn_end

#endif
