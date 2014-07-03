//
//  Camera.h
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __gamelib__Camera__
#define __gamelib__Camera__

#include "vnRefCounted.h"
#include "vnMatrix3.h"
#include "vnAABox2.h"

class Camera : public vn::RefCounted {
public:
    Camera();
    
    void update();
    void move(const vn::vector2f &delta);
    
    void setPosition(const vn::vector2f &pos);
    void setZoom(vn::f32 zoom);
    vn::f32 getZoom() const;
    
    void setViewFocus(const vn::vector2f &offset);
    void setViewSize(const vn::vector2f &size);
    
    const vn::matrix3 & viewMatrix() const;
    const vn::aabox2f & boundingBox() const;
    
    vn::vector2f convertToView(const vn::vector2f &point);
    vn::vector2f convertFromView(const vn::vector2f &point);
protected:
    vn::matrix3 m_viewMatrix;
    vn::vector2f m_position;
    vn::f32 m_zoom;
    vn::vector2f m_viewFocus;
    vn::vector2f m_viewSize;
    vn::aabox2f m_boundingBox;
    bool m_dirty;
    
    void _updateViewMatrix();
    void _updateBoundingBox();
};

#endif /* defined(__gamelib__Camera__) */
