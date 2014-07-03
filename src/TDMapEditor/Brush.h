//
//  Brush.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__Brush__
#define __TDMapEditor__Brush__

#include "vnRefCounted.h"
#include "vnVector2.h"
#include "SceneNode.h"
#include "MapSSO.h"

enum BrushName {
    kBrush_MoveCamera = 0,
    kBrush_BlockType,
	kBrush_ObjectNew,
	kBrush_ObjectEdit,
    kBrush_Decoration,
    kBrush_Collision,
    kBrush_SSO,
};

class MapObject;
class MapDecoration;

class Brush : public vn::RefCounted {
public:
    virtual void init() {}
    virtual void fini() {}
    virtual void begin(const vn::vector2f &pos) {}
    virtual void end(const vn::vector2f &pos) {}
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {}
    virtual bool busy() const { return false; }
    
    virtual void switchIn() {}
    virtual void switchOut() {}
    
    virtual void onKeyDown(vn::c32 chr, bool repeat) {}
    virtual void onMouseRightDown(const vn::vector2f &pos) {}
    virtual void onMouseRightUp(const vn::vector2f &pos) {}
    
    virtual void onMapObjectRemoved(MapObject *mobj) {}
    virtual void onMapDecorationRemoved(MapDecoration *deco) {}
    virtual void onMapSSORemoved(const MapSSOPtr &sso) {}
};

typedef vn::RefCountedPtr<Brush> BrushPtr;

class Brush_MoveCamera : public Brush {
public:
    
    virtual void begin(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    
protected:
    vn::vector2f m_last;
};

class BlockBrush : public Brush {
public:
    BlockBrush();
    virtual void init();
    virtual void fini();
    
    virtual void switchIn();
    virtual void switchOut();
protected:
    vn::vector2i m_index;
    vn::vector2i m_size;
    vn::aabox2f m_brushBox;
    SceneNodePtr m_brush;
    
    virtual void _resetBrushNode();
    void _updateBrush(const vn::vector2f &pos);
    void _updateBrushByIndex();
};

#endif /* defined(__TDMapEditor__Brush__) */
