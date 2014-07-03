//
//  MapCollision.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapCollision.h"
#include "EditorApp.h"
#include "BinaryHeap.h"

MapCollision::Item::Item(const vn::vector2i &idx, CollisionType t)
: index(idx)
, type(t)
, node(vnnew MapCollisionItemNode())
, objects(0) {
    node->init(this);
    EditorApp::instance().layer_collision()->addChild(node.ptr());
    
    path.pass = 0;
    path.open = -1;
    path.parent = 0;
    path.f_cost = path.g_cost = path.h_cost = 0;
}

MapCollision::Item::~Item() {
    EditorApp::instance().layer_collision()->removeChild(node.ptr());
}

MapCollision::MapCollision()
: m_pathFindingPass(0) {
    
}

MapCollision::~MapCollision() {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        delete *it;
    }
}

void MapCollision::reset(MapData &mapData) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        delete *it;
    }
    m_items.clear();
    m_size = mapData.size();
    m_items.reserve(m_size.area());
    for (vn::vector2i idx; idx.y < m_size.y; ++idx.y) {
        for (idx.x = 0; idx.x < m_size.x; ++idx.x) {
            m_items.push_back(vnnew Item(idx, _build(mapData, idx)));
        }
    }
}

void MapCollision::update(MapData &mapData, const vn::vector2i &index) {
    if (!(index >= vn::vector2i::kZero && index < m_size)) {
        return ;
    }
    Item *item = _item(index);
    CollisionType type = kCT_Invalid;
    if (!item->objects) {
        type = _build(mapData, index);
    }
    if (type != item->type) {
        item->type = type;
        item->node->itemUpdated();
    }
}

void MapCollision::incrementObject(const vn::vector2i &index) {
    if (!(index >= vn::vector2i::kZero && index < m_size)) {
        return ;
    }
    Item *item = _item(index);
    ++item->objects;
    if (item->type != kCT_Invalid) {
        item->type = kCT_Invalid;
        item->node->itemUpdated();
    }
}

void MapCollision::decrementObject(MapData &mapData, const vn::vector2i &index) {
    if (!(index >= vn::vector2i::kZero && index < m_size)) {
        return ;
    }
    Item *item = _item(index);
    vnassert(item->objects > 0);
    if (--item->objects == 0) {
        item->type = _build(mapData, index);
        item->node->itemUpdated();
    }
}

void MapCollision::clearPath() {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        (*it)->node->set(false);
    }
}

CollisionType MapCollision::itemType(const vn::vector2i &index) {
    if (!(index >= vn::vector2i::kZero && index < m_size)) {
        return kCT_Invalid;
    }
    return _item(index)->type;
}

MapCollision::Item * MapCollision::item(const vn::vector2i &index) {
    if (!(index >= vn::vector2i::kZero && index < m_size)) {
        return 0;
    }
    return _item(index);
}

bool MapCollision::findPath(const vn::vector2i &start, const vn::vector2i &destination, vn::u32 collisionMask, std::vector<const Item *> &result, int &cost) {
    Item *src = item(start);
    Item *dest = item(destination);
    if (!src || !src->type) {
        return false;
    }
    if (!dest || !dest->type) {
        return false;
    }
    return _findPath(src, dest, collisionMask, result, cost);
}

CollisionType MapCollision::_build(MapData &mapData, const vn::vector2i &index) {
    auto passed = [&](const vn::vector2i &idx) -> bool {
        MapBlock * _bs[8] = {
            mapData.block(vn::vector2i(idx.x, idx.y - 1)),
            mapData.block(vn::vector2i(idx.x, idx.y + 1)),
            mapData.block(vn::vector2i(idx.x - 1, idx.y)),
            mapData.block(vn::vector2i(idx.x + 1, idx.y)),
            mapData.block(vn::vector2i(idx.x + 1, idx.y + 1)),
            mapData.block(vn::vector2i(idx.x + 1, idx.y - 1)),
            mapData.block(vn::vector2i(idx.x - 1, idx.y + 1)),
            mapData.block(vn::vector2i(idx.x - 1, idx.y - 1)),
        };
        for (int i = 0; i < 8; ++i) {
            if (_bs[i] && _bs[i]->isField()) {
                return false;
            }
        }
        return true;
    };
    MapBlock *b = mapData.block(index);
    if (b) {
        if (b->type == kMBT_Land) {
            return passed(index) ? kCT_Land : kCT_Invalid;
        }
        if (b->type == kMBT_Water || b->type == kMBT_DeepWater) {
            return passed(index) ? kCT_Water : kCT_Invalid;
        }
    }
    return kCT_Invalid;
}

bool MapCollision::_findPath(Item *src, Item *dest, vn::u32 collisionMask, std::vector<const Item *> &result, int &cost) {
    
    ++m_pathFindingPass;
    BinaryHeap opens;
    Item *block;
    
    // 1. init
    src->path.pass = m_pathFindingPass;
    src->path.parent = 0;
    src->path.g_cost = 0;
    src->path.h_cost = 0;
    src->path.f_cost = src->path.g_cost + src->path.h_cost;
    opens.insert(src);
    
    // 2. finding...
    while ((block = opens.pop())) {
        if (block == dest) {
            // succeed.
            cost = block->path.g_cost;
            result.clear();
            do {
                result.push_back(block);
                block = block->path.parent;
            } while (block);
            return true;
        }
        Item *blocks[8];
        int weights[8];
        size_t cnt = _adjacentItems(block->index, collisionMask, blocks, weights);
        for (size_t i = 0; i < cnt; ++i) {
            Item *p = blocks[i];
            if (p->path.pass != m_pathFindingPass) {
                p->path.pass = m_pathFindingPass;
                p->path.g_cost = block->path.g_cost + weights[i];
                p->path.h_cost = 0;
                p->path.f_cost = p->path.g_cost + p->path.h_cost;
                p->path.parent = block;
                opens.insert(p);
            } else if (p->path.open != -1) {
                int t = block->path.g_cost + weights[i];
                if (t < p->path.g_cost) {
                    p->path.parent = block;
                    p->path.g_cost = t;
                    p->path.f_cost = t + p->path.h_cost;
                    opens.update(p);
                }
            }
        }
    }
    
    return false;
}

size_t MapCollision::_adjacentItems(const vn::vector2i &index, vn::u32 collisionMask, Item *blocks[], int weights[]) {
    size_t cnt = 0;
    Item *dir[4] = {
        item(index + vn::vector2i(0, -1)),
        item(index + vn::vector2i(1, 0)),
        item(index + vn::vector2i(0, 1)),
        item(index + vn::vector2i(-1, 0)),
    };
    for (int i = 0; i < 4; ++i) {
        if (!dir[i]) {
            continue;
        }
        if (dir[i]->type & collisionMask) {
            *blocks++ = dir[i];
            *weights++ = 100;
            ++cnt;
        } else {
            dir[i] = 0;
        }
    }
    Item *cross[4] = {
        item(index + vn::vector2i(1, -1)),
        item(index + vn::vector2i(1, 1)),
        item(index + vn::vector2i(-1, 1)),
        item(index + vn::vector2i(-1, -1)),
    };
    for (int i = 0; i < 4; ++i) {
        if (!cross[i]) {
            continue;
        }
        if (!dir[i] && !dir[(i + 1) % 4]) {
            continue;
        }
        if (cross[i]->type & collisionMask) {
            *blocks++ = cross[i];
            *weights++ = 141;
            ++cnt;
        }
    }
    return cnt;
}


///////////////////////////////////////////////////////////

MapCollisionItemNode::MapCollisionItemNode()
: m_node(vnnew SceneNode())
, m_item(0) {
    addChild(m_node, false);
    setLocalBoundingBox(vn::aabox2f(0, 0, 40, 40));
    m_node->setPosition(vn::vector2f(20, 20));
    m_node->setLocalBoundingBox(vn::aabox2f(-10, -10, 10, 10));
    m_node->setVisible(false);
    Quad *quad = vnnew Quad();
    m_node->addRenderable(quad, false);
    *quad << m_node->getLocalBoundingBox() << vn::color_t::kGreen;
}

void MapCollisionItemNode::init(MapCollision::Item *item) {
    m_item = item;
    setPosition(vn::vector2f(40, 40) * item->index);
    itemUpdated();
}

void MapCollisionItemNode::itemUpdated() {
    clearRenderables();
    if (!m_item->type) {
        Quad *quad = vnnew Quad();
        *quad << getLocalBoundingBox() << vn::color_t(0, 0, 0, 0x80);
        addRenderable(quad, false);
    }
}

void MapCollisionItemNode::set(bool value) {
    m_node->setVisible(value);
}