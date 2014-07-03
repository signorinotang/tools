//
//  MapObject_Tree1x1.cpp
//  TDMapEditor
//
//  Created by Wayne on 5/9/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_Tree1x1.h"
#include "vnEditTypeFactory.h"
#include "vnPictureManager.h"
#include "vnFontManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(tree1x1_type, "MapObject_Tree1x1_Type")
VN_DEFINE_ET_ENUMERATION_ITEM("Red_0", MapObjectDesc_Tree1x1::kType_Red)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_0", MapObjectDesc_Tree1x1::kType_Green)
VN_DEFINE_ET_ENUMERATION_ITEM("White_0", MapObjectDesc_Tree1x1::kType_White)
VN_DEFINE_ET_ENUMERATION_ITEM("Red_1", MapObjectDesc_Tree1x1::kType_Red_0)
VN_DEFINE_ET_ENUMERATION_ITEM("Red_2", MapObjectDesc_Tree1x1::kType_Red_1)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_1", MapObjectDesc_Tree1x1::kType_Green_0)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_2", MapObjectDesc_Tree1x1::kType_Green_1)
VN_DEFINE_ET_ENUMERATION_ITEM("White_1", MapObjectDesc_Tree1x1::kType_White_0)
VN_DEFINE_ET_ENUMERATION_ITEM("White_2", MapObjectDesc_Tree1x1::kType_White_1)
VN_DEFINE_ET_ENUMERATION_END(tree1x1_type)

VN_DEFINE_ET_ENUMERATION_BEGIN(tree1x1_quality, "MapObject_Tree1x1_Quality")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(tree1x1_quality)

VN_DEFINE_ET_OBJECT_BEGIN(tree1x1, "MapObject_Tree1x1")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Type", false, "MapObject_Tree1x1_Type")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Quality", false, "MapObject_Tree1x1_Quality")
VN_DEFINE_ET_OBJECT_ATTRIB_INT("Gold", false)
VN_DEFINE_ET_OBJECT_END(tree1x1)

IMPL_MAP_OBJECT(MapObject_Tree1x1, MapObjectDesc_Tree1x1)

MapObject_Tree1x1::MapObject_Tree1x1(MapObjectDesc_Tree1x1 *desc)
: MapObject(desc) {
    m_position = desc->getPosition();
	m_size.set(1, 1);
	m_node->setPosition(desc->getPosition() * 40);
    
    vn::aabox2f rect(0, 0, 40, 40);
	m_node->setLocalBoundingBox(rect);
    m_quad = vnnew Quad();
    m_node->addRenderable(m_quad, false);
    *m_quad << rect;
    
    m_gold = vnnew Text();
    m_node->addRenderable(m_gold, false);
    *m_gold << vn::color_t::kRed << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << rect;
    
	m_object = vn::EditTypeFactory::instance().etObject("MapObject_Tree1x1")->generate();
    static_cast<vn::EditValueInt *>(m_object->attribute(0))->setValue(desc->getType());
    m_object->attribute(0)->addObserver(&m_opType);
	static_cast<vn::EditValueInt *>(m_object->attribute(1))->setValue(desc->getQuality());
    m_object->attribute(1)->addObserver(&m_opQuality);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(2))->setValue(desc->getGold());
    m_object->attribute(2)->addObserver(&m_opGold);
    
    m_opType.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree1x1::_onTypeChanged));
    m_opQuality.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree1x1::_onQualityChanged));
    
    m_opGold.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree1x1::_onGoldChanged));
    
    _updateQuad(desc->getType());
    if (desc->getGold()) {
        std::ostringstream oss;
        oss << desc->getGold();
        *m_gold << oss.str();
    }
}

void MapObject_Tree1x1::setPosition(const vn::vector2i &position) {
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_Tree1x1 *>(m_desc.ptr())->setPosition(position);
}

void MapObject_Tree1x1::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
}

void MapObject_Tree1x1::removeNodeFromScene() {
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
}

void MapObject_Tree1x1::_onTypeChanged(vn::EditValue *value) {
    vn::u8 type = (vn::u8)static_cast<vn::EditValueInt *>(value)->getValue();
    static_cast<MapObjectDesc_Tree1x1 *>(m_desc.ptr())->setType(type);
    _updateQuad(type);
}

void MapObject_Tree1x1::_onQualityChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Tree1x1 *>(m_desc.ptr())->setQuality((vn::u8)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Tree1x1::_onGoldChanged(vn::EditValue *value) {
    vn::u32 gold = (vn::u32)static_cast<vn::EditValueInt *>(value)->getValue();
    static_cast<MapObjectDesc_Tree1x1 *>(m_desc.ptr())->setGold(gold);
    std::ostringstream oss;
    if (gold) {
        oss << gold;
    }
    *m_gold << oss.str();
}

void MapObject_Tree1x1::_updateQuad(vn::u8 type) {
    switch (type) {
        case MapObjectDesc_Tree1x1::kType_Red: {
            _updateQuad(MapObjectDesc_Tree1x1::kType_Red_0 + rand() % 2);
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_Green: {
            _updateQuad(MapObjectDesc_Tree1x1::kType_Green_0 + rand() % 2);
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_White: {
            _updateQuad(MapObjectDesc_Tree1x1::kType_White_0 + rand() % 2);
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_Red_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree101.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_Red_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree102.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_Green_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree111.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_Green_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree112.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_White_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree121.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree1x1::kType_White_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree122.png");
            *m_quad << pic.ptr();
            break;
        }
    }
}