//
//  MapObject_Tree.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_Tree.h"
#include "vnEditTypeFactory.h"
#include "vnPictureManager.h"
#include "vnFontManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(tree_type, "MapObject_Tree_Type")
VN_DEFINE_ET_ENUMERATION_ITEM("Red_0", MapObjectDesc_Tree::kType_Red)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_0", MapObjectDesc_Tree::kType_Green)
VN_DEFINE_ET_ENUMERATION_ITEM("White_0", MapObjectDesc_Tree::kType_White)
VN_DEFINE_ET_ENUMERATION_ITEM("Red_1", MapObjectDesc_Tree::kType_Red_0)
VN_DEFINE_ET_ENUMERATION_ITEM("Red_2", MapObjectDesc_Tree::kType_Red_1)
VN_DEFINE_ET_ENUMERATION_ITEM("Red_3", MapObjectDesc_Tree::kType_Red_2)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_1", MapObjectDesc_Tree::kType_Green_0)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_2", MapObjectDesc_Tree::kType_Green_1)
VN_DEFINE_ET_ENUMERATION_ITEM("Green_3", MapObjectDesc_Tree::kType_Green_2)
VN_DEFINE_ET_ENUMERATION_ITEM("White_1", MapObjectDesc_Tree::kType_White_0)
VN_DEFINE_ET_ENUMERATION_ITEM("White_2", MapObjectDesc_Tree::kType_White_1)
VN_DEFINE_ET_ENUMERATION_ITEM("White_3", MapObjectDesc_Tree::kType_White_2)
VN_DEFINE_ET_ENUMERATION_END(tree_type)

VN_DEFINE_ET_ENUMERATION_BEGIN(object_quality, "MapObject_Quality")
VN_DEFINE_ET_ENUMERATION_ITEM("1", 1)
VN_DEFINE_ET_ENUMERATION_ITEM("2", 2)
VN_DEFINE_ET_ENUMERATION_ITEM("3", 3)
VN_DEFINE_ET_ENUMERATION_ITEM("4", 4)
VN_DEFINE_ET_ENUMERATION_ITEM("5", 5)
VN_DEFINE_ET_ENUMERATION_ITEM("6", 6)
VN_DEFINE_ET_ENUMERATION_ITEM("7", 7)
VN_DEFINE_ET_ENUMERATION_ITEM("8", 8)
VN_DEFINE_ET_ENUMERATION_ITEM("9", 9)
VN_DEFINE_ET_ENUMERATION_END(object_quality)

VN_DEFINE_ET_OBJECT_BEGIN(tree, "MapObject_Tree")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Type", false, "MapObject_Tree_Type")
VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Quality", false, "MapObject_Quality")
VN_DEFINE_ET_OBJECT_ATTRIB_INT("Gold", false)
VN_DEFINE_ET_OBJECT_END(tree)

IMPL_MAP_OBJECT(MapObject_Tree, MapObjectDesc_Tree)

MapObject_Tree::MapObject_Tree(MapObjectDesc_Tree *desc)
: MapObject(desc) {
    m_position = desc->getPosition();
	m_size.set(2, 2);
	m_node->setPosition(desc->getPosition() * 40);
    
    vn::aabox2f rect(0, -20, 80, 80);
	m_node->setLocalBoundingBox(rect);
    m_quad = vnnew Quad();
    m_node->addRenderable(m_quad, false);
    *m_quad << rect;
    
    m_gold = vnnew Text();
    m_node->addRenderable(m_gold, false);
    *m_gold << vn::color_t::kRed << vn::FontManager::instance().defaultFont() << vn::RenderText::kCenter << rect;
    
	m_object = vn::EditTypeFactory::instance().etObject("MapObject_Tree")->generate();
    static_cast<vn::EditValueInt *>(m_object->attribute(0))->setValue(desc->getType());
    m_object->attribute(0)->addObserver(&m_opType);
	static_cast<vn::EditValueInt *>(m_object->attribute(1))->setValue(desc->getQuality());
    m_object->attribute(1)->addObserver(&m_opQuality);
    
    static_cast<vn::EditValueInt *>(m_object->attribute(2))->setValue(desc->getGold());
    m_object->attribute(2)->addObserver(&m_opGold);
    
    m_opType.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree::_onTypeChanged));
    m_opQuality.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree::_onQualityChanged));
    
    m_opGold.setFunction_Changed(vn::make_memfn(this, &MapObject_Tree::_onGoldChanged));
    
    _updateQuad(desc->getType());
    if (desc->getGold()) {
        std::ostringstream oss;
        oss << desc->getGold();
        *m_gold << oss.str();
    }
}

void MapObject_Tree::setPosition(const vn::vector2i &position) {
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_Tree *>(m_desc.ptr())->setPosition(position);
}

void MapObject_Tree::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
}

void MapObject_Tree::removeNodeFromScene() {
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
}

void MapObject_Tree::_onTypeChanged(vn::EditValue *value) {
    vn::u8 type = (vn::u8)static_cast<vn::EditValueInt *>(value)->getValue();
    static_cast<MapObjectDesc_Tree *>(m_desc.ptr())->setType(type);
    _updateQuad(type);
}

void MapObject_Tree::_onQualityChanged(vn::EditValue *value) {
    static_cast<MapObjectDesc_Tree *>(m_desc.ptr())->setQuality((vn::u8)static_cast<vn::EditValueInt *>(value)->getValue());
}

void MapObject_Tree::_onGoldChanged(vn::EditValue *value) {
    vn::u32 gold = (vn::u32)static_cast<vn::EditValueInt *>(value)->getValue();
    static_cast<MapObjectDesc_Tree *>(m_desc.ptr())->setGold(gold);
    std::ostringstream oss;
    if (gold) {
        oss << gold;
    }
    *m_gold << oss.str();
}

void MapObject_Tree::_updateQuad(vn::u8 type) {
    switch (type) {
        case MapObjectDesc_Tree::kType_Red: {
            _updateQuad(MapObjectDesc_Tree::kType_Red_0 + rand() % 3);
            break;
        }
            
        case MapObjectDesc_Tree::kType_Green: {
            _updateQuad(MapObjectDesc_Tree::kType_Green_0 + rand() % 3);
            break;
        }
            
        case MapObjectDesc_Tree::kType_White: {
            _updateQuad(MapObjectDesc_Tree::kType_White_0 + rand() % 3);
            break;
        }
            
        case MapObjectDesc_Tree::kType_Red_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree201.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_Red_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree202.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_Red_2: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree203.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_Green_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree211.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_Green_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree212.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_Green_2: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree213.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_White_0: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree221.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_White_1: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree222.png");
            *m_quad << pic.ptr();
            break;
        }
            
        case MapObjectDesc_Tree::kType_White_2: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|tree223.png");
            *m_quad << pic.ptr();
            break;
        }
    }
}