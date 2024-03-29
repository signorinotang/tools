//
//  vnUILabel.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILabel.h"
#include "vnUnicode.h"
#include "vnFontManager.h"

_vn_begin

UILabel::UILabel()
: m_textAlignment(0)
, m_textColor(color_t::kWhite) {
    m_enabled = false;
}

UILabel * UILabel::clone() const {
    return vnnew UILabel(*this);
}

u32 UILabel::getTextAlignment() const {
    return m_textAlignment;
}

void UILabel::setTextAlignment(u32 alignment) {
    m_textAlignment = alignment;
}

void UILabel::setFont(const FontPtr &font) {
    m_renderText.setFont(font);
}

const FontPtr & UILabel::getFont() const {
    return m_renderText.getFont();
}

void UILabel::setText(const str8 &text) {
    m_renderText.setText(m_text = text);
}

void UILabel::setText(const str32 &text) {
    m_renderText.setText(text);
    Unicode::convert(text.c_str(), text.length(), m_text);
}

const str8 & UILabel::getText() const {
    return m_text;
}

void UILabel::setTextColor(const color_t &color) {
    m_textColor = color;
}

const color_t & UILabel::getTextColor() const {
    return m_textColor;
}

const UIRenderText & UILabel::_text() const {
    return m_renderText;
}

void UILabel::init(const TreeDataObject *object) {
    UIElement::init(object);
    const TreeDataString *tdstr = object->queryAttributeString("font");
    if (tdstr) {
        m_renderText.setFont(FontManager::instance().findFont(tdstr->value()));
    } else {
        m_renderText.setFont(FontManager::instance().defaultFont());
    }
    tdstr = object->queryAttributeString("text");
    if (tdstr) {
        setText(tdstr->value());
    }
    m_textColor.convertFromBGRA(object->queryAttributeInteger("color", m_textColor.convertToBGRA()));
    m_textAlignment = object->queryAttributeInteger("alignment");
}

void UILabel::_onUpdate(f32 deltaTime) {
    m_renderText.update();
    vector2f pen;
    if (m_textAlignment) {
        switch (m_textAlignment & kTA_HorizontalCenter) {
            case kTA_HorizontalCenter:
                pen.x = (m_boundingBox.max_corner.x + m_boundingBox.min_corner.x - m_renderText.size().x) * 0.5f;
                break;
                
            case kTA_Right:
                pen.x = m_boundingBox.max_corner.x - m_renderText.size().x;
                break;
                
            default:
                pen.x = m_boundingBox.min_corner.x;
                break;
        }
        switch (m_textAlignment & kTA_VerticalCenter) {
            case kTA_VerticalCenter:
                pen.y = (m_boundingBox.max_corner.y + m_boundingBox.min_corner.y - m_renderText.size().y) * 0.5f;
                break;
                
            case kTA_Bottom:
                pen.y = m_boundingBox.max_corner.y - m_renderText.size().y;
                break;
                
            default:
                pen.y = m_boundingBox.min_corner.y;
                break;
        }
    } else {
        pen = m_boundingBox.min_corner;
    }
    m_textPosition = pen;
}

void UILabel::_onRender(UIRenderer *renderer) {
    m_renderText.render(renderer, m_textPosition, m_textColor);
}



_vn_end