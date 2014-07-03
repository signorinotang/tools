//
//  Camera.cpp
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "Camera.h"

Camera::Camera()
: m_viewMatrix(vn::matrix3::c_identity)
, m_zoom(1)
, m_dirty(false) {
    
}

void Camera::update() {
    if (m_dirty) {
        m_dirty = false;
        _updateViewMatrix();
        m_boundingBox.min_corner = convertFromView(vn::vector2f::kZero);
        m_boundingBox.max_corner = convertFromView(m_viewSize);
    }
}

void Camera::move(const vn::vector2f &delta) {
    m_position += delta / m_zoom;
    //m_position = vn::vector2i(m_position);
    m_dirty = true;
}

void Camera::setPosition(const vn::vector2f &pos) {
    m_position = vn::vector2i(pos);
    m_dirty = true;
}

void Camera::setZoom(vn::f32 zoom) {
    vnassert(zoom);
    m_zoom = zoom;
    m_dirty = true;
}

vn::f32 Camera::getZoom() const {
    return m_zoom;
}

void Camera::setViewFocus(const vn::vector2f &offset) {
    m_viewFocus = offset;
    m_dirty = true;
}

void Camera::setViewSize(const vn::vector2f &size) {
    m_viewSize = size;
    if (m_dirty) {
        m_dirty = false;
        _updateViewMatrix();
    }
    m_boundingBox.min_corner = convertFromView(vn::vector2f::kZero);
    m_boundingBox.max_corner = convertFromView(size);
}

const vn::matrix3 & Camera::viewMatrix() const {
    return m_viewMatrix;
}

const vn::aabox2f & Camera::boundingBox() const {
    return m_boundingBox;
}

vn::vector2f Camera::convertToView(const vn::vector2f &point) {
    return point * m_zoom + vn::vector2f(m_viewMatrix.m31, m_viewMatrix.m32);
}

vn::vector2f Camera::convertFromView(const vn::vector2f &point) {
    return (point - vn::vector2f(m_viewMatrix.m31, m_viewMatrix.m32)) / m_zoom;
}

void Camera::_updateViewMatrix() {
    m_viewMatrix.m11 = m_zoom;
    m_viewMatrix.m22 = m_zoom;
    vn::vector2f t = m_position + m_viewFocus;
    t *= m_zoom;
    t -= m_viewFocus;
    m_viewMatrix.m31 = -(float)(int)t.x;
    m_viewMatrix.m32 = -(float)(int)t.y;
}