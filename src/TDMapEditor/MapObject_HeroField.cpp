//
//  MapObject_HeroField.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/25/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject_HeroField.h"
#include "vnEditTypeFactory.h"
#include "vnPictureManager.h"
#include "EditorApp.h"

VN_DEFINE_ET_OBJECT_BEGIN(heroField, MapObject_HeroField::kObjectTypeName)
VN_DEFINE_ET_OBJECT_END(heroField)

IMPL_MAP_OBJECT(MapObject_HeroField, MapObjectDesc_HeroField)


MapObject_HeroField::MapObject_HeroField(MapObjectDesc_HeroField *desc)
: MapObject(desc) {
    m_position = desc->getPosition();
	m_size.set(2, 2);
	m_node->setPosition(desc->getPosition() * 40);
    
    vn::aabox2f rect(-19, -10, 98, 80);
	m_node->setLocalBoundingBox(rect);
    Quad *quad = vnnew Quad();
    m_node->addRenderable(quad, false);
    *quad << rect;
    
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|hero_field.png");
    *quad << pic.ptr();
    
	m_object = vn::EditTypeFactory::instance().etObject(kObjectTypeName)->generate();

}

void MapObject_HeroField::setPosition(const vn::vector2i &position) {
    MapObject::setPosition(position);
    static_cast<MapObjectDesc_HeroField *>(m_desc.ptr())->setPosition(position);
}

void MapObject_HeroField::addNodeToScene() {
    EditorApp::instance().layer_object_field()->addChild(m_node.ptr());
}

void MapObject_HeroField::removeNodeFromScene() {
    EditorApp::instance().layer_object_field()->removeChild(m_node.ptr());
}
