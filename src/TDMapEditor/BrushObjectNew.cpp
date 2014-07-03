//
//  BrushObjectNew.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BrushObjectNew.h"
#include "EditorApp.h"

#include "vnUILocatorRect.h"

Brush_ObjectNew::Brush_ObjectNew()
: m_currentObject(0)
, m_command(0)
, m_uiWindow(0) {
	
}

Brush_ObjectNew::~Brush_ObjectNew() {
	VN_SAFE_DROP(m_uiWindow);
	VN_SAFE_DELETE(m_command);
}

void Brush_ObjectNew::init() {
	BlockBrush::init();
	m_objectNode = vnnew SceneNode();
	m_bkg = vnnew Quad();
	m_objectNode->addRenderable(m_bkg, false);
	m_brush->addChild(m_objectNode.ptr());
	
	m_uiWindow = vnnew vn::UIWindow();
    m_uiWindow->setSizeable(false);
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow);
    locator->setSize(vn::vector2f(256, 512));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow, "1:/TDMapEditor/ui_brush_object_new.tdl");
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);
	vn::UILogicPtr logic = vn::UIFactory::instance().createLogicFromTDL("1:/common/property_list.tdl");
	m_propertyList = logic.dyn_cast<UIPropertyList>();
	if (!m_propertyList.null()) {
		m_propertyList->startup();
		namedElements["properties"]->addChild(m_propertyList->root());
	}
	
	vn::UIList *list = dynamic_cast<vn::UIList *>(namedElements["object_list"]);
	if (list) {
		list->bindAction_ItemClicked(vn::make_memfn(this, &Brush_ObjectNew::_onObjectListItemClicked));
		vn::str8 name;
		for (size_t i = 0, c = list->itemCount(); i < c; ++i) {
			vn::UIListItem *item = list->item(i);
			item->getText(name);
			MapObject *obj = MapObjectFactory::instance().create(name);
			item->setUserData(obj, false);
		}
		_onObjectListItemClicked(list, list->item(0));
	}
}

void Brush_ObjectNew::fini() {
	BlockBrush::fini();
	VN_SAFE_DROP(m_uiWindow);
}

void Brush_ObjectNew::begin(const vn::vector2f &pos) {
	_updateBrush(pos);
	m_command = vnnew Command_PaintMapObject();
	if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
		_paint();
	}
}

void Brush_ObjectNew::end(const vn::vector2f &pos) {
	_updateBrush(pos);
	if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
		_paint();
	}
    if (m_command->empty()) {
        delete m_command;
    } else {
        EditorApp::instance().commandList().do_(m_command);
    }
	m_command = 0;
}

void Brush_ObjectNew::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
	_updateBrush(pos);
	if (EditorApp::instance().isSpaceEmpty(m_index, m_size)) {
		*m_bkg << vn::color_t(0, 0xFF, 0, 0x40);
		if (paint) {
			_paint();
		}
	} else {
		*m_bkg << vn::color_t(0xFF, 0, 0, 0x40);
	}
}

void Brush_ObjectNew::switchIn() {
	BlockBrush::switchIn();
	EditorApp::instance().uiRootElement()->addChild(m_uiWindow);
}

void Brush_ObjectNew::switchOut() {
	BlockBrush::switchOut();
	EditorApp::instance().uiRootElement()->removeChild(m_uiWindow);
}

void Brush_ObjectNew::_paint() {
	MapObject *mobj = m_currentObject->clone();
	mobj->setPosition(m_index);
	EditorApp::instance().addMapObject(mobj, false);
	m_command->add(mobj);
}

void Brush_ObjectNew::_setCurrentObject(MapObject *object) {
	if (m_currentObject == object) {
		return ;
	}
	if (m_currentObject) {
		m_objectNode->removeChild(m_currentObject->node());
	}
	m_currentObject = object;
	
	m_size = m_currentObject->size();
	_resetBrushNode();
	m_objectNode->addChild(m_currentObject->node());
	m_currentObject->node()->setPosition(m_brushBox.min_corner);
	m_propertyList->setTarget(m_currentObject->object());
	*m_bkg << m_brushBox;
}

void Brush_ObjectNew::_onObjectListItemClicked(vn::UIElement *sender, vn::UIListItem *item) {
	if (!item) {
		return ;
	}
	vn::UIList *list = (vn::UIList *)sender;
	if (item == list->getSelectedItem()) {
		return ;
	}
	list->setSelectedItem(item);
	_setCurrentObject((MapObject *)item->getUserData());
}

//////////////////////////////////////////////////////////


void Command_PaintMapObject::_free() {
	delete this;
}

void Command_PaintMapObject::_do() {
	
}

void Command_PaintMapObject::_undo() {
	EditorApp &r = EditorApp::instance();
	for (ObjectList::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
		r.removeMapObject(it->ptr());
	}
}

void Command_PaintMapObject::_redo() {
	EditorApp &r = EditorApp::instance();
	for (ObjectList::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
		r.addMapObject(it->ptr());
	}
}

void Command_PaintMapObject::add(MapObject *obj) {
	obj->grab();
	m_objects.push_back(obj);
}

bool Command_PaintMapObject::empty() const {
    return m_objects.empty();
}