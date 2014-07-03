//
//  BrushCollision.h
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushCollision__
#define __TDMapEditor__BrushCollision__

#include "Brush.h"
#include "vnUIWindow.h"
#include "vnUIButton.h"

#include <map>

class Brush_Collision : public BlockBrush {
public:
    
    Brush_Collision();
    ~Brush_Collision();
    
    virtual void init();
    virtual void fini();
    virtual void begin(const vn::vector2f &pos);
    virtual void end(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    virtual bool busy() const;
    
    virtual void switchIn();
    virtual void switchOut();
    
    virtual void onKeyDown(vn::c32 chr, bool repeat);
    virtual void onMouseRightDown(const vn::vector2f &pos);
    virtual void onMouseRightUp(const vn::vector2f &pos);
    
    virtual void onMapObjectRemoved(MapObject *mobj);
    virtual void onMapDecorationRemoved(MapDecoration *deco);
    
protected:
    
    void _onWaterClicked(vn::UIElement *);
    
    vn::vector2f m_lastPos;
    bool m_moved;
    vn::UIWindow *m_uiWindow;
    vn::UIButton *m_uiWater;
    
    std::map<vn::u32, vn::vector2i> m_spawnPoints;
    vn::u32 m_collisionMask;
};

#endif /* defined(__TDMapEditor__BrushCollision__) */
