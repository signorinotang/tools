//
//  MapDecorationImage.h
//  TDMapEditor
//
//  Created by Wayne on 4/1/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapDecorationImage__
#define __TDMapEditor__MapDecorationImage__

#include "MapDecoration.h"

class MapDecorationImage : public MapDecoration {
public:
    MapDecorationImage();
    
    virtual Type type() const;
    
    virtual void setDesc(const MapDecorationDescPtr &desc, bool update = true);
    
    void setImagePath(const vn::str8 &imagePath);
    
    
protected:
    virtual void _onExtentUpdate();
    virtual void _onColorUpdate();
    virtual void _onLightModeUpdate();
    
    vn::RefCountedPtr<Quad> m_quad;
};

#endif /* defined(__TDMapEditor__MapDecorationImage__) */
