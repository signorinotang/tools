//
//  BrushObjectEdit.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushObjectEdit__
#define __TDMapEditor__BrushObjectEdit__

#include "Brush.h"
#include "MapObject.h"
#include "../common/UIPropertyList.h"

#include "vnUIWindow.h"
#include "vnUIListItem.h"
#include "vnCommandList.h"

#include <list>

class Command_MoveObject;
class Command_CopyObject;
class Command_RemoveObject;

class Brush_ObjectEdit : public BlockBrush {
public:
    Brush_ObjectEdit();
    virtual ~Brush_ObjectEdit();
    
    virtual void init();
    virtual void fini();
    virtual void begin(const vn::vector2f &pos);
    virtual void end(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    virtual bool busy() const;
    
    virtual void switchIn();
    virtual void switchOut();
    
    virtual void onKeyDown(vn::c32 chr, bool repeat);
    
    virtual void onMapObjectRemoved(MapObject *mobj);
protected:
    vn::UIWindow *m_uiWindow;
    vn::RefCountedPtr<UIPropertyList> m_propertyList;
    SceneNodePtr m_objectNode;
	Quad *m_objectBackground;
    
    MapObject *m_currentObject;
    MapObject *m_orignalObject;
    vn::vector2i m_orignalPos;
    
    void _setCurrentObject(MapObject *object);
    
    enum {
        kMode_Select = 0,
        kMode_Move = 1,
        kMode_Copy = 2,
    };
    vn::u32 m_mode;
    vn::UIButton *m_btnMove, *m_btnCopy, *m_btnRemove;
    
    void _select(MapObject *mobj);
    void _unselect();
    void _resetMode();
    void _enterMoveMode();
    void _enterCopyMode();
    
    void _onMoveClicked(vn::UIElement *);
    void _onCopyClicked(vn::UIElement *);
    void _onRemoveClicked(vn::UIElement *);
};

class Command_RemoveObject : public vn::Command, public vn::AllocatedObject {
public:
    Command_RemoveObject(MapObject *mobj);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
private:
    vn::RefCountedPtr<MapObject> m_object;
};

class Command_MoveObject : public vn::Command, public vn::AllocatedObject {
public:
    Command_MoveObject(MapObject *mobj, const vn::vector2i &oldPos, const vn::vector2i &newPos);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
private:
    vn::RefCountedPtr<MapObject> m_object;
    vn::vector2i m_oldPos, m_newPos;
};

class Command_CopyObject : public vn::Command, public vn::AllocatedObject {
public:
    Command_CopyObject(MapObject *mobj);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
private:
    vn::RefCountedPtr<MapObject> m_object;
};

#endif /* defined(__TDMapEditor__BrushObjectEdit__) */
