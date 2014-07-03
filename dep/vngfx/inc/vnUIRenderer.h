//
//  vnUIRenderer.h
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIRenderer_h
#define vngfx_vnUIRenderer_h

#include "vnGfxDef.h"
#include "vnPicture.h"
#include "vnColor.h"

#include <stack>

_vn_begin

class _vn_gfx_api UIRenderer : public AllocatedObject {
public:
    void pushClippingBox(const aabox2f &clipingBox);
	void popClippingBox();
    
    void drawRect(const aabox2f &rect, const color_t &color);
    void drawRect(const aabox2f &rect, const color_t &color, Texture2D *texture, const vector2f *texcoords);
    void drawRect(const aabox2f &rect, const color_t &color, Picture *picture);
    
private:
    typedef aabox2i rect_type;
    std::stack<aabox2f> m_clippingBoxes;
	bool _getClippingBox(const aabox2f *&clippingBox);
	void _draw(const rect_type &rect, color_t color);
	void _draw(const rect_type &rect, color_t color, Texture2D *texture, const vector2f *texcoords);
	static bool _hasIntersection(const aabox2f &a, const aabox2f &b);
	static bool _clipRect(const aabox2f &clippingBox, aabox2f &rect);
	static bool _clipRect(const aabox2f &clippingBox, aabox2f &rect, const vector2f src_texcoords[4], vector2f dest_texcoords[4]);
};

_vn_end


#endif
