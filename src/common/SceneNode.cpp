//
//  SceneNode.cpp
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "SceneNode.h"

SceneNode::SceneNode()
: m_parent(0)
, m_orientation(1, 0)
, m_rotation(0)
, m_scale(1, 1)
, m_visible(true)
, m_dirty(false)
, m_transformM(vn::matrix2::c_identity)
, m_inheritTransformM(true)
, m_inheritTransformT(true) {
    
}

SceneNode::~SceneNode() {
    clearRenderables();
	clearChildren();
}

void SceneNode::update(vn::f32 deltaTime) {
    cleanDirty();
	for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->update(deltaTime);
	}
}

void SceneNode::setPosition(const vn::vector2f &pos) {
	m_position = pos;
	m_dirty = true;
}

const vn::vector2f & SceneNode::getPosition() const {
	return m_position;
}

void SceneNode::setRotation(vn::real radians) {
	m_rotation = radians;
	vn::math::sincos(radians, m_orientation.y, m_orientation.x);
	m_dirty = true;
}

vn::real SceneNode::getRotation() const {
	return m_rotation;
}

void SceneNode::setScale(const vn::vector2f &scale) {
	m_scale = scale;
	m_dirty = true;
}

const vn::vector2f & SceneNode::getScale() const {
	return m_scale;
}

void SceneNode::setVisible(bool visible) {
	m_visible = visible;
}

bool SceneNode::isVisible() const {
	return m_visible;
}

void SceneNode::setLocalBoundingBox(const vn::aabox2f &rect) {
	m_localBoundingBox = rect;
	if (m_dirty) {
		m_dirty = false;
		_updateTransform();
	}
	_updateWorldBoundingBox();
}

const vn::aabox2f & SceneNode::getLocalBoundingBox() const {
	return m_localBoundingBox;
}

void SceneNode::setInheritTransformM(bool inherit) {
	m_inheritTransformM = inherit;
	m_dirty = true;
}

bool SceneNode::isInheritTransformM() const {
	return m_inheritTransformM;
}

void SceneNode::setInheritTransformT(bool inherit) {
	m_inheritTransformT = inherit;
	m_dirty = true;
}

bool SceneNode::isInheritTransformT() const {
	return m_inheritTransformT;
}

void SceneNode::addRenderable(Renderable *renderable, bool grab) {
    vnassert(renderable);
    if (grab) {
        renderable->grab();
    }
	if (renderable->m_sceneNode) {
		renderable->m_sceneNode->removeRenderable(renderable);
	}
	renderable->m_sceneNode = this;
    m_renderables.push_back(renderable);
	renderable->onTransformUpdated();
}

bool SceneNode::removeRenderable(Renderable *renderable) {
    for (Renderables::iterator it = m_renderables.begin(); it != m_renderables.end(); it++) {
        if (*it == renderable) {
            m_renderables.erase(it);
			renderable->m_sceneNode = 0;
			renderable->onTransformUpdated();
			renderable->drop();
            return true;
        }
    }
    return false;
}

void SceneNode::clearRenderables() {
    for (Renderables::iterator it = m_renderables.begin(); it != m_renderables.end(); it++) {
        (*it)->drop();
    }
    m_renderables.clear();
}

SceneNode * SceneNode::parent() const {
	return m_parent;
}

void SceneNode::addChild(SceneNode *child, bool grab) {
	vnassert(child);
	if (grab) {
		child->grab();
	}
	if (child->m_parent) {
		child->m_parent->removeChild(child);
	}
	child->m_parent = this;
	child->m_dirty = true;
	m_children.push_back(child);
}

void SceneNode::removeChild(SceneNode *child) {
	vnassert(child && child->m_parent == this);
	for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		if (*it == child) {
			m_children.erase(it);
			child->m_dirty = true;
			child->drop();
			return ;
		}
	}
}

void SceneNode::bringChildUp(SceneNode *child) {
    for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            Nodes::iterator next = it;
            if (++next != m_children.end()) {
                vn::math::swap(*it, *next);
            }
            return ;
        }
    }
}

void SceneNode::bringChildDown(SceneNode *child) {
    for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            if (it != m_children.begin()) {
                Nodes::iterator prev = it;
                --prev;
                vn::math::swap(*it, *prev);
            }
            return ;
        }
    }
}

void SceneNode::clearChildren() {
	for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->drop();
	}
	m_children.clear();
}

const vn::vector2f & SceneNode::oritentation() const {
	return m_orientation;
}

const vn::matrix2 & SceneNode::transformM() const {
	return m_transformM;
}

const vn::vector2f & SceneNode::transformT() const {
	return m_transformT;
}

const vn::aabox2f & SceneNode::worldBoundingBox() const {
	return m_worldBoundingBox;
}

vn::vector2f SceneNode::convertToWorld(const vn::vector2f &point) {
	return point * m_transformM + m_transformT;
}

vn::vector2f SceneNode::convertFromWorld(const vn::vector2f &point) {
	return (point - m_transformT) * vn::matrix2(m_transformM, vn::matrix2::c_inverse);
}

void SceneNode::markDirty() {
	m_dirty = true;
}

void SceneNode::cleanDirty() {
	if (!m_dirty) {
		return ;
	}
	m_dirty = false;
	_updateTransform();
	_updateWorldBoundingBox();
	for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->markDirty();
	}
}

void SceneNode::queryVisibleNodes(const vn::aabox2f &view, std::vector<SceneNode *> &nodes) {
    if (!m_visible) {
        return ;
    }
	if (view.intersects(m_worldBoundingBox)) {
		nodes.push_back(this);
	}
	queryVisibleChildren(view, nodes);
}

void SceneNode::queryVisibleChildren(const vn::aabox2f &view, std::vector<SceneNode *> &nodes) {
    for (Nodes::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->queryVisibleNodes(view, nodes);
	}
}

void SceneNode::updateRenderables() {
	for (Renderables::iterator it = m_renderables.begin(); it != m_renderables.end(); it++) {
        (*it)->update();
    }

}

void SceneNode::renderRenderables() {
	for (Renderables::iterator it = m_renderables.begin(); it != m_renderables.end(); it++) {
        (*it)->render();
    }
}

void SceneNode::_updateTransform() {
	m_transformM.m11 = m_orientation.x * m_scale.x;
	m_transformM.m12 = m_orientation.y * m_scale.x;
	m_transformM.m21 = -m_orientation.y * m_scale.y;
	m_transformM.m22 = m_orientation.x * m_scale.y;
	m_transformT = m_position;
	if (m_parent) {
		if (m_inheritTransformM) {
			m_transformM *= m_parent->m_transformM;
			m_transformT *= m_parent->m_transformM;
		}
		if (m_inheritTransformT) {
			m_transformT += m_parent->m_transformT;
		}
	}
	for (Renderables::iterator it = m_renderables.begin(); it != m_renderables.end(); it++) {
        (*it)->onTransformUpdated();
    }
}

void SceneNode::_updateWorldBoundingBox() {
	m_worldBoundingBox.reset(convertToWorld(m_localBoundingBox.corner_0()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_1()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_2()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_3()));
}

