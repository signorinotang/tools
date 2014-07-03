//
//  MapObject_Cage.cpp
//  TDMapEditor
//
//  Created by Wayne on 6/25/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_Cage.h"
#include "vnPictureManager.h"
#include "vnEditTypeFactory.h"
#include "vnFontManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(cage_quality, "CageQuality")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(cage_quality)

VN_DEFINE_ET_OBJECT_BEGIN(cage, "MapObject_Cage")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Quality", false, "CageQuality")
VN_DEFINE_ET_OBJECT_ATTRIB_INT("CloseId", false)
VN_DEFINE_ET_OBJECT_ATTRIB_INT("OpenId", false)
VN_DEFINE_ET_OBJECT_ATTRIB_STRING("Name", false)
VN_DEFINE_ET_OBJECT_END(cage)

IMPL_MAP_OBJECT(MapObject_Cage, MapObjectDesc_Cage)

const vn::vector2i MapObject_Cage::s_collisionIndex[4] = {
    {0, 0}, {1, 0}, {0, 1}, {1, 1}
};

MapObject_Cage::MapObject_Cage(MapObjectDesc_Cage *desc)
: MapObject(desc)
, m_addedToScene(false)
, m_name(vnnew Text()) {
    m_position = desc->getPosition();
    m_size.set(2, 2);
    m_node->setPosition(m_position * 40);

    vn::aabox2f rect(0, 0, 80, 80);
	m_node->setLocalBoundingBox(rect);
    Quad *quad = vnnew Quad();
    m_node->addRenderable(quad, false);
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1:/TDMapEditor/base.imgs|cage.png");
    *quad << rect << pic.ptr();
    
    *m_name << vn::color_t::kWhite << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << rect;
    m_node->addRenderable(m_name.ptr());
    
    m_object = vn::EditTypeFactory::instance().etObject("MapObject_Cage")->generate();
    
    size_t index = 0;
    static_cast<vn::EditValueInt *>(m_object->attribute(index))->setValue(desc->getQuality());
    m_object->attribute(index)->addObserver(&m_opQuality);
    ++index;
    
    static_cast<vn::EditValueInt *>(m_object->attribute(index))->setValue(desc->getCloseId());
    m_object->attribute(index)->addObserver(&m_opCloseId);
    ++index;
    
    static_cast<vn::EditValueInt *>(m_object->attribute(index))->setValue(desc->getOpenId());
    m_object->attribute(index)->addObserver(&m_opOpenId);
    ++index;
    
    static_cast<vn::EditValueString *>(m_object->attribute(index))->setValue(desc->getName());
    m_object->attribute(index)->addObserver(&m_opName);
    ++index;
    
    m_opQuality.setFunction_Changed(vn::make_memfn(this, &MapObject_Cage::_onQualityChanged));
    m_opCloseId.setFunction_Changed(vn::make_memfn(this, &MapObject_Cage::_onCloseIdChanged));
    m_opOpenId.setFunction_Changed(vn::make_memfn(this, &MapObject_Cage::_onOpenIdChanged));
    m_opName.setFunction_Changed(vn::make_memfn(this, &MapObject_Cage::_onNameChanged));
    
    _updateName();
}

void MapObject_Cage::setPosition(const vn::vector2i &position) {
    if (m_addedToScene) {
        MapData &md = EditorApp::instance().mapData();
        MapCollision &mc = EditorApp::instance().mapCollision();
        for (int i = 0; i < 4; ++i) {
            mc.decrementObject(md, m_position + s_collisionIndex[i]);
            mc.incrementObject(position + s_collisionIndex[i]);
        }
    }
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->setPosition(position);
}

void MapObject_Cage::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 4; ++i) {
        mc.incrementObject(m_position + s_collisionIndex[i]);
    }
    m_addedToScene = true;
}

void MapObject_Cage::removeNodeFromScene() {
    m_addedToScene = false;
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
    MapData &md = EditorApp::instance().mapData();
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 4; ++i) {
        mc.decrementObject(md, m_position + s_collisionIndex[i]);
    }
}

void MapObject_Cage::_updateName() {
    *m_name << static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->getName();
}

void MapObject_Cage::_onQualityChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->setQuality((vn::u8)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Cage::_onCloseIdChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->setCloseId((vn::u32)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Cage::_onOpenIdChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->setOpenId((vn::u32)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Cage::_onNameChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Cage *>(m_desc.ptr())->setName(static_cast<vn::EditValueString *>(value)->getValue());
    _updateName();
}