//
//  vnUIWindow.cpp
//  vngfx
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIWindow.h"
#include "vnUIRoot.h"
#include "vnGfxApplication.h"

_vn_begin

UIWindow::UIWindow()
: m_borderLeft(8)
, m_borderTop(8)
, m_borderRight(8)
, m_borderBottom(8)
, m_minSize(16, 16)
, m_sizeable(true)
, m_mouseDown(0) {
    
}

UIWindow * UIWindow::clone() const {
    return vnnew UIWindow(*this);
}

void UIWindow::setBorderSize(BorderIndex index, f32 size) {
    switch (index) {
        case kBorderLeft:
            m_borderLeft = size;
            break;
            
        case kBorderTop:
            m_borderTop = size;
            break;
            
        case kBorderRight:
            m_borderRight = size;
            break;
            
        case kBorderBottom:
            m_borderBottom = size;
            break;
    }
}

f32 UIWindow::getBorderSize(BorderIndex index) const {
    switch (index) {
        case kBorderLeft:
            return m_borderLeft;
        case kBorderTop:
            return m_borderTop;
        case kBorderRight:
            return m_borderRight;
        case kBorderBottom:
            return m_borderBottom;
    }
    return 0;
}

void UIWindow::setMinSize(const vector2f &size) {
    m_minSize = size;
}

const vector2f & UIWindow::getMinSize() const {
    return m_minSize;
}

void UIWindow::setSizeable(bool sizeable) {
    m_sizeable = sizeable;
}

bool UIWindow::isSizeable() const {
    return m_sizeable;
}

void UIWindow::init(const TreeDataObject *object) {
    UIElement::init(object);
    m_borderLeft = object->queryAttributeFloat("border_left", m_borderLeft);
    m_borderTop = object->queryAttributeFloat("border_top", m_borderTop);
    m_borderRight = object->queryAttributeFloat("border_right", m_borderRight);
    m_borderBottom = object->queryAttributeFloat("border_bottom", m_borderBottom);
    m_sizeable = object->queryAttributeBoolean("sizeable", m_sizeable);
    m_minSize.x = object->queryAttributeFloat("min_width", m_minSize.x);
    m_minSize.y = object->queryAttributeFloat("min_height", m_minSize.y);
    f32 bw = m_borderLeft + m_borderRight;
    f32 bh = m_borderTop + m_borderBottom;
    if (m_minSize.x < bw) {
        m_minSize.x = bw;
    }
    if (m_minSize.y < bh) {
        m_minSize.y = bh;
    }
}

void UIWindow::onMouseLeftDown(const vector2f &position) {
    UIRoot::instance().captureMouse(this);
    if (m_sizeable) {
        if (position.x < m_boundingBox.min_corner.x + m_borderLeft) {
            m_mouseDown = kBorderLeft;
            if (position.y < m_boundingBox.min_corner.y + m_borderTop) {
                m_mouseDown |= kBorderTop;
            } else if (position.y >= m_boundingBox.max_corner.y - m_borderBottom) {
                m_mouseDown |= kBorderBottom;
            }
        } else if (position.x >= m_boundingBox.max_corner.x - m_borderRight) {
            m_mouseDown = kBorderRight;
            if (position.y < m_boundingBox.min_corner.y + m_borderTop) {
                m_mouseDown |= kBorderTop;
            } else if (position.y >= m_boundingBox.max_corner.y - m_borderBottom) {
                m_mouseDown |= kBorderBottom;
            }
        } else {
            if (position.y < m_boundingBox.min_corner.y + m_borderTop) {
                m_mouseDown |= kBorderTop;
            } else if (position.y >= m_boundingBox.max_corner.y - m_borderBottom) {
                m_mouseDown |= kBorderBottom;
            } else {
                m_mouseDown = 15;
            }
        }
    } else {
        m_mouseDown = 15;
    }
    m_lastMousePos = position;
}

void UIWindow::onMouseLeftUp(const vector2f &position) {
    if (m_mouseDown) {
        m_mouseDown = 0;
        UIRoot::instance().releaseMouse(this);
    }
}

void UIWindow::onMouseCaptureCancelled() {
    m_mouseDown = 0;
}


void UIWindow::_onUpdate(f32 deltaTime) {
    if (!m_mouseDown) {
        return ;
    }
    aabox2f rect = m_boundingBox;
    vector2f pos = GfxApplication::instance().getMousePosition();
    vector2f delta = pos - m_lastMousePos;
    if (!delta.x && !delta.y) {
        return ;
    }
    m_lastMousePos = pos;
    if (m_sizeable) {
        switch (m_mouseDown & (kBorderLeft + kBorderRight)) {
            case kBorderLeft + kBorderRight:
                rect.min_corner.x += delta.x;
                rect.max_corner.x += delta.x;
                break;
                
            case kBorderLeft:
                rect.min_corner.x += delta.x;
                delta.x = rect.max_corner.x - rect.min_corner.x - m_minSize.x;
                if (delta.x < 0) {
                    rect.min_corner.x = rect.max_corner.x - m_minSize.x;
                    m_lastMousePos.x += delta.x;
                }
                break;
            
            case kBorderRight:
                rect.max_corner.x += delta.x;
                delta.x = rect.max_corner.x - rect.min_corner.x - m_minSize.x;
                if (delta.x < 0) {
                    rect.max_corner.x = rect.min_corner.x + m_minSize.x;
                    m_lastMousePos.x -= delta.x;
                }
                break;
        }
        switch (m_mouseDown & (kBorderTop + kBorderBottom)) {
            case kBorderTop + kBorderBottom:
                rect.min_corner.y += delta.y;
                rect.max_corner.y += delta.y;
                break;
                
            case kBorderTop:
                rect.min_corner.y += delta.y;
                delta.y = rect.max_corner.y - rect.min_corner.y - m_minSize.y;
                if (delta.y < 0) {
                    rect.min_corner.y = rect.max_corner.y - m_minSize.y;
                    m_lastMousePos.y += delta.y;
                }
                break;
                
            case kBorderBottom:
                rect.max_corner.y += delta.y;
                delta.y =rect.max_corner.y - rect.min_corner.y - m_minSize.y;
                if (delta.y < 0) {
                    rect.max_corner.y = rect.min_corner.y + m_minSize.y;
                    m_lastMousePos.y -= delta.y;
                }
                break;
        }
        if (rect != m_boundingBox) {
            setLocation(rect);
        }
    } else {
        rect += delta;
        setLocation(rect);
    }
}


_vn_end