//
//  BrushObjectNew.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushObjectNew__
#define __TDMapEditor__BrushObjectNew__

#include "Brush.h"
#include "MapObject.h"
#include "../common/UIPropertyList.h"

#include "vnUIWindow.h"
#include "vnUIListItem.h"
#include "vnCommandList.h"

#include <list>

class Command_PaintMapObject;

class Brush_ObjectNew : public BlockBrush {
public:
	Brush_ObjectNew();
	virtual ~Brush_ObjectNew();
	virtual void init();
    virtual void fini();
    virtual void begin(const vn::vector2f &pos);
    virtual void end(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    
    virtual void switchIn();
    virtual void switchOut();
	
protected:
	
	MapObject *m_currentObject;
	SceneNodePtr m_objectNode;
	Quad *m_bkg;
	
	vn::UIWindow *m_uiWindow;
	vn::RefCountedPtr<UIPropertyList> m_propertyList;
	
	Command_PaintMapObject *m_command;
	
	void _setCurrentObject(MapObject *object);
	
	void _onObjectListItemClicked(vn::UIElement *, vn::UIListItem *item);

	void _paint();
};

class Command_PaintMapObject : public vn::Command, public vn::AllocatedObject {
public:
	virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
	
	void add(MapObject *obj);
	bool empty() const;
private:
	typedef std::list< vn::RefCountedPtr<MapObject> > ObjectList;
	ObjectList m_objects;
};

#endif /* defined(__TDMapEditor__BrushObjectNew__) */
