//
//  BrushBlockType.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-31.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushBlockType__
#define __TDMapEditor__BrushBlockType__

#include "Brush.h"
#include "MapData.h"
#include "vnUIWindow.h"
#include "vnUIList.h"
#include "vnCommandList.h"

class Command_PaintBlockType : public vn::Command, public vn::AllocatedObject {
public:
	virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
	
	void add(const vn::vector2i &index, MapBlockType oldType, MapBlockType newType);
	bool empty() const;
private:
	struct Item {
		vn::vector2i index;
		MapBlockType oldType, newType;
	};
	
	typedef std::vector<Item> Items;
	Items m_items;;
};

class Brush_BlockType : public BlockBrush {
public:
    Brush_BlockType();
    virtual ~Brush_BlockType();
    virtual void init();
    virtual void fini();
    virtual void begin(const vn::vector2f &pos);
    virtual void end(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    
    virtual void switchIn();
    virtual void switchOut();
    
    virtual void onKeyDown(vn::c32 chr, bool repeat);
    
protected:
    void _setBrushSize(vn::s32 _size);
    void _paint();
    MapBlockType m_type;
    vn::UIWindow *m_uiWindow;
    vn::UIList *m_uiTypeList;
    
    void _onTypeListItemClicked(vn::UIElement *, vn::UIListItem *item);
	
	Command_PaintBlockType *m_command;
};




#endif /* defined(__TDMapEditor__UIBrushBlockType__) */
