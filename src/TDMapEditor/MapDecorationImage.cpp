//
//  MapDecorationImage.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/1/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapDecorationImage.h"
#include "vnPictureManager.h"

MapDecorationImage::MapDecorationImage()
: m_quad(vnnew Quad()) {
    m_node->addRenderable(m_quad.ptr());
}

MapDecoration::Type MapDecorationImage::type() const {
    return kImage;
}

void MapDecorationImage::setDesc(const MapDecorationDescPtr &desc, bool update) {
    MapDecoration::setDesc(desc, update);
    if (update && !desc.null()) {
        setImagePath(desc->file.getFullPath());
    }
}

void MapDecorationImage::setImagePath(const vn::str8 &imagePath) {
    vn::FilePath path(imagePath.c_str());
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture(imagePath.c_str());
    *m_quad << pic.ptr();
    if (!m_desc.null()) {
        m_desc->file = path;
    }
}

void MapDecorationImage::_onExtentUpdate() {
    vn::vector2f half = m_extent * 0.5f;
    *m_quad << vn::aabox2f(-half, half);
}

void MapDecorationImage::_onColorUpdate() {
    *m_quad << m_color;
}

void MapDecorationImage::_onLightModeUpdate() {
    *m_quad << (m_lightMode ? vn::Render2DInterface::kAlphaAdditive : vn::Render2DInterface::kAlphaBlend);
}

