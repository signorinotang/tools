//
//  BrushObjectEdit.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BrushObjectEdit.h"
#include "vnUILocatorRect.h"
#include "EditorApp.h"

Brush_ObjectEdit::Brush_ObjectEdit()
: m_currentObject(0)
, m_orignalObject(0)
, m_uiWindow(0)
, m_objectBackground(0)
, m_mode(kMode_Select) {
    
}


Brush_ObjectEdit::~Brush_ObjectEdit() {
    VN_SAFE_DROP(m_uiWindow);
    VN_SAFE_DROP(m_currentObject);
}

void Brush_ObjectEdit::init() {
	BlockBrush::init();
	m_objectNode = vnnew SceneNode();
	m_objectBackground = vnnew Quad();
	m_objectNode->addRenderable(m_objectBackground, false);
    m_brush->setVisible(false);
	//m_brush->addChild(m_objectNode.ptr());
	
	m_uiWindow = vnnew vn::UIWindow();
    m_uiWindow->setSizeable(false);
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow);
    locator->setSize(vn::vector2f(256, 512));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow, "1:/TDMapEditor/ui_brush_object_edit.tdl");
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);
	vn::UILogicPtr logic = vn::UIFactory::instance().createLogicFromTDL("1:/common/property_list.tdl");
	m_propertyList = logic.dyn_cast<UIPropertyList>();
	if (!m_propertyList.null()) {
		m_propertyList->startup();
		namedElements["properties"]->addChild(m_propertyList->root());
	}
	
    m_btnMove = dynamic_cast<vn::UIButton *>(namedElements["move"]);
    m_btnCopy = dynamic_cast<vn::UIButton *>(namedElements["copy"]);
    m_btnRemove = dynamic_cast<vn::UIButton *>(namedElements["remove"]);
    
    m_btnMove->bindAction_Clicked(vn::make_memfn(this, &Brush_ObjectEdit::_onMoveClicked));
    m_btnCopy->bindAction_Clicked(vn::make_memfn(this, &Brush_ObjectEdit::_onCopyClicked));
    m_btnRemove->bindAction_Clicked(vn::make_memfn(this, &Brush_ObjectEdit::_onRemoveClicked));
    
    m_btnMove->setEnabled(false);
    m_btnCopy->setEnabled(false);
    m_btnRemove->setEnabled(false);
}

void Brush_ObjectEdit::fini() {
    BlockBrush::fini();
	VN_SAFE_DROP(m_uiWindow);
    VN_SAFE_DROP(m_currentObject);
    VN_SAFE_DROP(m_orignalObject);
}

void Brush_ObjectEdit::begin(const vn::vector2f &pos) {
    if (m_mode == kMode_Select) {
        MapObject *mobj = EditorApp::instance().pickMapObjectByViewPos(pos);
        _select(mobj);
    }
}

void Brush_ObjectEdit::end(const vn::vector2f &pos) {
    if (m_mode == kMode_Move) {
        _updateBrush(pos);
        if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
            m_currentObject->setPosition(m_index);
            EditorApp::instance().addMapObject(m_currentObject);
            m_btnMove->setPushed(false);
            m_brush->removeChild(m_objectNode.ptr());
            m_mode = 0;
            EditorApp::instance().commandList().do_(vnnew Command_MoveObject(m_currentObject, m_orignalPos, m_index));
        } else {
            *m_objectBackground << vn::color_t(0xFF, 0, 0, 0x40);
        }
    } else if (m_mode == kMode_Copy) {
        _updateBrush(pos);
        if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
            m_orignalObject->drop();
            m_orignalObject = 0;
            
            m_currentObject->setPosition(m_index);
            EditorApp::instance().addMapObject(m_currentObject);
            m_btnCopy->setPushed(false);
            m_brush->removeChild(m_objectNode.ptr());
            m_mode = 0;
            EditorApp::instance().commandList().do_(vnnew Command_CopyObject(m_currentObject));
        } else {
            *m_objectBackground << vn::color_t(0xFF, 0, 0, 0x40);
        }
    }
}

void Brush_ObjectEdit::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    if (m_mode) {
        _updateBrush(pos);
        if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
            *m_objectBackground << vn::color_t(0, 0xFF, 0, 0x40);
        } else {
            *m_objectBackground << vn::color_t(0xFF, 0, 0, 0x40);
        }
    } else if (m_currentObject) {
        m_index = m_currentObject->getPosition();
        _updateBrushByIndex();
    }
}

bool Brush_ObjectEdit::busy() const {
    return m_mode != 0;
}

void Brush_ObjectEdit::switchIn() {
    BlockBrush::switchIn();
    EditorApp::instance().uiRootElement()->addChild(m_uiWindow);
}

void Brush_ObjectEdit::switchOut() {
    BlockBrush::switchOut();
    _unselect();
    EditorApp::instance().uiRootElement()->removeChild(m_uiWindow);
}

void Brush_ObjectEdit::onKeyDown(vn::c32 chr, bool repeat) {
    if (repeat) {
        return ;
    }
    switch (chr) {
        case 0x1B: // ESC
            _resetMode();
            break;
            
        case 'A':
        case 'a':
            if (m_btnMove->isEnabled()) {
                _onMoveClicked(m_btnMove);
            }
            break;
            
        case 'S':
        case 's':
            if (m_btnCopy->isEnabled()) {
                _onCopyClicked(m_btnCopy);
            }
            break;
            
        case 'D':
        case 'd':
            if (m_btnRemove->isEnabled()) {
                _onRemoveClicked(m_btnRemove);
            }
            break;
    }
}

void Brush_ObjectEdit::onMapObjectRemoved(MapObject *mobj) {
    if (m_currentObject == mobj) {
        if (m_mode == kMode_Select) {
            _unselect();
        }
    }
}

void Brush_ObjectEdit::_setCurrentObject(MapObject *object) {
    if (m_currentObject == object) {
        return ;
    }
    if (m_currentObject) {
        m_currentObject->drop();
    }
    if ((m_currentObject = object)) {
        object->grab();
        m_propertyList->setTarget(object->object());
    } else {
        m_propertyList->setTarget(0);
    }
}

void Brush_ObjectEdit::_select(MapObject *mobj) {
    _resetMode();
    _setCurrentObject(mobj);
    if (mobj) {
        m_index = mobj->getPosition();
        m_size = mobj->size();
        _resetBrushNode();
        _updateBrushByIndex();
        *m_objectBackground << m_brushBox;
        m_brush->setVisible(true);
        m_btnMove->setEnabled(true);
        m_btnCopy->setEnabled(true);
        m_btnRemove->setEnabled(true);
    } else {
        m_brush->setVisible(false);
        m_btnMove->setEnabled(false);
        m_btnCopy->setEnabled(false);
        m_btnRemove->setEnabled(false);
    }
}

void Brush_ObjectEdit::_unselect() {
    _resetMode();
    _setCurrentObject(0);
    m_brush->setVisible(false);
    m_btnMove->setEnabled(false);
    m_btnCopy->setEnabled(false);
    m_btnRemove->setEnabled(false);
}

void Brush_ObjectEdit::_resetMode() {
    if (m_mode == kMode_Move) {
        m_currentObject->setPosition(m_orignalPos);
        EditorApp::instance().addMapObject(m_currentObject);
        m_btnMove->setPushed(false);
        
        m_brush->removeChild(m_objectNode.ptr());
        m_index = m_currentObject->getPosition();
        _updateBrushByIndex();
    } else if (m_mode == kMode_Copy) {
        _setCurrentObject(m_orignalObject);
        m_orignalObject->drop();
        m_orignalObject = 0;
        
        m_btnCopy->setPushed(false);
        
        m_brush->removeChild(m_objectNode.ptr());
        m_index = m_currentObject->getPosition();
        _updateBrushByIndex();
    }
    m_mode = 0;
}

void Brush_ObjectEdit::_enterMoveMode() {
    if (m_mode) {
        if (m_mode != kMode_Move) {
            _resetMode();
        } else {
            return ;
        }
    }
    m_mode = kMode_Move;
    vnassert(m_currentObject);
    m_orignalPos = m_currentObject->getPosition();
    EditorApp::instance().removeMapObject(m_currentObject);
    m_objectNode->addChild(m_currentObject->node());
    m_currentObject->node()->setPosition(m_brushBox.min_corner);
    m_brush->addChild(m_objectNode.ptr());
    
    m_btnMove->setPushed(true);
}

void Brush_ObjectEdit::_enterCopyMode() {
    if (m_mode) {
        if (m_mode != kMode_Copy) {
            _resetMode();
        } else {
            return ;
        }
    }
    m_mode = kMode_Copy;
    vnassert(m_currentObject);
    m_orignalObject = m_currentObject;
    m_orignalObject->grab();
    _setCurrentObject(m_currentObject->clone());
    m_currentObject->drop();
    
    m_objectNode->addChild(m_currentObject->node());
    m_currentObject->node()->setPosition(m_brushBox.min_corner);
    m_brush->addChild(m_objectNode.ptr());
    
    m_btnCopy->setPushed(true);
}

void Brush_ObjectEdit::_onMoveClicked(vn::UIElement *) {
    _enterMoveMode();
}

void Brush_ObjectEdit::_onCopyClicked(vn::UIElement *) {
    _enterCopyMode();
}

void Brush_ObjectEdit::_onRemoveClicked(vn::UIElement *) {
    _resetMode();
    vnassert(m_currentObject);
    EditorApp::instance().commandList().do_(vnnew Command_RemoveObject(m_currentObject));
}


///////////////////////////////////////////////////////////////
Command_RemoveObject::Command_RemoveObject(MapObject *mobj)
: m_object(mobj) {
    mobj->grab();
}

void Command_RemoveObject::_free() {
    delete this;
}

void Command_RemoveObject::_do() {
    EditorApp::instance().removeMapObject(m_object.ptr());
}

void Command_RemoveObject::_undo() {
    EditorApp::instance().addMapObject(m_object.ptr());
}

void Command_RemoveObject::_redo() {
    EditorApp::instance().removeMapObject(m_object.ptr());
}

///////////////////////////////////////////////////////////////
Command_MoveObject::Command_MoveObject(MapObject *mobj, const vn::vector2i &oldPos, const vn::vector2i &newPos)
: m_object(mobj)
, m_oldPos(oldPos)
, m_newPos(newPos) {
    mobj->grab();
}

void Command_MoveObject::_free() {
    delete this;
}

void Command_MoveObject::_do() {
    
}

void Command_MoveObject::_undo() {
    EditorApp::instance().setMapObjectPosition(m_object.ptr(), m_oldPos);
}

void Command_MoveObject::_redo() {
    EditorApp::instance().setMapObjectPosition(m_object.ptr(), m_newPos);
}

///////////////////////////////////////////////////////////////
Command_CopyObject::Command_CopyObject(MapObject *mobj)
: m_object(mobj) {
    mobj->grab();
}

void Command_CopyObject::_free() {
    delete this;
}

void Command_CopyObject::_do() {
    
}

void Command_CopyObject::_undo() {
    EditorApp::instance().removeMapObject(m_object.ptr());
}

void Command_CopyObject::_redo() {
    EditorApp::instance().addMapObject(m_object.ptr());
}