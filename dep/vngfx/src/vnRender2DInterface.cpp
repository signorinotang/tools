//
//  vnRender2DInterface.cpp
//  vngfx
//
//  Created by Wayne on 13-12-11.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnRender2DInterface.h"

_vn_begin

Render2DInterface * Render2DInterface::ms_instance = 0;

Render2DInterface::Render2DInterface() {
    vnassert(ms_instance == 0);
    ms_instance = this;
}

Render2DInterface::~Render2DInterface() {
    ms_instance = 0;
}

Render2DInterface & Render2DInterface::instance() {
    vnassert(ms_instance);
    return *ms_instance;
}

void Render2DInterface::drawQuad(const vector2f *corners, const vn::color_t *colors, const vector2f *texcoords) {
    static u16 s_indices[] = {0, 1, 2, 0, 2, 3};
    drawTriangles(2, corners, colors, texcoords, 4, s_indices);
}

void Render2DInterface::drawPicture(const aabox2f &rect, const vn::color_t &color, vn::Picture *picture) {
    vector2f corner[4];
    color_t diffuse[4] = { color, color, color, color };
    

    corner[0] = rect.min_corner;
    corner[1].set(rect.max_corner.x, rect.min_corner.y);
    corner[2] = rect.max_corner;
    corner[3].set(rect.min_corner.x, rect.max_corner.y);
    
    if (picture) {
        setTexture(picture->texture.ptr());
        drawQuad(corner, diffuse, picture->texcoords);
    } else {
        vector2f texcoord[4];
        setTexture(0);
        drawQuad( corner, diffuse, texcoord);
    }
}

_vn_end