//
//  MapObject_SpawnPoint.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-23.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_SpawnPoint.h"
#include "vnFontManager.h"
#include "vnEditTypeFactory.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(spawn_point_id, "SpawnPointId")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(spawn_point_id)

VN_DEFINE_ET_OBJECT_BEGIN(spawn_point, "MapObject_SpawnPoint")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("ID", false, "SpawnPointId")
VN_DEFINE_ET_OBJECT_END(spawn_point)

IMPL_MAP_OBJECT(MapObject_SpawnPoint, MapObjectDesc_SpawnPoint)

MapObject_SpawnPoint::MapObject_SpawnPoint(MapObjectDesc_SpawnPoint *desc)
: MapObject(desc)
, m_id(vnnew Text()) {
    
    vn::aabox2f rect(0, 0, 40, 40);
    
    Quad *quad = vnnew Quad();
    *quad << vn::color_t(0, 0, 0xFF, 0x80) << rect;
    m_node->addRenderable(quad, false);
    
    *m_id << vn::color_t::kWhite << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << rect;
    m_node->addRenderable(m_id.ptr());
    
    m_position = desc->getPosition();
    m_size.set(1, 1);
    m_node->setPosition(desc->getPosition() * 40);
    m_node->setLocalBoundingBox(rect);
    
    m_object = vn::EditTypeFactory::instance().etObject("MapObject_SpawnPoint")->generate();
    
    static_cast<vn::EditValueInt *>(m_object->attribute(0))->setValue(desc->getId());
    m_object->attribute(0)->addObserver(&m_opId);
    
    m_opId.setFunction_Changed(vn::make_memfn(this, &MapObject_SpawnPoint::_onIdChanged));
    
    _updateId();
}

void MapObject_SpawnPoint::_updateId() {
    char buf[32];
    sprintf(buf, "%d", static_cast<MapObjectDesc_SpawnPoint *>(m_desc.ptr())->getId());
    *m_id << buf;
}

void MapObject_SpawnPoint::setPosition(const vn::vector2i &position) {
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_SpawnPoint *>(m_desc.ptr())->setPosition(position);
}

void MapObject_SpawnPoint::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
}

void MapObject_SpawnPoint::removeNodeFromScene() {
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
}

void MapObject_SpawnPoint::_onIdChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_SpawnPoint *>(m_desc.ptr())->setId(static_cast<vn::EditValueInt *>(value)->getValue());
    _updateId();
}
