//
//  MapObject_Building.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/30/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_Building.h"
#include "vnPictureManager.h"
#include "vnEditTypeFactory.h"
#include "vnFontManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(building_spawn_point_id, "BuildingSPID")
VN_DEFINE_ET_ENUMERATION_ITEM("0_null", 0)
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(building_spawn_point_id)

VN_DEFINE_ET_ENUMERATION_BEGIN(building_quality, "BuildingQuality")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(building_quality)

VN_DEFINE_ET_ENUMERATION_BEGIN(sp_pos, "BuildingSPPos")
VN_DEFINE_ET_ENUMERATION_ITEM("0_null", 0)
VN_DEFINE_ET_ENUMERATION_ITEM("top1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("top2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("right1", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("right2", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("bottom1", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("bottom2", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("left1", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("left2", 8)
VN_DEFINE_ET_ENUMERATION_END(sp_pos)

VN_DEFINE_ET_OBJECT_BEGIN(building, "MapObject_Building")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Quality", false, "BuildingQuality")
VN_DEFINE_ET_OBJECT_ATTRIB_INT("Gold", false)
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("SP_ID", false, "BuildingSPID")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("SP_Pos", false, "BuildingSPPos")
VN_DEFINE_ET_OBJECT_ATTRIB_INT("Buff_ID", false)
VN_DEFINE_ET_OBJECT_END(building)

IMPL_MAP_OBJECT(MapObject_Building, MapObjectDesc_Building)

const vn::vector2i MapObject_Building::s_collisionIndex[4] = {
    {0, 0}, {1, 0}, {0, 1}, {1, 1}
};

MapObject_Building::MapObject_Building(MapObjectDesc_Building *desc)
: MapObject(desc)
, m_addedToScene(false)
, m_num(vnnew Text())
, m_spid(vnnew Text())
, m_spNode(vnnew SceneNode()) {
    m_position = desc->getPosition();
    m_size.set(2, 2);
    m_node->setPosition(desc->getPosition() * 40);
    m_node->addChild(m_spNode.ptr());
    
    m_spNode->setLocalBoundingBox(vn::aabox2f(0, 0, 40, 40));
    Quad *quad = vnnew Quad();
    *quad << vn::color_t(0, 0, 0xFF, 0x80) << m_spNode->getLocalBoundingBox();
    *m_spid << vn::color_t::kWhite << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << m_spNode->getLocalBoundingBox();
    m_spNode->addRenderable(quad, false);
    m_spNode->addRenderable(m_spid.ptr());
    
    vn::aabox2f rect(0, 0, 80, 80);
	m_node->setLocalBoundingBox(rect);
    quad = vnnew Quad();
    m_node->addRenderable(quad, false);
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|house.png");
    *quad << rect << pic.ptr();
    
    *m_num << vn::color_t::kWhite << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << rect;
    m_node->addRenderable(m_num.ptr());
    
    m_object = vn::EditTypeFactory::instance().etObject("MapObject_Building")->generate();
    
    static_cast<vn::EditValueInt *>(m_object->attribute(0))->setValue(desc->getQuality());
    m_object->attribute(0)->addObserver(&m_opQuality);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(1))->setValue(desc->getGold());
    m_object->attribute(1)->addObserver(&m_opGold);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(2))->setValue(desc->getSpawnPointId());
    m_object->attribute(2)->addObserver(&m_opSPId);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(3))->setValue(desc->getSpawnPointPosition());
    m_object->attribute(3)->addObserver(&m_opSPPos);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(4))->setValue(desc->getBuffId());
    m_object->attribute(4)->addObserver(&m_opBuffId);
    
    m_opQuality.setFunction_Changed(vn::make_memfn(this, &MapObject_Building::_onQualityChanged));
    m_opGold.setFunction_Changed(vn::make_memfn(this, &MapObject_Building::_onGoldChanged));
    m_opSPId.setFunction_Changed(vn::make_memfn(this, &MapObject_Building::_onSPIdChanged));
    m_opSPPos.setFunction_Changed(vn::make_memfn(this, &MapObject_Building::_onSPPosChanged));
    m_opBuffId.setFunction_Changed(vn::make_memfn(this, &MapObject_Building::_onBuffIdChanged));
    
    _updateNum();
    _updateSpawnPoint();
}



void MapObject_Building::setPosition(const vn::vector2i &position) {
    if (m_addedToScene) {
        MapData &md = EditorApp::instance().mapData();
        MapCollision &mc = EditorApp::instance().mapCollision();
        for (int i = 0; i < 4; ++i) {
            mc.decrementObject(md, m_position + s_collisionIndex[i]);
            mc.incrementObject(position + s_collisionIndex[i]);
        }
    }
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setPosition(position);
}

void MapObject_Building::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 4; ++i) {
        mc.incrementObject(m_position + s_collisionIndex[i]);
    }
    m_addedToScene = true;
}

void MapObject_Building::removeNodeFromScene() {
    m_addedToScene = false;
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
    MapData &md = EditorApp::instance().mapData();
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 4; ++i) {
        mc.decrementObject(md, m_position + s_collisionIndex[i]);
    }
}

void MapObject_Building::_updateNum() {
    char buf[32];
    sprintf(buf, "%d", (int)static_cast<MapObjectDesc_Building *>(m_desc.ptr())->getQuality());
    *m_num << buf;
}

void MapObject_Building::_updateSpawnPoint() {
    MapObjectDesc_Building *desc = static_cast<MapObjectDesc_Building *>(m_desc.ptr());
    if (desc->getSpawnPointId() && desc->getSpawnPointPosition()) {
        m_spNode->setVisible(true);
        char buf[32];
        sprintf(buf, "%u", static_cast<MapObjectDesc_Building *>(m_desc.ptr())->getSpawnPointId());
        *m_spid << buf;
        m_spNode->setPosition((desc->buildSpawnPoint() - m_position) * 40);
    } else {
        m_spNode->setVisible(false);
    }
    char buf[32];
    sprintf(buf, "%u", static_cast<MapObjectDesc_Building *>(m_desc.ptr())->getSpawnPointId());
    *m_spid << buf;
}

void MapObject_Building::_onQualityChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setQuality((vn::u8)static_cast<vn::EditValueInt *>(value)->getValue());
    _updateNum();
}

void MapObject_Building::_onGoldChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setGold((vn::u32)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Building::_onSPIdChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setSpawnPointId(static_cast<vn::EditValueInt *>(value)->getValue());
    _updateSpawnPoint();
}

void MapObject_Building::_onSPPosChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setSpawnPointPosition(static_cast<vn::EditValueInt *>(value)->getValue());
    _updateSpawnPoint();
}

void MapObject_Building::_onBuffIdChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Building *>(m_desc.ptr())->setBuffId(static_cast<vn::EditValueInt *>(value)->getValue());
}
