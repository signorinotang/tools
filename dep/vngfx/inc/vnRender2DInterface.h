//
//  vnRender2DInterface.h
//  vngfx
//
//  Created by Wayne on 13-12-11.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnRender2DInterface_h
#define vngfx_vnRender2DInterface_h

#include "vnGfxDef.h"
#include "vnTexture2D.h"
#include "vnColor.h"
#include "vnPicture.h"
#include "vnFilePath.h"

_vn_begin

class _vn_gfx_api Render2DInterface {
public:
    enum BlendMode {
        kAlphaBlend = 0,
        kAlphaAdditive
    };
    Render2DInterface();
    ~Render2DInterface();
    
    static Render2DInterface & instance();
    
    virtual Texture2D * createTexture(const vector2i &size) = 0;
    
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void setTexture(Texture2D *texture) = 0;
    virtual void setBlendMode(BlendMode mode) = 0;
    virtual void drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices) = 0;
    
    void drawQuad(const vector2f *corners, const color_t *colors, const vector2f *texcoords);
    void drawPicture(const aabox2f &rect, const color_t &color, Picture *picture);
    
private:
    static Render2DInterface *ms_instance;
};

_vn_end

#endif
