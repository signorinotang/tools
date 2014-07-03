//
//  vnUIScrollBar.cpp
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIScrollBar.h"
#include "vnUIRoot.h"
#include "vnGfxApplication.h"
#include "vnLog.h"

_vn_begin

UIScrollBar::UIScrollBar()
: m_style(kST_Default)
, m_bkgColor(color_t::kWhite)
, m_bkgPicture(vnnew UIRenderPictureEmpty())
, m_sliderColor(color_t::kWhite)
, m_sliderPicture(vnnew UIRenderPicture0())
, m_sliderDirty(false)
, m_sliderUnit(1)
, m_sliderRange(100)
, m_sliderPos(0)
, m_sliderSize(50)
, m_mouseDown(false)
, m_mouseDownOffCenter(0)
, m_sliderLimited(true)
, m_sliderInterval(0) {
	
}

UIScrollBar::UIScrollBar(const UIScrollBar &other)
: UIElement(other)
, m_style(other.m_style)
, m_bkgColor(other.m_bkgColor)
, m_bkgPicture(other.m_bkgPicture->clone())
, m_sliderColor(other.m_sliderColor)
, m_sliderPicture(other.m_sliderPicture->clone())
, m_sliderRect(other.m_sliderRect)
, m_sliderDirty(other.m_sliderDirty)
, m_sliderPos(other.m_sliderPos)
, m_sliderSize(other.m_sliderSize)
, m_sliderRange(other.m_sliderRange)
, m_sliderUnit(other.m_sliderUnit)
, m_mouseDown(false)
, m_mouseDownOffCenter(0) {
	
}

UIScrollBar * UIScrollBar::clone() const {
	return vnnew UIScrollBar(*this);
}

void UIScrollBar::setStyle(ScrollType st) {
    if (m_style != st) {
        m_style = st;
        m_sliderDirty = true;
    }
}

ScrollType UIScrollBar::getStyle() const {
    return m_style;
}

void UIScrollBar::setSliderPosition(f32 position) {
    if (m_sliderLimited) {
        if (position < 0) {
            position = 0;
        } else if (position > m_sliderRange) {
            position = m_sliderRange;
        }
    }
    if (m_sliderInterval) {
        position = (s64)(position / m_sliderInterval) * m_sliderInterval;
    }
	if (m_sliderPos != position) {
		m_sliderPos = position;
		m_sliderDirty = true;
        onScrolling();
	}
}

f32 UIScrollBar::getSliderPosition() const {
	return m_sliderPos;
}

void UIScrollBar::setSliderRange(f32 range)  {
	if (m_sliderRange != range) {
		m_sliderRange = range;
		m_sliderDirty = true;
	}
}

f32 UIScrollBar::getSliderRange() const {
	return m_sliderRange;
}

void UIScrollBar::setSliderSize(f32 size) {
	if (m_sliderSize != size) {
		m_sliderSize = size;
		m_sliderDirty = true;
	}
}

f32 UIScrollBar::getSliderSize() const {
	return m_sliderSize;
}

void UIScrollBar::setSliderUnit(f32 unit) {
	m_sliderUnit = unit;
}

f32 UIScrollBar::getSliderUnit() const {
	return m_sliderUnit;
}

void UIScrollBar::setSliderInterval(f32 interval) {
    m_sliderInterval = interval;
}

f32 UIScrollBar::getSliderInterval() const {
    return m_sliderInterval;
}

void UIScrollBar::init(const TreeDataObject *object) {
	UIElement::init(object);
	m_style = (ScrollType)object->queryAttributeInteger("style");
	
	m_bkgColor.convertFromBGRA(object->queryAttributeInteger("bkg_color", m_bkgColor.convertToBGRA()));
	m_sliderColor.convertFromBGRA(object->queryAttributeInteger("slider_color", m_sliderColor.convertToBGRA()));
	const TreeDataObject *tdobj = object->queryAttributeObject("bkg_image");
	UIRenderPicture *pic = 0;
	if (tdobj) {
		pic = UIFactory::instance().createRenderPicture(tdobj);
	}
	if (pic) {
		m_bkgPicture = pic;
	}
	tdobj = object->queryAttributeObject("slider_image");
	pic = 0;
	if (tdobj) {
		pic = UIFactory::instance().createRenderPicture(tdobj);
	}
	if (pic) {
		m_sliderPicture = pic;
	}
    
    m_sliderRange = object->queryAttributeFloat("slider_range", m_sliderRange);
    m_sliderSize = object->queryAttributeFloat("slider_size", m_sliderSize);
    
    m_sliderUnit = object->queryAttributeFloat("slider_unit", m_sliderUnit);
    m_sliderInterval = object->queryAttributeFloat("slider_interval", m_sliderInterval);
    m_sliderLimited = object->queryAttributeBoolean("slider_limited", m_sliderLimited);
}

void UIScrollBar::bindAction_Scrolling(const function<void (vn::UIElement *, f32)> &func) {
    m_fnScrolling = func;
}

void UIScrollBar::slide(f32 delta) {
    if (delta == 0) {
        return ;
    }
    f32 d = (delta > 0 ? 1 : -1);
    f32 m = math::abs(delta) * m_sliderUnit;
    if (m < m_sliderInterval) {
        m = m_sliderInterval;
    }
	setSliderPosition(m_sliderPos - d * m);
}

void UIScrollBar::onMouseLeftDown(const vector2f &pos) {
    UIRoot::instance().captureMouse(this);
    m_mouseDown = true;
    if (m_sliderRect.min_corner <= pos && m_sliderRect.max_corner > pos) {
        if (m_style == kST_Horizontal) {
            m_mouseDownOffCenter = pos.x - (m_sliderRect.max_corner.x + m_sliderRect.min_corner.x) * 0.5f;
        } else {
            m_mouseDownOffCenter = pos.y - (m_sliderRect.max_corner.y + m_sliderRect.min_corner.y) * 0.5f;
        }
    } else {
        m_mouseDownOffCenter = 0;
        if (m_style == kST_Horizontal) {
            f32 hw = (m_sliderRect.max_corner.x - m_sliderRect.min_corner.x) * 0.5f;
            f32 d = pos.x - hw - m_boundingBox.min_corner.x;
            if (d < 0) {
                m_mouseDownOffCenter = d;
            } else {
                d = pos.x + hw - m_boundingBox.max_corner.x;
                if (d > 0) {
                    m_mouseDownOffCenter = d;
                }
            }
        } else {
            f32 hh = (m_sliderRect.max_corner.y - m_sliderRect.min_corner.y) * 0.5f;
            f32 d = pos.y - hh - m_boundingBox.min_corner.y;
            if (d < 0) {
                m_mouseDownOffCenter = d;
            } else {
                d = pos.y + hh - m_boundingBox.max_corner.y;
                if (d > 0) {
                    m_mouseDownOffCenter = d;
                }
            }
        }
    }
}

void UIScrollBar::onMouseLeftUp(const vector2f &pos) {
    if (m_mouseDown) {
        m_mouseDown = false;
        UIRoot::instance().releaseMouse(this);
    }
}

void UIScrollBar::onMouseWheel(const vector2f &pos, const vector2f &delta) {
	if (m_style == kST_Horizontal) {
		slide(delta.x);
	} else {
		slide(delta.y);
	}
}

void UIScrollBar::onScrolling() {
    if (!m_fnScrolling.null()) {
        m_fnScrolling(this, m_sliderPos);
    }
}

void UIScrollBar::_onBoundingBoxUpdated() {
	m_sliderDirty = true;
	m_bkgPicture->setLocation(m_boundingBox);
}

void UIScrollBar::_onUpdate(f32 deltaTime) {
	if (m_sliderDirty) {
		m_sliderDirty = false;
		vector2f std_size = m_sliderPicture->calculateMinimumSize();
		vector2f size = m_boundingBox.size();
        f32 total_range = m_sliderRange + m_sliderSize;
		if (m_style == kST_Horizontal) {
			m_sliderRect.min_corner.y = m_boundingBox.min_corner.y;
			m_sliderRect.max_corner.y = m_boundingBox.max_corner.y;
			size.x -= std_size.x;
			f32 slider = m_sliderSize / total_range * size.x;
			f32 pos = m_sliderPos / total_range * size.x;
            size.x -= slider;
			if (pos < 0) {
				slider += pos;
				pos = 0;
                if (slider < 0) {
                    slider = 0;
                }
			} else if (pos > size.x) {
				slider -= pos - size.x;
                if (slider < 0) {
                    pos += slider;
                    slider = 0;
                }
			}
			m_sliderRect.min_corner.x = m_boundingBox.min_corner.x + pos;
			m_sliderRect.max_corner.x = m_sliderRect.min_corner.x + std_size.x + slider;
		} else {
			m_sliderRect.min_corner.x = m_boundingBox.min_corner.x;
			m_sliderRect.max_corner.x = m_boundingBox.max_corner.x;
			size.y -= std_size.y;
			f32 slider = m_sliderSize / total_range * size.y;
			f32 pos = m_sliderPos / total_range * size.y;
            size.y -= slider;
			if (pos < 0) {
				slider += pos;
				pos = 0;
                if (slider < 0) {
                    slider = 0;
                }
			} else if (pos > size.y) {
				slider -= pos - size.y;
                if (slider < 0) {
                    pos += slider;
                    slider = 0;
                }
			}
			m_sliderRect.min_corner.y = m_boundingBox.min_corner.y + pos;
			m_sliderRect.max_corner.y = m_sliderRect.min_corner.y + std_size.y + slider;
		}
		m_sliderPicture->setLocation(m_sliderRect);
	}
	m_sliderPicture->update(deltaTime);
	m_sliderPicture->update(deltaTime);
    
    if (m_mouseDown) {
        vector2f pt = GfxApplication::instance().getMousePosition();
        f32 pos = 0;
        if (m_style == kST_Horizontal) {
            f32 center = pt.x - m_mouseDownOffCenter;
            f32 hw = (m_sliderRect.max_corner.x - m_sliderRect.min_corner.x) * 0.5f;
            f32 left = m_boundingBox.min_corner.x + hw;
            f32 right = m_boundingBox.max_corner.x - hw;
            pos = (center - left) / (right - left) * m_sliderRange;
        } else {
            f32 center = pt.y - m_mouseDownOffCenter;
            f32 hh = (m_sliderRect.max_corner.y - m_sliderRect.min_corner.y) * 0.5f;
            f32 top = m_boundingBox.min_corner.y + hh;
            f32 bottom = m_boundingBox.max_corner.y - hh;
            pos = (center - top) / (bottom - top) * m_sliderRange;
        }
        if (pos < 0) {
            pos = 0;
        } else if (pos > m_sliderRange) {
            pos = m_sliderRange;
        }
        setSliderPosition(pos);
    }
}

void UIScrollBar::_onRender(vn::UIRenderer *renderer) {
	m_bkgPicture->render(renderer, m_bkgColor);
	m_sliderPicture->render(renderer, m_sliderColor);
}

bool UIScrollBar::_bindAction(const c8 *name, RefCounted *func_impl) {
    VN_UI_ELEMENT_BIND_ACTION(UIScrollBar, "scrolling", m_fnScrolling, name, func_impl);
    return UIElement::_bindAction(name, func_impl);
}

_vn_end