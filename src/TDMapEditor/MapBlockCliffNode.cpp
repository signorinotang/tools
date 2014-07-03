//
//  MapBlockCliffNode.cpp
//  TDMapEditor
//
//  Created by Wayne on 3/6/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapBlockCliffNode.h"
#include "vnPictureManager.h"

MapBlockCliffNode::MapBlockCliffNode(MapBlockCliff *cliff)
: m_cliff(cliff) {
    setPosition(cliff->position * 40);
    setLocalBoundingBox(vn::aabox2f(-20, 0, 20, 60));
    updateCliff();
}

void MapBlockCliffNode::updateCliff() {
    clearRenderables();
    Quad *quad = vnnew Quad;
    addRenderable(quad, false);
    *quad << getLocalBoundingBox();
    vn::c8 path[256];
    vn::u8 type = m_cliff->type.value & 0xF;
    sprintf(path, "1002:/map.imgs|cliff_%u_%u.png", m_cliff->type.type, type);
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture(path);
    if (!pic.null()) {
        vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
        vn::vector2f size = rect.size() / 3;
        int x = m_cliff->position.x % 3;
        int y = m_cliff->position.y % 3;
        //if (type == 1) {
        //    x = (x + 1) % 3;
        //} else if (type == 2) {
        //    x = (x + 2) % 3;
        //}
        rect.min_corner.x += x * size.x;
        rect.min_corner.y += y * size.y;
        rect.max_corner = rect.min_corner + size;
        pic->buildTexcoord(rect);
    }
    *quad << pic.ptr();
}
