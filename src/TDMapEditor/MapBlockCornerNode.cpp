//
//  MapBlockCornerNode.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-31.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "MapBlockCornerNode.h"
#include "vnPictureManager.h"

MapBlockCornerNode::MapBlockCornerNode(MapBlockCorner *corner)
: m_corner(corner) {
    vnassert(corner);
    updateCorner();
    setPosition(corner->position * 40);
}

void MapBlockCornerNode::updateCorner() {
    clearRenderables();
    vn::c8 path[256];
    sprintf(path, "1002:/map.imgs|corner_%u_%u.png", m_corner->type.type , m_corner->type.value & 0xF);
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture(path);
    Quad *quad = vnnew Quad();
    addRenderable(quad, false);
    
    if (m_corner->type.type == 4) {
        *quad << pic.ptr();
        switch (m_corner->type.value & 0xF) {
            // 1 1
            // 0 0
            case 3:
                setLocalBoundingBox(vn::aabox2f(-20, 0, 20, 20));
                break;
                
            // 0 1
            // 0 1
            case 6:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 0, 20));
                break;
                
            // 0 0
            // 1 1
            case 12:
                
                setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 0));
                break;
                
            // 1 0
            // 1 0
            case 9:
                setLocalBoundingBox(vn::aabox2f(0, -20, 20, 20));
                break;
                
            // 0 0
            // 0 1
            case 4:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 20));
                break;
                
            // 1 0
            // 0 0
            case 1:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 20));
                break;
                
            // 0 1
            // 0 0
            case 2:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 20));
                break;
                
            // 0 0
            // 1 0
            case 8:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 20));
                break;
                
            // 0 1
            // 1 1
            case 14:
                setLocalBoundingBox(vn::aabox2f(-20, -20, 0, 0));
                break;
                
            // 1 0
            // 1 1
            case 13:
                setLocalBoundingBox(vn::aabox2f(0, -20, 20, 0));
                break;
                
            // 1 1
            // 1 0
            case 11:
                setLocalBoundingBox(vn::aabox2f(0, 0, 20, 20));
                break;
                
            // 1 1
            // 0 1
            case 7:
                setLocalBoundingBox(vn::aabox2f(-20, 0, 0, 20));
                break;
                
        }
        *quad << getLocalBoundingBox();
        return ;
    }
    
    
    setLocalBoundingBox(vn::aabox2f(-20, -20, 20, 20));
    *quad << getLocalBoundingBox();
    if (!pic.null()) {
        if (pic->size > vn::vector2f(40, 40)) {
            vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
            vn::vector2f size = rect.size() / 3;
            int x = m_corner->position.x % 3;
            int y = m_corner->position.y % 3;
            rect.min_corner.x += x * size.x;
            rect.min_corner.y += y * size.y;
            rect.max_corner = rect.min_corner + size;
            pic->buildTexcoord(rect);
        }
        *quad << pic.ptr();
    }
}

