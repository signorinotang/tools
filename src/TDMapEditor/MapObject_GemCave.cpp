//
//  MapObject_GemCave.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_GemCave.h"
#include "vnEditTypeFactory.h"
#include "vnPictureManager.h"
#include "vnFontManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(num_gems, "NumGems")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_END(num_gems)

VN_DEFINE_ET_OBJECT_BEGIN(gem_cave, "MapObject_GemCave")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("NumGems", false, "NumGems")
VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Hole_Up", false);
VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Hole_Right", false);
VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Hole_Down", false);
VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Hole_Left", false);
VN_DEFINE_ET_OBJECT_END(gem_cave)

IMPL_MAP_OBJECT(MapObject_GemCave, MapObjectDesc_GemCave)

const vn::vector2i MapObject_GemCave::s_collisionIndex[16] = {
    {0, 0}, {1, 0}, {2, 0}, {3, 0},
    {4, 0}, {4, 1}, {4, 2}, {4, 3},
    {4, 4}, {3, 4}, {2, 4}, {1, 4},
    {0, 4}, {0, 3}, {0, 2}, {0, 1}
};

MapObject_GemCave::MapObject_GemCave(MapObjectDesc_GemCave *desc)
: MapObject(desc)
, m_caveBackground(vnnew Quad())
, m_caveGems(vnnew Text())
, m_addedToScene(false) {
    
    m_cavePart[0] = vnnew Quad();
    *m_cavePart[0] << vn::aabox2f(0, 0, 100, 100);
    m_cavePart[1] = vnnew Quad();
    *m_cavePart[1] << vn::aabox2f(100, 0, 200, 100);
    m_cavePart[2] = vnnew Quad();
    *m_cavePart[2] << vn::aabox2f(100, 100, 200, 200);
    m_cavePart[3] = vnnew Quad();
    *m_cavePart[3] << vn::aabox2f(0, 100, 100, 200);
    for (int i = 0; i < 4; ++i) {
        m_node->addRenderable(m_cavePart[i].ptr());
    }
    *m_caveGems << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << vn::aabox2f(100, 100, 100, 100);
    m_node->addRenderable(m_caveGems.ptr());
    
    m_position = desc->getPosition();
    m_size.set(5, 5);
    m_node->setPosition(desc->getPosition() * 40);
    
    vn::aabox2f rect(0, 0, 200, 200);
    m_node->setLocalBoundingBox(rect);
    *m_caveBackground << rect;
    
    m_object = vn::EditTypeFactory::instance().etObject("MapObject_GemCave")->generate();
    
    static_cast<vn::EditValueInt *>(m_object->attribute(0))->setValue(desc->getNumGems());
    m_object->attribute(0)->addObserver(&m_opNumGems);
    
    static_cast<vn::EditValueBool *>(m_object->attribute(1))->setValue(desc->isHoleEnabled(0));
    m_object->attribute(1)->addObserver(&m_opHole0);
    
    static_cast<vn::EditValueBool *>(m_object->attribute(2))->setValue(desc->isHoleEnabled(1));
    m_object->attribute(2)->addObserver(&m_opHole1);
    
    static_cast<vn::EditValueBool *>(m_object->attribute(3))->setValue(desc->isHoleEnabled(2));
    m_object->attribute(3)->addObserver(&m_opHole2);
    
    static_cast<vn::EditValueBool *>(m_object->attribute(4))->setValue(desc->isHoleEnabled(3));
    m_object->attribute(4)->addObserver(&m_opHole3);
    
    m_opNumGems.setFunction_Changed(vn::make_memfn(this, &MapObject_GemCave::_onNumGemsChanged));
    m_opHole0.setFunction_Changed(vn::make_memfn(this, &MapObject_GemCave::_onHole0Changed));
    m_opHole1.setFunction_Changed(vn::make_memfn(this, &MapObject_GemCave::_onHole1Changed));
    m_opHole2.setFunction_Changed(vn::make_memfn(this, &MapObject_GemCave::_onHole2Changed));
    m_opHole3.setFunction_Changed(vn::make_memfn(this, &MapObject_GemCave::_onHole3Changed));
    
    _updateCaveParts();
    _updateCaveGems();
    
    m_collision[0] = true;
    m_collision[1] = true;
    m_collision[2] = !desc->isHoleEnabled(0);
    m_collision[3] = true;
    m_collision[4] = true;
    m_collision[5] = true;
    m_collision[6] = !desc->isHoleEnabled(1);
    m_collision[7] = true;
    m_collision[8] = true;
    m_collision[9] = true;
    m_collision[10] = !desc->isHoleEnabled(2);
    m_collision[11] = true;
    m_collision[12] = true;
    m_collision[13] = true;
    m_collision[14] = !desc->isHoleEnabled(3);
    m_collision[15] = true;
}

void MapObject_GemCave::_updateCaveParts() {
    MapObjectDesc_GemCave *desc = (MapObjectDesc_GemCave *)m_desc.ptr();
    vn::str8 base = "1002:/map.imgs|gem_cave_";
    for (int i = 0; i < 4; ++i) {
        vn::str8 name = base;
        name += '0' + i;
        int idx = 0;
        if (desc->isHoleEnabled((i + 3) % 4)) {
            idx |= 1;
        }
        if (desc->isHoleEnabled(i)) {
            idx |= 2;
        }
        name += '0' + idx;
        name += ".png";
        vn::PicturePtr pic = vn::PictureManager::instance().createPicture(name.c_str());
        *m_cavePart[i] << pic.ptr();
    }
}

void MapObject_GemCave::_updateCaveGems() {
    char buf[32];
    sprintf(buf, "Gem:%d", static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->getNumGems());
    *m_caveGems << buf;
}


void MapObject_GemCave::setPosition(const vn::vector2i &position) {
    if (m_addedToScene) {
        MapData &md = EditorApp::instance().mapData();
        MapCollision &mc = EditorApp::instance().mapCollision();
        for (int i = 0; i < 16; ++i) {
            if (m_collision[i]) {
                mc.decrementObject(md, m_position + s_collisionIndex[i]);
                mc.incrementObject(position + s_collisionIndex[i]);
            }
        }
    }
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setPosition(position);
}

void MapObject_GemCave::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 16; ++i) {
        if (m_collision[i]) {
            mc.incrementObject(m_position + s_collisionIndex[i]);
        }
    }
    m_addedToScene = true;
}

void MapObject_GemCave::removeNodeFromScene() {
    m_addedToScene = false;
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
    MapData &md = EditorApp::instance().mapData();
    MapCollision &mc = EditorApp::instance().mapCollision();
    for (int i = 0; i < 16; ++i) {
        if (m_collision[i]) {
            mc.decrementObject(md, m_position + s_collisionIndex[i]);
        }
    }
}

void MapObject_GemCave::_onHole0Changed(vn::EditValue *value) {
    bool hole = static_cast<vn::EditValueBool *>(value)->getValue();
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setHoleEnabled(0, hole);
    _updateCaveParts();
    _updateCollision(2, !hole);
}

void MapObject_GemCave::_onHole1Changed(vn::EditValue *value) {
    bool hole = static_cast<vn::EditValueBool *>(value)->getValue();
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setHoleEnabled(1, hole);
    _updateCaveParts();
    _updateCollision(6, !hole);
}

void MapObject_GemCave::_onHole2Changed(vn::EditValue *value) {
    bool hole = static_cast<vn::EditValueBool *>(value)->getValue();
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setHoleEnabled(2, hole);
    _updateCaveParts();
    _updateCollision(10, !hole);
}

void MapObject_GemCave::_onHole3Changed(vn::EditValue *value) {
    bool hole = static_cast<vn::EditValueBool *>(value)->getValue();
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setHoleEnabled(3, hole);
    _updateCaveParts();
    _updateCollision(14, !hole);
}

void MapObject_GemCave::_onNumGemsChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_GemCave *>(m_desc.ptr())->setNumGems((vn::u8)static_cast<vn::EditValueInt *>(value)->getValue());
    _updateCaveGems();
}

void MapObject_GemCave::_updateCollision(size_t index, bool value) {
    m_collision[index] = value;
    if (m_addedToScene) {
        if (value) {
            EditorApp::instance().mapCollision().incrementObject(m_position + s_collisionIndex[index]);
        } else {
            EditorApp::instance().mapCollision().decrementObject(EditorApp::instance().mapData(), m_position + s_collisionIndex[index]);
        }
    }
}