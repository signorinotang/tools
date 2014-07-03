//
//  vnUIListItem.cpp
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIListItem.h"
#include "vnUIList.h"
#include "vnUnicode.h"
#include "vnUILabel.h"
#include "vnUIPicture.h"

_vn_begin


UIListItem::UIListItem()
: m_userData(0)
, m_list(0) {
    
}

UIListItem::UIListItem(const UIListItem &other)
: UILogic(other)
, m_userData(other.m_userData)
, m_list(0) {
    if (m_userData) {
        m_userData->grab();
    }
}

UIListItem::~UIListItem() {
    VN_SAFE_DROP(m_userData);
}

void UIListItem::setList(UIList *list) {
    m_list = list;
}

UIList * UIListItem::getList() const {
    return m_list;
}

void UIListItem::set(const vn::TreeDataObject *object) {
    
}

void UIListItem::setText(const str8 &text) {
    
}

void UIListItem::setText(const str32 &text) {
    
}

void UIListItem::getText(str8 &text) {
    
}

void UIListItem:: onSelected() {
    
}

void UIListItem::onUnselected() {
    
}

void UIListItem::setUserData(RefCounted *object, bool grab) {
    if (object && grab) {
        object->grab();
    }
    if (m_userData) {
        m_userData->drop();
    }
    m_userData = object;
}

RefCounted * UIListItem::getUserData() const {
    return m_userData;
}

//////////////////////////////////////////////////

UIListItemText::UIListItemText()
: m_text(0)
, m_bkg_selected(0)
, m_textNormalColor(color_t::kWhite)
, m_textSelectedColor(color_t::kBlue) {
    
}

UIListItemText::UIListItemText(const UIListItemText &other)
: UIListItem(other)
, m_size(other.m_size)
, m_text(0)
, m_bkg_selected(0)
, m_textNormalColor(other.m_textNormalColor)
, m_textSelectedColor(other.m_textSelectedColor) {
    
}

UIListItemText * UIListItemText::clone() const {
    return vnnew UIListItemText(*this);
}

void UIListItemText::set(const vn::TreeDataObject *object) {
    vnassert(object);
    const TreeDataString *tds = object->queryAttributeString("text");
    if (tds) {
        setText(tds->value());
    }
    m_size.x = object->queryAttributeFloat("width", m_size.x);
    m_size.y = object->queryAttributeFloat("height", m_size.y);
    
    color_t clr;
    clr.convertFromBGRA(object->queryAttributeInteger("text_normal_color", m_textNormalColor.convertToBGRA()));
    setTextNormalColor(clr);
    
    clr.convertFromBGRA(object->queryAttributeInteger("text_selected_color", m_textSelectedColor.convertToBGRA()));
    setTextSelectedColor(clr);
}

void UIListItemText::init(const TreeDataObject *object) {
    UIListItem::init(object);
    m_size.x = object->queryAttributeFloat("width");
    m_size.y = object->queryAttributeFloat("height");
    m_textNormalColor.convertFromBGRA(object->queryAttributeInteger("text_normal_color", m_textNormalColor.convertToBGRA()));
    m_textSelectedColor.convertFromBGRA(object->queryAttributeInteger("text_selected_color", m_textSelectedColor.convertToBGRA()));
}

f32 UIListItemText::calculateWidth() {
    return m_size.x;
}

void UIListItemText::setPositionAndHeight(const vector2f &pos, f32 height) {
    aabox2f rect;
    rect.min_corner = pos;
    rect.max_corner = pos + vector2f(m_size.x, height);
    m_root->setLocation(rect);
}

f32 UIListItemText::calculateHeight() {
    return m_size.y;
}

void UIListItemText::setPositionAndWidth(const vector2f &pos, f32 width) {
    aabox2f rect;
    rect.min_corner = pos;
    rect.max_corner = pos + vector2f(width, m_size.y);
    m_root->setLocation(rect);
}

void UIListItemText::setText(const str8 &text) {
    m_text->setText(text);
}

void UIListItemText::setText(const str32 &text) {
    m_text->setText(text);
}

void UIListItemText::getText(str8 &text) {
    text = m_text->getText();
}

void UIListItemText::setTextNormalColor(const color_t &color) {
    m_textNormalColor = color;
    if (m_list && m_list->getSelectedItem() != this) {
        m_text->setTextColor(color);
    }
}

void UIListItemText::setTextSelectedColor(const color_t &color) {
    m_textSelectedColor = color;
    if (m_list && m_list->getSelectedItem() == this) {
        m_text->setTextColor(color);
    }
}

void UIListItemText::onSelected() {
    m_text->setTextColor(m_textSelectedColor);
    if (m_bkg_selected) {
        m_bkg_selected->setVisible(true);
    }
}

void UIListItemText::onUnselected() {
    m_text->setTextColor(m_textNormalColor);
    if (m_bkg_selected) {
        m_bkg_selected->setVisible(false);
    }
}

bool UIListItemText::_startup(UIFactory::ElementMap &namedElements) {
    vnassert(!m_text && !m_bkg_selected);
    std::map<str8, UIElement *>::iterator it;
    it = namedElements.find("text");
    if (it == namedElements.end()) {
        return false;
    }
    m_text = dynamic_cast<UILabel *>(it->second);
    if (!m_text) {
        return false;
    }
    it = namedElements.find("bkg_selected");
    if (it != namedElements.end()) {
        m_bkg_selected = dynamic_cast<UIPicture *>(it->second);
    }
    
    // init elements.
    if (m_bkg_selected) {
        m_bkg_selected->setVisible(false);
    }
    m_text->setTextColor(m_textNormalColor);
    return true;
}


_vn_end