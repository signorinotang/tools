//
//  MapBlockNode.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "MapBlockNode.h"
#include "Renderable.h"
#include "vnPictureManager.h"

MapBlockNode::MapBlockNode(MapBlock *block)
: m_block(block) {
    vnassert(block);
    updateBlock();
    setLocalBoundingBox(vn::aabox2f(0, 0, 40, 40));
    setPosition(block->position * 40);
}

MapBlock * MapBlockNode::block() const {
    return m_block;
}

void MapBlockNode::updateBlock() {
    clearRenderables();
    Quad *p = vnnew Quad();
    *p << vn::aabox2f(0, 0, 40, 40);
    switch (m_block->type) {
        case kMBT_Land: {
            //*p << vn::color_t::kYellow;
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|land_5x5.png");
            if (!pic.null()) {
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / 5;
                int x = m_block->position.x % 5;
                int y = m_block->position.y % 5;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }
            break;
        }
            
        case kMBT_Water: {
            //*p << vn::color_t::kCyan;
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|water_3x3.png");
            if (!pic.null()) {
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / 3;
                int x = m_block->position.x % 3;
                int y = m_block->position.y % 3;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }
            break;
        }
            
        case kMBT_DeepWater: {
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|deep_water_1x1.png");
            if (!pic.null()) {
                const int kCells = 1;
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / kCells;
                int x = m_block->position.x % kCells;
                int y = m_block->position.y % kCells;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }

            break;
        }
            
        case kMBT_Field_Red: {
            //*p << vn::color_t::kRed;
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|red_3x3.png");
            if (!pic.null()) {
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / 3;
                int x = m_block->position.x % 3;
                int y = m_block->position.y % 3;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }
            break;
        }
            
        case kMBT_Field_Green: {
            //*p << vn::color_t::kGreen;
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|green_3x3.png");
            if (!pic.null()) {
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / 3;
                int x = m_block->position.x % 3;
                int y = m_block->position.y % 3;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }
            break;
        }
            
        case kMBT_Field_White: {
            //*p << vn::color_t::kWhite;
            vn::PicturePtr pic = vn::PictureManager::instance().createPicture("1002:/map.imgs|white_3x3.png");
            if (!pic.null()) {
                vn::aabox2f rect(pic->texcoords[0], pic->texcoords[2]);
                vn::vector2f size = rect.size() / 3;
                int x = m_block->position.x % 3;
                int y = m_block->position.y % 3;
                rect.min_corner.x += x * size.x;
                rect.min_corner.y += y * size.y;
                rect.max_corner = rect.min_corner + size;
                pic->buildTexcoord(rect);
                *p << pic.ptr();
            }
            break;
        }
            
        case kMBT_Field_HighGround:
            *p << vn::color_t::kMegenta;
            break;
    }
    addRenderable(p, false);
}
