//
//  BrushDecoration.h
//  TDMapEditor
//
//  Created by Wayne on 3/31/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushDecoration__
#define __TDMapEditor__BrushDecoration__

#include "Brush.h"
#include "vnUIWindow.h"
#include "vnUIButton.h"
#include "vnUIList.h"
#include "vnUIScrollBar.h"
#include "vnCommandList.h"

#include "MapDecoration.h"

#include "AnimationController.h"


class Brush_Decoration : public Brush, public AnimationController::Handler {
public:
    Brush_Decoration();
    virtual ~Brush_Decoration();
    
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
    
    virtual void anim_frame(const aabox2f &rect, const PicturePtr &image);
    
protected:
    vn::RefCountedPtr<vn::UIWindow> m_uiWindow;
    vn::UIButton *m_btnCreate0, *m_btnCreate1;
    vn::UIList *m_lstImages, *m_lstAnimations;
    vn::UIButton *m_btnFlipX, *m_btnFlipY, *m_btnLightMode, *m_btnDelete;
    vn::UIScrollBar *m_sbOpacity, *m_sbRed, *m_sbGreen, *m_sbBlue;
    
    void _onCreateClicked(vn::UIElement *);
    void _onImageItemClicked(vn::UIElement *, vn::UIListItem *item);
    void _onAnimItemClicked(vn::UIElement *, vn::UIListItem *item);
    void _onDeleteClicked(vn::UIElement *);
    void _onFlipXClicked(vn::UIElement *);
    void _onFlipYClicked(vn::UIElement *);
    void _onLightModeClicked(vn::UIElement *);
    void _onOpacityScrolling(vn::UIElement *, vn::f32);
    void _onRedScrolling(vn::UIElement *, vn::f32);
    void _onGreenScrolling(vn::UIElement *, vn::f32);
    void _onBlueScrolling(vn::UIElement *, vn::f32);
    
    void _beginNewImage(const vn::str8 &imagePath);
    void _beginNewAnimation(const vn::str8 &animationPath);
    void _cancelNew();
    
    SceneNodePtr m_node;
    AnimationControllerPtr m_animController;
    
    enum State {
        kEdit = 0,
        kNewImage,
        kNewAnimation,
    } m_state;
    vn::str8 m_decorationFilePath;
    SceneNode *m_decoration;
    Quad *m_quad;
    bool m_flipX, m_flipY;
    bool m_lightMode;
    vn::vector2f m_extent;
    vn::color_t m_color;
    
    MapDecoration *m_edit;
    bool m_editMoving;
    bool m_editSizing;
    vn::vector2f m_editLastPos;
    vn::vector2f m_editOffset;
    
    void _updateOptions();
    void _setEditDecoration(MapDecoration *deco);
};

class Command_NewDecoration : public vn::Command, public vn::AllocatedObject {
public:
    Command_NewDecoration(MapDecoration::Type type, const vn::vector2f &pos, const vn::str8 &file, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
    
protected:
    MapDecoration::Type m_type;
    vn::vector2f m_pos, m_extent;
    vn::str8 m_file;
    bool m_flipX, m_flipY, m_lightMode;
    vn::color_t m_color;
    MapDecorationPtr m_deco;
};

class Command_DeleteDecoration : public vn::Command, public vn::AllocatedObject {
public:
    Command_DeleteDecoration(const MapDecorationPtr &deco);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
    
protected:
    MapDecorationPtr m_deco;
};

class Command_MoveDecoration : public vn::Command, public vn::AllocatedObject {
public:
    Command_MoveDecoration(const MapDecorationPtr &deco, const vn::vector2f &oldPos, const vn::vector2f &newPos);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
    
protected:
    MapDecorationPtr m_deco;
    vn::vector2f m_oldPos, m_newPos;
};

class Command_SizeDecoration : public vn::Command, public vn::AllocatedObject {
public:
    Command_SizeDecoration(const MapDecorationPtr &deco, const vn::vector2f &oldExtent, const vn::vector2f &newExtent);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
    
protected:
    MapDecorationPtr m_deco;
    vn::vector2f m_oldExtent, m_newExtent;
};

#endif /* defined(__TDMapEditor__BrushDecoration__) */
