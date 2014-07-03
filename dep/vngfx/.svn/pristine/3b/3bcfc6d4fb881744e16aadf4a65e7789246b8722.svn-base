//
//  vnTexture2DImpl.h
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef __vngfx__vnTexture2DImpl__
#define __vngfx__vnTexture2DImpl__

#include "vnTexture2D.h"

_vn_begin

namespace internal {
    class Texture2DImpl : public Texture2D {
    public:
        Texture2DImpl();
        virtual ~Texture2DImpl();
        
        void bind();
        bool init(const vector2i &size, const void *pixels);
        
        virtual bool updatePixels(const vector2i &offset, const vector2i &size, const void *pixels);
        
    private:
        u32 m_object;
    };
    
    typedef RefCountedPtr<Texture2DImpl> Texture2DImplPtr;
}

_vn_end

#endif /* defined(__vngfx__vnTexture2DImpl__) */
