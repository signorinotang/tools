//
//  MapDecorationAnimation.h
//  TDMapEditor
//
//  Created by Wayne on 5/21/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapDecorationAnimation__
#define __TDMapEditor__MapDecorationAnimation__

#include "MapDecoration.h"
#include "AnimationController.h"

class MapDecorationAnimation : public MapDecoration, AnimationController::Handler {
public:
    MapDecorationAnimation();
    
    virtual Type type() const;
    
    virtual void setDesc(const MapDecorationDescPtr &desc, bool update = true);
    
    void setAnimationPath(const vn::str8 &animationPath);
    
    virtual void update(vn::f32 deltaTime);
    
    virtual bool canOperate(const vn::vector2f &point);
    
    virtual void anim_frame(const aabox2f &rect, const PicturePtr &image);
    
protected:
    virtual void _onExtentUpdate();
    virtual void _onColorUpdate();
    virtual void _onLightModeUpdate();
    
    AnimationControllerPtr m_controller;
    Quad *m_quad;
    SceneNodePtr m_animNode;
    vn::vector2f m_animBoundingBox;
};

#endif /* defined(__TDMapEditor__MapDecorationAnimation__) */
