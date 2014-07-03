//
//  vnUIButtonDefault.cpp
//  vngfx
//
//  Created by Wayne on 13-12-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIButtonDefault.h"
#include "vnFontManager.h"
#include "vnUIRenderer.h"

_vn_begin

UIButtonDefault * UIButtonDefault::clone() const {
    return vnnew UIButtonDefault(*this);
}

void UIButtonDefault::init(const TreeDataObject *object) {
    UIButton::init(object);
    const TreeDataString *tdstr = object->queryAttributeString("font");
    if (tdstr) {
        m_renderText.setFont(FontManager::instance().findFont(tdstr->value()));
    } else {
        m_renderText.setFont(FontManager::instance().defaultFont());
    }
    tdstr = object->queryAttributeString("text");
    if (tdstr) {
        m_renderText.setText(tdstr->value());
    }
}

void UIButtonDefault::_onUpdate(f32 deltaTime) {
    m_renderText.update();
    m_textPosition = (m_boundingBox.min_corner + m_boundingBox.max_corner - m_renderText.size()) * 0.5f;
}

void UIButtonDefault::_onRender(UIRenderer *renderer) {
    static const color_t c_bkg_normal(0x40, 0x40, 0x40);
    static const color_t c_bkg_normal_hover(0x50, 0x50, 0x50);
    static const color_t c_bkg_disabled(0x40, 0x40, 0x40);
    static const color_t c_bkg_pushed(0x0, 0x40, 0x0);
    static const color_t c_bkg_pushed_hover(0x0, 0x50, 0x0);
    static const color_t c_txt_normal(0xE0, 0xE0, 0xE0);
    static const color_t c_txt_normal_hover(0xF0, 0xF0, 0xF0);
    static const color_t c_txt_disabled(0x80, 0x80, 0x80);
    static const color_t c_txt_pushed(0xA0, 0xE0, 0xA0);
    static const color_t c_txt_pushed_hover(0xB0, 0xF0, 0xB0);
    switch (m_state) {
        case UIButton::kDisabled: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_disabled);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_disabled);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_disabled);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_disabled);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(2, 2);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_disabled);
            
            m_renderText.render(renderer, m_textPosition, c_txt_disabled);
            break;
        }
            
        case UIButton::kNormal: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_normal);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_normal);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_normal);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_normal);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(2, 2);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_normal);
            
            m_renderText.render(renderer, m_textPosition, c_txt_normal);
            break;
        }
            
        case UIButton::kNormalMouseEntered: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(2, 2);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            m_renderText.render(renderer, m_textPosition, c_txt_normal_hover);
            break;
        }
            
        case UIButton::kNormalPressed: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(4, 4);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_normal_hover);
            
            m_renderText.render(renderer, m_textPosition + vector2f(2, 2), c_txt_normal_hover);
            break;
        }
            
        case UIButton::kPushed: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_pushed);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_pushed);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_pushed);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_pushed);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(3, 3);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_pushed);
            
            m_renderText.render(renderer, m_textPosition + vector2f(1, 1), c_txt_pushed);
            break;
        }
            
        case UIButton::kPushedMouseEntered: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(3, 3);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            m_renderText.render(renderer, m_textPosition + vector2f(1, 1), c_txt_pushed_hover);
            break;
        }
            
        case UIButton::kPushedPressed: {
            aabox2f rect;
            rect.min_corner = m_boundingBox.min_corner;
            rect.max_corner.set(m_boundingBox.max_corner.x, m_boundingBox.min_corner.y + 1);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.max_corner.set(m_boundingBox.min_corner.x + 1, m_boundingBox.max_corner.y);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.max_corner = m_boundingBox.max_corner;
            rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - 1);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.min_corner.set(m_boundingBox.max_corner.x - 1, m_boundingBox.min_corner.y);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            rect.min_corner = m_boundingBox.min_corner + vector2f(4, 4);
            rect.max_corner = m_boundingBox.max_corner - vector2f(2, 2);
            renderer->drawRect(rect, c_bkg_pushed_hover);
            
            m_renderText.render(renderer, m_textPosition + vector2f(2, 2), c_txt_pushed_hover);
            break;
        }
    }
}

_vn_end
