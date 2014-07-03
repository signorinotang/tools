//
//  MapObject.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapObject.h"

MapObject::MapObject(MapObjectDesc *desc)
: m_desc(desc)
, m_node(vnnew SceneNode()) {
    desc->grab();
}

MapObject * MapObject::clone() const {
    return vnnew MapObject(m_desc->clone());
}

vn::EditValueObject * MapObject::object() const {
    return m_object.ptr();
}

MapObjectDesc * MapObject::desc() const {
    return m_desc.ptr();
}

SceneNode * MapObject::node() const {
    return m_node.ptr();
}

void MapObject::setPosition(const vn::vector2i &position) {
    m_position = position;
    m_node->setPosition(position * 40);
}

const vn::vector2i & MapObject::getPosition() const {
    return m_position;
}

const vn::vector2i & MapObject::size() const {
    return m_size;
}

void MapObject::addNodeToScene() {
    
}

void MapObject::removeNodeFromScene() {
    
}


/////////////////////////////////////////////////////////////


MapObjectSpace::MapObjectSpace()
: m_space(0) {
    
}

MapObjectSpace::~MapObjectSpace() {
    if (m_space) {
        vnfree(m_space);
        m_space = 0;
    }
}

void MapObjectSpace::reset(const vn::vector2i &size) {
    vnassert(size.x > 0 && size.y > 0);
    if (m_space) {
        vnfree(m_space);
    }
	m_size = size;
    size_t count = m_size.area();
    m_space = vnmalloc(MapObject *, count);
    memset(m_space, 0, count * sizeof(MapObject *));
}

bool MapObjectSpace::isEmpty(const vn::vector2i &position, const vn::vector2i &size) const {
    vn::vector2i pt = position + size;
    if (position.x < 0 || position.y < 0 || pt.x > m_size.x || pt.y > m_size.y) {
        return false;
    }
    for (int y = position.y; y < pt.y; ++y) {
        for (int x = position.x; x < pt.x; ++x) {
            if (m_space[y * m_size.x + x]) {
                return false;
            }
        }
    }
    return true;
}

void MapObjectSpace::place(const vn::vector2i &position, const vn::vector2i &size, MapObject *obj) {
    vn::aabox2i box(position, position + size);
    if (box.min_corner.x < 0) {
        box.min_corner.x = 0;
    }
    if (box.min_corner.y < 0) {
        box.min_corner.y = 0;
    }
    if (box.max_corner.x > m_size.x) {
        box.max_corner.x = m_size.x;
    }
    if (box.max_corner.y > m_size.y) {
        box.max_corner.y = m_size.y;
    }
    for (int y = box.min_corner.y; y < box.max_corner.y; ++y) {
        for (int x = box.min_corner.x; x < box.max_corner.x; ++x) {
            m_space[y * m_size.x + x] = obj;
        }
    }
}

MapObject * MapObjectSpace::pick(const vn::vector2i &position) const {
    if (position.x < 0 || position.y < 0 || position.x >= m_size.x || position.y >= m_size.y) {
        return 0;
    }
    return m_space[position.y * m_size.x + position.x];
}

//////////////////////////////////////////////////////////////////

MapObject * MapObjectFactory::create(MapObjectDesc *desc) {
    Factory::iterator it = m_factory.find(desc->thisName());
    if (it == m_factory.end()) {
        return 0;
    }
    return it->second(desc);
}

MapObject * MapObjectFactory::create(const vn::str8 &name) {
    vn::RefCountedPtr<MapObjectDesc> desc = MapObjectDescFactory::instance().createObjectDesc(name);
    if (desc.null()) {
        return 0;
    }
    Factory::iterator it = m_factory.find(desc->thisName());
    if (it == m_factory.end()) {
        return 0;
    }
    return it->second(desc.ptr());
}