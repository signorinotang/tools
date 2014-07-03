//
//  vnRender2DDevice.h
//  vngfx
//
//  Created by Wayne on 13-12-11.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnRender2DDevice_h
#define vngfx_vnRender2DDevice_h

#include "vnGfxDef.h"
#include "vnMatrix3.h"
#include "vnTexture2D.h"

_vn_begin

class Render2DDevice : public RefCounted {
public:
    virtual void resize(u32 width, u32 height) = 0;
    virtual void setViewMatrix(const matrix3 &mat) = 0;
    virtual void clear() = 0;
    virtual bool begin() = 0;
    virtual void end() = 0;
    virtual void present() = 0;
    
    static Render2DDevice * create(void *window, u32 width, u32 height);
};

_vn_end

#endif
